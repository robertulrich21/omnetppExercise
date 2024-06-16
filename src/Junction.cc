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

#include "Junction.h"
#include "Vehicle_m.h"
#include <string>
#include <queue>
#include <utility>

Define_Module(Junction);

// changes the from direction to facing direction
std::map<std::string, Junction::Direction> Junction::inGateToDirection = {
    {"north$i", SOUTH},
    {"east$i", WEST},
    {"south$i", NORTH},
    {"west$i", EAST},
};

std::map<Junction::Direction, std::string> Junction::directionToOutGate = {
    {NORTH, "north$o"},
    {EAST, "east$o"},
    {SOUTH, "south$o"},
    {WEST, "west$o"},
};

std::map<Junction::Direction, char> Junction::directionToChar = {
        {NORTH, 'N'},
        {EAST, 'E'},
        {SOUTH, 'S'},
        {WEST, 'W'},
};

std::map<char, Junction::Direction> Junction::charToDirection = {
        {'N', NORTH},
        {'E', EAST},
        {'S', SOUTH},
        {'W', WEST},
};

inline bool Junction::checkJunction(Direction direction, std::string junction){
    char charDir = directionToChar.at(direction);
    return junction[4] == charDir || junction[5] == charDir;
}

inline bool Junction::checkEndpoint(Direction direction, std::string endpoint){
    char charDir = directionToChar.at(direction);
    return endpoint[3] == charDir || endpoint[4] == charDir;
}

inline Junction::Direction Junction::getEndpointDirection(std::string endpoint){
    return charToDirection.at(endpoint[3]);
}


inline char Junction::getTurnDirection(Direction arrivalDirection, Direction turnDirection){
    int i = (4 + arrivalDirection - turnDirection) % 4;

    switch(i){
    case 0:
        return 'S'; // streight
    case 1:
        return 'R'; // right
    case 2:
        return 'B'; // backwards
    case 3:
        return 'L'; // left
    }
    return 0;
}

Junction::Direction Junction::getOutDirection(Vehicle *vehicle){
    cGate *arrivalGate = vehicle->getArrivalGate();
    if(!arrivalGate){
        EV << "Error arrival gate is null" << endl;
        return NORTH;
    }

    EV << "gateName:" << arrivalGate->getName() << endl;
    Direction arrivalDirection = inGateToDirection.at(std::string(arrivalGate->getName()));
    std::string destination = vehicle->getDstEndpoint();

    std::string junctionName = this->getName();

    Direction outDir;
    if(!checkJunction(arrivalDirection, junctionName) && checkEndpoint(arrivalDirection, destination)){
        // go streight if the target contains the the other direction DONE
        // one could check for the possibility to go left or right to get a better junction utilization.
        outDir = arrivalDirection;
    } else{
        // we can only go streight or backwards if we have a direct endpoint
        if(
                (checkEndpoint(turnRight(arrivalDirection, 3), destination) && checkJunction(turnRight(arrivalDirection, 3), junctionName)) ||
                (checkEndpoint(turnRight(arrivalDirection, 1), destination) && checkJunction(turnRight(arrivalDirection, 1), junctionName))
        ){
            outDir = getEndpointDirection(destination);
        }
        else {
            // turn left or right depending on what is relevant
            outDir = checkEndpoint(turnRight(arrivalDirection, 1), destination) ? turnRight(arrivalDirection, 1) : turnRight(arrivalDirection, 3);
        }
    }
    return outDir;
}

void Junction::updateLane(Direction direction){

    Direction inverseDirection = turnRight(direction, 2);

    if(vehiclequeue.at(direction).empty()){
        // notify other direction if not scheduled but vehicle available
        if(!laneMessage[inverseDirection]->isScheduled() && !vehiclequeue.at(inverseDirection).empty())
           scheduleAfter(0, laneMessage[inverseDirection]);
        return;
    }

    Vehicle *vehicle = vehiclequeue.at(direction).front();

    Direction outDir = getOutDirection(vehicle);
    switch(getTurnDirection(direction, outDir)){
    case 'L':
    case 'B':
        // if other lane is free otherwise block until awaken
        if(!laneMessage[inverseDirection]->isScheduled()){
            simtime_t intersectionCrossingDuration = 1; // TODO define this constant!!
            scheduleAfter(intersectionCrossingDuration, laneMessage[inverseDirection]);// for this to work laneMessage2 must be handled before laneMessage0
            scheduleAfter(intersectionCrossingDuration, laneMessage[direction]);
            vehiclequeue.at(direction).pop();
            sendDelayed(vehicle, intersectionCrossingDuration, directionToOutGate.at(outDir).c_str());
        }
        break;
    case 'R':
    case 'S':
        // default behaviour send selfMessage and send the Vehicle on its way
        simtime_t intersectionCrossingDuration = 1; // TODO define this constant!!
        scheduleAfter(intersectionCrossingDuration, laneMessage[direction]);
        vehiclequeue.at(direction).pop();
        sendDelayed(vehicle, intersectionCrossingDuration, directionToOutGate.at(outDir).c_str());
        break;
    }
}


