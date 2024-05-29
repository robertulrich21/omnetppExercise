package task2.simulations;

simple Producer
{
    parameters:
        double interArrivalTime @unit(s) = default(exponential(1s));
        @display("i=block/source");
        
        
        
    gates:
        output out;

}

simple Queue
{
 parameters:
        
    @signal[queueInterArrivalTime](type=simtime_t);
    @signal[queueWaitingTime](type=simtime_t);
    @signal[queueFillLevel](type=unsigned int);
    
    @signal[queueEmptyTime](type=simtime_t);
    @signal[queueNonEmptyTime](type=simtime_t);
    
    @statistic[meanQueueInterArrivalTime](title="meanQueueInterArrivalTime";source=queueInterArrivalTime;record=mean);
	@statistic[meanQueueWaitingTime](title="meanQueueWaitingTime";source=queueWaitingTime;record=mean);
	@statistic[meanQueueFillLevel](title="meanQueueFillLevel";source=queueFillLevel;record=mean);
	
	@statistic[totalQueueEmptyTime](title="totalQueueEmptyTime";source=queueEmptyTime;record=sum);
	@statistic[totalQueueNonEmptyTime](title="totalQueueNonEmptyTime";source=queueNonEmptyTime;record=sum);
    
    @display("i=block/queue");
    gates:
        input in;
        input requestIn;
        input eieruhrIn @directIn;
        output out;
                
    
        
}

simple ServiceUnit
{
    
    parameters:
        double interServiceTime @unit(s) = default(exponential(1s));    
        @display("i=block/filter");
        
        @signal[serviceUnitWaitingTime](type=simtime_t);
        @signal[serviceUnitFillLevel](type=unsigned int);
        
        @statistic[meanServiceUnitWaitingTime](title="meanServiceUnitWaitingTime";source=serviceUnitWaitingTime;record=mean);
        @statistic[meanServiceUnitFillLevel](title="meanServiceUnitFillLevel";source=serviceUnitFillLevel;record=mean);
        
        
    gates:
        input in;
        input eieruhrIn @directIn;
        output out;
        output requestOut;
}

simple Sink
{
  
    @display("i=block/sink");
    gates:
        input in;
}

simple Eieruhr
{
    parameters:
        double measurementIntervall @unit(s) = default(1s);
    	@display("i=status/hourglass");
}

network MM1
{
    submodules:
        producer: Producer;
        queue: Queue;
        serviceUnit: ServiceUnit;
        sink: Sink;
        eieruhr: Eieruhr;
        
    connections:
        producer.out    -->  queue.in;
        queue.out       -->  serviceUnit.in;
        queue.requestIn <--  serviceUnit.requestOut;
        serviceUnit.out -->  sink.in;
}


