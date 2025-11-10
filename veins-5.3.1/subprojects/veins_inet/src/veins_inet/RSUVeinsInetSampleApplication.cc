#include "veins_inet/RSUVeinsInetSampleApplication.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"

#include "veins_inet/VeinsInetSampleMessage_m.h"
#include "inet/applications/base/ApplicationPacket_m.h"

using namespace inet;
using namespace veins;

Define_Module(RSUVeinsInetSampleApplication);

RSUVeinsInetSampleApplication::RSUVeinsInetSampleApplication()
{
}

bool RSUVeinsInetSampleApplication::startApplication()
{
    EV_INFO << "[RSUSafeVeinsInetSampleApplication::startApplication]\n";

    EV_INFO << "802.11p socket bound on port " << localPort
            << " and Wi-Fi socket bound on port " << localPortWifi << endl;

    // initial broadcast from RSU:
    //if (getParentModule()->getIndex() == 0) {
    //    auto payload = makeShared<VeinsInetSampleMessage>();
    //    payload->setChunkLength(B(100));
    //    payload->setRoadId("init_msg");
    //    timestampPayload(payload);

    //    auto packet = createPacket("RSU_Init_Broadcast");
    //    packet->insertAtBack(payload);

    //    L3Address wifiGroup = L3AddressResolver().resolve("224.0.0.4");
    //    socketWifi.sendTo(packet.release(), wifiGroup, localPortWifi);
    //    EV_INFO << "Initial RSU broadcast sent to Wi-Fi group 224.0.0.2\n";
    //}

    return true;
}


bool RSUVeinsInetSampleApplication::stopApplication()
{
    return true;
}

RSUVeinsInetSampleApplication::~RSUVeinsInetSampleApplication()
{
}

void RSUVeinsInetSampleApplication::processPacket(std::shared_ptr<Packet> pk)
{
    EV_INFO << "[RSUVeinsInetSampleApplication::processPacket] Received packet\n";
    auto packet = pk->peekAtFront<VeinsInetSampleMessage>();

    if (packet == nullptr) {
        EV_WARN << "Packet does not contain a VeinsInetSampleMessage, dropping.\n";
        return;
    }
    std::string id = packet->getExternalId();
    std::string roadId = packet->getRoadId();

    EV_INFO << "[RSUVeinsInetSampleApplication::processPacket] Received from "
            << id
            << " roadId=" << packet->getRoadId()
            << " speed=" << packet->getSpeed() << "\n";

    // for relaying
    //auto outPkt = pk->dup();
    //outPkt->setName("relay");

    auto payload = makeShared<VeinsInetSampleMessage>();
    payload->setChunkLength(B(100));
    payload->setExternalId(id.c_str());
    payload->setRoadId(roadId.c_str());
    payload->setSpeed(packet->getSpeed());
    timestampPayload(payload);

    auto relayPacket = createPacket("Relay_Packet");
    relayPacket->insertAtBack(payload);
    sendPacket(std::move(relayPacket));
    //std::unique_ptr<inet::Packet> relayPacket(outPkt);
    //sendPacket(std::move(relayPacket));
}
