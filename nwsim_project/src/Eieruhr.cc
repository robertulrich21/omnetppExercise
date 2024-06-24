/*
 * Eieruhr.cpp
 *
 *  Created on: May 23, 2024
 *      Author: dag
 */

#include "Eieruhr.h"
#include <vector>

Eieruhr::Eieruhr(){

    selfMessage = nullptr;

}

Eieruhr::~Eieruhr(){
    cancelAndDelete(selfMessage);
}

void Eieruhr::initialize(){
    measurementIntervall = par("measurementIntervall");
    selfMessage = new cMessage("selfMessage");
    scheduleAt(simTime(), selfMessage);
}

void Eieruhr::handleMessage(cMessage *msg){

    if (msg != selfMessage){
        EV << "Wrong message recieved!!";
        delete msg;
        return;
    }

    // send messages to all subscribed parties!!!!
    for(cGate *gate: observables){

        cMessage *sendMessage = new cMessage("Eieruhr", EIERUHR_KIND);
        sendDirect(sendMessage, gate);
    }

    // schedule to myself
    scheduleAt(simTime() +  measurementIntervall, msg);
}

void Eieruhr::registerGate(cGate *gate){
    observables.push_back(gate);
}
