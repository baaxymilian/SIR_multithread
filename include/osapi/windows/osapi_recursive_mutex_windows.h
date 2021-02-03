#ifndef OSAPI_RECURSIVE_MUTEX_WINDOWS_H
#define OSAPI_RECURSIVE_MUTEX_WINDOWS_H

class RecursiveMutex : public MutexInterface
{
private:
	HANDLE ghMutex;


public:
	RecursiveMutex() {
		this->ghMutex = CreateMutexW(
		  NULL,
		  TRUE,
		  NULL
		);
	}
	virtual ~RecursiveMutex() {
		 if(this->ghMutex != NULL)
			 CloseHandle(ghMutex);
	}
	virtual bool lock(unsigned int timeout) {
		int state = WaitForSingleObject(this->ghMutex, timeout);
		if(state == WAIT_OBJECT_0)
			return true;
		else
			return false;
	}
	virtual void unlock() {
		ReleaseMutex(this->ghMutex);
	}
};


#endif // OSAPI_RECURSIVE_MUTEX_WINDOWS_H
