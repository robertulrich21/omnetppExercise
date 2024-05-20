
#include <omnetpp.h>
#include "TracedMessage_m.h"

using namespace omnetpp;

#define REQUEST_KIND 195

class Producer : public cSimpleModule {
public:
    Producer();
    ~Producer();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

private:
    cMessage *selfMessage;
};

Define_Module(Producer);

Producer::Producer()
{
    selfMessage = nullptr;
}

Producer::~Producer()
{
    cancelAndDelete(selfMessage);
}

void Producer::initialize()
{
    selfMessage = new cMessage("delayMessage");
    scheduleAt(simTime(), selfMessage);
}

void Producer::handleMessage(cMessage *msg)
{
    if (msg != selfMessage){
        EV << "Wrong message recieved!!";
        delete msg;
        return;
    }

    TracedMessage *produced = new TracedMessage();
    send(produced, gate("out"));

    simtime_t interArrivalTime = par("interArrivalTime");
    scheduleAt(simTime() + interArrivalTime, msg);
}





class Queue : public cSimpleModule {
public:
    Queue();
    ~Queue();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    bool isEmpty();

private:
    cQueue *queue;
    bool requested = false;
};

Define_Module(Queue);

Queue::Queue()
{
    queue = nullptr;
}

Queue::~Queue()
{
    while (!queue->isEmpty()){
        delete queue->pop();
    }
    delete queue;

}

void Queue::initialize()
{
    queue = new cQueue();
}

void Queue::handleMessage(cMessage *msg)
{
    if(msg->getKind() == REQUEST_KIND){
        if(queue->isEmpty()){
            requested = true;
        } else{
            cMessage *mmsg = check_and_cast<cMessage *>(queue->pop());
            send(mmsg, gate("out"));
        }
        delete msg;
        return;
    }


    TracedMessage * tracedMsg = check_and_cast<TracedMessage *>(msg);
    queue->insert(tracedMsg);

    if(requested){
        requested = false;

        cMessage *mmsg = check_and_cast<cMessage *>(queue->pop());
        send(mmsg, gate("out"));

    }
    // do stuff for enabling the consuming eg
}

bool Queue::isEmpty()
{
    return queue->isEmpty();

}


class ServiceUnit : public cSimpleModule {
public:
    ServiceUnit();
    ~ServiceUnit();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

private:
    TracedMessage *servicedMessage;
    cMessage *delayMessage;
};

Define_Module(ServiceUnit);

ServiceUnit::ServiceUnit()
{
    servicedMessage = nullptr;
    delayMessage = nullptr;
}

ServiceUnit::~ServiceUnit()
{
    cancelAndDelete(servicedMessage);
    cancelAndDelete(delayMessage);
}

void ServiceUnit::initialize()
{
    delayMessage = new cMessage("delayMessage");
    cMessage *request = new cMessage("", REQUEST_KIND);
    send(request, gate("requestOut"));
}

void ServiceUnit::handleMessage(cMessage *msg)
{
    if(msg == delayMessage){
        send(servicedMessage, gate("out"));

        cMessage *request = new cMessage("", REQUEST_KIND);
        send(request, gate("requestOut"));
        return;
    }

    simtime_t interServiceTime = par("interServiceTime");

    servicedMessage = check_and_cast<TracedMessage *>(msg);
    scheduleAt(simTime() + interServiceTime, delayMessage);


}



class Sink : public cSimpleModule {
public:
    //virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Sink)

void Sink::handleMessage(cMessage *msg){
    TracedMessage *mmsg = check_and_cast<TracedMessage *>(msg);

    delete mmsg;
}

