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

#ifndef SRC_GENERATOR_H_
#define SRC_GENERATOR_H_
#include <omnetpp.h>
#include "Customer_m.h"
using namespace omnetpp;

class Generator : public cSimpleModule {
public:
    Generator();
    virtual ~Generator();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

private:
    long customer_count = 0;
};
Define_Module(Generator);

#endif /* SRC_GENERATOR_H_ */
