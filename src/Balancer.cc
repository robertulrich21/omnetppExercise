#include "Balancer.h"
#include <limits>

Balancer::Balancer() {
    // TODO Auto-generated constructor stub

}

Balancer::~Balancer() {
    // TODO Auto-generated destructor stub
}
void Balancer::initialize(){

}
void Balancer::handleMessage(cMessage *msg){
  int lowest_index = -1;
  int lowest_value = std::numeric_limits<int>::max();


  if((bool)par("use_random_strategy")){
      //send custoers random to cashiers
      int random = intuniform(0,(int)par("cashier_count")-1,0);
      send(msg,"out", random);

  }
  else{
   //Find lowest chashier queue
  for (int i = 0; i < (int)par("cashier_count");i++){
          Cashier* c = check_and_cast<Cashier *>(getSystemModule()->getSubmodule("cashiers",i));
          if (c->getQueueSize() < lowest_value){
              lowest_value = c->getQueueSize();
              lowest_index = i;
          }
      }

       send(msg, "out", lowest_index);
  }
}

