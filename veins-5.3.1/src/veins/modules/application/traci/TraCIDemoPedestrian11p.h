/*
 * TraCIDemoPedestrian11p.h
 *      Author: opp_env
 */

#ifndef SRC_VEINS_MODULES_APPLICATION_TRACI_TRACIDEMOPEDESTRIAN11P_H_
#define SRC_VEINS_MODULES_APPLICATION_TRACI_TRACIDEMOPEDESTRIAN11P_H_

#pragma once

#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/mobility/traci/TraCIPedestrianMobility.h"

namespace veins {

class TraCIDemoPedestrian11p : public DemoBaseApplLayer {
public:
    void initialize(int stage) override;
protected:
    virtual void onWSM(BaseFrame1609_4* wsm) override;
    virtual void onWSA(DemoServiceAdvertisment* wsa) override;
    virtual void handleSelfMsg(cMessage* msg) override;
    virtual void handlePositionUpdate(cObject* obj) override;

    TraCIPedestrianMobility* mobility = nullptr;
    std::string externalId;
};

} // namespace veins


#endif /* SRC_VEINS_MODULES_APPLICATION_TRACI_TRACIDEMOPEDESTRIAN11P_H_ */
