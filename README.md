# actor-system 

C++ implementation of an actor model. 

--- 

The Actor Model provides a higher level of abstraction for writing concurrent and distributed systems. 

In response to a message it receives, an actor can: make local decisions, create more actors, send more messages, and determine how to respond to the next message received.

Actors may modify their own private state, but can only affect each other indirectly through messaging. 


## Actor 

```cpp
#include "actor.hpp"

int main(){
	Actor actor; 
	return 0; 
}; 
```

### Buffer  
Each actor instance has a message buffer allowing to receive events from other actors, or from itself.

### Consumer 
The consumer method of an actor allows to read and process (with a custom function) the new messages stored in its buffer. 

#### Standard consumer 
To start a consumer in a new thread : 
```cpp 
#include <iostream> 
#include <thread> 
#include <sstream>
#include "actor.hpp"

void customConsumerFunc(std::string payload){
	std::stringstream out; 
	out << "Consumer data processing : " << payload; 
	std::cout << out.str() << std::endl; 
}; 

int main(){

	Actor actor; 

	// Consumer Thread
	std::thread consumerThread([&](){
		actor.consumer(customConsumerFunc); 
	});

	consumerThread.join(); 
	return 0; 

}; 

```

#### Acknowledge a message
The consumer method can be overloaded in order to send back an acknowledgment after the processing of each message.

The acknowledgment message is sent to an actor's buffer. 

```cpp 
#include <iostream> 
#include <thread> 
#include <sstream>
#include "actor.hpp"

void customConsumerFunc(std::string payload){
	std::stringstream out; 
	out << "Consumer data processing : " << payload; 
	std::cout << out.str() << std::endl; 
}; 

int main(){

	Actor actor_1; 
	Actor actor_2; 

	// actor_2 Consumer Thread, with acknowledgement to actor_1
	std::thread consumerThread([&](){
		actor_2.consumer(customConsumerFunc, actor_1); 
	});

	consumerThread.join(); 
	return 0; 

}; 
```
Default acknowledgment message value is `__ACK__`. 

To use your own acknowledgment message : 
```cpp 
actor_2.consumer(customConsumerFunc, actor_1, "message received"); 
```

### Producer 
The producer method  of an actor allows to send a message to the buffer of an other actor, or to its own buffer. 

To start a producer in a new thread : 
```cpp 
#include <iostream> 
#include <thread> 
#include <sstream>
#include "actor.hpp"

int main(){

	Actor actor_1; 
	Actor actor_2; 

	// Producer Thread, to send a message to actor_2
	std::thread producerThread([&](){
		std::string payload = "{\"foo\" : \"bar\"}"; 
		actor_1.producer(actor_2, payload);
	});

	producerThread.join(); 
	return 0; 

}; 

```

### Full example  
```cpp
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
		actor_2.consumer(actor_2CustomConsumerFunc, actor_1, "ack from actor_2"); 
	});

	// actor_1 Consumer Thread
	std::thread actor_1ConsumerThread([&](){
		actor_1.consumer(actor_1CustomConsumerFunc); 
	});

	// actor_1 Producer Thread
	std::thread actor_1ProducerThread([&](){
		std::string payload = "{\"foo\" : \"bar\"}"; 
		actor_1.producer(actor_2, payload);
	});


	actor_2ConsumerThread.join();
	actor_1ConsumerThread.join();
	actor_1ProducerThread.join();
	return 0; 
}; 

```

Build : 
```bash 
g++ -pthread *.cpp -o actor-system
```

Run : 
```bash 
./actor-system
```

Output : 
```
actor_2 Consumer data processing : {"foo" : "bar"}
actor_1 Consumer data processing : ack from actor_2
```



