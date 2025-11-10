/*
 * PedVeinsInetSampleApplication.h
 *
 *  Created on: Oct 22, 2025
 *      Author: opp_env
 */

#ifndef VEINS_INET_PEDVEINSINETSAMPLEAPPLICATION_H_
#define VEINS_INET_PEDVEINSINETSAMPLEAPPLICATION_H_

#pragma once

#include "veins_inet/veins_inet.h"

#include "veins_inet/PedVeinsInetApplicationBase.h"

class VEINS_INET_API PedVeinsInetSampleApplication : public veins::PedVeinsInetApplicationBase {
protected:
    bool haveForwarded = false;
    veins::TimerManager::TimerHandle sendTimer;

protected:
    virtual void initialize(int stage) override;
    virtual bool startApplication() override;
    virtual bool stopApplication() override;
    virtual int findClosestRSU() override; 
    virtual void sendPeriodicPacket();
    virtual void sendPacket(std::unique_ptr<inet::Packet> pk) override;


public:
    PedVeinsInetSampleApplication();
    ~PedVeinsInetSampleApplication();
};



#endif /* VEINS_INET_PEDVEINSINETSAMPLEAPPLICATION_H_ */
