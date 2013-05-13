/********************************************************************
	Filename:	Soundscape.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	similar to the IGameCore interface the change is that SetupGame()
	now takes a XAcore variable. this is because an instance of this
	class should be created inside SoundCues.cpp. in there the XACore
	that object creates would be passed to this one which will then
	initialize the ambient sounds.
*********************************************************************/

#ifndef __SOUNDSCAPE_HPP_1_0__
#define __SOUNDSCAPE_HPP_1_0__

#include <xaudio2.h>
#include <list>
using std::list;

#include "IGameCore.hpp"
using AllanMilne::IGameCore;

class XACore;
class XASound;
class AudioRenderable;

class Soundscape{
public:
	//--- gets passed a XACore so that the sounds can be created.
	//--- this will create all the ambient sound elements.
	bool SetupGame (XACore *aCore);

	//--- process a single game frame.
	//--- this will call the relevant AudioRender() functions.
	void ProcessGameFrame (const float deltaTime);

	//--- Release all XACore, XASound and other audio resources.
	void CleanupGame ();

	//=== Game specific behaviour.

	//--- Default constructor.
	Soundscape () 
	: mWind(NULL)
	{ 
	} // end constructor.


private:
	//--- the sounds to be played.
	XASound *mWind;
	list<AudioRenderable*> mRenderedSounds;
}; // end Soundscape1 class.

#endif