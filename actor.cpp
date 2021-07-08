#include <iostream>
#include <sstream>
#include "actor.hpp"
#include <unistd.h>

Actor::Actor(){

/* 
 * Standard constructor, initialization of the buffer 
 * */

	for (int i=0; i < BUFFER_SIZE ; i++){
		buffer[i] = __EMPTY_REGISTER__; 
	};
}; 

Actor::~Actor(){ 
/* 
 * Standard destructor
 * */
}


void Actor::producer(Actor &targetActor,  std::string payload){

/* 
 * Start Actor producer 
 * Send a message to another actor by pushing a payload in the buffer of the targeted actor
 * */

	while(targetActor.buffer[0] != __EMPTY_REGISTER__) {
		// Waiting for the target buffer register to be free
		sleep(0.1);
	};

	if (targetActor.buffer[0] == __EMPTY_REGISTER__){
		targetActor.lock.lock(); 
		targetActor.buffer[0] = payload; 
		targetActor.lock.unlock(); 
	}; 

}; 

void Actor::consumer(std::function<void (std::string)> customConsumer){

/* 
 * Start Actor consumer 
 * Consumption of received message buffer, by applying customConsumer function on pull payload
 * */
	while (true) {
		lock.lock(); 
		std::string pullPayload = buffer[BUFFER_SIZE - 1];
		if (pullPayload != __EMPTY_REGISTER__){
			customConsumer(pullPayload); 
		}; 
		bufferUpdate(); 
		lock.unlock(); 
		sleep(0.1);
	}; 
}; 

void Actor::consumer(std::function<void (std::string)> customConsumer, Actor &senderActor, std::string ackMessage){

/* 
 * Start Actor overloaded consumer with acknowledgment message
 * Consumption of received message buffer, by applying customConsumer function on pull payload
 * Produce back an acknowledgment message
 * */
	while (true) {
		lock.lock(); 
		std::string pullPayload = buffer[BUFFER_SIZE - 1];
		if (pullPayload != __EMPTY_REGISTER__){
			customConsumer(pullPayload);
			producer(senderActor, ackMessage);
		}; 
		bufferUpdate(); 
		lock.unlock(); 
		sleep(0.1);
	}; 
}; 

void Actor::defaultConsumer(){

/* 
 * Consumption of received message buffer, by displaying the messages 
 * */

	while (true) {
		lock.lock(); 
		std::string pullPayload = buffer[BUFFER_SIZE - 1];
		if (pullPayload != __EMPTY_REGISTER__){
			std::stringstream out; 
			out << "consumer : " << pullPayload ; 
			std::cout << out.str() << std::endl; 
		}; 
		bufferUpdate(); 
		lock.unlock(); 
		sleep(0.1);
	}; 
}; 

void Actor::bufferUpdate(){

/*
 * Reorganization of the buffer
 * */

	for (int i= BUFFER_SIZE -1 ; i >= 0 ; i--){
		if (i>0) {
			buffer[i] = buffer[i-1]; 
		}; 
	}; 
	buffer[0] = __EMPTY_REGISTER__; 
};

void Actor::displayBuffer(){

	std::stringstream out;

	out << "[";

	for (int i=0; i < BUFFER_SIZE;i++){
		if (i== BUFFER_SIZE-1){
			out << buffer[i];
		} else {
			out << buffer[i] << ",";
		};
	};
	out << "]";
	std::cout << out.str() << std::endl;
};

int Actor::getBufferSize(){
	int size = sizeof(buffer)/sizeof(buffer[0]);
	return size;
};



