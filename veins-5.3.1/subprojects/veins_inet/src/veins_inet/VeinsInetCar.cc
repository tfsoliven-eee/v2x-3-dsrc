#include "VeinsInetCar.h"

Define_Module(VeinsInetCar);

void VeinsInetCar::initialize()
{
    // nothing special; you can add initialization logic here
    EV_INFO << "VeinsInetCar initialized: " << getFullPath() << endl;
}

void VeinsInetCar::handleMessage(cMessage *msg)
{
    // cars normally don’t process messages directly
    delete msg;
}
