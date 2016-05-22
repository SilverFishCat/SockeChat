#include "Socket.hpp"
#include "MessageReceivingThread.hpp"
#include "MessageReceiver.hpp"
#include <iostream>

void handleIncomingMessages(Socket* s);

const std::string EXIT_PHRASE = "goodbye";
const char* PORT = "3940";

class ReceiveMessages : public MessageReceiver{
	public:
		ReceiveMessages(Socket *socket) : MessageReceiver(socket){}
		virtual void handleMessage(const char* msg){
			std::cout << msg;
		}
};

int main(int argc, char** args){
	
	if(argc != 2){
		std::cerr << "Usage:client.exe ip" << std::endl;
		exit(1);
	}
	
	Socket *s = NULL;
	Thread *t = NULL;
	try{
		s = new Socket(PORT);
		std::cout << "created socket" << s->getID() << std::endl;
		char* addr = args[1];
		addrinfo theirinfo = Socket::getTheirAddr(addr,PORT);
		s->connectSocket(theirinfo);
		std::cout << "Connected!" << std::endl;
		
		char motd[Socket::MAX_MSG_LENGTH];
		s->recvSocket(motd);
		std::cout << "MOTD:" << motd << std::endl;
		
		std::cout << "Type a message or " << EXIT_PHRASE << " to exit" << std::endl;
		std::cout << "Type \\command to send a command" << std::endl;
		
		t = new MessageReceivingThread(new ReceiveMessages(s));
		t->start(); //TODO: check that is really started
		
		char msg[Socket::MAX_MSG_LENGTH];
		std::cin.getline(msg,Socket::MAX_MSG_LENGTH,'\n');
		std::string strmsg = msg;
		while(strmsg.compare(EXIT_PHRASE)!=0){
			s->sendSocket(msg);
			std::cin.getline(msg,Socket::MAX_MSG_LENGTH,'\n');
			strmsg = msg;
		}
	}
	catch(SocketException e){
		std::cerr << e.what() << std::endl;
		std::cerr << "Connection aborted." << std::endl;
	}
	
	if(s != NULL){
		if(t != NULL)
			t->terminate();
		std::cout << "closed socket " << s->getID() << std::endl;
		if(t != NULL)
			delete(t);
		delete(s);
	}
	
	WSACleanup();
}
