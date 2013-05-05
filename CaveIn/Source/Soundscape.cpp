/*
	File:	Soundscape1.cpp
	Version:	1.0
	Date:	11th January 2013.
	Author:	Allan C. Milne.

	Exposes:	Soundscape1 implementation.
	Requires:	Sounds\rain.wav, XACore, XASound, AudioRenderable, Bird, Frogs.
	
	Description:
	This is the implementation of the Soundscape1 class members;
	*	see Soundscape1.hpp for the class definition and further information on the application functionality.

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

//=== Implementation of the IGameCore interface.

//--- create and initialize XACore and the sounds to be played.
//--- Since this game has no graphics the HWND argument is not used.
//--- this will create all the sound elements and play the rain & bird sounds.
bool Soundscape::SetupGame (XACore *aCore){
	// Create the rain sound object; check if ok.
	mWind = aCore->CreateSound ("Sounds/Atmosphere/Wind.wav");
	if (mWind == NULL){
		MessageBox (NULL, "Error loading rain.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	mWind->SetLooped(true);

	// Create the renderable objects and add to the renderable component list.
	Drip *bats = new Drip(aCore);
	if (!bats->IsOk()){
		MessageBox (NULL, "Error loading bird.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		delete mWind;
		delete bats;
		return false;
	}
	Horror *horror= new Horror(aCore);
	if (!horror->IsOk()){
		MessageBox (NULL, "Error loading frogs.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		delete mWind;
		delete bats;
		delete horror;
		return false;
	}
	mRenderedSounds.push_back((AudioRenderable*)bats);
	mRenderedSounds.push_back((AudioRenderable*)horror);
	
	// Play the rain and bird sounds; frogs will be played through the game loop.
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

//--- Release all XACore and XASound resources.
//--- Note the order of destruction is important; XAudio2 destroys voices when the engine is destroyed, any calls to the voices AFTER this is an error, so any voice->DestroyVoice() should always be called before the engine is destroyed.
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