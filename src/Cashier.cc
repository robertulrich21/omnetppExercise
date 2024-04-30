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

#include "Cashier.h"

Cashier::Cashier() {


}

Cashier::~Cashier() {
    // TODO Auto-generated destructor stub
}

void Cashier::initialize() {

    queue = new cQueue();
    selfMessage = new cMessage("self Message", 0);

    //init signals for statistic
    finished_signal = registerSignal("customerFinished");
    update_queue = registerSignal("updateQueue");
    update_serviceTime = registerSignal("updateServiceTime");
    update_idleTime = registerSignal("updateIdleTime");

    //variables to determine if cashier is in idle or not
    isIdle = true;
    startIdle = simTime();
    startService = simTime();

    scheduleAt(0.0, selfMessage);


}
void Cashier::handleMessage(cMessage *msg){
    // if msg is a self message, update customer queue and stats
    if (msg->getKind() == 0){
        if(queue->getLength() > 0){
            if (isIdle){
                isIdle = false;
                emit(update_idleTime,simTime()-startIdle);
                startService = simTime();

            }
            //get first customer in queue
            Customer * cst = check_and_cast<Customer *>(queue->get(0));

            //take item from customer
            if(cst->getItems() >0){
                cst->setItems(cst->getItems()-1);
                cMessage* m = new cMessage(*selfMessage);
                scheduleAt(simTime()+uniform((simtime_t)par("min_time_per_item"),(simtime_t)par("max_time_per_item"),3),m);
            }
            else{
                // if no item left, update stats
                bubble("Customer finished");

                queue->pop();

                simtime_t waitingTime =simTime()-cst->getWaitingTime();
                emit(finished_signal,(waitingTime));
                emit(update_queue, queue->length());

                //cast self message
                cMessage* m = new cMessage(*selfMessage);
                scheduleAt(simTime(),m);
            }
        }
        //If the customer queue is empty go into idle mode
        else{
            if(isIdle == false){
                isIdle = true;
                emit(update_serviceTime,simTime()-startService);
                startIdle = simTime();
            }
        }

    }
    // If message comes from balancer
    else if(msg->getKind() == 1){

        Customer* cst = check_and_cast<Customer *>(msg);
        if(queue->getLength() == 0){
            cMessage* m = new cMessage(*selfMessage);
            scheduleAt(simTime()+uniform((double)par("min_time_per_item"),(double)par("max_time_per_item"),2),m);
        }
        queue->insert(cst);
        emit(update_queue, queue->length());

    }

}
int Cashier::getQueueSize(){
    return queue->getLength();
}
void Cashier::refreshDisplay() const {
    char buf[20];
    sprintf(buf,"queue size: %i",queue->getLength());
    getDisplayString().setTagArg("t", 0, buf);
}

