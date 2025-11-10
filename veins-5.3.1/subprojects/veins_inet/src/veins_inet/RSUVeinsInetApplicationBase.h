/*
 * RSUVeinsInetApplicationBase.h
 *
 *  Created on: Oct 12, 2025
 *      Author: opp_env
 */

#ifndef VEINS_INET_RSUVeinsInetApplicationBase_H_
#define VEINS_INET_RSUVeinsInetApplicationBase_H_

#pragma once
#include "veins_inet/VeinsInetApplicationBase.h"
#include <vector>

namespace veins {

/**
 * A version of VeinsInetApplicationBase that works even if the node
 * doesn't have a VeinsInetMobility (e.g. RSUs with StationaryMobility).
 */
class RSUVeinsInetApplicationBase : public VeinsInetApplicationBase {
protected:
    inet::L3Address destAddressWifi;
    int localPortWifi;
    int destPortWifi;
    inet::UdpSocket socketWifi;

    std::vector<inet::L3Address> pedAddresses; 
    inet::Ipv4InterfaceData localAddress;
protected:
    virtual void initialize(int stage) override;
    virtual void handleStartOperation(inet::LifecycleOperation* operation) override;
    virtual void handleStopOperation(inet::LifecycleOperation* doneCallback) override;
    virtual void handleCrashOperation(inet::LifecycleOperation* doneCallback) override;

    virtual void handleMessageWhenUp(inet::cMessage* msg) override;
    virtual void sendPacket(std::unique_ptr<inet::Packet> pk) override;
};

} // namespace veins




#endif /* VEINS_INET_RSUVeinsInetApplicationBase_H_ */
