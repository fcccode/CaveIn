/*
	File:	Soundscape.cpp
	Date:	8th May 2013.
	Author:	Jake Morey based upon Allan C. Milne SoundScape1.cpp.

	Exposes:	Soundscape implementation.
	Requires:	Sounds\Atmosphere\Wind.wav, XACore, XASound, AudioRenderable, Drip, Horror.
	Description:
	This is the implementation of the Soundscape class members inspired by the IGameCore interface;
	*	this class is used to play the background sounds for the application
	*	see Soundscape.hpp for the class definition and further information on the application functionality.

*/

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include<list>
using std::list;

#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;

#include "Soundscape.hpp"
#include "Drip.hpp"
#include "Horror.hpp"

/*
* Similar to IGameCore Interface but takes a XACore
* as a parameter so it can initialize sounds using 
* the SoundCues class.
*/
bool Soundscape::SetupGame (XACore *aCore){
	// Create the wind sound object; check if ok.
	mWind = aCore->CreateSound ("Sounds/Atmosphere/Wind.wav");
	if (mWind == NULL){
		MessageBox (NULL, "Error loading wind.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	mWind->SetLooped(true);

	// Create the renderable objects and add to the renderable component list.
	Drip *drips = new Drip(aCore);
	if (!drips->IsOk()){
		MessageBox (NULL, "Error loading drips.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		delete mWind;
		delete drips;
		return false;
	}
	Horror *horror= new Horror(aCore);
	if (!horror->IsOk()){
		MessageBox (NULL, "Error loading horror.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		delete mWind;
		delete drips;
		delete horror;
		return false;
	}
	mRenderedSounds.push_back((AudioRenderable*)drips);
	mRenderedSounds.push_back((AudioRenderable*)horror);
	
	// Play the wind and horror sounds; drips will be played through the game loop.
	mWind->Play(0);
	horror->Play();
	return true;		// All has been setup without error.
} // end SetupGame function.

//--- process a single game frame.
//--- this will call the relevant AudioRender() functions.
void Soundscape::ProcessGameFrame (const float deltaTime){
	list<AudioRenderable*>::const_iterator iter;
	for (iter=mRenderedSounds.begin(); iter!=mRenderedSounds.end(); ++iter)	{
		(*iter)->RenderAudio(deltaTime);
	}
} // end ProcessGameFrame function.
//Clean up the game by removing all sounds.
void Soundscape::CleanupGame (){
	if (mWind != NULL) {
		delete mWind;
		mWind = NULL;
	}
	list<AudioRenderable*>::const_iterator iter;
	for (iter=mRenderedSounds.begin(); iter!=mRenderedSounds.end(); ++iter){
		delete *iter;
	}
	mRenderedSounds.clear();
} // end CleanupGame function.

//=== end of code.