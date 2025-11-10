/*
 * RSUVeinsInetApplicationBase.cc
 *
 *  Created on: Oct 12, 2025
 *      Author: opp_env
 */
#include "veins_inet/RSUVeinsInetApplicationBase.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "veins_inet/VeinsInetSampleMessage_m.h"

using namespace inet;
using namespace veins;

void RSUVeinsInetApplicationBase::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        localPort = par("localPort");
        destPort = par("destPort");

        localPortWifi = par("localPortWifi");
        destPortWifi = par("destPortWifi");
    }
}

void RSUVeinsInetApplicationBase::handleStartOperation(LifecycleOperation* operation)
{
    EV_INFO << "[RSUVeinsInetApplicationBase::handleStartOperation] start\n";

    // Resolve multicast addresses
    //if (!L3AddressResolver().tryResolve(par("destAddress"), destAddress))
    //    EV_WARN << "Could not resolve 224.0.0.1 "<<L3Address()<< endl;
    //if (!L3AddressResolver().tryResolve(par("destAddressWifi"), destAddressWifi))
    //    EV_WARN << "Could not resolve "<<L3Address()<< endl;

    EV_INFO << "[RSUVeinsInetApplicationBase::handleStartOperation]  Found gates socketOut and socketIn" << destAddressWifi << endl;

    //const char* ifTablePath = par("interfaceTableModule").stringValue();

    IInterfaceTable* ift = nullptr;
    ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
    //if (ifTablePath && ifTablePath[0]) {}

    const char* iface80211p = par("interface").stringValue();
    const char* ifaceWifi = par("interfaceWifi").stringValue();

    EV_INFO << "[RSUVeinsInetApplicationBase::handleStartOperation] interface (DSRC) = \"" << iface80211p << "\"; interfaceWifi = \"" << ifaceWifi << "\"\n";

    // Find interface entries
#if INET_VERSION >= 0x0403
    NetworkInterface* ie80211p = ift->findInterfaceByName(iface80211p);
    NetworkInterface* ieWifi    = ift->findInterfaceByName(ifaceWifi);
#else
    InterfaceEntry* ie80211p = ift->findInterfaceByName(iface80211p);
    InterfaceEntry* ieWifi    = ift->findInterfaceByName(ifaceWifi);
#endif

    Ipv4Address localAddress = ie80211p->getProtocolData<Ipv4InterfaceData>()->getIPAddress();
    // Bind DSRC socket to 9001 wlan0
    socket.setOutputGate(gate("socketOut"));
    socket.bind(L3Address(localAddress), localPort);
    //socket.setMulticastOutputInterface(ie80211p->getInterfaceId());
    socket.joinLocalMulticastGroups(ift->collectMulticastGroups());
    socket.setCallback(this);
    socket.setBroadcast(true);
    EV_INFO << "[RSUVeinsInetApplicationBase::handleStartOperation] DSRC socket on port " << localPort << " interface " << iface80211p << "\n";

    Ipv4Address localAddressWifi = ieWifi->getProtocolData<Ipv4InterfaceData>()->getIPAddress();
    // Bind Wi-Fi socket (share same gate) to 1000 wlan1
    socketWifi.setOutputGate(gate("socketOut"));
    socketWifi.bind(L3Address(localAddressWifi), localPortWifi);
    socketWifi.setMulticastOutputInterface(ieWifi->getInterfaceId());
    //socketWifi.joinLocalMulticastGroups(ift->collectMulticastGroups());
    socketWifi.setCallback(this);
    socketWifi.setBroadcast(true);
    EV_INFO << "[RSUVeinsInetApplicationBase::handleStartOperation] Wi-Fi socket on port " << localPortWifi << " interface " << ifaceWifi << "\n";

    EV_INFO << "[RSUVeinsInetApplicationBase::handleStartOperation] end; to call startApplication\n";
    bool ok = startApplication();
    //processPacket(pk)
    if (!ok)
        throw cRuntimeError("startApplication() returned false");
    
}

void RSUVeinsInetApplicationBase::handleStopOperation(LifecycleOperation* operation)
{
    bool ok = stopApplication();
    ASSERT(ok);

    socket.close();
    socketWifi.close();
}

void RSUVeinsInetApplicationBase::handleCrashOperation(LifecycleOperation* operation)
{
    socket.destroy();
    socketWifi.destroy();
}

void RSUVeinsInetApplicationBase::handleMessageWhenUp(cMessage* msg)
{
    EV_INFO << "[RSUVeinsInetApplicationBase::handleMessageWhenUp] start\n";

    if (timerManager.handleMessage(msg)) return;

    if (msg->isSelfMessage()) {
        throw cRuntimeError("This module does not use custom self messages");
        return;
    }
    EV_INFO << "[RSUVeinsInetApplicationBase::handleMessageWhenUp] to processMessage\n";
        // Handle UDP sockets
    if (socket.belongsToSocket(msg)) {
        socket.processMessage(msg);
        return;  
    }
    else if (socketWifi.belongsToSocket(msg)) {
        socketWifi.processMessage(msg);
        return; 
    }
    delete msg;
    
}

void RSUVeinsInetApplicationBase::sendPacket(std::unique_ptr<inet::Packet> pk)
{
    emit(packetSentSignal, pk.get());

    auto payload = pk->peekAtFront<VeinsInetSampleMessage>();
    std::string id = payload->getExternalId();

    int numPeds = 2;

    if (id.rfind("jeep", 0) == 0) {                      // Jeep -> Pedestrian via Wifi
        EV_INFO << "::sendPacket received jeep packet"<<endl;
        for (int i = 0; i < numPeds; i++){
            auto relayPkt = pk->dup();
            relayPkt->setName("Relay_Jeep_Update");

            std::string destAddressNew = "draft.ped[" + std::to_string(i) + "]";
            L3Address dest = L3AddressResolver().resolve(destAddressNew.c_str());

            EV_INFO << "[RSUVeinsInetApplicationBase::sendPacket] Jeep -> Pedestrian via Wi-Fi: " << dest << ":" << destPortWifi << endl;
            socketWifi.sendTo(relayPkt, dest, destPortWifi);
        }  
    } 
    else {//if (id.rfind("ped", 0) == 0) {     // Pedestrian -> Jeep via DSRC
        EV_INFO<<"HELLO"<<endl;
        auto relayPkt = pk->dup();
        relayPkt->setName("Relay_Pedestrian_Update");

        std::string destAddressNew = "draft.jeep[672]"; // jeep index: low = 672 | med = 703 | high = 733
        L3Address dest = L3AddressResolver().resolve(destAddressNew.c_str());

        //L3Address dest = L3AddressResolver().resolve(par("destAddress"));
        EV_INFO << "[RSUVeinsInetApplicationBase::sendPacket] Pedestrian -> Jeep via DSRC: " << dest << ":" << destPort << endl;
        socket.sendTo(relayPkt, dest, destPort);
    }
}

