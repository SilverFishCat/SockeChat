#include "ListeningSocket.hpp"

#include <sstream>

ListeningSocket::ListeningSocket(const char* port, int backlog) :
		Socket(port){
	backlog_ = backlog;
	isListening_ = false;
}

// Props
int ListeningSocket::getBacklog(){
	return backlog_;
}
bool ListeningSocket::isListening(){
	return isListening_;
}

// Methods
void ListeningSocket::startListening() throw(SocketException){
	int listres = listen(getID(), backlog_);
	if(listres == -1){
		std::stringstream ss;
		ss << "Error listening on socket: " << WSAGetLastError();
		throw SocketException(this, ss.str());
	}
	isListening_ = true;
}
Socket* ListeningSocket::acceptSocket() throw(SocketException){
	if(!isListening_){
		throw SocketException(this, "Socket is not in listening mode");
	}
	
	sockaddr_storage* theiraddr;
	int* a;
	int id = accept(getID(), (sockaddr*)theiraddr, a);
	if(id == -1){
		std::stringstream ss;
		ss << "Error accepting connection: " << WSAGetLastError();
		throw SocketException(this, ss.str());
	}
	Socket* s = new Socket(id,getPort());
	s->theiraddr_ = theiraddr;
	return s;
}
