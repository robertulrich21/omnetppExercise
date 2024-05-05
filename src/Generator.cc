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

#include "Generator.h"

Generator::Generator() {
    selfMessage = nullptr;

}

Generator::~Generator() {
    cancelAndDelete(selfMessage);
}

void Generator::initialize(){

    WATCH(customer_count);

    // relocated spawn time distribution to ned file
    selfMessage = new cMessage("self Message",0);
    // scheduleAt(uniform((double)par("min_customer_spawn_time"),(double)(par("max_customer_spawn_time")),5),selfMessage);
    scheduleAt(simTime() + par("customer_spawn_time"), selfMessage);
}

void Generator::handleMessage(cMessage* msg){
    if(msg == selfMessage){
        // generate Customer
        Customer* cst = new Customer("customer",1);
        //generate items
        cst->setItems(intuniform((int)par("min_item_count"), (int)par("max_item_count")));
        cst->setWaitingTime(simTime());
        send(cst,gate("out"));
        EV << "Generated Customer with: " << cst->getItems() << " Items" <<std::endl;
        customer_count++;

        //Self message runs 1000 times
        if(customer_count >= (long)par("rounds")){
            return;
        }

        // shouldnt this be also rng channel 5?
        //simtime_t time = uniform((double)par("min_customer_spawn_time"),(double)(par("max_customer_spawn_time")));
        simtime_t time = par("customer_spawn_time");
        scheduleAt(simTime() + time, selfMessage);
        EV << "next customer appear in: " << time <<std::endl;


    }

}

