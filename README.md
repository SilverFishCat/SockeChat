#SOCKEchat V1.2

##Unencrypted socket based chat

Dependencies: make (`mingw32_make.exe`) and g++, only on windows.

The target at bin/, server.exe and client.exe, are respectively the server and client of SOCKEchat. Run `server.exe` on a target machine to run the server and run `client.exe [server ip]` on another machine to connect to that server.

The utility class Socket and ListeningSocket are used to create sockets, and the Thread class implements a thread, to be used for receiving and sending messages seamlessly.

You can subclass the server class to implement new commands and change the MOTD, to even improve it.
