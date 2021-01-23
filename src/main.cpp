#include "osapi.h"
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
#define SIZE 20

static volatile int threadN;
static volatile int sum;
static volatile bool endFlag;

sir::Population pop(SIZE);

const COLORREF rgbRed   =  0x000000FF;
const COLORREF rgbGreen =  0x0000FF00;
const COLORREF rgbBlue  =  0x00FF0000;

class ComputeThread : public osapi::Thread {
public:
	ComputeThread() : Thread(4, 512, osapi::NOT_JOINABLE) { this->number = threadN; threadN++;}
	int number;
private:
	virtual void body() {

		this->suspend();
	}
};

class MainThread : public osapi::Thread {
public:
	MainThread() : Thread(2, 512, osapi::NOT_JOINABLE) { ; }
private:
	virtual void body() {
	    sir::displayWindow();

	    ComputeThread *c = new ComputeThread[NUMBER_OF_THREADS];
		for(int i = 0; i < NUMBER_OF_THREADS; i++){
		}
		c[0].run();
	    for(auto i = 0; i < 200; i++){
	        pop.updateRange(0, SIZE*SIZE - 1);
	        Sleep(100);
			sir::updateWindow(SIZE, pop);
			Sleep(100);
	    }

	    endFlag = true;
	}
};


auto initialize() -> void{
	std::vector<int> infected_list = {3, 5, 8, 33, 115, 23};
	pop.changeCells(infected_list);
}


auto main(int argc, char** argv) -> int {
    // save start time
    auto startTime = std::chrono::system_clock::now();


    glutInit(&argc, argv);

    initialize();


    // create and run the thread
    MainThread mThread;
    mThread.run();

    // wait for the threads to finish
    while(!endFlag){

    }



    // save end time
    auto endTime = std::chrono::system_clock::now();

    // get thread result
    std::cout << "Found " << sum << " prime numbers." << std::endl;

    // print duration
    auto duration = endTime - startTime;
    std::cout << "Duration: " << std::chrono::duration <double, std::milli>(duration).count() << "ms." << std::endl;
    return 0;
}
