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

#ifndef __CROSSTRAFFIC_JUNCTION_H_
#define __CROSSTRAFFIC_JUNCTION_H_

#include <omnetpp.h>
#include "Vehicle_m.h"
#include <queue>

#define SELF_MESSAGE_KIND 135

using namespace omnetpp;

class Junction : public cSimpleModule {
public:
    Junction();
    ~Junction();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage* msg);
    virtual void refreshDisplay() const override;
private:
    enum Direction {
        NORTH = 0,
        EAST = 1,
        SOUTH = 2,
        WEST = 3
    };

    static std::map<std::string, Direction> inGateToDirection;
    static std::map<Direction, std::string> directionToOutGate;
    static std::map<Direction, char> directionToChar;
    static std::map<char, Direction> charToDirection;

    std::queue<Vehicle *> vehiclequeueNorth;
    std::queue<Vehicle *> vehiclequeueEast;
    std::queue<Vehicle *> vehiclequeueSouth;
    std::queue<Vehicle *> vehiclequeueWest;
    std::map<Direction, std::queue<Vehicle *>> vehiclequeue;

    simsignal_t lengtVehicleQueueNorth;
    simsignal_t lengtVehicleQueueEast;
    simsignal_t lengtVehicleQueueSouth;
    simsignal_t lengtVehicleQueueWest;

    cMessage *selfMessage;
    cMessage *laneMessage[4];

    cMessage *trafficLightUpdate;

    bool greenDir;// true north/south, false east/west
    bool isgreen;// when false no car can enter the intersection.

    bool checkJunction(Direction direction, std::string junction);
    bool checkEndpoint(Direction direction, std::string endpoint);
    bool checkEndpointDirection(Direction direction, std::string endpoint);
    Direction getEndpointDirection(std::string endpoint);
    char getTurnDirection(Direction arrivalDirection, Direction turnDirection);
    Direction getOutDirection(Vehicle *vehicle);
    void updateLane(Direction direction);
    Direction turnRight(Direction direction, int count);

};

#endif
