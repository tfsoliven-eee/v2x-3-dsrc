/*
 * PedVeinsInetSampleApplication.cc
 *
 *  Created on: Oct 22, 2025
 *      Author: opp_env
 */

#include "veins_inet/PedVeinsInetSampleApplication.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"

#include "veins_inet/VeinsInetSampleMessage_m.h"

using namespace inet;

Define_Module(PedVeinsInetSampleApplication);

PedVeinsInetSampleApplication::PedVeinsInetSampleApplication()
{
}

void PedVeinsInetSampleApplication::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        sendInterval = par("sendInterval");
    }
}

bool PedVeinsInetSampleApplication::startApplication()
{
    EV_INFO << "[PedVeinsInetSampleApplication::startApplication]\n";

    //auto payload = makeShared<VeinsInetSampleMessage>();
    //payload->setChunkLength(B(100));
    
    //payload->setExternalId(mobilityPed->getExternalId().c_str());
    //payload->setRoadId(traciPed->getRoadId().c_str());
    //timestampPayload(payload);

    //auto packet = createPacket("Ped_Init_Broadcast");
    //packet->insertAtBack(payload);
    //sendPacket(std::move(packet));

    sendInterval = par("sendInterval");
    auto callback = [this]() { sendPeriodicPacket(); };
    veins::TimerSpecification spec(callback);
    spec.interval(sendInterval).absoluteStart(simTime() + sendInterval);
    sendTimer = timerManager.create(spec, "sendTimer"); 

    return true;
}

bool PedVeinsInetSampleApplication::stopApplication()
{
    timerManager.cancel(sendTimer);
    return true;
}

PedVeinsInetSampleApplication::~PedVeinsInetSampleApplication()
{
}

int PedVeinsInetSampleApplication::findClosestRSU()
{
    // Get pedestrian’s current position
    auto pedPos = mobilityPed->getCurrentPosition();

    // Access the top-level network module (e.g. "draft")
    cModule* network = getSimulation()->getSystemModule();
    //int numRSU = network->par("numRSU");  // optional, or use network->getSubmodule("rsu", i)

    int numRSU = getSimulation()->getSystemModule()->getSubmoduleVectorSize("rsu");

    double minDist = std::numeric_limits<double>::max();
    int closestIdx = -1;

    // Loop through all RSU modules
    for (int i = 0; i < numRSU; i++) {
        cModule* rsu = network->getSubmodule("rsu", i);
        if (!rsu) continue;

        // Access its mobility module
        cModule* mobility = rsu->getSubmodule("mobility");
        if (!mobility) continue;

        inet::IMobility* rsuMob = check_and_cast<inet::IMobility*>(mobility);
        inet::Coord rsuPos = rsuMob->getCurrentPosition();

        // Compute Euclidean distance
        double dx = pedPos.x - rsuPos.x;
        double dy = pedPos.y - rsuPos.y;
        double dist = sqrt(dx*dx + dy*dy);

        if (dist < minDist) {
            minDist = dist;
            closestIdx = i;
        }
    }

    EV_INFO << "Closest RSU index for " << getParentModule()->getFullName()
            << " is rsu[" << closestIdx << "] at distance " << minDist << " m\n";

    return closestIdx;
}


void PedVeinsInetSampleApplication::sendPeriodicPacket()
{

    EV_INFO << "[PedVeinsInetSampleApplication::sendPeriodicPacket]" << endl;

    getParentModule()->getDisplayString().setTagArg("i", 1, "green");

    auto payload = makeShared<VeinsInetSampleMessage>();
    payload->setChunkLength(B(100));
    payload->setExternalId(mobilityPed->getExternalId().c_str());
    payload->setRoadId(traciPed->getRoadId().c_str());
    payload->setSenderX(mobilityPed->getCurrentPosition().x);
    payload->setSenderY(mobilityPed->getCurrentPosition().y);
    timestampPayload(payload);

    auto packet = createPacket("Pedestrian_Update");
    packet->insertAtBack(payload);
    sendPacket(std::move(packet));
}

void PedVeinsInetSampleApplication::sendPacket(std::unique_ptr<inet::Packet> pk)
{
    emit(packetSentSignal, pk.get());

    int closestRSU = findClosestRSU();

    if (closestRSU >= 0) {
        auto relayPkt = pk->dup();

        //std::string destAddressNew = "draft.rsu[" + std::to_string(closestRSU) + "].wlan[1]";
        //L3Address dest = L3AddressResolver().resolve(destAddressNew.c_str());

        std::string rsuPath = "draft.rsu[" + std::to_string(closestRSU) + "]";
        cModule* rsuMod = getModuleByPath(rsuPath.c_str());
        if (!rsuMod) {
            EV_ERROR << "RSU module not found: " << rsuPath << endl;
            return;
        }

        auto ift = check_and_cast<IInterfaceTable*>(rsuMod->getSubmodule("interfaceTable"));
        if (!ift) {
            EV_ERROR << "No interfaceTable found in " << rsuPath << endl;
            return;
        }

        // Get RSU's Wi-Fi interface name parameter (same one used in RSU code)
        const char* ifaceWifi = "wlan1";
    #if INET_VERSION >= 0x0403
        NetworkInterface* ieWifi = ift->findInterfaceByName(ifaceWifi);
    #else
        InterfaceEntry* ieWifi = ift->findInterfaceByName(ifaceWifi);
    #endif

        if (!ieWifi) {
            EV_ERROR << "RSU " << rsuPath << " has no interface named " << ifaceWifi << endl;
            return;
        }

        // Extract its IPv4 address
        Ipv4Address rsuWifiAddr = ieWifi->getProtocolData<Ipv4InterfaceData>()->getIPAddress();
        L3Address dest(rsuWifiAddr);

        EV_INFO << "[PedVeinsInetSampleApplication::sendPacket] "
                << getParentModule()->getFullName() << " sending to RSU "
            << rsuPath << " Wi-Fi IP " << dest << ":" << par("destPort") << endl;


        EV_INFO << "[PedVeinsInetApplicationBase::sendPacket] sending to " << dest << ":" <<  par("destPort") << " (pkt=" << pk.get() << ")\n";
        EV_INFO << "[PedVeinsInetApplicationBase::sendPacket] Ped " << getParentModule()->getFullName()
            << " sending Pedestrian_Update to closest RSU: " << rsuPath
            << " (" << dest << ")" << endl;

        socket.sendTo(relayPkt, dest, par("destPort"));
    }
    
}