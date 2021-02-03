#ifndef OSAPI_MUTEX_WINDOWS_H
#define OSAPI_MUTEX_WINDOWS_H


class Mutex : public MutexInterface
{
private:
	HANDLE ghMutex;
	bool MutexState;

public:
	Mutex() {
		this->MutexState = false;
		this->ghMutex = CreateMutexW(
		  NULL,
		  FALSE,
		  NULL
		);
	}
	virtual ~Mutex() {
		 if(this->ghMutex != NULL)
			 CloseHandle(ghMutex);
	}
	virtual bool lock(unsigned int timeout) {
		if(this->MutexState == false){
			int state = WaitForSingleObject(this->ghMutex, timeout);
			this->MutexState = true;
			if(state == WAIT_OBJECT_0)
				return true;
			else
				return false;
		}else
			return false;
	}

	virtual void unlock() {
		if(this->MutexState == true){
			ReleaseMutex(this->ghMutex);
			this->MutexState = false;
		}

	}
};



#endif // OSAPI_MUTEX_WINDOWS_H
