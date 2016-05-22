#include "Server.hpp"

#include "Socket.hpp"
#include "ListeningSocket.hpp"
#include "RemoteClient.hpp"
#include "MessageReceiver.hpp"
#include "ConnectionCreator.hpp"
#include "CommandSource.hpp"
#include "Command.hpp"
#include "commands.hpp"
#include <iostream>
#include <sstream>

const int Server::BACKLOG = 10;
const std::string Server::MOTD = "Welcome to the SockeChat server v1.2!";

class HandleClientMessage : public ClientMessageReceiver{
	private:
		Server* serv_;
	public:
		HandleClientMessage(Socket *socket, Server* serv, RemoteClient* client) : 
				ClientMessageReceiver(socket, client){
			serv_ = serv;
		}
		virtual void handleMessage(char const* msg){
			if(msg[0] != '\\'){
				std::stringstream formattedString;
				formattedString << getClient()->getName() << ": " << msg << std::endl;
				serv_->broadcastMessage(formattedString.str().c_str());
			}
			else{
				std::string msgStr = msg;
				msgStr = msgStr.substr(1, msgStr.npos);
				serv_->handleClientCommand(getClient(), msgStr.c_str());
			}
		}
		class Factory : public ClientMessageReceiver::Factory{
			private:
				Server* serv_;
			
			public:
				Factory(Server* serv){
					serv_ = serv;
				}
				
				virtual ClientMessageReceiver* create(Socket* socket, RemoteClient* client){
					return new HandleClientMessage(socket,serv_,client);
				}
		};
};

class HandleClientConnection : public RemoteClientThread{
	private:
		Server* serv_;
	
	public:
		HandleClientConnection(ListeningSocket* socket, ClientMessageReceiver::Factory* factory, Server* server) :
				RemoteClientThread(socket, factory){
			serv_ = server;
		}
		virtual void onAccept(){
			std::stringstream connectionNotice;
			connectionNotice << "Client " << getRemoteClient()->getSocket()->getID() << " has connected" << std::endl;
			serv_->broadcastMessage(connectionNotice.str().c_str());
			std::cout << "sending motd to socket " << getRemoteClient()->getSocket()->getID() << std::endl;
			/*int sentbytes = */ getRemoteClient()->getSocket()->sendSocket(Server::MOTD.c_str());
		}
		class Factory : public RemoteClientThread::Factory{
			private:
				Server* serv_;
			public:
				Factory(Server* serv){
					serv_ = serv;
				}
				virtual RemoteClientThread* create(ListeningSocket* socket, ClientMessageReceiver::Factory* factory){
					return new HandleClientConnection(socket,factory,serv_);
				}
		};
};

// Props
bool Server::isUp(){
	return started_;
}
bool Server::isAcceptingCommunications(){
	return acceptingComs_;
}
ListeningSocket* Server::listeningSocket(){
	return lstSock_;
}
size_t Server::maxConnections(){
	return maxConnections_;
}
threadList* Server::clientThreads(){
	return &clientThreads_;
}
ConnectionCreator* Server::getConnectionCreator(){
	return connectionCreator_;
}
void Server::setMaxConnections(size_t maxConnections){
	maxConnections_ = maxConnections;
}
void Server::setConnectionCreator(ConnectionCreator* connectionCreator){
	connectionCreator_ = connectionCreator;
}
void Server::addCommand(Command* command){
	commands_.push_back(command);
}
void Server::setShouldClose(bool shouldClose){
	shouldExit_ = shouldClose;
}
commandList Server::getCommandList(){
	return commands_;
}

// Constructors
Server::Server(const char* port){
	started_ = false;
	lstSock_ = new ListeningSocket(port,BACKLOG);
	std::cout << "created socket" << lstSock_->getID() << std::endl;
	connectionCreator_ = new ConnectionCreator(this,
			new HandleClientConnection::Factory(this),
			new HandleClientMessage::Factory(this));
	maxConnections_ = BACKLOG;
	shouldExit_ = false;
}

// Destructors
Server::~Server(){
	connectionCreator_->suspend();
	for(threadList::iterator it = clientThreads_.begin();
			it != clientThreads_.end();
			it++){
		RemoteClientThread* thrd = *it;
		clientThreads_.erase(it);
		delete(thrd);
	}
	if(lstSock_ != NULL){
		lstSock_->close();
		delete(lstSock_);
		std::cout << "closed socket " << lstSock_->getID() << std::endl;
	}
	delete(connectionCreator_);
}

