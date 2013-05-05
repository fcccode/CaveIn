/*
	file:	Bird.cpp
	Version:	1.0
	Date:	11th January 2013.
	Author:	allan c. Milne.

	Exposes:	Bird implementation.
	Requires:	Sounds/bird.wav, XACore, XASound.

	Description:
	Implements the AudioRenderable Bird class that will play a bird chirp moving towards and away from the listener.
	A XASound object is used to model the bird sound sample.
	Uses volume adjustment to provide the perception of movement.
	
	The constructor sets up the XASound object.
	Play() starts the sound playing.
	AudioRender(...) implements the dynamic movement.

	The volume (amplitude multiplier) is adjusted by applying a further multiplier:
	*	0.8 (4/5) decreasing multiplier;
	*	1.25 (5/4) increasing multiplier;
	i.e. value will go 1.0, 0.8, 0.64, 0.512, 0.4096, .. and then up again.
	Volume is adjusted every second.
	Note these values have been used due to the effect they produce rather than being derived from the underlying physics of bird speed, etc.
*/


#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;

#include "Horror.hpp"

//--- Called every game frame to perform the audio animation of the bird's movement.
//--- Parameter is time in seconds since last call.
void Horror::RenderAudio (const float deltaTime)
{
	// guard against invalid object - do nothing if invalid.
	if (!IsOk()) return;

	mElapsedTime += deltaTime;
	if(mElapsedTime > mPauseTime){
		mPauseTime = float(rand()%100)/10;
		mElapsedTime = 0.0f;
		mHorror->Play(0);
	}
} // end RenderAudio function.

//--- indicates if the object has been created correctly.
inline bool Horror::IsOk () const { return (mHorror != NULL); }

	//--- constructor will setup the bird sound object.
Horror::Horror (XACore *aCore)
	: mHorror (NULL), mElapsedTime (0.0f), mVolumeAdjustment (1.1f), mPauseTime(1.0f)
{
	mHorror = aCore->CreateSound("Sounds/Atmosphere/Horror.wav");
} // end constructor function.

Horror::~Horror()
{
	if (mHorror !=NULL)
	{
		delete mHorror;
		mHorror = NULL;
	}
} // end destructor function.

//--- Starts the bird sound, does not initiate any movement.
void Horror::Play (){
	mHorror->Play(0);
} // end Play method.

//=== end of code.