/*
	File:	Main_Soundscape1.cpp
	Version:	2.2
	Date: 17th December 2012.
	Author:	Allan C. Milne

	Requires:	AllanMilne::WinCore, AllanMilne::IGameCore.

	Description:
	A skeleton for a main code file containing the Windows application entry point
	using the WinCore framework abstraction.

	Edit this program by finding the '//xxx\ comments and
	*	add 'Game = new MyClass ();' - where MyClass is your concrete game class.
	*	Change Initialize(...) parameters for window title text, size, and full/windowed flag.

	Define a concrete class ('MyClass' in the above note) that implements (inherits) the IGameCore interface.

	Notes: 
	*	See WinCore.hpp for details of the framework abstraction and use of strategy pattern.
	*	See IGameCore.hpp for the game processing logic within the strategy pattern.
	* see //xxx comments for where you can tailor this program to your own requirements.

*/

#include <windows.h>

#include "WinCore.hpp"
#include "IGameCore.hpp"
using AllanMilne::WinCore;
using AllanMilne::IGameCore;
#include "Soundscape.hpp"
#include "SoundCues.hpp"
//xxx Add #include for your class header file.

//=== Application entry point.
int WINAPI WinMain (HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	WinCore *windowApp;
	IGameCore *game;

	//xxx following line should be uncommented and XXX replaced with your game processing class (that implements IGameCore).
	//game = new Soundscape();
	game = new SoundCues();
	windowApp = new WinCore (game);

	//--- Create the application window and application-wide resources.
	//xxx amend parameters to reflect your application.
	bool ok = windowApp->Initialize (
				"Cave In",		//xxx Windows title bar text.
				800, 600,			//xxx width x height
				false,				//xxx use full screen; change to true if windowed is required.
				hinstance );
	if (!ok) {
		MessageBox (NULL, TEXT ("Error occurred while initializing WinCore; application aborted."), TEXT ("Initialize - FAILED"), MB_OK | MB_ICONERROR );
		return 0;
	}

	//--- start the Windows message loop and associated game frame processing.
	windowApp->Start ();

	// Delete WinCore and game objects; calls their destructor methods.
	delete windowApp;
	windowApp = NULL;
	delete game;
	game = NULL;
	return 0;
} // end WinMain function.

// end of code.