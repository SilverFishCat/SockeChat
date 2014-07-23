#include "CommandSource.hpp"

#include "Server.hpp"
#include <iostream>

// Properties
RemoteClient* CommandSource::getClient(){
	return client_;
}
bool CommandSource::isConsole(){
	return isConsole_;
}
Server* CommandSource::getServer(){
	return server_;
}

// Constructors
CommandSource::CommandSource(Server* server, RemoteClient* client, bool isConsole){
	isConsole_ = isConsole;
	client_ = client;
	server_ = server;
}

// Methods
void CommandSource::printToSource(std::string message){
	if(isConsole_){
		std::cout << message;
	}
	else{
		server_->sendMessageTo(client_, message.c_str());
	}
}
void CommandSource::printToSource(const char* message){
	if(isConsole_){
		std::cout << message;
	}
	else{
		server_->sendMessageTo(client_, message);
	}
}
CommandSource* CommandSource::clientSource(Server* server, RemoteClient* client){
	return new CommandSource(server, client, false);
}
CommandSource* CommandSource::consoleSource(Server* server){
	return new CommandSource(server, NULL, true);
}
