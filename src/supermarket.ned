
simple Balancer{
    parameters:
        @display("i=block/dispatch");
		int cashier_count;
		bool use_random_strategy = default(false); 
	
	gates:
	    input inp;
	    output out[];
	    
	    	    
}
simple Generator{
	parameters:	    
	    int min_item_count = default(1);
        int max_item_count = default(25);
      	double min_customer_spawn_time = default(5.0);
      	double max_customer_spawn_time = default(6.0);
      	int rounds = default(1000);
	    @display("i=block/source");
	
	gates:
	    output out;
	    
}
simple Cashier{
    parameters:
       	double min_time_per_item = default(0.5);
       	double max_time_per_item = default(2);
        @display("i=block/sink");
        
       	@signal[updateServiceTime](type="simtime_t");
    	@signal[updateIdleTime](type="simtime_t");
       	@statistic[serviceTimePerCashier](title="ServiceTimePerCashier";source=updateServiceTime;record=sum);
       	@statistic[idleTimePerCashier](title="IdleTimePerCashier";source=updateIdleTime;record=sum);
        
       	
	gates: 
	    input inp;
	    
}

network Supermarket
{
    parameters:
        int cashier_count = default(4);
    	@signal[customerFinished](type="simtime_t");
    	@signal[updateQueue](type="int");
    	@signal[updateServiceTime](type="simtime_t");
    	@signal[updateIdleTime](type="simtime_t");
    	
    	@statistic[queueSizeOverTime](title="queueSizeOverTime";source=updateQueue;record=vector);
    	
    	
        
        @statistic[customerWaitingTime](title="cust waiting time";source=customerFinished;record=vector;interpolationmode=none);
        @statistic[custTotalWaitingTime](title="cust total waiting time";source=customerFinished;record=sum);
        @statistic[custMeanWaitingTime](title="Cust mean waiting time";source=customerFinished;record=mean);
         
         
         
    submodules:
        generator: Generator;
        cashiers[cashier_count]: Cashier;
        balancer: Balancer{
        	cashier_count = parent.cashier_count;
        };
        
    connections:
        generator.out -->  balancer.inp;
        for i=0..cashier_count-1{
        	balancer.out++ --> cashiers[i].inp;
        
        }
        

	
}
