/*
 * TraCIDemoPedestrian11p.cc
 *
 *      Author: opp_env
 */

#include "veins/modules/application/traci/TraCIDemoPedestrian11p.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"
#include "veins/base/utils/FindModule.h"


using namespace veins;

Define_Module(veins::TraCIDemoPedestrian11p);


void TraCIDemoPedestrian11p::initialize(int stage) {
    BaseApplLayer::initialize(stage);

    if (stage == 0) {
        mobility = FindModule<TraCIPedestrianMobility*>::findSubModule(getParentModule());
        ASSERT(mobility);

        externalId = mobility->getExternalId();
        EV_INFO << "((TraCIDemoPedestrian11p::initialize)) Pedestrian app " << externalId << endl;

        mobility->subscribe(TraCIPedestrianMobility::mobilityStateChangedSignal, this);
    }
}

void TraCIDemoPedestrian11p::onWSM(BaseFrame1609_4* wsm) {
    auto* msg = check_and_cast<TraCIDemo11pMessage*>(wsm);
    EV_INFO << "((TraCIDemoPedestrian11p::onWSM)) Pedestrian " << externalId
            << " received WSM from node " << msg->getSenderAddress()
            << " with roadID: " << msg->getRoadID() << endl;
}

void TraCIDemoPedestrian11p::onWSA(DemoServiceAdvertisment* wsa) {
    // pedestrians ignore WSAs
}

void TraCIDemoPedestrian11p::handleSelfMsg(cMessage* msg) {
    if (auto* beacon = dynamic_cast<TraCIDemo11pMessage*>(msg)) {
        sendDown(beacon->dup());
        EV_INFO << "((TraCIDemoPedestrian11p::handleSelfMsg)) Pedestrian " << externalId << " rebroadcasted a beacon" << endl;
        delete beacon;
    } else {
        DemoBaseApplLayer::handleSelfMsg(msg);
    }
}

void TraCIDemoPedestrian11p::handlePositionUpdate(cObject* obj) {
    DemoBaseApplLayer::handlePositionUpdate(obj);

    Coord pos = mobility->getCurrentPosition();

    auto* beacon = new TraCIDemo11pMessage();
    populateWSM(beacon);

    std::ostringstream ss;
    ss << "((TraCIDemoPedestrian11p::handlePositionUpdate)) Pedestrian " << externalId;
    beacon->setRoadID(ss.str().c_str());
    beacon->setExternalID(ss.str().c_str());
    //beacon->setSpeed(ss.str().c_str());

    sendDown(beacon);
    EV_INFO << "((TraCIDemoPedestrian11p::handlePositionUpdate)) Pedestrian " << externalId << " sent beacon: " << ss.str() << endl;
}



