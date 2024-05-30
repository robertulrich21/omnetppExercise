
#include <omnetpp.h>
#include "TracedMessage_m.h"
#include <tuple>
#include <string>
#include <vector>
#include <deque>
#include "Eieruhr.h"

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
    EV << interArrivalTime<<endl;

    scheduleAt(simTime() + interArrivalTime, msg);
}





class Queue : public cSimpleModule {
public:
    Queue();
    ~Queue();
    virtual void initialize() override;
    virtual void finish() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
    bool isEmpty();

private:
    std::deque<std::tuple<TracedMessage *, simtime_t>> queue;
    bool requested = false;
    simtime_t lastMessageArrivalTime;
    simtime_t queueFillStatusChangedTime;

    simsignal_t queueInterArrivalTime;
    simsignal_t queueWaitingTime;
    simsignal_t queueFillLevel;

    simsignal_t queueEmptyTime;
    simsignal_t queueNonEmptyTime;
};

Define_Module(Queue);

Queue::Queue()
{
}

Queue::~Queue()
{
    while (!queue.empty()){
        std::tuple<TracedMessage *, simtime_t> tuple = queue.front();
        delete std::get<TracedMessage *>(tuple);
        queue.pop_front();
    }
    queue.clear();

}

void Queue::initialize()
{
    lastMessageArrivalTime = -1;
    queueFillStatusChangedTime = simTime();

    queueInterArrivalTime = registerSignal("queueInterArrivalTime");
    queueWaitingTime = registerSignal("queueWaitingTime");
    queueFillLevel = registerSignal("queueFillLevel");


    queueEmptyTime = registerSignal("queueEmptyTime");
    queueNonEmptyTime = registerSignal("queueNonEmptyTime");

    GET_EIERUHR()->registerGate(this->gate("eieruhrIn"));

    queue.clear();


}

void Queue::finish(){
    // add the missing queue empty time
    emit(queue.empty() ? queueEmptyTime : queueNonEmptyTime, simTime() - queueFillStatusChangedTime);

}

void Queue::handleMessage(cMessage *msg)
{
    // handle the regular Message from the Eieruhr
    if(msg->arrivedOn("eieruhrIn") && msg->getKind() == EIERUHR_KIND){

        emit(queueFillLevel, queue.size());
        delete msg;
        return;
    }

    if(msg->getKind() == REQUEST_KIND){
        if(queue.empty()){
            requested = true;
        } else{
            //TODO Duplicate code!!!
            std::tuple<TracedMessage *, simtime_t> messageWithTime = queue.front();
            queue.pop_front();
            TracedMessage *mmsg = std::get<0>(messageWithTime);
            // calculate waiting time
            simtime_t queueEnterTime = std::get<1>(messageWithTime);
            simtime_t queueLeaveTime = simTime();
            EV << "queueWaitingTime" << queueLeaveTime - queueEnterTime << " enter: "<< queueEnterTime << " leave: " << queueLeaveTime;
            emit(queueWaitingTime, queueLeaveTime - queueEnterTime);

            send(mmsg, gate("out"));

            //if queue is empty
            if (queue.empty()){
            // emit time that was empty
            // save timestamp for full duration

                emit(queueNonEmptyTime, simTime() - queueFillStatusChangedTime);
                queueFillStatusChangedTime = simTime();
            }
        }
        delete msg;
        return;
    }


    TracedMessage * tracedMsg = check_and_cast<TracedMessage *>(msg);

    // record inter arrival time
    simtime_t currentMessageArrivalTime = tracedMsg->getArrivalTime();
    if (lastMessageArrivalTime != -1){
        emit(queueInterArrivalTime, currentMessageArrivalTime - lastMessageArrivalTime);
    }
    lastMessageArrivalTime = currentMessageArrivalTime;


    queue.push_back(std::tuple<TracedMessage *, simtime_t>(tracedMsg, currentMessageArrivalTime));

    if(requested){
        requested = false;

        std::tuple<TracedMessage *, simtime_t> messageWithTime = queue.front();

        queue.pop_front();
        TracedMessage *mmsg = std::get<0>(messageWithTime);

        // calculate waiting time
        simtime_t queueEnterTime = std::get<1>(messageWithTime);
        simtime_t queueLeaveTime = simTime();
        EV << "queueWaitingTime" << queueLeaveTime - queueEnterTime;
        emit(queueWaitingTime, queueLeaveTime - queueEnterTime);

        send(mmsg, gate("out"));
    } else if(queue.size() == 1) {
        // the queue was empty but is not anymore
        emit(queueEmptyTime, simTime() - queueFillStatusChangedTime);
        queueFillStatusChangedTime = simTime();

    }
}

void Queue::refreshDisplay () const{
    char buf[40];
    sprintf(buf, "Queue length: %lu", queue.size());
    getDisplayString().setTagArg("t", 0, buf);
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
    simtime_t messageArrivalTime;

    simsignal_t serviceUnitWaitingTime;
    simsignal_t serviceUnitFillLevel;
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
    serviceUnitWaitingTime = registerSignal("serviceUnitWaitingTime");
    serviceUnitFillLevel = registerSignal("serviceUnitFillLevel");

    GET_EIERUHR()->registerGate(this->gate("eieruhrIn"));
}

void ServiceUnit::handleMessage(cMessage *msg)
{
    // handle the regular Message from the Eieruhr
    if(msg->arrivedOn("eieruhrIn") && msg->getKind() == EIERUHR_KIND){

        emit(serviceUnitFillLevel, (unsigned int)(delayMessage->isScheduled() ? 1 : 0));
        delete msg;
        return;
    }

    if(msg == delayMessage){
        send(servicedMessage, gate("out"));
        servicedMessage = nullptr;

        emit(serviceUnitWaitingTime, msg->getArrivalTime() - messageArrivalTime);

        cMessage *request = new cMessage("", REQUEST_KIND);
        send(request, gate("requestOut"));
        return;
    }

    simtime_t interServiceTime = par("interServiceTime");

    messageArrivalTime = msg->getArrivalTime();

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
