/*
	File:	Soundscape1.cpp
	Version:	0.1
	Date:	9th January 2013.
	Author:	Allan C. Milne.

	Requires:	XACore.

	Description:
	This is the implementation of the Soundscape1 class members;
	this is the base version with no sounds yet added.

*/

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include <list>
using std::list;

#include "XACore.hpp"
using AllanMilne::Audio::XACore;
#include "XASound.hpp"
using AllanMilne::Audio::XASound;

#include "Soundscape1Base.hpp"

//=== Implementation of the IGameCore interface.

//--- create and initialize XACore.
//--- Since this game has no graphics the HWND argument is not used.
bool Soundscape1::SetupGame (HWND aWindow)
{
	
	
	// set up the XAudio2 engine and mastering voice; check if ok.
	mXACore = new XACore ();
	if (mXACore->GetEngine() == NULL || mXACore->GetMasterVoice() == NULL) {
		return false;
	}

	DWORD channelMask;
	XAUDIO2_DEVICE_DETAILS voiceDetails = mXACore->GetDeviceDetails();
	channelMask = voiceDetails.OutputFormat.dwChannelMask;

	X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, mX3DInstance);
	return true;		// All has been setup without error.
} // end SetupGame function.

//--- process a single game frame.
void Soundscape1::ProcessGameFrame (const float deltaTime)
{
	static bool keyPress = false;
} // end ProcessGameFrame function.

//--- Release all XACore resources.
//--- Note the order of destruction is important; XAudio2 destroys voices when the engine is destroyed, any calls to the voices AFTER this is an error, so any voice->DestroyVoice() should always be called before the engine is destroyed.
void Soundscape1::CleanupGame ()
{
	if (mXACore != NULL) {
		delete mXACore;
		mXACore = NULL;
	}
} // end CleanupGame function.

//=== end of code.