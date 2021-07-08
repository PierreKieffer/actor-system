#include <functional>
#include <mutex>

#define BUFFER_SIZE 10
#define __EMPTY_REGISTER__ "__EMPTY_REGISTER__"
#define __ACK__ "__ACK__"

class Actor {
	public: 
		Actor();
		~Actor();
		std::string buffer[BUFFER_SIZE]; 
		std::mutex lock; 


		// Producer 
/* void producer(std::string targetBuffer[BUFFER_SIZE], std::string inputPayload); */
		void producer(Actor &targetActor, std::string inputPayload); 

		// Consumer 
		void consumer(std::function<void (std::string)> customConsumer);  
		void consumer(std::function<void (std::string)> customConsumer, Actor &senderActor, std::string ackMessage = __ACK__);  
		void defaultConsumer();  

	private : 
		// Buffer 
		void bufferUpdate(); 
		void displayBuffer();
		int getBufferSize(); 
}; 
