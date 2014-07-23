#include "Thread.hpp"

#include <stdarg.h>
#include <windef.h>
#include <winnt.h>
#include <winbase.h>

void _threadStartHack(Thread* self);

typedef struct Thread::_implThread{
	HANDLE handle;
	Thread::ThreadState state;
	bool failed;
} implThread;

Thread::Thread(){
	members_ = new implThread;
	members_->state = STOPPED;
	members_->failed = false;
}
Thread::~Thread(){
	//terminate();
	delete(members_);
}

Thread::ThreadState Thread::getState(){
	return members_->state;
}
bool Thread::isFailed(){
	return members_->failed;
}

void Thread::start(){
	if(members_->state == STOPPED){
		members_->handle = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)(_threadStartHack),
			(void*)this,
			0,
			0);
		members_->state = RUNNING;
	}
}
void Thread::suspend(){
	if(members_->state == RUNNING){
		SuspendThread(members_->handle);
		members_->state = SUSPENDED;
	}
}
void Thread::resume(){
	if(members_->state == SUSPENDED){
		ResumeThread(members_->handle);
		members_->state = RUNNING;
	}
}
void Thread::terminate(){
	if(members_->state != STOPPED){
		if(TerminateThread(members_->handle, DWORD(0))){
			members_->state = STOPPED;
		}
	}
}
void Thread::restart(){
	terminate();
	start();
}

// A static method as an entry point, calls onStartThread()
void _threadStartHack(Thread* self){
	self->onStartThread();
}
