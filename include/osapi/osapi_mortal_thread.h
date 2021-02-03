#ifndef OSAPI_MORTAL_THREAD_H
#define OSAPI_MORTAL_THREAD_H

class MortalThread : public Thread
{

  public:
    MortalThread(int priority, unsigned int stackSize, const char* name = "unnamed") : Thread(priority, stackSize, JOINABLE, name) {
			this->killSignal = 0;
		}

    virtual ~MortalThread() {
    }

    /** Sends termination signal to the thread. */
    void kill() {
				this->killSignal = 1;
    }

  private:
			sig_atomic_t killSignal;


    /** Implementation of the body method */
    virtual void body(void) {
				begin();
				while(this->killSignal == 0){
					loop();
				}
				end();
    }

    protected:

        virtual void begin() = 0;

        virtual void loop() = 0;

        virtual void end() = 0;
};

#endif // OSAPI_MORTAL_THREAD_H
