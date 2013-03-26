/***
for stereo input need matrix 4*4 first two how much of the left channel going to left and right the second two for right channel
****/
/*
	File:	Soundscape1.hpp
	Version:	0.1
	Date:	10th January 2013.
	Author:	Allan C. Milne.

	Requires:	IGameCore, XACore.

	Description:
	This is a concrete implementation of the IGameCore interface for the application logic.
	This is the concrete strategy class called  from the context WinCore class as part of a strategy pattern.
	*	See IGameCore.hpp for details of this interface.

	this application will eventually play a soundscape made up of a variety of sound elements.
	this version is the base definition of the class with no sound elements.
*/

#ifndef __SOUNDSCAPE1_HPP_0__
#define __SOUNDSCAPE1_HPP_0__

#include <xaudio2.h>
#include <X3DAudio.h>
#include "IGameCore.hpp"
using AllanMilne::IGameCore;
#include "AudioRenderable.hpp"
#include "X3DSound.hpp"
#include <list>
using std::list;

class XACore;
class XASound;
class X3DSound;

class Soundscape1 : public IGameCore {
public:

	//=== Implementation of the IGameCore interface.

	//--- create and initialize XACore and the sounds to be played.
	//--- since this game has no graphics the argument is not used.
	bool SetupGame (HWND aWindow);

	//--- process a single game frame.
	void ProcessGameFrame (const float deltaTime);

	//--- Release all XACore, XASound and other audio resources.
	void CleanupGame ();

	//=== Game specific behaviour.

	//--- Default constructor.
	Soundscape1 () 
	: mXACore (NULL)
	{ 
		memset((void*)&mEmitter,0,sizeof(X3DAUDIO_EMITTER));
		memset((void*)&mDSPSettings,0,sizeof(X3DAUDIO_DSP_SETTINGS));
	} // end constructor.


private:
	
	//--- the XAudio2 engine encapsulation.
	XACore *mXACore;
	XAUDIO2_FILTER_PARAMETERS mFParams;
	X3DSound *m3DSound;
	X3DAUDIO_DSP_SETTINGS mDSPSettings;
	X3DAUDIO_EMITTER mEmitter;
}; // end Soundscape1 class.

#endif