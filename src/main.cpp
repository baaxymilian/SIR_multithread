#include "osapi.h"
#include <iostream>
#include <cstdint>
#include <chrono>
#include <vector>
#include "display.h"
#include "cell.h"
#include "population.h"


#define NUMBER_OF_THREADS 2
#define SIZE 8


static volatile int threadN;
static volatile int sum;
static volatile bool endFlag;
sir::Population pop(SIZE);



class ComputeThread : public osapi::Thread {
public:
	ComputeThread() : Thread(2, 512, osapi::NOT_JOINABLE) { this->number = threadN; threadN++;}
	int number;
private:
	virtual void body() {
		sum++;
	}
};

class MainThread : public osapi::Thread {
public:
	MainThread() : Thread(3, 512, osapi::NOT_JOINABLE) { ; }
private:
	virtual void body() {
		ComputeThread *comp = new ComputeThread[NUMBER_OF_THREADS];
		for(int i = 0; i < NUMBER_OF_THREADS; i++){
			comp[i].run();
	    }
		Sleep(100);
	    endFlag = true;
	}
};


auto initialize() -> void{
	std::vector<int> infected_list = {3, 5, 8};
	pop.changeCells(infected_list);
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return NULL;
}


auto WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) -> int {

    // save start time
    auto startTime = std::chrono::system_clock::now();


    initialize();

    pop.print();

    for(auto i = 0; i < 1000; i++){
        pop.updateRange(0, SIZE*SIZE - 1);


    }
    pop.print();

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
