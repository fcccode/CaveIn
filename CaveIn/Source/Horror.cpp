/********************************************************************
	Filename:	Horror.cpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	implements the class described in Horrow.hpp.
	plays the audio after a random period of time.
	influenced by Birds.cpp
*********************************************************************/
#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;
#include "Horror.hpp"

namespace{
	//--- Get a random pause time between water drops playing; units are seconds.
	float GetRandomPause(){
		static const int range = 10;

		return float(rand() % (5*range))/10;
	} // end GetRandomPause function.
}

//--- Called every game frame to check if the audio can be played again.
//--- Parameter is time in seconds since last call.
void Horror::RenderAudio (const float deltaTime)
{
	// guard against invalid object - do nothing if invalid.
	if (!IsOk()) return;

	mElapsedTime += deltaTime;
	if(mElapsedTime > mPauseTime){
		mPauseTime = GetRandomPause();
		mElapsedTime = 0.0f;
		mHorror->Play(0);
	}
} // end RenderAudio function.

//--- constructor sets up the XASound object.
Horror::Horror (XACore *aCore)
	: mHorror (NULL), mElapsedTime (0.0f), mPauseTime(1.0f)
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

//--- Starts the horror sound
void Horror::Play (){
	mHorror->Play(0);
} // end Play method.
//=== end of code.