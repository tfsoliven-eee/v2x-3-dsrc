//
// Copyright (C) 2018 Christoph Sommer <sommer@ccs-labs.org>
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

#include "veins_inet/VeinsInetSampleApplication.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"

#include "veins_inet/VeinsInetSampleMessage_m.h"

using namespace inet;

Define_Module(VeinsInetSampleApplication);

VeinsInetSampleApplication::VeinsInetSampleApplication()
{
}

void VeinsInetSampleApplication::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        sendInterval = par("sendInterval");
    }
}

bool VeinsInetSampleApplication::startApplication()
{
    EV_INFO << "[VeinsInetSampleApplication::startApplication]\n";

    //auto payload = makeShared<VeinsInetSampleMessage>();
    //payload->setChunkLength(B(100));
    
    //payload->setExternalId(mobility->getExternalId().c_str());
    //payload->setRoadId(traciVehicle->getRoadId().c_str());
    //payload->setSpeed(traciVehicle->getSpeed());
    //timestampPayload(payload);

    //auto packet = createPacket("Jeep_Init_Broadcast");
    //packet->insertAtBack(payload);
    //sendPacket(std::move(packet));

    //L3Address group11p = L3AddressResolver().resolve("224.0.0.1");
    //socket.sendTo(packet.release(), group11p, portNumber);

    sendInterval = par("sendInterval");
    auto callback = [this]() { sendPeriodicPacket(); };
    veins::TimerSpecification spec(callback);
    spec.interval(sendInterval).absoluteStart(simTime() + sendInterval);
    sendTimer = timerManager.create(spec, "sendTimer"); 


    return true;
}

bool VeinsInetSampleApplication::stopApplication()
{
    timerManager.cancel(sendTimer);
    return true;
}

VeinsInetSampleApplication::~VeinsInetSampleApplication()
{
}

void VeinsInetSampleApplication::sendPeriodicPacket()
{

    EV_INFO << "[VeinsInetSampleApplication::sendPeriodicPacket]" << endl;

    getParentModule()->getDisplayString().setTagArg("i", 1, "green");

    auto payload = makeShared<VeinsInetSampleMessage>();
    payload->setChunkLength(B(100));
    payload->setExternalId(mobility->getExternalId().c_str());
    payload->setRoadId(traciVehicle->getRoadId().c_str());
    payload->setSpeed(traciVehicle->getSpeed());
    payload->setSenderX(mobility->getCurrentPosition().x);
    payload->setSenderY(mobility->getCurrentPosition().y);
    timestampPayload(payload);

    auto packet = createPacket("Jeep_Update");
    packet->insertAtBack(payload);
    sendPacket(std::move(packet));
}