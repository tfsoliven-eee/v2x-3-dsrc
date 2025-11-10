/*
 * PedVeinsInetApplicationBase.h
 *
 *  Created on: Oct 23, 2025
 *      Author: opp_env
 */

#ifndef VEINS_INET_PEDVEINSINETAPPLICATIONBASE_H_
#define VEINS_INET_PEDVEINSINETAPPLICATIONBASE_H_

#pragma once
#include "veins_inet/VeinsInetApplicationBase.h"
#include "veins_inet/VeinsInetPedMobility.h"

namespace veins {

/**
 * A version of VeinsInetApplicationBase that works even if the node
 * doesn't have a VeinsInetMobility (e.g. RSUs with StationaryMobility).
 */
class PedVeinsInetApplicationBase : public VeinsInetApplicationBase {
protected:
    veins::VeinsInetPedMobility* mobilityPed;
    veins::TraCICommandInterface::Person* traciPed;
protected:
    virtual void handleStartOperation(inet::LifecycleOperation* operation) override;
};

} // namespace veins

#endif /* VEINS_INET_PEDVEINSINETAPPLICATIONBASE_H_ */
