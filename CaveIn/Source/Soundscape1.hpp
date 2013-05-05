/*
	File:	Soundscape1.hpp
	Version:	1.0
	Date:	10th January 2013.
	Author:	Allan C. Milne.

	Requires:	IGameCore, XACore, XASound, AudioRenderable.

	Description:
	This is a concrete implementation of the IGameCore interface for the application logic.
	This is the concrete strategy class called  from the context WinCore class as part of a strategy pattern.
	*	See IGameCore.hpp for details of this interface.

	this application plays a soundscape made up of a variety of sound elements.
	It illustrates both the use of XAudio2 and relevant OO architecturing.
	the sounds played are

	*	a looped rain sound.
		this uses the XASound framework class and illustrates a static background ambience.

	*	a chirpingg bird moving towards and away from the listener.
		This is encapsulated in the Bird class that implements the AudioRenderable interface.
		Uses volume adjustment to provide the perception of movement.

	*	frogs in harmony at random times.
		this is encapsulated in the Frogs class implementing the AudioRenderable interface.
		Uses frequency adjustment to provide the harmony.

*/

#ifndef __SOUNDSCAPE1_HPP_1_0__
#define __SOUNDSCAPE1_HPP_1_0__

#include <xaudio2.h>
#include <list>
using std::list;

#include "IGameCore.hpp"
using AllanMilne::IGameCore;

class XACore;
class XASound;
class AudioRenderable;

class Soundscape1 : public IGameCore {
public:

	//=== Implementation of the IGameCore interface.

	//--- create and initialize XACore and the sounds to be played.
	//--- since this game has no graphics the argument is not used.
	//--- this will create all the sound elements.
	bool SetupGame (HWND aWindow);

	//--- process a single game frame.
	//--- this will call the relevant AudioRender() functions.
	void ProcessGameFrame (const float deltaTime);

	//--- Release all XACore, XASound and other audio resources.
	void CleanupGame ();

	//=== Game specific behaviour.

	//--- Default constructor.
	Soundscape1 () 
	: mXACore (NULL), mRain(NULL)
	{ 
	} // end constructor.


private:
	
	//--- the XAudio2 engine encapsulation.
	XACore *mXACore;

	//--- the sounds to be played.
	XASound *mRain;
	list<AudioRenderable*> mRenderedSounds;
	
}; // end Soundscape1 class.

#endif