#ifndef __THREAD_HPP
#define __THREAD_HPP

/**
	A class for abstracting threads.
	
	Implement the method onStartThread with what you want the thread to do.
*/
class Thread;

class Thread{
	protected:
		struct _implThread;
		_implThread *members_;
		
		// Methods
		/**
			The method to run when the thread starts.
		*/
		virtual void onStartThread()=0;
		
		friend void _threadStartHack(Thread* self); // A static method as an entry point, runs onStartThread()
		
	public:
		// Types
		/**
			A state of  a single thread.
		*/
		enum ThreadState{
			STOPPED,
			RUNNING,
			SUSPENDED
		};
		
		// Constructors
		/**
			Create a thread object but don't start it.
		*/
		Thread();
		virtual ~Thread();
		
		// Props
		/**
			Get the state of the thread.
			
			@return The state of the thread.
		*/
		ThreadState getState();
		//TODO: remove when implemented
		/**
			Check if the thread failed. (Not implemented)
		*/
		bool isFailed();
		
		// Thread control methods
		/**
			Start the thread if it is stopped.
		*/
		virtual void start();
		/**
			Suspend the thread, allowing the user to resume it at some point.
		*/
		virtual void suspend();
		/**
			Resume the thread if suspended.
		*/
		virtual void resume();
		/**
			Stop the thread's execution if the thread is not stopped.
		*/
		virtual void terminate();
		/**
			Restarts the thread even if it is currently not stopped.
		*/
		virtual void restart();
};

#endif
