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

#include "AmbientRat.hpp"

//--- Called every game frame to perform the audio animation of the bird's movement.
//--- Parameter is time in seconds since last call.
void AmbientRat::RenderAudio (const float deltaTime)
{
	// constants used to define animation of the bird movement.
	static const float minVolume = 0.1f;		// level at which to bounce volume adjustment up again.
	static const float maxVolume = 1.0f;		// level at which to bounce volume down again.
	static const float volumeUp = 1.25f;		// multiplier for increasing volume = 5/4.
	static const float volumeDown = 0.8f;		// multiplier for decreasing volume = 4/5.
	static const float pauseTime = 1.0f;		// change volume every 1 second.

	// guard against invalid object - do nothing if invalid.
	if (!IsOk()) return;

	mElapsedTime += deltaTime;
	if (mElapsedTime > pauseTime)
	{
		mElapsedTime = 0.0f;
		float volume = mRat->GetVolume();
		if (volume < minVolume) 
			mVolumeAdjustment = volumeUp;		// bounce and move bird towards listener.
		else if (volume >maxVolume) 
			mVolumeAdjustment = volumeDown;		// bounce and move bird away from listener.
		mRat->AdjustVolume(mVolumeAdjustment);
	}
} // end RenderAudio function.

//--- indicates if the object has been created correctly.
inline bool AmbientRat::IsOk () const { return (mRat != NULL); }

	//--- constructor will setup the bird sound object.
AmbientRat::AmbientRat (XACore *aCore)
	: mRat (NULL), mElapsedTime (0.0f), mVolumeAdjustment (1.1f)
{
	mRat = aCore->CreateSound("Sounds/Atmosphere/Rats.wav");
} // end constructor function.

AmbientRat::~AmbientRat()
{
	if (mRat !=NULL)
	{
		delete mRat;
		mRat = NULL;
	}
} // end destructor function.

//--- Starts the bird sound, does not initiate any movement.
void AmbientRat::Play ()
{
	mRat->SetLooped(true);
	mRat->Play(0);
} // end Play method.

//=== end of code.