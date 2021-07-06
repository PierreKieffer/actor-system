#include <iostream> 
#include <sstream> 
#include <unistd.h> 
#include <thread> 
#include "actor.hpp"


// actor_1 custom consumer  
void actor_1CustomConsumerFunc(std::string payload){
        std::stringstream out; 
        out << "actor_1 Consumer data processing : " << payload; 
        std::cout << out.str() << std::endl; 
}; 

// actor_2 custom consumer  
void actor_2CustomConsumerFunc(std::string payload){
        std::stringstream out; 
        out << "actor_2 Consumer data processing : " << payload; 
        std::cout << out.str() << std::endl; 
}; 

int main(){
	Actor actor_1; 
        Actor actor_2; 

        // actor_2 Consumer Thread, with acknowledgment to actor_1
        std::thread actor_2ConsumerThread([&](){
                actor_2.consumer(actor_2CustomConsumerFunc, actor_1.buffer, "ack from actor_2"); 
	});

        // actor_1 Consumer Thread
        std::thread actor_1ConsumerThread([&](){
                actor_1.consumer(actor_1CustomConsumerFunc); 
	});

        // actor_1 Producer Thread
        std::thread actor_1ProducerThread([&](){
                std::string payload = "{\"foo\" : \"bar\"}"; 
                actor_1.producer(actor_2.buffer, payload);
	});


        actor_2ConsumerThread.join();
        actor_1ConsumerThread.join();
        actor_1ProducerThread.join();
        return 0; 
}; 


