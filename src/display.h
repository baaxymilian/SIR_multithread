/*
 * display.h
 *
 *  Created on: 22 sty 2021
 *      Author: Admin
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <windows.h>

namespace sir{

	typedef struct _WNDCLASSEX {
		UINT cbSize;
		UINT style;
		WNDPROC lpfnWndPric; //pointer to window procedure for this window class
		int cbClsExtra;
		int cbWndExtra;
		HINSTANCE hInstance;
		HICON hIcon;
		HICON hIconSm;
		HCURSOR hCursor;
		HBRUSH hbrBackground; //the background brush for this window class
		LPCSTR lpszMenuName;
		LPCSTR lpszClassName;
	} WNDCLASSEX;




}

#endif /* DISPLAY_H_ */
