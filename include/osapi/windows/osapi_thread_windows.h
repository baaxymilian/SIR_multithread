#ifndef OSAPI_THREAD_WINDOWS_H
#define OSAPI_THREAD_WINDOWS_H

/** Thread interface implementation for Windows. */
class Thread : public ThreadInterface
{
	private:
		HANDLE  hThread;
		DWORD   ThreadId;
		SIZE_T	ThreadStackSize;
		const char* 	ThreadName;
		Joinable ThreadIsJoinable;
		int 	 ThreadPriority;

	public:
	
		/** Thread constructor.
	     *  @param[in] priority thread priority
	     *  @param[in] stackSize thread stack size in bytes
         *  @param[in] isJoinable decides if the thread supports join operation or not
         *  @param[in] name optional thread name
         */
        Thread(int priority, unsigned int stackSize, Joinable isJoinable, const char* name = "unnamed") {
        	this->ThreadIsJoinable = isJoinable;
        	this->ThreadStackSize = (SIZE_T)stackSize;
        	this->ThreadPriority = priority;
        	this->ThreadName = name;
		}
		
		/** Virtual destructor required to properly destroy derived class objects. */
		virtual ~Thread() { 
			if(this->hThread != NULL)
				CloseHandle(this->hThread);
		}
		
		/** Runs the thread.
		 *  @retval true if the thread was started successfully, 
		 *  @retval false if the thread was not started successfully, or the thread was already running
		 */
		virtual bool run() { 
        	this->hThread = CreateThread(
        	            NULL,                   // default security attributes
						this->ThreadStackSize,  // use default stack size
						threadFunction,       	// thread function name
        	            this,          			// argument to thread function
        	            0,                      // use default creation flags
        	            &(this->ThreadId));   	// returns the thread identifier
        	if(this->hThread == NULL)
        		return false;
        	else
        		return true;
		}
		
		/** Checks if the thread is running.
		 *  @retval true if the thread is running
         *  @retval false if the thread is not running
		 */
		virtual bool isRunning() { 
			if(this->hThread != NULL)
				return true;
			else
				return false;
		}	

		/** Waits for the thread to finish executing, with a given timeout.
         *  @param timeout[in] number of milliseconds to wait for the thread to finish executing
		 *  @retval true if the thread was successfully joined in the given time
		 *  @retval false if the thread was not joined within the given time or the thread is not joinable at all
		 */
		virtual bool join(unsigned int timeout) {
			if(this->ThreadIsJoinable){
				int state =  WaitForSingleObject(this->hThread, timeout);
				if(state == WAIT_OBJECT_0)
					return true;
				else
					return false;
			}
			else
				return false;
		}

		/** Checks, if the thread is joinable.
		 *  @retval true if the thread is joinable
		 *  @retval false if the thread is not joinable
		 */
		virtual bool isJoinable() {
			if(this->ThreadIsJoinable)
				return true;
			else
				return false;
		}

		/** Suspends thread execution.
		 *  @retval true if the thread was suspended successfully
		 *  @retval false if the thread was not suspended for some reason
		 */
		virtual bool suspend() {
			int state = SuspendThread(this->hThread);
			if(state < 0)
				return false;
			else
				return true;
		}

		/** Resumes thread execution.
		 *  @retval true if the thread was resumed successfully
		 *  @retval false if the thread was not resumed for some reason
		 */
		virtual bool resume() {
			int state = ResumeThread(this->hThread);
			if(state < 0)
				return false;
			else
				return true;
		}

		/** Sets thread priority
		 *  @param[in] priority new thread priority
		 *  @retval true if the priority for the thread was set successfully
		 *  @retval false if the priority for the thread was not set successfully for some reason
		 */
		virtual	bool setPriority(int priority) {
			if(SetThreadPriority(this->hThread, priority)){
				this->ThreadPriority = priority;
				return true;
			}
			else
				return false;
		}

		/** Gets the thread priority
		 *  @return current thread priority
		 */
		virtual	int getPriority() {
            return this->ThreadPriority;
        }
        
        /** Gets thread name
         *  @return name of the thread
         */
        virtual const char* getName() {
            return this->ThreadName;
        }

		static DWORD WINAPI threadFunction(LPVOID lpParam) {
				Thread* osapiThreadObject = reinterpret_cast<Thread*>(lpParam);
				if (osapiThreadObject) {
						osapiThreadObject->body();
				}
				if(osapiThreadObject->hThread != NULL)
					CloseHandle(osapiThreadObject->hThread);
		}


	protected:
		
		/** Delays thread execution for a given time.
		 *  @param time[in] number of milliseconds to delay thread execution
		 */
		virtual void delay(unsigned int time) {
			Sleep(time);
		}

		/** Causes the thread to temporarily yield execution in favor of other threads.
		 */
		virtual void yield() {
			SwitchToThread();
		}
};


#endif /* OSAPI_THREAD_WINDOWS_H */
