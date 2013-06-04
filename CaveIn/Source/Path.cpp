/********************************************************************
	Filename:	Path.cpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	Implementation of Path.hpp.
	It plays the 'Hey.wav' to inform the user they have to go forward
	as long as they haven't rotated.
*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;
#include "Path.hpp"

namespace{
	//--- Get a random pause time between water drops playing; units are seconds.
	float GetRandomPause(){
		static const int range = 10;

		return float(rand() % (3*range))/10;
	} // end GetRandomPause function.
	//=== end of code.
}

Path::Path(XACore *aCore)
	:mPath(NULL), mElapsedTime(0.0f), mPaused(0)
{
	mPath = aCore->CreateSound("Sounds/Hey.wav");
}
/*
* Clean Up the class.
*/
Path::~Path(){
	if(mPath!=NULL){
		delete mPath;
		mPath = NULL;
	}
}
/*
* Play the Sound once.
*/
void Path::Play(){
	mPath->Play(0);
}
/*
* Pause the Sound
*/
void Path::Pause(){
	mPath->Pause();
}
/*
* Play the sound again once a set time has passed.
*/
void Path::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
	if(mPath->IsPlaying() == false){
		mElapsedTime+=deltaTime;
		if(mElapsedTime>mPaused){
			mPaused = GetRandomPause();
			mElapsedTime = 0.0f;
			mPath->Play(0);
		}
	}
}
/*
* Initialize the Emitter with zero vectors.
*/
void Path::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mPath->GetSourceVoice()->GetVoiceDetails(&details);
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
void Path::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
/*
* Update the Emitters position vector
*/
void Path::UpdateEmitterPos(X3DAUDIO_VECTOR pos){
	mEmitter.Position = pos;
}
/*
* Update the Emitters velocity vector
*/
void Path::UpdateEmitterVelo(X3DAUDIO_VECTOR velo){
	mEmitter.Velocity = velo;
}
/*
* Return the XASound Source Voice
*/
IXAudio2SourceVoice* Path::getSourceVoice(){
	return mPath->GetSourceVoice();
}
