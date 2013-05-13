/********************************************************************
	Filename:	Bats.cpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	implements the bat.hpp file. 
	can be created with any of the bats sounds.
*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;
#include "Bats.hpp"
/*
* Constructor that initializes different sounds based upon integer flag passed to it.
*/
Bat::Bat(XACore *aCore, int sound)
	:mBat(NULL)
{
	switch(sound){
	case 0: mBat = aCore->CreateSound("Sounds/Warning/Bats/Bats.wav"); break;
	case 1: mBat = aCore->CreateSound("Sounds/Warning/Bats/Bats1.wav"); break;
	case 2: mBat = aCore->CreateSound("Sounds/Warning/Bats/Bats2.wav"); break;
	case 3: mBat = aCore->CreateSound("Sounds/Warning/Bats/Bats3.wav"); break;
	case 4: mBat = aCore->CreateSound("Sounds/Warning/Bats/Bats4.wav"); break;
	}
}
/*
* Clean Up the class.
*/
Bat::~Bat(){
	if(mBat!=NULL){
		delete mBat;
		mBat = NULL;
	}
}
/*
* Set the sound to be looped.
*/
void Bat::Play(){
	mBat->SetLooped(true);
	mBat->Play(0);
}
/*
* Pause the Sound
*/
void Bat::Pause(){
	mBat->Pause();
}
/*
* Do nothing only here as required by parent class
*/
void Bat::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
/*
* Initialize the Emitter with zero vectors.
*/
void Bat::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mBat->GetSourceVoice()->GetVoiceDetails(&details);
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
void Bat::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
/*
* Update the Emitters position vector
*/
void Bat::UpdateEmitterPos(X3DAUDIO_VECTOR pos){
	mEmitter.Position = pos;
}
/*
* Update the Emitters velocity vector
*/
void Bat::UpdateEmitterVelo(X3DAUDIO_VECTOR velo){
	mEmitter.Velocity = velo;
}
/*
* Return the XASound Source Voice
*/
IXAudio2SourceVoice* Bat::getSourceVoice(){
	return mBat->GetSourceVoice();
}