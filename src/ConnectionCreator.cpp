#include "ConnectionCreator.hpp"

#include "RemoteClient.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include <iostream>

// Constructors
ConnectionCreator::ConnectionCreator(Server* server, RemoteClientThread::Factory* threadFactory,
		ClientMessageReceiver::Factory* receiverFactory){
	server_ = server;
	threadFactory_ = threadFactory;
	receiverFactory_ = receiverFactory;
}

// Props
Server* ConnectionCreator::getServer(){
	return server_;
}

// Methods
void ConnectionCreator::onStartThread(){
	try{
		while(1){
			if(server_->clientThreads()->size() < server_->maxConnections()){
				RemoteClientThread *t = threadFactory_->create(server_->listeningSocket(), receiverFactory_);
				server_->clientThreads()->push_back(t);
				t->start();
			}
			_sleep(1000);
		}
	}
	catch(SocketException e){
		std::cerr << "Error on connection manager thread." << std::endl;
		Socket *exas = e.getSocket();
		if(exas != NULL){
			std::cerr << "Error on socket:" << exas->getID() << std::endl;
		}
		std::cerr << e.what() << std::endl;
	}
}
