#ifndef __SERVER_HPP
#define __SERVER_HPP

class Socket;
class ListeningSocket;
class ConnectionCreator;
class RemoteClientThread;
class RemoteClient;
class CommandSource;
class Command;
#include <list>
#include <exception>
#include <string>

typedef std::list<RemoteClientThread*> threadList;
typedef std::list<Command*> commandList;

class ServerException : public std::exception{

};
class Server{
	private:
		ListeningSocket* lstSock_;
		threadList clientThreads_;
		size_t maxConnections_;
		ConnectionCreator* connectionCreator_;
		bool shouldExit_;
		commandList commands_;
		
		bool started_;
		bool acceptingComs_;
	
	public:
		static const int BACKLOG; ///< The maximum number of the connections to the server.
		static const std::string MOTD; ///< The server's Message Of The Day.
		
		// Props
		/**
			Is the server up.
			
			@return true if the server has been started, false otherwise.
		*/
		bool isUp();
		/**
			Is the server accepting communications.
			
			@return true If the server accepting communications, false otherwise.
		*/
		bool isAcceptingCommunications();
		/**
			The maximum number of connections to accept.
			
			@return The maximum number of connections to accept.
		*/
		size_t maxConnections();
		/**
			The threads created for each client.
			Some might be pending, meaning the thread is waiting for a new connection.
			
			@return The threads created for each client.
		*/
		threadList* clientThreads();
		/**
			Get the connection creator.
		*/
		ConnectionCreator* getConnectionCreator();
		/**
			The listening socket used to accept connections.
		*/
		ListeningSocket* listeningSocket();
		/**
			Set the maximum number of connections to accpet
			
			@param [in] maxConnections The maximum number of connections to accept.
		*/
		void setMaxConnections(size_t maxConnections);
		/**
			Set the connection creator.
			
			@param [in] connectionCreator The connection creator.
		*/
		void setConnectionCreator(ConnectionCreator* connectionCreator);
		/**
			Add a command to the server's command list.
			
			@param command The command to add.
		*/
		void addCommand(Command* command);
		/**
			Set if the server should close after next command is executed.
			
			@param shouldClose Whether the server should close.
		*/
		void setShouldClose(bool shouldClose);
		/**
			Get the list of command assigned to this server.
			
			@return The commands active on this server.
		*/
		commandList getCommandList();
		
		// Constructors
		/**
			Create a new server.
			
			@param port The port to use for communications.
		*/
		Server(const char* port);
		
		// Destructors
		virtual ~Server();
		
		// Methods
		/**
			Independently start, open communications and waits for commands.
		*/
		void run();
		/**
			Starts the server.
		*/
		virtual void start();
		/**
			Accept incoming communications.
			Server should be started or ServerException will be thrown.
		*/
		void openCommunications() throw(ServerException);
		/**
			Stop accepting incoming communications.
			Server should be started or ServerException will be thrown.
		*/
		void closeCommunications() throw(ServerException);
		/**
			Stops the server if it is running.
		*/
		virtual void stop();
		/**
			Receives input from the server user and executes the commands they type.
		*/
		void listenToServerCommands();
		
		/**
			Broadcasts a message to all connected client.
			
			@param [in] msg The message to broadcast.
		*/
		void broadcastMessage(const char* msg);
		/**
			Send a message to a single client.
			
			@param client The client to send the message to.
			@param msg The message to send.
		*/
		void sendMessageTo(RemoteClient* client, const char* msg);
		/**
			Get a list of the names of all clients.
			
			@return The list of client names.
		*/
		std::list<std::string> getClientNames();
		/**
			Search for a client with the given name.
			
			@param name The client's name.
			@return The client with the name, NULL if none exists.
		*/
		RemoteClient* getClientByName(std::string name);
		/**
			Set the name of one of the clients.
			
			@param client the client issuing the command.
			@param name The new name.
		*/
		void setClientName(RemoteClient* client, std::string name);
		
		/**
			Parse and execute a command given by a client.
			
			@param client The client issuing the command.
			@param msg The command to execute.
		*/
		virtual void handleClientCommand(RemoteClient* client, const char* msg);
		/**
			Parse and execute a command given by the server.
			
			@param msg The command to execute.
		*/
		virtual void handleServerCommand(const char* msg);
		/**
			Execute a command if valid and by authorized entity.
			
			@param cmd The command string.
			@param args The arguments string.
			@param source The source who called the command.
		*/
		virtual void handleCommand(const char* rawCommand, CommandSource* source);
};

#endif
