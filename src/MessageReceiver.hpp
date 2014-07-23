#ifndef __MESSAGE_RECEIVER_HPP
#define __MESSAGE_RECEIVER_HPP

class Socket;

/**
	Gets messages from a socket and does something with them.
*/
class MessageReceiver{
	private:
		// Members
		Socket* socket_;
		bool ignoreBlackLines_;
	
	public:
		// Properties
		/**
			Get the socket receiving the messages.
			
			@return the socket receiving message
		*/
		Socket* socket();
		/**
			Set the socket receiving messages.
			
			@param socket The socket receiving messages
		*/
		void setSocket(Socket* socket);
		/**
			Is this thread ignoring blank lines.
			
			@return True if ignoring black lines false otherwise
		*/
		bool isIgnoringBlackLines();
		/**
			Set whether to ignore blank lines.
			
			@param ignoreBlankLines Whether to ignore blank lines
		*/
		void setIgnoreBlankLines(bool ignoreBlackLines);
		
		// Constructors
		/**
			Create a message receiver.
			
			@param socket The socket receiving messages
		*/
		MessageReceiver(Socket* socket);
		virtual ~MessageReceiver();
		
		// Methods
		void run();
		/**
			Handle incoming messages.
			
			@param msg The incoming message
		*/
		virtual void handleMessage(const char* msg)=0;
		
		/**
			Creates instances of MessageReceiver.
		*/
		class Factory{
			public:
				/**
					Create an instance of MessageReceiver.
				*/
				virtual MessageReceiver* create(Socket* socket)=0;
		};
};

#endif
