//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

import inet.physicallayer*.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;


import org.car2x.veins.subprojects.veins_inet.VeinsInetManager;
import inet.visualizer*.common.IntegratedVisualizer;

import inet.environment.common.ExtendedPhysicalEnvironment;
import org.car2x.veins.visualizer.roads.RoadsCanvasVisualizer;
import org.car2x.veins.visualizer.roads.RoadsOsgVisualizer;

import org.car2x.veins.subprojects.veins_inet.VeinsInetJeep;
import org.car2x.veins.subprojects.veins_inet.VeinsInetRSU;
import org.car2x.veins.subprojects.veins_inet.VeinsInetPedestrian;
import org.car2x.veins.subprojects.veins_inet.VeinsInetCar;

network draft
{
    parameters:
        bool useOsg = default(false);
        @display("bgb=319,384");
    submodules:
        configurator: Ipv4NetworkConfigurator;
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=64,224");
        }
        radioMediumWifi: Ieee80211ScalarRadioMedium {
            @display("p=64,224");
        }
        manager: VeinsInetManager {
            @display("p=192,320");
        }
        visualizer: IntegratedVisualizer {
            @display("p=64,320");
        }
        physicalEnvironment: ExtendedPhysicalEnvironment {
            @display("p=192,224");
        }
        roadsCanvasVisualizer: RoadsCanvasVisualizer {
            @display("p=64,416");
        }
        roadsOsgVisualizer: RoadsOsgVisualizer if useOsg {
            @display("p=192,416");
        }
        jeep[0]: VeinsInetJeep;
        rsu[6]: VeinsInetRSU {
            @display("i=veins/sign/yellowdiamond;is=vs");
        }
        ped[0]: VeinsInetPedestrian {
            @display("i=veins/node/pedestrian;is=vs");
        }
        car[0]: VeinsInetCar;
}
