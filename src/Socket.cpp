#include "Socket.hpp"

#include <sstream>
#include <iostream>

const int Socket::MAX_MSG_LENGTH = 255;

bool Socket::WSAStarted = false;
addrinfo Socket::hints_;

// Props
int Socket::getID(){
	return id_;
}
const char* Socket::getPort(){
	return port_;
}

// Constructors
Socket::Socket(const char* port) throw(SocketException){
	setUpWSA();
	port_ = port;
	setMyAddress();
	
	id_ = socket(myaddr_->ai_family, myaddr_->ai_socktype, 0);
	if(id_ == -1){
		std::stringstream ss;
		ss << "Error creating socket: " << WSAGetLastError();
		throw SocketException(this, ss.str());
	}
}
Socket::Socket(int id, const char* port) throw(SocketException){
	setUpWSA();
	port_ = port;
	setMyAddress();
	
	id_ = id;
}
Socket::~Socket(){
	close();
}

// Methods
void Socket::bindSocket() throw(SocketException){
	int bindres = bind(id_, myaddr_->ai_addr, myaddr_->ai_addrlen);
	if(bindres == -1){
		std::stringstream ss;
		ss << "Error binding socket: " << WSAGetLastError();
		throw SocketException(this, ss.str());
	}
}
void Socket::connectSocket(addrinfo theiraddr) throw(SocketException){
	int conres = connect(id_, theiraddr.ai_addr, theiraddr.ai_addrlen);
	if(conres == -1){
		std::stringstream ss;
		ss << "Error connecting: " << WSAGetLastError();
		throw SocketException(this, ss.str());
	}
}
int Socket::sendSocket(const char* msg) throw(SocketException){
	int sendres = send(id_, msg, MAX_MSG_LENGTH, 0);
	if(sendres == -1){
		std::stringstream ss;
		ss << "Error sending message: " << WSAGetLastError();
		throw SocketException(this, ss.str());
	}
	return sendres;
}
int Socket::recvSocket(char* msg) throw(SocketException){
	int recres = recv(id_, msg, MAX_MSG_LENGTH, 0);
	if(recres == -1){
		std::stringstream ss;
		ss << "Error receiving message: " << WSAGetLastError();
		throw SocketException(this, ss.str());
	}
	return recres;
}
void Socket::close(){
	closesocket(id_);
}
addrinfo Socket::getTheirAddr(char* addr, const char* port){
	addrinfo* newaddr;
	setHints();
	int status = getaddrinfo(addr, port, &hints_, &newaddr);
	if(status < 0){
		//std::stringstream ss;
		//ss << "Error accepting connection: " << WSAGetLastError();
		throw SocketException(NULL, "Error getting addr info: " /*+ gai_strerror(status)*/);
	}
	return *newaddr;
}

void Socket::setHints(){
	memset(&hints_, 0, sizeof hints_);
	hints_.ai_family = AF_UNSPEC;
	hints_.ai_socktype = SOCK_STREAM;
	hints_.ai_flags = AI_PASSIVE;
}
void Socket::setMyAddress() throw(SocketException){
	setHints();
	int status = getaddrinfo("", port_, &hints_, &myaddr_);
	if(status < 0){
		//std::stringstream ss;
		//ss << "Error accepting connection: " << WSAGetLastError();
		throw SocketException(NULL, "Error getting addr info: " /*+ gai_strerror(status)*/);
	}
}
void Socket::setUpWSA(){
	if(!WSAStarted){
		WSAData data;
		WSAStartup(WORD(2), &data);
		WSAStarted = true;
		//WSACleanup(void);
	}
}

Socket* SocketException::getSocket(){
	return socket_;
}
SocketException::SocketException(Socket* socket, std::string what){
	socket_ = socket;
	what_ = what;
}
SocketException::~SocketException() throw(){
}
const char* SocketException::what() const throw(){
	return what_.c_str();
}
