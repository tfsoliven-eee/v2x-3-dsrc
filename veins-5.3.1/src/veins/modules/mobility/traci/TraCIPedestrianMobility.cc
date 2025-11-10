/*
 * TraCIPedestrianMobility.cc
 *
 *      Author: opp_env
 */

 #include "veins/modules/mobility/traci/TraCIPedestrianMobility.h"
//#include "veins/base/utils/Heading.h"

using namespace veins;

using veins::TraCIPedestrianMobility;

Define_Module(veins::TraCIPedestrianMobility);

const simsignal_t TraCIPedestrianMobility::mobilityStateChangedSignal = registerSignal("mobilityStateChanged");
//const simsignal_t TraCIPedestrianMobility::mobilityStateChangedDetailedSignal = registerSignal("mobilityStateChangedDetailed");


void TraCIPedestrianMobility::initialize(int stage) {
    if (stage == 0) {
        BaseMobility::initialize(stage);
        isPreInitialized = false;
    }
}

void TraCIPedestrianMobility::finish() {
}

void TraCIPedestrianMobility::handleSelfMsg(cMessage* msg) {
    delete msg; // no self-messages needed for pedestrians
}

void TraCIPedestrianMobility::preInitialize(std::string external_id, const Coord& position, std::string road_id, double speed, double angle)
{
    this->external_id = external_id;
    this->roadPosition = position;
    this->road_id = road_id;
    this->speed = speed;
    this->angle = angle;
    this->isPreInitialized = true;

    isPreInitialized = true;

    EV_INFO << "[TraCIPedestrianMobility::preInitialize]" << endl;
}

void TraCIPedestrianMobility::nextPosition(const Coord& position, std::string road_id, double speed, double angle)
{
    EV_DEBUG << "nextPosition " << roadPosition << " " << road_id << " " << speed << " " << angle << std::endl;
    isPreInitialized = false;
    this->roadPosition = position;
    this->road_id = road_id;
    this->speed = speed;
    this->angle = angle;

    changePosition();
}

void TraCIPedestrianMobility::changePosition()
{
    move.setStart(roadPosition);
    //move.setDirectionByVector(Heading::fromDegrees(angle).toCoord());
    //move.setOrientationByVector(Heading::fromDegrees(angle).toCoord());
    move.setSpeed(speed);

    updatePosition();
    emit(mobilityStateChangedSignal, this);
}




