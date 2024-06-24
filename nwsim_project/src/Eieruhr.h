/*
 * Eieruhr.h
 *
 *  Created on: May 23, 2024
 *      Author: dag
 */

#ifndef EIERUHR_H_
#define EIERUHR_H_

#include <omnetpp.h>
#include <vector>

#define EIERUHR_KIND 196
#define GET_EIERUHR() check_and_cast<Eieruhr *>(getSystemModule()->getSubmodule("eieruhr"))

using namespace omnetpp;

class Eieruhr : public cSimpleModule {
public:
    Eieruhr();
    ~Eieruhr();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    void registerGate(cGate *gate);



private:
    std::vector<cGate *> observables;
    cMessage *selfMessage;
    simtime_t measurementIntervall;
};

Define_Module(Eieruhr)

#endif /* EIERUHR_H_ */
