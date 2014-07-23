#ifndef __CONNECTION_CREATOR_HPP
#define __CONNECTION_CREATOR_HPP
#include "Thread.hpp"

#include "RemoteClientThread.hpp"
#include "RemoteClient.hpp"
#include <list>
class ListeningSocket;
class Server;

/**
	A connection manager.
	
	The connection manager tries to accept a set number of connections from a listening socket,
	and starts a new thread for every connection (remote client) that is accepts, adding it
	to a thread list.
*/
class ConnectionCreator : public Thread{
	private:
		Server* server_;
		RemoteClientThread::Factory* threadFactory_;
		ClientMessageReceiver::Factory* receiverFactory_;
	
	public:
		// Properties
		/**
			Get the server that the connections are created for.
		*/
		Server* getServer();
		
		// Constructors
		/**
			Create a new connection manager thread.
			
			@param [in] server The server to create the connections for.
			@param [in] threadFactory A factory for creating the client threads.
			@param [in] receiverFactory A factory for creating the message receiver.
		*/
		ConnectionCreator(Server* server, RemoteClientThread::Factory* threadFactory,
			ClientMessageReceiver::Factory* receiverFactory);
		
		// Methods
		virtual void onStartThread(); // do on thread start
};

#endif
