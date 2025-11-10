/*
 * TraCIPedestrianMobility.h
 *
 *  Created on: Oct 6, 2025
 *      Author: opp_env
 */

#ifndef SRC_VEINS_MODULES_MOBILITY_TRACI_TRACIPEDESTRIANMOBILITY_H_
#define SRC_VEINS_MODULES_MOBILITY_TRACI_TRACIPEDESTRIANMOBILITY_H_

#pragma once

#include <string>

#include "veins/base/modules/BaseMobility.h"
#include "veins/base/utils/Heading.h"
#include "veins/base/utils/Coord.h"

namespace veins {
    class VEINS_API TraCIPedestrianMobility : public BaseMobility {
    public:
        static const simsignal_t mobilityStateChangedSignal;
        //static const simsignal_t mobilityStateChangedDetailedSignal;

        TraCIPedestrianMobility()
            : BaseMobility()
            , isPreInitialized(false)
            , speed(-1)
        {
        }
        ~TraCIPedestrianMobility() override
        {
        }
        void initialize(int stage) override;
        void finish() override;

        void handleSelfMsg(cMessage* msg) override;
        virtual void preInitialize(std::string external_id, const Coord& position, std::string road_id = "", double speed = -1, double angle = 0);
        virtual void nextPosition(const Coord& position, std::string road_id = "", double speed = -1, double angle = 0);
        virtual void changePosition();
        virtual void setExternalId(std::string external_id)
        {
            this->external_id = external_id;
        }
        virtual std::string getExternalId() const
        {
            if (external_id == "") throw cRuntimeError("TraCIMobility::getExternalId called with no external_id set yet");
            return external_id;
        }
        virtual std::string getRoadId() const
        {
            if (road_id == "") throw cRuntimeError("TraCIMobility::getRoadId called with no road_id set yet");
            return road_id;
        }
        virtual double getSpeed() const
        {
            if (speed == -1) throw cRuntimeError("TraCIMobility::getSpeed called with no speed set yet");
            return speed;
        }
        virtual double getAngle() const
        {
            if (speed == -1) throw cRuntimeError("TraCIMobility::getAngle called with no angle set yet");
            return angle;
        }
        virtual Coord getCurrentPosition() const
        {
            if (road_id == "") throw cRuntimeError("TraCIMobility::getCurrentPosition called with no road_id set yet");
            return roadPosition;
        }
    
    protected:
        bool isPreInitialized;
        std::string external_id;
        std::string road_id;
        double speed;
        double angle;
        Coord roadPosition;
    };

}

#endif /* SRC_VEINS_MODULES_MOBILITY_TRACI_TRACIPEDESTRIANMOBILITY_H_ */
