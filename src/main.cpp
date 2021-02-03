#include "osapi/osapi.h"
#include <iostream>
#include <cstdint>
#include <chrono>
#include <vector>
#include "display.h"
#include "cell.h"
#include "population.h"
#include <stdlib.h>
#include <GL/glut.h>

#define NUMBER_OF_THREADS 1
#define SIZE 200

static volatile int threadN = 0;
static volatile bool startFlag;
static int rows = SIZE / NUMBER_OF_THREADS;


sir::Population pop(SIZE);

class MyMutex: public osapi::RecursiveMutex {

};

MyMutex mut;

class ComputeThread : public osapi::MortalThread {
public:
	ComputeThread() : MortalThread(3, 512) {
		this->id = threadN;
		threadN++;
		this->startRow = this->id*rows;
		if((this->id + 1) % NUMBER_OF_THREADS > 0){
			this->stopRow = (this->id+1)*rows;
		}else{
			// Last thread takes additional remainder rows
			// If number of threads is greater than number of rows, only one thread is works
			this->stopRow = (this->id+1)*rows + (SIZE % NUMBER_OF_THREADS);
		}
}
	int id;
	int startRow;
	int stopRow;

private:
	virtual void begin(){

			std::cout << "MY SLICE: " << startRow << " " << stopRow << std::endl;
	}

	virtual void loop() {
			pop.updateRange(startRow*SIZE, stopRow*SIZE);
			this->suspend();
	}

	virtual void end(){

	}
};

ComputeThread *c =  new ComputeThread[NUMBER_OF_THREADS];

auto initializeCells() -> void{
	std::vector<int> infected_list = {13, 35, 88, 101, 115, 140};
	pop.changeCells(infected_list);
}

auto idleCallback() -> void{
	if(startFlag){
		if(mut.lock(10000)){

			for(auto i = 0; i < NUMBER_OF_THREADS; i++){
				c[i].resume();
		    }
			//Sleep(1000);
		    mut.unlock();
			sir::updateWindow(SIZE, pop);
		}
	}

}

auto keyboardCallback(unsigned char key, int x, int y) -> void{
  switch (key)
  {
    /* Exit on escape key press */
    case '\x1B':
    {
      exit(EXIT_SUCCESS);
      break;
    }
    case '\x73':
    {
	/* START on 's' key, but only once */
    	if(startFlag == false){
        	startFlag = true;
        	for(int i = 0; i < NUMBER_OF_THREADS; i++){
        		c[i].run();
        	}
        	break;
    	}
    }

  }
}

auto displayCallback() -> void
{
	//This callback is empty
}


auto main(int argc, char** argv) -> int {
    glutInit(&argc, argv);
    initializeCells();

    sir::displayWindow("SIR", 1200, 900);

	glutKeyboardFunc(&keyboardCallback);
	glutDisplayFunc(&displayCallback);
	glutIdleFunc(&idleCallback);
	glutMainLoop();
    return 0;
}
