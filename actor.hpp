#include <functional>

#define BUFFER_SIZE 10
#define __EMPTY_REGISTER__ "__EMPTY_REGISTER__"
#define __ACK__ "__ACK__"

class Actor {
	public: 
		Actor();
		std::string buffer[BUFFER_SIZE]; 

		// Producer 
		void producer(std::string targetBuffer[BUFFER_SIZE], std::string inputPayload); 

		// Consumer 
		void consumer(std::function<void (std::string)> customConsumer);  
		void consumer(std::function<void (std::string)> customConsumer, std::string ackBuffer[BUFFER_SIZE], std::string ackMessage = __ACK__);  
		void defaultConsumer();  

	private : 
		// Buffer 
		void bufferUpdate(); 
		void displayBuffer();
		int getBufferSize(); 
}; 
