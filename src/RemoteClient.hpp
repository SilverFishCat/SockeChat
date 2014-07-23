#ifndef __REMOTE_CLIENT_HPP
#define __REMOTE_CLIENT_HPP

#include "MessageReceiver.hpp"
#include <string>
class Socket;
class ListeningSocket;
class ClientMessageReceiver;
class RemoteClient;
class CommandSource;

class ClientMessageReceiver : public MessageReceiver{
	private:
		RemoteClient* client_;
	public:
		ClientMessageReceiver(Socket* socket, RemoteClient* client);
		
		RemoteClient* getClient();
		
		class Factory{
			public:
				virtual ClientMessageReceiver* create(Socket* socket, RemoteClient* client)=0;
		};
};

/**
	A remote client that is connected to some server.
*/
class RemoteClient{
	public:
		// Types
		enum Status{
			DUMMY, ///< no socket
			PENDING, ///< trying to accept a connection
			ACCEPTED, ///< accepted a connection
			RECEIVING_MESSAGES, ///< receiving message from remote client
			CONNECTION_CLOSED ///< the connection has been closed
		};
	
	private:
		// Members
		Socket* socket_;
		//Server* server_;
		ClientMessageReceiver* msgReceiver_;
		Status status_;
		std::string name_;
	
	public:
		// Constructors
		/**
			Create a new remote client object.
		*/
		RemoteClient();
		/**
			Create a new remote client with a nickname.
			
			@param name The client's nickname
		*/
		RemoteClient(std::string name);
		/**
			Deletes the socket and thread.
		*/
		~RemoteClient();
		
		// Props
		/**
			Get the nickname of this client.
		*/
		std::string getName();
		/**
			Get the socket connected to this remote.
		*/
		Socket* getSocket();
		/**
			Get the status of the client.
		*/
		Status getStatus();
		/**
			Set the nickname of this cleint.
			
			@param name The new nickname of this client.
		*/
		void setName(std::string name);
		
		// Methods
		/**
			Accept a connection to a remote client.
			
			@param listeningSocket a listening socket.
		*/
		void acceptSocketFrom(ListeningSocket* listeningSocket);
		/**
			Create the message receiver from a factory.
		*/
		void createMessageReceiver(ClientMessageReceiver::Factory* factory); // Create the message thread.
		/**
			Start receiving messages from the client.
		*/
		void openCommunications();
		/**
			Stop receiving messages from the client.
		*/
		void closeCommunications();
};

#endif
