#include "RemoteClient.hpp"

#include "MessageReceivingThread.hpp"
#include "MessageReceiver.hpp"
#include "Socket.hpp"
#include "ListeningSocket.hpp"
#include "CommandSource.hpp"
#include <iostream>
#include <sstream>

ClientMessageReceiver::ClientMessageReceiver(Socket* socket, RemoteClient* client) :
		MessageReceiver(socket){
	client_ = client;
}
RemoteClient* ClientMessageReceiver::getClient(){
	return client_;
}

// Constructors
RemoteClient::RemoteClient(){
	socket_ = NULL;
	msgReceiver_ = NULL;
	status_ = DUMMY;
	name_ = "";
}
RemoteClient::RemoteClient(std::string name){
	socket_ = NULL;
	msgReceiver_ = NULL;
	status_ = DUMMY;
	name_ = name;
}
RemoteClient::~RemoteClient(){
	closeCommunications();
	if(msgReceiver_ != NULL)
		delete(msgReceiver_);
	if(socket_ != NULL)
		delete(socket_);
}

// Props
std::string RemoteClient::getName(){
	return name_;
}
Socket* RemoteClient::getSocket(){
	return socket_;
}
RemoteClient::Status RemoteClient::getStatus(){
	return status_;
}
void RemoteClient::setName(std::string name){
	name_ = name;
}

// Methods
void RemoteClient::acceptSocketFrom(ListeningSocket* listeningSocket){
	status_ = PENDING;
	if(socket_ != NULL)
		delete(socket_);
	socket_ = listeningSocket->acceptSocket();
	status_ = ACCEPTED;
	if(name_ == ""){
		std::stringstream tos;
		tos << socket_->getID();
		name_ = tos.str();
	}
}
void RemoteClient::createMessageReceiver(ClientMessageReceiver::Factory* factory){
	if(status_ != ACCEPTED){
		throw SocketException(getSocket(),"CreateMessageReceiver called without accepting a connection.");
	}
	else{
		if(msgReceiver_ != NULL)
			delete(msgReceiver_);
		msgReceiver_ = factory->create(this->getSocket(), this);
	}
}
void RemoteClient::openCommunications(){
	if(msgReceiver_ == NULL){
		throw SocketException(getSocket(),"Communications impossible without a message thread.");
	}
	else if(status_ != ACCEPTED){
		throw SocketException(getSocket(),"openCommunications called without accepting a connection.");
	}
	else{
		std::cout << "Opening communications with " << socket_->getID() << std::endl;
		status_ = RECEIVING_MESSAGES;
		msgReceiver_->run();
		status_ = CONNECTION_CLOSED;
		std::cout << "Connection closed on " << socket_->getID() << std::endl;
	}
}
void RemoteClient::closeCommunications(){
	/*if(incomingThread_ != NULL){
		incomingThread_->terminate();
		socket_->close();
	}*/ // TODO: ????
}
