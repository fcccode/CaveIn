/*
	File:	IGameCore.hpp
	Version:	1.1
	Date:	18th August 2012.
	Author:	Allan Milne

	Namespace:	AllanMilne
	Exposes:	IGameCore

	Description:
	This defines an interface (pure abstract class) for use in a strategy pattern
	in the context of the WinCore class.
	the interface exposes the following behaviour required of a concrete strategy class
	that is the game specific logic:
	*	bool SetupGame (HWND aWindow)
		create and initialize any game-wide resources.
		the argument is the handle to the parent window; allows a concrete strategy class accesss to the window.
		Return false if any errors are detected.
		Called from WinCore::Initialize.
	*	void ProcessGameFrame (float deltaTime)
		The logic for processing a single frame in the game.
		the parameter is the time in seconds since this function was last called.
		Called from WinCore::Start every time around the Windows message loop.
	*	void CleanupGame ()
		Release/free/delete game-wide resources allocated in the concrete strategy class.
		Called from WinCore::Start when the Windows message loop exits.

	These are all defined as pure virtual functions and your concrete strategy (game) class
	should be subclassed from this interface and implement the functions appropriately.

*/

#ifndef __IGAMECORE_HPP_1_1
#define __IGAMECORE_HPP_1_1

namespace AllanMilne {

class IGameCore {
public:

	//--- create and initialize game-wide resources.
	virtual bool SetupGame (HWND aWindow) = 0;

	//--- process a single game frame.
	virtual void ProcessGameFrame (const float deltaTime) = 0;

	//--- Release all game-wide resources that have been allocated.
	virtual void CleanupGame () = 0;

}; // end IGameCore interface.

} // end AllanMilne namespace.

# endif