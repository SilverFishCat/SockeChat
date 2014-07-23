#include "commands.hpp"

#include "Server.hpp"
#include "CommandSource.hpp"
#include "RemoteClient.hpp"
#include <iostream>
#include <sstream>

const std::string ExitCommand::NAME = "close";
const std::string ExitCommand::HELP = "closes the server";
ExitCommand::~ExitCommand(){}
std::string ExitCommand::getName(){
	return NAME;
}
std::string ExitCommand::getHelp(){
	return HELP;
}
void ExitCommand::act(std::string* args, CommandSource* source){
	if(!source->isConsole()){
		std::stringstream notice;
		notice << "Can only close server through server console" << std::endl;
		source->printToSource(notice.str());
	}
	else{
		std::cout << "Stopping server polling" << std::endl;
		source->getServer()->setShouldClose(true);
	}
}

const std::string HelpCommand::NAME = "help";
const std::string HelpCommand::HELP = "shows this help message";
HelpCommand::~HelpCommand(){}
std::string HelpCommand::getName(){
	return NAME;
}
std::string HelpCommand::getHelp(){
	return HELP;
}
void HelpCommand::act(std::string* args, CommandSource* source){
	commandList commands = source->getServer()->getCommandList();
	std::stringstream helpLine;
	
	for(commandList::iterator it = commands.begin();
			it != commands.end();
			it++){
		Command* command = *it;
		helpLine << command->getName() << " - " << command->getHelp() << std::endl;
	}
	
	source->printToSource(helpLine.str());
}

const std::string ShoutCommand::NAME = "shout";
const std::string ShoutCommand::HELP = "broadcast a message to all clients";
ShoutCommand::~ShoutCommand(){}
std::string ShoutCommand::getName(){
	return NAME;
}
std::string ShoutCommand::getHelp(){
	return HELP;
}
void ShoutCommand::act(std::string* args, CommandSource* source){
	if(!source->isConsole()){
		// ignore
	}
	else{
		if(args != NULL){
			std::stringstream formatstream;
			formatstream << "CONSOLE: " << *args << std::endl;
			std::cout << "Shouting message" << std::endl;
			source->getServer()->broadcastMessage(formatstream.str().c_str());
		}
		else{
			std::cout << "Need to supply a message to shout" << std::endl;
		}
	}
}

const std::string WhisperCommand::NAME = "whisper";
const std::string WhisperCommand::HELP = "send a message to one client (client_name message)";
WhisperCommand::~WhisperCommand(){}
std::string WhisperCommand::getName(){
	return NAME;
}
std::string WhisperCommand::getHelp(){
	return HELP;
}
void WhisperCommand::act(std::string* args, CommandSource* source){
	if(args == NULL){
		// Notify sender
		std::stringstream notice;
		notice << "Need to supply to client's name" << std::endl;
		source->printToSource(notice.str());
	}
	else{
		std::string targetName, *message;
		split(*args, &targetName, &message, ' ');
		if(message == NULL){
			// Notify sender
			std::stringstream notice;
			notice << "Need to supply a message" << std::endl;
			source->printToSource(notice.str());
		}
		else{
			RemoteClient* target = source->getServer()->getClientByName(targetName);
			if(target == NULL){
				// Notify sender
				std::stringstream notice;
				notice << "Client not found" << std::endl;
				source->printToSource(notice.str());
			}
			else{
				std::string sourceName;
				if(!source->isConsole()){
					sourceName = source->getClient()->getName();
				}
				else{
					sourceName = "CONSOLE";
				}
				
				std::stringstream msgFormat;
				msgFormat << sourceName << "(shh): " << *message << std::endl;
				std::string formattedString = msgFormat.str();
				source->getServer()->sendMessageTo(target, formattedString.c_str());
				
				std::cout << sourceName << " whispered to " << target->getName() << std::endl;
			}
			
			delete message;
		}
	}
}

const std::string NickCommand::NAME = "nick";
const std::string NickCommand::HELP = "change name of client";
NickCommand::~NickCommand(){}
std::string NickCommand::getName(){
	return NAME;
}
std::string NickCommand::getHelp(){
	return HELP;
}
void NickCommand::act(std::string* args, CommandSource* source){
	RemoteClient* target = NULL;
	std::string oldName, newName;
	if(source->isConsole()){
		if(args == NULL){
			// Notify sender
			std::stringstream notice;
			notice << "Need to supply a target client" << std::endl;
			source->printToSource(notice.str());
		}
		else{
			split(*args, &oldName, &args, ' ');
			target = source->getServer()->getClientByName(oldName);
			if(target == NULL){
				// Notify sender
				std::stringstream notice;
				notice << "Client not found" << std::endl;
				source->printToSource(notice.str());
			}
		}
	}
	else{
		target = source->getClient();
		oldName = target->getName();
	}
	
	if(target != NULL){
		if(args == NULL){
			// Notify sender
			std::stringstream notice;
			notice << "Need to supply a new nickname" << std::endl;
			source->printToSource(notice.str());
		}
		else{
			split(*args, &newName, &args, ' ');
			source->getServer()->setClientName(target,newName);
			
			std::stringstream nameChangeNotice;
			nameChangeNotice << oldName << " changed name to " << newName << std::endl;
			source->getServer()->broadcastMessage(nameChangeNotice.str().c_str());
		}
	}
}

const std::string ListCommand::NAME = "list";
const std::string ListCommand::HELP = "list the clients connected to the server";
ListCommand::~ListCommand(){}
std::string ListCommand::getName(){
	return NAME;
}
std::string ListCommand::getHelp(){
	return HELP;
}
void ListCommand::act(std::string* args, CommandSource* source){
	std::stringstream list;
	list << "Client list:" << std::endl;
	std::list<std::string> names = source->getServer()->getClientNames();
	for(std::list<std::string>::iterator it = names.begin();
			it != names.end();
			it++){
		list << *it << std::endl;
	}
	source->printToSource(list.str());
}
