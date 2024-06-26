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

package nwsim_project;

simple Junction
{
    parameters:
        @display("i=block/routing");
        volatile double intersectionCrossingDurationStreight @unit(s) = default(4s);
        volatile double intersectionCrossingDurationRight @unit(s) = default(4s);
        volatile double intersectionCrossingDurationLeft @unit(s) = default(6s);
        volatile double intersectionCrossingDurationTurn @unit(s) = default(6s);
        
        @signal[lengtVehicleQueueNorth](type=simtime_t);
        @signal[lengtVehicleQueueEast] (type=simtime_t);
        @signal[lengtVehicleQueueSouth](type=simtime_t);
        @signal[lengtVehicleQueueWest] (type=simtime_t);
        
        @statistic[averageLengthVehicleQueueNorth](title="averageLengthVehicleQueueNorth";source=lengtVehicleQueueNorth;record=mean);
        @statistic[averageLengthVehicleQueueEast] (title="averageLengthVehicleQueueEast" ;source=lengtVehicleQueueEast ;record=mean);
        @statistic[averageLengthVehicleQueueSouth](title="averageLengthVehicleQueueSouth";source=lengtVehicleQueueSouth;record=mean);
        @statistic[averageLengthVehicleQueueWest] (title="averageLengthVehicleQueueWest" ;source=lengtVehicleQueueWest ;record=mean);
    gates:
        inout north;
        inout east;
        inout south;
        inout west;
        
        input EieruhrIn @directIn;
}
