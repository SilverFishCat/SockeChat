#ifndef __COMMAND_SOURCE_HPP
#define __COMMAND_SOURCE_HPP

#include <string>
class RemoteClient;
class Server;

/**
	A source of a server command.
*/
class CommandSource{
	private:
		RemoteClient* client_;
		Server* server_;
		bool isConsole_;
		
		// Constructors
		/**
			Create a new command source.
		*/
		CommandSource(Server* server, RemoteClient* client, bool isConsole);
	
	public:
		// Props
		/**
			Get the client that sent the command.
			
			@return If sent a client, the client, else NULL
		*/
		RemoteClient* getClient();
		/**
			Is the source the console?
		*/
		bool isConsole();
		/**
			The server the command will execute on
		*/
		Server* getServer();
		
		// Methods
		/**
			Print a message to the source.
			
			@param message The message to print
		*/
		void printToSource(std::string message);
		/**
			Print a message to the source.
			
			@param message The message to print
		*/
		void printToSource(const char* message);
		
		/**
			Create a client command source.
			
			@param server The server that the command will be executed on
			@param client The client that sent the command
		*/
		static CommandSource* clientSource(Server* server, RemoteClient* client);
		/**
			Create a console command source.
			
			@param server The server that the command will be executed on
		*/
		static CommandSource* consoleSource(Server* server);
};

#endif
