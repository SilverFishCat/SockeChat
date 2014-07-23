#ifndef __MESSAGE_RECEIVING_THREAD
#define __MESSAGE_RECEIVING_THREAD

#include "Thread.hpp"
class MessageReceiver;

/**
	A thread that handle incoming data over a socket.
*/
class MessageReceivingThread : public Thread{
	private:
		// Members
		MessageReceiver* msgReceiver_;
	
	public:
		// Constructors
		/**
			Create a thread that receives message.
			
			@param socket The socket to receive messages with.
		*/
		MessageReceivingThread(MessageReceiver* messageReceiver);
		
		// Props
		/**
			Get the socket receiving messages.
			
			@return The socket receiving messages.
		*/
		MessageReceiver* messageReceiver();
		/**
			Set the socket receiving messages.
			
			@param socket The socket receiving messages.
		*/
		void setMessageReceiver(MessageReceiver* messageReceiver);
		
		// Methods
		virtual void onStartThread(); // do on thread start
};

#endif
