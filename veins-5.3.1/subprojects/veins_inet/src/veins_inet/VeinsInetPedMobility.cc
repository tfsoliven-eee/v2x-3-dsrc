//
// Copyright (C) 2006-2018 Christoph Sommer <sommer@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

//
// Veins Mobility module for the INET Framework
// Based on inet::MovingMobilityBase of INET Framework v4.0.0
//

#include "veins_inet/VeinsInetPedMobility.h"

#include "inet/common/INETMath.h"
#include "inet/common/Units.h"
#include "inet/common/geometry/common/GeographicCoordinateSystem.h"

namespace veins {

using namespace inet::units::values;

Register_Class(VeinsInetPedMobility);

VeinsInetPedMobility::VeinsInetPedMobility()
{
}

VeinsInetPedMobility::~VeinsInetPedMobility()
{
    delete personCommandInterface;
}

void VeinsInetPedMobility::preInitialize(std::string external_id, const inet::Coord& position, std::string road_id, double speed, double angle)
{
    Enter_Method_Silent();
    this->external_id = external_id;
    lastPosition = position;
    lastVelocity = inet::Coord(cos(angle), -sin(angle)) * speed;
    lastOrientation = inet::Quaternion(inet::EulerAngles(rad(-angle), rad(0.0), rad(0.0)));
}

void VeinsInetPedMobility::initialize(int stage)
{
    MobilityBase::initialize(stage);

    // We patch the OMNeT++ Display String to set the initial position. Make sure this works.
    ASSERT(hasPar("initFromDisplayString") && par("initFromDisplayString"));
}

void VeinsInetPedMobility::nextPosition(const inet::Coord& position, std::string road_id, double speed, double angle)
{
    Enter_Method_Silent();

    lastPosition = position;
    lastVelocity = inet::Coord(cos(angle), -sin(angle)) * speed;
    lastOrientation = inet::Quaternion(inet::EulerAngles(rad(-angle), rad(0.0), rad(0.0)));

    // Update display string to show node is getting updates
    auto hostMod = getParentModule();
    if (std::string(hostMod->getDisplayString().getTagArg("veins", 0)) == ". ") {
        hostMod->getDisplayString().setTagArg("veins", 0, " .");
    }
    else {
        hostMod->getDisplayString().setTagArg("veins", 0, ". ");
    }

    emitMobilityStateChangedSignal();
}

#if INET_VERSION >= 0x0403
const inet::Coord& VeinsInetPedMobility::getCurrentPosition()
{
    return lastPosition;
}

const inet::Coord& VeinsInetPedMobility::getCurrentVelocity()
{
    return lastVelocity;
}

const inet::Coord& VeinsInetPedMobility::getCurrentAcceleration()
{
    throw cRuntimeError("Invalid operation");
}

const inet::Quaternion& VeinsInetPedMobility::getCurrentAngularPosition()
{
    return lastOrientation;
}

const inet::Quaternion& VeinsInetPedMobility::getCurrentAngularVelocity()
{
    return lastAngularVelocity;
}

const inet::Quaternion& VeinsInetPedMobility::getCurrentAngularAcceleration()
{
    throw cRuntimeError("Invalid operation");
}
#else

inet::Coord VeinsInetPedMobility::getCurrentPosition()
{
    return lastPosition;
}

inet::Coord VeinsInetPedMobility::getCurrentVelocity()
{
    return lastVelocity;
}

inet::Coord VeinsInetPedMobility::getCurrentAcceleration()
{
    throw cRuntimeError("Invalid operation");
}

inet::Quaternion VeinsInetPedMobility::getCurrentAngularPosition()
{
    return lastOrientation;
}

inet::Quaternion VeinsInetPedMobility::getCurrentAngularVelocity()
{
    return lastAngularVelocity;
}

inet::Quaternion VeinsInetPedMobility::getCurrentAngularAcceleration()
{
    throw cRuntimeError("Invalid operation");
}
#endif
void VeinsInetPedMobility::setInitialPosition()
{
    subjectModule->getDisplayString().setTagArg("p", 0, lastPosition.x);
    subjectModule->getDisplayString().setTagArg("p", 1, lastPosition.y);
    MobilityBase::setInitialPosition();
}

void VeinsInetPedMobility::handleSelfMessage(cMessage* message)
{
}

std::string VeinsInetPedMobility::getExternalId() const
{
    if (external_id == "") throw cRuntimeError("TraCIMobility::getExternalId called with no external_id set yet");
    return external_id;
}

TraCIScenarioManager* VeinsInetPedMobility::getManager() const
{
    if (!manager) manager = TraCIScenarioManagerAccess().get();
    return manager;
}

TraCICommandInterface* VeinsInetPedMobility::getCommandInterface() const
{
    if (!commandInterface) commandInterface = getManager()->getCommandInterface();
    return commandInterface;
}

TraCICommandInterface::Person* VeinsInetPedMobility::getPersonCommandInterface() const
{
    if (!personCommandInterface) personCommandInterface = new TraCICommandInterface::Person(getCommandInterface()->person(getExternalId()));
    return personCommandInterface;
}

} // namespace veins
