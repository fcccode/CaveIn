/********************************************************************
	Filename:	Rats.cpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	implementation of Rats.hpp
	it can be created to store any of the rat sounds. 
*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;
#include "Rats.hpp"
/*
* Constructor that initializes different sounds based upon integer flag passed to it.
*/
Rat::Rat(XACore *aCore, int sound)
	:mRat(NULL)
{
	switch(sound){
	case 0: mRat = aCore->CreateSound("Sounds/Warning/Rats/Rats.wav"); break;
	case 1: mRat = aCore->CreateSound("Sounds/Warning/Rats/Rats1.wav"); break;
	case 2: mRat = aCore->CreateSound("Sounds/Warning/Rats/Rats2.wav"); break;
	}
}
/*
* Clean Up the class.
*/
Rat::~Rat(){
	if(mRat!=NULL){
		delete mRat;
		mRat = NULL;
	}
}
/*
* Set the sound to be looped.
*/
void Rat::Play(){
	mRat->SetLooped(true);
	mRat->Play(0);
}
/*
* Pause the Sound
*/
void Rat::Pause(){
	mRat->Pause();
}
/*
* Do nothing only here as required by parent class
*/
void Rat::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
/*
* Initialize the Emitter with zero vectors.
*/
void Rat::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mRat->GetSourceVoice()->GetVoiceDetails(&details);
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
void Rat::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
/*
* Update the Emitters position vector
*/
void Rat::UpdateEmitterPos(X3DAUDIO_VECTOR pos){
	mEmitter.Position = pos;
}
/*
* Update the Emitters velocity vector
*/
void Rat::UpdateEmitterVelo(X3DAUDIO_VECTOR velo){
	mEmitter.Velocity = velo;
}
/*
* Return the XASound Source Voice
*/
IXAudio2SourceVoice* Rat::getSourceVoice(){
	return mRat->GetSourceVoice();
}