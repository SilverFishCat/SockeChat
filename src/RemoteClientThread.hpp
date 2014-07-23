#ifndef __REMOTE_CLIENT_THREAD_HPP
#define __REMOTE_CLIENT_THREAD_HPP
#include "Thread.hpp"

#include "RemoteClient.hpp"
class RemoteClient;
class ListeningSocket;

class RemoteClientThread : public Thread{
	private:
		// Members
		RemoteClient* client_;
		ListeningSocket* lstSock_;
		ClientMessageReceiver::Factory* factory_;
	
	public:
		// Properties
		/**
			Get the remote client that this thread created.
		*/
		RemoteClient* getRemoteClient();
		
		// Constructors
		/**
			Create a remote client thread.
		*/
		RemoteClientThread(ListeningSocket* listenSocket, ClientMessageReceiver::Factory* factory);
		~RemoteClientThread();
		
		// Methods
		virtual void onStartThread(); //do on thread start
		virtual void onAccept()=0;
		
		class Factory{
			public:
				virtual RemoteClientThread* create(ListeningSocket* listeningSocket, ClientMessageReceiver::Factory* factory)=0;
		};
};

#endif
