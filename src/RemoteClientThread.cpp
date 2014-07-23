#include "RemoteClientThread.hpp"

#include "RemoteClient.hpp"

// Properties
RemoteClient* RemoteClientThread::getRemoteClient(){
	return client_;
}

// Constructors
RemoteClientThread::RemoteClientThread(ListeningSocket* listeningSocket, ClientMessageReceiver::Factory* factory){
	client_ = new RemoteClient();
	lstSock_ = listeningSocket;
	factory_ = factory;
}
RemoteClientThread::~RemoteClientThread(){
	delete(client_);
}

// Methods
void RemoteClientThread::onStartThread(){
	client_->acceptSocketFrom(lstSock_);
	onAccept();
	client_->createMessageReceiver(factory_);
	client_->openCommunications();
}
