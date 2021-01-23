#include "osapi.h"
#include <iostream>
#include <cstdint>
#include <chrono>
#include <vector>
#include "display.h"
#include "cell.h"
#include "population.h"
#include <windows.h>


#define NUMBER_OF_THREADS 2
#define SIZE 16


static volatile int threadN;
static volatile int sum;
static volatile bool endFlag;

sir::Population pop(SIZE);

const COLORREF rgbRed   =  0x000000FF;
const COLORREF rgbGreen =  0x0000FF00;
const COLORREF rgbBlue  =  0x00FF0000;

class ComputeThread : public osapi::Thread {
public:
	ComputeThread() : Thread(2, 512, osapi::NOT_JOINABLE) { this->number = threadN; threadN++;}
	int number;
private:
	virtual void body() {
		std::cout << threadN;
	}
};

class MainThread : public osapi::Thread {
public:
	MainThread() : Thread(3, 512, osapi::NOT_JOINABLE) { ; }
private:
	virtual void body() {
		ComputeThread *c = new ComputeThread[NUMBER_OF_THREADS];
		for(int i = 0; i < NUMBER_OF_THREADS; i++){
			c[i].run();
		}
		Sleep(1000);
	    endFlag = true;
	}
};


auto initialize() -> void{
	std::vector<int> infected_list = {3, 5, 8, 33, 115, 23};
	pop.changeCells(infected_list);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+2));


            RECT rcItem;
            rcItem.left = 50;
            rcItem.right = 70;
            rcItem.top = 50;
            rcItem.bottom = 70;

            SelectObject(ps.hdc, GetStockObject(DC_BRUSH));

            auto cell = sir::Cell();
            for(auto i = 0; i < SIZE*SIZE; i++){
            	cell = pop.getCell(i);
            	switch(cell.getState()){
        		case sir::susceptible:
        			SetDCBrushColor(hdc, rgbBlue);
        			break;
        		case sir::infectous:
        			SetDCBrushColor(hdc, rgbRed);
        			break;
        		case sir::recovered:
        			SetDCBrushColor(hdc, rgbGreen);
                    break;
        		default:
        			break;
            	}
            	Rectangle(hdc, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);

                rcItem.left += 25;
                rcItem.right += 25;
                if((i + 1) % SIZE == 0){
                    rcItem.left = 50;
                    rcItem.right = 70;
                    rcItem.top += 25;
                    rcItem.bottom += 25;
                }

            }

            EndPaint(hwnd, &ps);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



auto WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) -> int {

    // save start time
    auto startTime = std::chrono::system_clock::now();

    initialize();

    // Register the window class.
    const char CLASS_NAME[]  = "Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "SIR Multithread",    			// Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);



    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    for(auto i = 0; i < 100; i++){
        pop.updateRange(0, SIZE*SIZE - 1);
    }


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

LRESULT CALLBACK winproc(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp)
{
    return DefWindowProc(hwnd, wm, wp, lp);
}
