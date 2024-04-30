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
      int random = intuniform(0,(int)par("cashier_count")-1);
      send(msg,"out", random);

  }
  else{

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

