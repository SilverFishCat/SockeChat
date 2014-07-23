#include "MessageReceivingThread.hpp"
#include "MessageReceiver.hpp"
#include <iostream>

// Constructors
MessageReceivingThread::MessageReceivingThread(MessageReceiver* messageReceiver){
	setMessageReceiver(messageReceiver);
}

// Props
MessageReceiver* MessageReceivingThread::messageReceiver(){
	return msgReceiver_;
}
void MessageReceivingThread::setMessageReceiver(MessageReceiver* messageReceiver){
	msgReceiver_ = messageReceiver;
}

// Methods
void MessageReceivingThread::onStartThread(){
	msgReceiver_->run();
}
