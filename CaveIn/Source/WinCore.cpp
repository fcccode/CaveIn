/*
	File:	WinCore.cpp
	Version:	2.0.1
	Date: 18th August 2012; 2006.
	Author:	Allan C. Milne

	Namespace:	AllanMilne
	Exposes:	WinCore implementation.
	Requires:	IGameCore, Gametimer.

	Description:
	A basic Windows framework providing 
	*	Windows message loop handling;
	*	a game frame process function with time delta information.

	notes:
	*	Any project using this class must have its character set to multibyte, not unicode.
	*	this implementation defines the escape key to exit the application.
	*	See WinCore.hpp for behaviour and use.
	*	See IGameCore.hpp for behaviour and use of game processing.
	* See GameTimer.h for details on the timer implementation used.

	this model  is intended as a basic framework for building your own Windows applications.
	You can, of course, use your own Windows framework to create these applications, this is a simple example and starting point.

*/

#include <windows.h>

#include "WinCore.hpp"
#include "IGameCore.hpp"
#include "Gametimer.h"

namespace AllanMilne {

//=== Windows message processing.
//=== Escape key terminates application.
LRESULT CALLBACK WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch( msg ){
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		//--- check for escape key; if pressed then terminate application.
		if( wParam == VK_ESCAPE )
			DestroyWindow(hwnd);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
} // end WndProc function.

//=== Constructor to set up strategy pattern.
WinCore::WinCore (IGameCore *aGame){
	mGame = aGame;
	mHwnd = NULL;
	mTimer = new GameTimer ();
} // end WinCore constructor.

//=== Destructor.
//=== Notes:
//=== 1. we do not call mGame->CleanupGame() since this is called in the Start() method.
//=== 2. We do not delete the mGame field since this object was not responsible for creating it - it was passed to it in the constructor.
WinCore::~WinCore (){
	if (mTimer != NULL){
		delete mTimer;
		mTimer = NULL;
	}
} // end WinCore destructor.

//=== Create the application window; returns false if any errors detected.
//=== Calls the SetupGame() function via the strategy pattern; and resets the timer.
bool WinCore::Initialize (
		const string &aTitle,		// Text to display in title bar.
		int aWidth,					// width of back-buffer window.
		int aHeight,				// height of back-buffer window.
		bool isWindowed,			// true=windowed; false=full screen.
		HINSTANCE anHInstance		// handle to calling process.
		)
{
	const char *title = aTitle.c_str();
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = anHInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = (LPCSTR)title;
	if (!RegisterClass(&wc) ) {
		MessageBox (NULL, TEXT ("Error registering application class with Windows in WinCore."), TEXT ("RegisterClass() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	HWND hwnd = 0;
	hwnd = CreateWindow ( (LPCSTR)title, (LPCSTR)title, 
		WS_EX_TOPMOST,
		0, 0, aWidth, aHeight,
		0 /*parent hwnd*/, 0 /* menu */, anHInstance, 0 /*extra*/); 
	if( !hwnd ){
		MessageBox(NULL, TEXT ("Error creating window in WinCore."), TEXT ("CreateWindow() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	mHwnd = hwnd;
	ShowWindow (mHwnd, SW_SHOW);
	UpdateWindow (mHwnd);

	//=== Window is now created so allocate game resources.
	bool ok;
	ok = mGame->SetupGame (mHwnd);
	if (!ok) {
		MessageBox (NULL, TEXT ("Error setting up game from WinCore."), TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	mTimer->Reset ();		// start timing.
	return true;
} // end Initialize function.


//===  Encapsulates calling of the windows message processing function.
//=== Calls the game object ProcessGameFrame() function every time around the messsage loop.
//=== The game object Cleanup() function is called when the message loop is terminated.
void WinCore::Start (){
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	//--- The Windows message processing loop.
	while (msg.message != WM_QUIT){
		if (PeekMessage (&msg, 0, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			// get elapsed time for the current call.
			mTimer->Tick ();
			//--- call the game frame processing function. ---
			mGame->ProcessGameFrame (mTimer->GetDeltaTime());
			if(mGame->FinishedGame()==true){
				DestroyWindow(mHwnd);
			}
		}
	} // end while loop.
	//--- clean up game-wide resources we have allocated.
	mGame->CleanupGame ();
} // end Start function.

} // end AllanMilne namespace.