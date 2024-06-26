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

#ifndef __CROSSTRAFFIC_ENDPOINT_H_
#define __CROSSTRAFFIC_ENDPOINT_H_

#include <omnetpp.h>

using namespace omnetpp;

class Endpoint : public cSimpleModule {
public:
    virtual ~Endpoint();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage* msg);

private:
    cMessage* spawnTimer = nullptr;
    static unsigned int vehicleCounter;
    static std::map<char, std::string> directionMapping;

    void spawnVehicle();
    std::string getDstEndpoint();

    simsignal_t totalTransitTime;
    simsignal_t queueWaitingTime;
    simsignal_t drivingTime;
    simsignal_t hopCount;
};

#endif
