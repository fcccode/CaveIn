/********************************************************************
	Filename:	Rats.cpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	implements directions.hpp
	can be created with any of the good sound effects.
	also holds emitter changing functions
*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;
#include "Directions.hpp"

namespace{
	//--- Get a random pause time between water drops playing; units are seconds.
	float GetRandomPause(){
		static const int range = 10;

		return float(rand() % (3*range))/10;
	} // end GetRandomPause function.
	//=== end of code.
}
/*
* Constructor that initializes different sounds based upon integer flag passed to it.
*/
Directions::Directions(XACore *aCore, int sound)
	:mGood(NULL), mElapsedTime(0.0f), mVolumeAdjustment(1.1f), mPaused(0)
{
	switch(sound){
	case 0: mGood = aCore->CreateSound("Sounds/Good/ComeOn.wav"); break;
	case 1: mGood = aCore->CreateSound("Sounds/Good/ComeOverHere.wav"); break;
	case 2: mGood = aCore->CreateSound("Sounds/Good/HeyYou.wav"); break;
	case 3: mGood = aCore->CreateSound("Sounds/Good/Whistle.wav"); break;
	}
}
/*
* Clean Up the class.
*/
Directions::~Directions(){
	if(mGood!=NULL){
		delete mGood;
		mGood = NULL;
	}
}
/*
* Play the Sound once.
*/
void Directions::Play(){
	mGood->Play(0);
}
/*
* Pause the Sound
*/
void Directions::Pause(){
	mGood->Pause();
}
/*
* Play the sound again once a set time has passed.
*/
void Directions::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
	if(mGood->IsPlaying() == false){
		mElapsedTime+=deltaTime;
		if(mElapsedTime>mPaused){
			mPaused = GetRandomPause();
			mElapsedTime = 0.0f;
			mGood->Play(0);
		}
	}
}
/*
* Initialize the Emitter with zero vectors.
*/
void Directions::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mGood->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	X3DAUDIO_VECTOR tempVector = { 0.0f, 0.0f, 0.0f};
	mEmitter.Position = tempVector;
	mEmitter.Velocity = tempVector;

	mDSPSettings.SrcChannelCount = mEmitter.ChannelCount;
	mDSPSettings.DstChannelCount = xacore->GetChannelCount();
	mDSPSettings.pMatrixCoefficients = new FLOAT32[mDSPSettings.SrcChannelCount * mDSPSettings.DstChannelCount];
}
/*
* Update the Emitters position and velocity vectors
*/
void Directions::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
/*
* Update the Emitters position vector
*/
void Directions::UpdateEmitterPos(X3DAUDIO_VECTOR pos){
	mEmitter.Position = pos;
}
/*
* Update the Emitters velocity vector
*/
void Directions::UpdateEmitterVelo(X3DAUDIO_VECTOR velo){
	mEmitter.Velocity = velo;
}
/*
* Return the XASound Source Voice
*/
IXAudio2SourceVoice* Directions::getSourceVoice(){
	return mGood->GetSourceVoice();
}

