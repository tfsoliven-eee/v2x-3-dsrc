/*
 * VeinsInetCar.h
 *
 *  Created on: Nov 2, 2025
 *      Author: opp_env
 */

#ifndef __ORG_CAR2X_VEINS_SUBPROJECTS_VEINS_INET_VEINSINETCAR_H
#define __ORG_CAR2X_VEINS_SUBPROJECTS_VEINS_INET_VEINSINETCAR_H

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Empty module class for VeinsInetCar.
 * VeinsInetManager dynamically creates instances of this class.
 * You can extend this later (e.g. to attach applications or sensors).
 */
class VeinsInetCar : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif
