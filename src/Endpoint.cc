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

#include "Endpoint.h"
#include "Vehicle_m.h"

Define_Module(Endpoint);

unsigned int Endpoint::vehicleCounter = 0;
std::map<char, std::string> Endpoint::directionMapping = {
    {'N', "S"},
    {'S', "N"},
    {'E', "W"},
    {'W', "E"},
};

Endpoint::~Endpoint()
{
    cancelAndDelete(spawnTimer);
}

void Endpoint::initialize()
{
    spawnTimer = new cMessage("spawnTimer");
    simtime_t spawnInterval = par("spawnInterval");
    simtime_t spawnOffset = par("spawnOffset");
    if (spawnInterval > 0) {
        ASSERT(spawnOffset >= 0);
        scheduleAt(simTime() + spawnOffset, spawnTimer);
    }

    totalTransitTime = registerSignal("totalTransitTime");
    queueWaitingTime = registerSignal("queueWaitingTime");
    drivingTime =      registerSignal("drivingTime");
    hopCount =         registerSignal("hopCount");
}

void Endpoint::handleMessage(cMessage* msg)
{
    if (msg == spawnTimer) {
        spawnVehicle();
        simtime_t spawnInterval = par("spawnInterval");
        ASSERT(spawnInterval > 0);
        scheduleAt(simTime() + spawnInterval, spawnTimer);
    }
    else if (dynamic_cast<Vehicle*>(msg)) {
        Vehicle* veh = dynamic_cast<Vehicle*>(msg);
        ASSERT(std::string(veh->getDstEndpoint()) == std::string(getName()));
        veh->setTotalTransitTime(simTime() - veh->getEnterTime());

        emit(totalTransitTime, veh->getTotalTransitTime());
        emit(queueWaitingTime, veh->getQueueWaitingTime());
        emit(drivingTime,      veh->getDrivingTime());
        emit(hopCount,         veh->getHopCount());

        delete msg;
    }
}

void Endpoint::spawnVehicle()
{
    Vehicle* veh = new Vehicle();
    veh->setVehNumber(vehicleCounter++);
    veh->setSrcEndpoint(getName());
    std::string dstEndpoint = getDstEndpoint();
    veh->setDstEndpoint(dstEndpoint.c_str());


    veh->setQueueExitTime(simTime());
    veh->setEnterTime(simTime());
    veh->setHopCount(0);
    veh->setQueueWaitingTime(0);
    veh->setDrivingTime(0);

    veh->setName(std::string("for " + dstEndpoint).c_str());

    send(veh, "conn$o");
}

std::string Endpoint::getDstEndpoint()
{
#define COMPLEX_ROUTING
#ifndef COMPLEX_ROUTING
    std::string myName = getName();
    std::string dstName = "End" + directionMapping.at(myName.at(3));
    dstName.push_back(myName.at(4));
    ASSERT(myName.size() == dstName.size());
    EV << dstName << endl;
    return dstName;

#else /* COMPLEX_ROUTING */


    std::string myName = getName();
    int dir = intuniform(0, 3);
    std::string dirchar;
    switch(dir){
    case 0:
        dirchar = "N";
        break;
    case 1:
        dirchar = "E";
        break;
    case 2:
        dirchar = "S";
        break;
    case 3:
        dirchar = "W";
        break;
    }
    int dir2 = (dir + (intuniform(0, 1) ? 1 : 3)) % 4;
    std::string dir2char;
    switch(dir2){
        case 0:
            dir2char = "N";//
            break;
        case 1:
            dir2char = "E";
            break;
        case 2:
            dir2char = "S";
            break;
        case 3:
            dir2char = "W";
            break;
        }
    std::string dstName = "End" + dirchar + dir2char;
    //EV << dstName << endl;
    ASSERT(myName.size() == dstName.size());
    return dstName;


#endif /* COMPLEX_ROUTING */
}
