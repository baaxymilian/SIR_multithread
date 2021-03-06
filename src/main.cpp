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

#define NUMBER_OF_THREADS 2
// 2D matrix is SIZE * SIZE
#define SIZE 500


static volatile int threadN = 0;
static volatile bool slowMode = false;
static volatile bool startFlag = false;
static int rows = SIZE / NUMBER_OF_THREADS;
static int iterations = 1;
static double total_time = 0;
char text_buff[200];

sir::Population pop(SIZE);

class ComputeThread : public osapi::MortalThread {
public:
	ComputeThread() : MortalThread(3, 512) {
		this->id = threadN;
		threadN++;

		//assing rows according to thread's id
		this->startRow = this->id*rows;
		if((this->id + 1) % NUMBER_OF_THREADS > 0){
			this->stopRow = (this->id+1)*rows;
		}else{
			// Last thread takes additional remaining rows
			// If number of threads is greater than number of rows, only one thread is works
			this->stopRow = (this->id+1)*rows + (SIZE % NUMBER_OF_THREADS);
		}
}
	int id;
	int startRow;
	int stopRow;

private:
	virtual void begin(){
			//std::cout << "MY SLICE: " << startRow << " " << stopRow << std::endl;
	}

	virtual void loop() {
			//compute interation and suspend after it is done
			pop.updateRange(startRow*SIZE, stopRow*SIZE);
			this->suspend();
	}

	virtual void end(){

	}
};

ComputeThread *threads =  new ComputeThread[NUMBER_OF_THREADS];

/*
	Edit this vector to change input: infected cells
*/
auto initializeCells() -> void{
	std::vector<int> infected_list = {13, 35, 88, 101, 115, 140};
	pop.changeCells(infected_list);
}

auto countAverageTime(double average) ->double{
	total_time += average;
	return total_time/iterations;
}

auto idleCallback() -> void{
	if(startFlag){
    		auto startTime = std::chrono::system_clock::now();
			
			//resume threads to compute new iteration
			for(auto i = 0; i < NUMBER_OF_THREADS; i++){
				threads[i].resume();
		    }
			sir::updateWindow(SIZE, pop);

			//measure time
			auto endTime = std::chrono::system_clock::now();
			auto iteration_duration = std::chrono::duration <double, std::milli>(endTime - startTime).count();
			auto average_time = countAverageTime(iteration_duration);

			//display info in UI window
			sprintf(text_buff, "Average computing time: %0.2f ms", average_time);
			sir::displayText(text_buff, -0.5, 0.9);
			sprintf(text_buff, "Iterations: %d", iterations);
			sir::displayText(text_buff, 0.4, 0.9);
			sprintf(text_buff, "Number of threads: %d", NUMBER_OF_THREADS);
			sir::displayText(text_buff, -0.2, -0.9);

			//go to sleep if slow mode enabled
			iterations++;
			if(slowMode)
				Sleep(200);
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
	/* START in slow mode pressing 'S' key, but only once */
    	if(startFlag == false){
			slowMode = true;
        	startFlag = true;
        	for(int i = 0; i < NUMBER_OF_THREADS; i++){
        		threads[i].run();
        	}
        	break;
    	}
    }
    case '\x72':
    {
	/* START in rapid mode pressing 'R' key, but only once */
    	if(startFlag == false){
			slowMode = false;
        	startFlag = true;
        	for(int i = 0; i < NUMBER_OF_THREADS; i++){
        		threads[i].run();
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
