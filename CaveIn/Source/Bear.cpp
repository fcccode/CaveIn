/********************************************************************
	Filename:	Bear.cpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	Implements Bear.hpp
	this class can be created so that it store either of the bear sounds
*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;
#include "Bear.hpp"
/*
* Constructor that initializes different sounds based upon integer flag passed to it.
*/
Bear::Bear(XACore *aCore, int sound)
	:mBear(NULL)
{
	switch(sound){
	case 0: mBear = aCore->CreateSound("Sounds/Warning/Bear/BearRoar.wav"); break;
	case 1: mBear = aCore->CreateSound("Sounds/Warning/Bear/BearRoar1.wav"); break;
	}
}
/*
* Clean Up the class.
*/
Bear::~Bear(){
	if(mBear!=NULL){
		delete mBear;
		mBear = NULL;
	}
}
/*
* Set the sound to be looped.
*/
void Bear::Play(){
	mBear->SetLooped(true);
	mBear->Play(0);
}
/*
* Pause the Sound
*/
void Bear::Pause(){
	mBear->Pause();
}
/*
* Do nothing only here as required by parent class
*/
void Bear::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
/*
* Initialize the Emitter with zero vectors.
*/
void Bear::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mBear->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	X3DAUDIO_VECTOR tempVector = { 0.0f, 0.0f, 1.0f};
	mEmitter.Position = tempVector;
	mEmitter.Velocity = tempVector;

	mDSPSettings.SrcChannelCount = mEmitter.ChannelCount;
	mDSPSettings.DstChannelCount = xacore->GetChannelCount();
	mDSPSettings.pMatrixCoefficients = new FLOAT32[mDSPSettings.SrcChannelCount * mDSPSettings.DstChannelCount];
}
/*
* Update the Emitters position and velocity vectors
*/
void Bear::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
/*
* Update the Emitters position vector
*/
void Bear::UpdateEmitterPos(X3DAUDIO_VECTOR pos){
	mEmitter.Position = pos;
}
/*
* Update the Emitters velocity vector
*/
void Bear::UpdateEmitterVelo(X3DAUDIO_VECTOR velo){
	mEmitter.Velocity = velo;
}
/*
* Return the XASound Source Voice
*/
IXAudio2SourceVoice* Bear::getSourceVoice(){
	return mBear->GetSourceVoice();
}