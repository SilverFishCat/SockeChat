#ifndef __SOCKET_HPP
#define __SOCKET_HPP

#define _WIN32_WINNT 0x501 // a hack to make windows less bad

#include <exception>
#include <string>
#include <winsock2.h>
#include <Ws2tcpip.h>

class Socket;

/**
	A general exception to raise when a socket operation just fails.
*/
class SocketException : public std::exception{
	private:
		Socket* socket_;
		std::string what_;
	
	public:
		/**
			Get the socket associated with this exception.
			Is null when thrown on socket constructor.
		*/
		Socket* getSocket();
		/**
			Create a new socket exception.
			
			@param socket The socket associated with this exception, if any
			@param what Error description
		*/
		SocketException(Socket* socket, std::string what);
		virtual ~SocketException() throw();
		virtual const char* what() const throw();
};

/**
	A socket class to hide ugly c-style implementation of sockets.
	
	Sockets enable receiving and sending messages over a network.
*/
class Socket{
	private:
		int id_;
		addrinfo* myaddr_;
		const char* port_;
		
		static addrinfo hints_;
		static bool WSAStarted;
	
	public:
		sockaddr_storage* theiraddr_; // might be removed
		static const int MAX_MSG_LENGTH; ///< Max length of message to send
		
		// Props
		/**
			Get the internal id number of the socket.
		*/
		int getID();
		/**
			Get the port defined for this socket.
		*/
		const char* getPort();
		
		// Constructors
		/**
			Create a new socket.
			
			@param port The port to use
		*/
		Socket(const char* port) throw(SocketException);
		/**
			Create a socket from an already existing id.
			
			@param id The id of the existing socket
			@param port The port defined for the socket
		*/
		Socket(int id, const char* port) throw(SocketException);
		~Socket();
		
		/**
			Bind the socket to the port.
			
			@post Socket is bound or exception
		*/
		void bindSocket() throw(SocketException);
		/**
			Connect to specified address.
			Note: No need to bind port if connect is used.
			
			@param theiraddr The address to connect to
			@post Socket is connected to a listening remote socket, or exception
		*/
		void connectSocket(addrinfo theiraddr) throw(SocketException);
		/**
			Send a message to the remote socket.
			
			@param msf The message to send, max length is MAX_MSG_LENGTH
			@return Number of bytes actually sent
			@post Message is sent or exception
		*/
		int sendSocket(const char* msg) throw(SocketException);
		/**
			Receive a message from the remote socket.
			
			@param msg Where to store the message, must be at least MAX_MSG_LENGTH long
			@return 0 If message is over, otherwise length of message
		*/
		int recvSocket(char* msg) throw(SocketException);
		/**
			Closes the connection.
		*/
		void close();
		/**
			A utility method for getting the addrinfo of an ipv4 address.
			
			@param add The address to get
			@param port The port to set on the addrinfo object
		*/
		static addrinfo getTheirAddr(char* addr, const char* port);
	
	private:
		/**
			Set the underlined hint object.
		*/
		static void setHints();
		/**
			Set the internal address for this host.
		*/
		void setMyAddress() throw(SocketException);
		/**
			Set up nasty windows socket api.
		*/
		static void setUpWSA();
};

#endif
