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

#include "Soundscape1.hpp"
#include "Bird.hpp"
#include"Frogs.hpp"

//=== Implementation of the IGameCore interface.

//--- create and initialize XACore and the sounds to be played.
//--- Since this game has no graphics the HWND argument is not used.
//--- this will create all the sound elements and play the rain & bird sounds.
bool Soundscape1::SetupGame (HWND aWindow)
{
	// set up the XAudio2 engine and mastering voice; check if ok.
	mXACore = new XACore ();
	if (mXACore->GetEngine() == NULL || mXACore->GetMasterVoice() == NULL) {
		return false;
	}

	// Create the rain sound object; check if ok.
	mRain = mXACore->CreateSound ("Sounds/rain.wav");
	if (mRain == NULL) {
		MessageBox (NULL, "Error loading rain.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	mRain->SetLooped(true);

	// Create the renderable objects and add to the renderable component list.
	Bird *bird = new Bird(mXACore);
	if (!bird->IsOk())
	{
		MessageBox (NULL, "Error loading bird.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		delete mRain;
		delete bird;
		return false;
	}
	Frogs *frogs= new Frogs(mXACore);
	if (!frogs->IsOk())
	{
		MessageBox (NULL, "Error loading frogs.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		delete mRain;
		delete bird;
		delete frogs;
		return false;
	}
	mRenderedSounds.push_back((AudioRenderable*)bird);
	mRenderedSounds.push_back((AudioRenderable*)frogs);
	
	// Play the rain and bird sounds; frogs will be played through the game loop.
	mRain->Play(0);
	bird->Play();
	return true;		// All has been setup without error.
} // end SetupGame function.

//--- process a single game frame.
//--- this will call the relevant AudioRender() functions.
void Soundscape1::ProcessGameFrame (const float deltaTime)
{
	list<AudioRenderable*>::const_iterator iter;
	for (iter=mRenderedSounds.begin(); iter!=mRenderedSounds.end(); ++iter)
	{
		(*iter)->RenderAudio(deltaTime);
	}
} // end ProcessGameFrame function.

//--- Release all XACore and XASound resources.
//--- Note the order of destruction is important; XAudio2 destroys voices when the engine is destroyed, any calls to the voices AFTER this is an error, so any voice->DestroyVoice() should always be called before the engine is destroyed.
void Soundscape1::CleanupGame ()
{
	if (mRain != NULL) {
		delete mRain;
		mRain = NULL;
	}
	list<AudioRenderable*>::const_iterator iter;
	for (iter=mRenderedSounds.begin(); iter!=mRenderedSounds.end(); ++iter)
	{
		delete *iter;
	}
	mRenderedSounds.clear();
if (mXACore != NULL) {
		delete mXACore;
		mXACore = NULL;
	}
} // end CleanupGame function.

//=== end of code.