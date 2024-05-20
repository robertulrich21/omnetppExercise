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
 
    @display("i=block/queue");
    gates:
        input in;
        input requestIn;
        output out;
        
        
}

simple ServiceUnit
{
    
    parameters:
        double interServiceTime @unit(s) = default(exponential(1s));    
        @display("i=block/filter");
        
    gates:
        input in;
        output out;
        output requestOut;
}

simple Sink
{
  
    @display("i=block/sink");
    gates:
        input in;
}

network MM1
{
    submodules:
        producer: Producer;
        queue: Queue;
        serviceUnit: ServiceUnit;
        sink: Sink;
    connections:
        producer.out    -->  queue.in;
        queue.out       -->  serviceUnit.in;
        queue.requestIn <--  serviceUnit.requestOut;
        serviceUnit.out -->  sink.in;
}