Junction::Direction Junction::turnRight(Junction::Direction direction, int count){
    return static_cast<Junction::Direction>( (static_cast<int>(direction) + count) % 4);
}

Junction::Junction(){
    selfMessage = nullptr;
    for(int i = 0; i < 4; i++){
        laneMessage[i] = nullptr;
    }
    trafficLightUpdate = nullptr;
}

Junction::~Junction(){
    cancelAndDelete(selfMessage);
    for(int i = 0; i < 4; i++){
        cancelAndDelete(laneMessage[i]);
    }
    cancelAndDelete(trafficLightUpdate);
}

void Junction::initialize()
{
    selfMessage = new cMessage("selfMessage", SELF_MESSAGE_KIND);
    for(int i = 0; i < 4; i++){
        laneMessage[i] = new cMessage("laneMessage", SELF_MESSAGE_KIND);
    }
    trafficLightUpdate = new cMessage("trafficLightUpdate", SELF_MESSAGE_KIND);

    vehiclequeue.insert(std::pair<Direction, std::queue<Vehicle *>>(NORTH, vehiclequeueNorth));
    vehiclequeue.insert(std::pair<Direction, std::queue<Vehicle *>>(EAST, vehiclequeueEast));
    vehiclequeue.insert(std::pair<Direction, std::queue<Vehicle *>>(SOUTH, vehiclequeueSouth));
    vehiclequeue.insert(std::pair<Direction, std::queue<Vehicle *>>(WEST, vehiclequeueWest));

    isgreen = false;
    greenDir = false;
    scheduleAfter(0, trafficLightUpdate);
}

void Junction::handleMessage(cMessage* msg)
{
    if(msg->getKind() == SELF_MESSAGE_KIND){
        if(msg == trafficLightUpdate){
            if(isgreen){
                // turn light red, toggle greenDir and send for redPhase;
                greenDir = !greenDir;
                isgreen = false;
                scheduleAfter(10, msg);
            } else{
                // turn light green and send for greenphase
                isgreen = true;
                scheduleAfter(100, msg);
                if(!selfMessage->isScheduled()) scheduleAfter(0, selfMessage);
            }
            return;
        }

        if(msg == selfMessage && isgreen){
            if(greenDir){
                // notify north/south if required
                if(!laneMessage[NORTH]->isScheduled()) scheduleAfter(0, laneMessage[NORTH]);
                if(!laneMessage[SOUTH]->isScheduled()) scheduleAfter(0, laneMessage[SOUTH]);
            } else{
                // notify east/west if required
                if(!laneMessage[EAST]->isScheduled()) scheduleAfter(0, laneMessage[EAST]);
                if(!laneMessage[WEST]->isScheduled()) scheduleAfter(0, laneMessage[WEST]);
            }
            return;
        }

        // handle lane messages!! in many cases we can ignore them
        if(isgreen){
            if(greenDir){
                if(msg == laneMessage[NORTH]){
                    updateLane(NORTH);
                } else if(msg == laneMessage[SOUTH]){
                    updateLane(SOUTH);
                }
            } else {
                if(msg == laneMessage[EAST]){
                    updateLane(EAST);
                } else if(msg == laneMessage[WEST]){
                    updateLane(WEST);
                }
            }
        }
        return;

    }
    // insert new vehicle into corresponding queue
    Vehicle *vehicle = check_and_cast<Vehicle *>(msg);
    Direction inDir = inGateToDirection.at(std::string(vehicle->getArrivalGate()->getName()));
    vehiclequeue.at(inDir).push(vehicle);
    // notify lane that a vehicle has arrived

    if(!laneMessage[inDir]->isScheduled()){
        scheduleAfter(0, laneMessage[inDir]);
    }
}
