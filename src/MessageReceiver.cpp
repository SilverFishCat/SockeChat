#include "MessageReceiver.hpp"

#include "Socket.hpp"
#include <iostream>

Socket* MessageReceiver::socket(){
	return socket_;
}
void MessageReceiver::setSocket(Socket* socket){
	socket_ = socket;
}
bool MessageReceiver::isIgnoringBlackLines(){
	return  ignoreBlackLines_;
}
void MessageReceiver::setIgnoreBlankLines(bool ignoreBlackLines){
	ignoreBlackLines_ = ignoreBlackLines;
}

// Constructors
MessageReceiver::MessageReceiver(Socket* socket){
	socket_ = socket;
	ignoreBlackLines_ = true;
}
MessageReceiver::~MessageReceiver(){}

// Methods
void MessageReceiver::run(){
	try{
		char msg[Socket::MAX_MSG_LENGTH];
		int recvres = socket_->recvSocket((char*)msg);
		while(recvres!=0){
			std::string strMsg = msg;
			if(!(ignoreBlackLines_ && strMsg.compare("") == 0))
				handleMessage(msg);
			recvres = socket_->recvSocket((char*)msg);
		}
	}
	catch(SocketException e){
		std::cerr << "Error on message receiver:" << std::endl;
		std::cerr << e.what() << std::endl;
	}
}