// Methods
void Server::run(){
	start();
	openCommunications();
	listenToServerCommands();
}
void Server::start(){
	lstSock_->bindSocket();
	std::cout << "socket bound" << std::endl;
	lstSock_->startListening();
	std::cout << "listening on socket" << std::endl;
}
void Server::openCommunications() throw(ServerException){
	std::cout << "connection manager starting" << std::endl;
	connectionCreator_->start();
}
void Server::closeCommunications() throw(ServerException){
	broadcastMessage("SERVER CLOSING");
	connectionCreator_->suspend();
	while(!clientThreads_.empty()){
		threadList::iterator it = clientThreads_.begin();
		RemoteClientThread* thread = *it;
		clientThreads_.erase(it);
		thread->terminate();
		delete(thread);
	}
	std::cout << "connection manager stopping" << std::endl;
	connectionCreator_->terminate();
}
void Server::stop(){
	lstSock_->close();
}
const int MAX_COMMAND = 1000;
void Server::listenToServerCommands(){
	while(!shouldExit_){
		char msg[MAX_COMMAND];
		std::cin.getline(msg,MAX_COMMAND,'\n');
		handleServerCommand(msg);
	}
}

void Server::broadcastMessage(const char* msg){
	for(threadList::iterator it = clientThreads_.begin();
			it != clientThreads_.end();
			it++){
		RemoteClientThread* thrd = *it;
		if(thrd->getRemoteClient()->getStatus() == RemoteClient::RECEIVING_MESSAGES){
			thrd->getRemoteClient()->getSocket()->sendSocket(msg);
		}
	}
	
	std::cout << msg;
}
void Server::sendMessageTo(RemoteClient* client, const char* msg){
	if(client == NULL){
		throw ServerException();
	}
	else if(client->getStatus() != RemoteClient::RECEIVING_MESSAGES){
		throw ServerException();
	}
	else{
		client->getSocket()->sendSocket(msg);
	}
}
std::list<std::string> Server::getClientNames(){
	std::list<std::string> result;
	
	for(threadList::iterator it = clientThreads_.begin();
			it != clientThreads_.end();
			it++){
		RemoteClientThread* thrd = *it;
		if(thrd->getRemoteClient()->getStatus() == RemoteClient::RECEIVING_MESSAGES){
			result.push_back(thrd->getRemoteClient()->getName());
		}
	}
	
	return result;
}
RemoteClient* Server::getClientByName(std::string name){
	RemoteClient* result = NULL;
	
	for(threadList::iterator it = clientThreads_.begin();
			it != clientThreads_.end() && result == NULL;
			it++){
		RemoteClientThread* thrd = *it;
		if(thrd->getRemoteClient()->getStatus() == RemoteClient::RECEIVING_MESSAGES){
			if(thrd->getRemoteClient()->getName() == name)
				result = thrd->getRemoteClient();
		}
	}
	
	return result;
}
void Server::setClientName(RemoteClient* client, std::string name){
	if(client == NULL){
		throw ServerException();
	}
	else if(client->getStatus() != RemoteClient::RECEIVING_MESSAGES){
		throw ServerException();
	}
	else{
		client->setName(name);
	}
}
void Server::handleClientCommand(RemoteClient* client, const char* msg){
	CommandSource* source = CommandSource::clientSource(this, client);
	handleCommand(msg,source);
	delete(source);
}
void Server::handleServerCommand(const char* msg){
	CommandSource* source = CommandSource::consoleSource(this);
	handleCommand(msg,source);
	delete(source);
}
void Server::handleCommand(const char* rawCommand, CommandSource* source){
	std::string cmd, *args = new std::string;
	Command::split(rawCommand, &cmd, &args, ' ');
	Command* toExec = NULL;
	for(commandList::iterator it = commands_.begin();
			it != commands_.end();
			it++){
		Command* command = *it;
		if(cmd == command->getName()){
			toExec = command;
			break;
		}
	}
	
	if(toExec != NULL){
		toExec->act(args, source);
	}
	else{
		std::stringstream outString;
		outString << "Unknown command: " << cmd << std::endl;
		source->printToSource(outString.str());
	}
	delete(args);
}

int main(int argc, char** args){
	Server* server = new Server("3940");
	try{
		server->addCommand(new ExitCommand());
		server->addCommand(new HelpCommand());
		server->addCommand(new ShoutCommand());
		server->addCommand(new WhisperCommand());
		server->addCommand(new NickCommand());
		server->addCommand(new ListCommand());
		server->run();
		
		server->closeCommunications();
	}
	catch(SocketException e){
		std::cerr << e.what() << std::endl;
		std::cerr << "Error was on listening socket" << std::endl;
	}
	
	WSACleanup();
}
