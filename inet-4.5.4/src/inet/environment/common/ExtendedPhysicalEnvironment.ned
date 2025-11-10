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

package inet.environment.common;

import inet.environment.contract.IGround;
import inet.environment.contract.IObjectCache;
import inet.environment.contract.IPhysicalEnvironment;

module ExtendedPhysicalEnvironment like IPhysicalEnvironment
{
    parameters:
        string coordinateSystemModule = default(""); // module path of the geographic coordinate system module
        double temperature @unit(K) = default(293K);
        double spaceMinX @unit(m) = default(nan m); // lower bound of x coordinates in space, NaN means determined by physical objects, NaN by default
        double spaceMinY @unit(m) = default(nan m); // lower bound of y coordinates in space, NaN means determined by physical objects, NaN by default
        double spaceMinZ @unit(m) = default(nan m); // lower bound of z coordinates in space, NaN means determined by physical objects, NaN by default
        double spaceMaxX @unit(m) = default(nan m); // upper bound of x coordinates in space, NaN means determined by physical objects, NaN by default
        double spaceMaxY @unit(m) = default(nan m); // upper bound of y coordinates in space, NaN means determined by physical objects, NaN by default
        double spaceMaxZ @unit(m) = default(nan m); // upper bound of z coordinates in space, NaN means determined by physical objects, NaN by default
        xml config = default(xml("<config/>"));
        @display("i=misc/town");
        @class(ExtendedPhysicalEnvironment);

    submodules:
        objectCache: <default("")> like IObjectCache if typename != "" {
            parameters:
                @display("p=100,100");
        }
        ground: <default("")> like IGround if typename != "" {
            parameters:
                @display("p=100,200");
        }
}