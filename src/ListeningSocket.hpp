#ifndef __LISTENING_SOCKET_HPP
#define __LISTENING_SOCKET_HPP
#include "Socket.hpp"

// Finally, a socket that listens!
/**
	A socket that listens to incoming connections on the given port.
	
	Can accept connection after startListening() is used.
*/
class ListeningSocket : public Socket{
	private:
		int backlog_;
		bool isListening_;
	public:
		/**
			Create a new listening socket.
			
			@param port The port to listen to and used
			@param backlog The number of connections to accept at the same time.
		*/
		ListeningSocket(const char* port, int backlog);
		/**
			Get the number of connections accepted before they are backlogged
		*/
		int getBacklog();
		/**
			Is the socket listening yet.
		*/
		bool isListening();
		/**
			Start listening for incoming connections.
		*/
		void startListening() throw(SocketException);
		/**
			Wait for and accept an incoming connection.
		*/
		Socket* acceptSocket() throw(SocketException);
};

#endif

