OBJDIR = obj
SRCDIR = src
CXX = g++
FLAGS = -pg -Wall -g
LIBFLGS = -lws2_32
BINDIR = bin

all: create-dir-tree client server
rebuild: clear all

create-dir-tree: $(OBJDIR) $(BINDIR)
$(OBJDIR):
	mkdir $@ -p
$(BINDIR):
	mkdir $@ -p

CLIENT_OBJS = $(OBJDIR)/Socket.o $(OBJDIR)/Client.o $(OBJDIR)/Thread.o $(OBJDIR)/MessageReceivingThread.o $(OBJDIR)/MessageReceiver.o
SERVER_OBJS = $(OBJDIR)/Socket.o $(OBJDIR)/Server.o $(OBJDIR)/Thread.o $(OBJDIR)/RemoteClientThread.o $(OBJDIR)/RemoteClient.o $(OBJDIR)/ConnectionCreator.o $(OBJDIR)/ListeningSocket.o $(OBJDIR)/MessageReceiver.o $(OBJDIR)/CommandSource.o $(OBJDIR)/Command.o $(OBJDIR)/commands.o

CLIENT_EXEC = $(BINDIR)/client.exe
client: $(CLIENT_EXEC)
$(CLIENT_EXEC): $(CLIENT_OBJS)
	$(CXX) $(FLAGS) $(CLIENT_OBJS) -o $(CLIENT_EXEC) $(LIBFLGS)
SERVER_EXEC = $(BINDIR)/server.exe
server: $(SERVER_EXEC)
$(SERVER_EXEC): $(SERVER_OBJS)
	$(CXX) $(FLAGS) $(SERVER_OBJS) -o $(SERVER_EXEC) $(LIBFLGS)

$(OBJDIR)/Socket.o: $(SRCDIR)/Socket.cpp $(SRCDIR)/Socket.hpp
	$(CXX) $(FLAGS) -c $(SRCDIR)/Socket.cpp -o $@
$(OBJDIR)/ListeningSocket.o: $(SRCDIR)/ListeningSocket.cpp $(SRCDIR)/ListeningSocket.hpp $(OBJDIR)/Socket.o
	$(CXX) $(FLAGS) -c $(SRCDIR)/ListeningSocket.cpp -o $@
$(OBJDIR)/Client.o: $(SRCDIR)/Client.cpp $(OBJDIR)/Thread.o $(OBJDIR)/Socket.o $(OBJDIR)/MessageReceivingThread.o
	$(CXX) $(FLAGS) -c $(SRCDIR)/Client.cpp -o $@
$(OBJDIR)/Thread.o: $(SRCDIR)/Thread.cpp $(SRCDIR)/Thread.hpp
	$(CXX) $(FLAGS) -c $(SRCDIR)/Thread.cpp -o $@
$(OBJDIR)/MessageReceivingThread.o: $(SRCDIR)/MessageReceivingThread.cpp $(SRCDIR)/MessageReceivingThread.hpp $(OBJDIR)/Socket.o $(OBJDIR)/Thread.o $(OBJDIR)/MessageReceiver.o
	$(CXX) $(FLAGS) -c $(SRCDIR)/MessageReceivingThread.cpp -o $@
$(OBJDIR)/Server.o: $(SRCDIR)/Server.cpp $(SRCDIR)/Server.hpp $(OBJDIR)/Thread.o $(OBJDIR)/Socket.o $(OBJDIR)/RemoteClient.o $(OBJDIR)/MessageReceiver.o $(OBJDIR)/ConnectionCreator.o $(OBJDIR)/ListeningSocket.o $(OBJDIR)/RemoteClientThread.o $(OBJDIR)/CommandSource.o $(OBJDIR)/Command.o $(OBJDIR)/commands.o
	$(CXX) $(FLAGS) -c $(SRCDIR)/Server.cpp -o $@
$(OBJDIR)/MessageReceiver.o: $(SRCDIR)/MessageReceiver.cpp $(SRCDIR)/MessageReceiver.hpp $(OBJDIR)/Socket.o
	$(CXX) $(FLAGS) -c $(SRCDIR)/MessageReceiver.cpp -o $@
$(OBJDIR)/ConnectionCreator.o: $(SRCDIR)/ConnectionCreator.cpp $(SRCDIR)/ConnectionCreator.hpp $(OBJDIR)/Thread.o $(OBJDIR)/RemoteClientThread.o $(OBJDIR)/Socket.o $(OBJDIR)/MessageReceiver.o
	$(CXX) $(FLAGS) -c $(SRCDIR)/ConnectionCreator.cpp -o $@
$(OBJDIR)/RemoteClient.o: $(SRCDIR)/RemoteClient.cpp $(SRCDIR)/RemoteClient.hpp $(OBJDIR)/Socket.o $(OBJDIR)/ListeningSocket.o $(OBJDIR)/MessageReceiver.o $(OBJDIR)/CommandSource.o
	$(CXX) $(FLAGS) -c $(SRCDIR)/RemoteClient.cpp -o $@
$(OBJDIR)/RemoteClientThread.o: $(SRCDIR)/RemoteClientThread.cpp $(SRCDIR)/RemoteClientThread.hpp $(OBJDIR)/RemoteClient.o
	$(CXX) $(FLAGS) -c $(SRCDIR)/RemoteClientThread.cpp -o $@
$(OBJDIR)/CommandSource.o: $(SRCDIR)/CommandSource.cpp $(SRCDIR)/CommandSource.hpp 
	$(CXX) $(FLAGS) -c $(SRCDIR)/CommandSource.cpp -o $@
$(OBJDIR)/Command.o: $(SRCDIR)/Command.cpp $(SRCDIR)/Command.hpp $(OBJDIR)/CommandSource.o
	$(CXX) $(FLAGS) -c $(SRCDIR)/Command.cpp -o $@
$(OBJDIR)/commands.o: $(SRCDIR)/commands.cpp $(SRCDIR)/commands.hpp $(OBJDIR)/Command.o $(OBJDIR)/RemoteClient.o
	$(CXX) $(FLAGS) -c $(SRCDIR)/commands.cpp -o $@

clear:
	rm -f $(CLIENT_OBJS) $(SERVER_EXEC)
	rm -f $(SERVER_OBJS) $(CLIENT_EXEC)
