/*
	File:	winCore.hpp
	Version:	2.0
	Date: 17th August 2012; 2006.
	Author:	Allan C. Milne

	Namespace:	AllanMilne
	Exposes:	WinCore.
	Requires:	IGameCore, Gametimer.

	Description:
	A class encapsulating a basic Windows framework
	with a strategy pattern used to incorporate game processing.

	You should instantiate an object of this WinCore class,
	passing the pointer to an object of a class implementing IGameCore as an argument to the constructor.

*/

#ifndef __WINCORE_HPP_2_0
#define __WINCORE_HPP_2_0

#include <windows.h>
#include <string>
using std::string;

#include "IGameCore.hpp"
#include "GameTimer.h"

namespace AllanMilne {

class WinCore {
public:

	//--- constructor requires an IGameCore object pointer for the strategy pattern.
	WinCore (IGameCore *aGame);
	//--- destructor should call the game cleanup function.
	virtual ~WinCore ();

	//--- Create the application window; 
	//--- calls the game object SetupGame() function to create game specific resources.
	//--- Returns false if any errors detected.
	bool Initialize (
		const string &aTitle,		// Text to display in title bar.
		int aWidth,					// width of back-buffer window.
		int aHeight,				// height of back-buffer window.
		bool isWindowed,			// true=windowed; false=full screen.
		HINSTANCE anHInstance		// handle to calling process.
	);

	//--- set up and start the windows message processing loop.
	//--- calls the game object ProcessGameFrame() function every time around the loop.
	//--- The game object CleanupGame() function is called when the loop terminates.
	void Start ();


private:

	HWND mHwnd;			// Handle to current window.
	GameTimer *mTimer;		// Pointer to a timing class for use in calculating delta times between frames.
	IGameCore *mGame;		// pointer to game processing object.

}; // end WinCore class.

} // end AllanMilne namespace.

#endif