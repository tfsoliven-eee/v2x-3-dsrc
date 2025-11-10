//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
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

#include "veins/modules/application/traci/TraCIDemoRSU11p.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

Define_Module(veins::TraCIDemoRSU11p);

void TraCIDemoRSU11p::onWSA(DemoServiceAdvertisment* wsa) {
    if (wsa->getPsid() == 42) {         // Listens for service advertisements (but only reacts to PSID 42).
        mac->changeServiceChannel(static_cast<Channel>(wsa->getTargetChannel()));
    }
}

void TraCIDemoRSU11p::onWSM(BaseFrame1609_4* frame) {
    auto* wsm = check_and_cast<TraCIDemo11pMessage*>(frame);

    EV_INFO << "RSU " << myId 
            << " received WSM from node " << wsm->getSenderAddress()
            << " with data: " << wsm->getRoadID() << endl;

    // Rebroadcast to everyone (vehicles + pedestrians)
    auto* rebroadcast = wsm->dup();
    sendDelayedDown(rebroadcast, 0.5 + uniform(0.01, 0.1));

    EV_INFO << "RSU " << myId 
        << " rebroadcasted WSM with data: " << wsm->getRoadID() << endl;

}
