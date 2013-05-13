/********************************************************************
	Filename:	Rocks.cpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	Implements Rocks.hpp
	Used to store the XASound for the rock sound.
	Also has functions for updating emitter. 
*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;
#include "Rocks.hpp"

Rocks::Rocks(XACore *aCore)
	:mRocks(NULL)
{
	mRocks = aCore->CreateSound("Sounds/Warning/Rocks.wav");
}
/*
* Clean Up the class.
*/
Rocks::~Rocks(){
	if(mRocks!=NULL){
		delete mRocks;
		mRocks = NULL;
	}
}
/*
* Set the sound to be looped.
*/
void Rocks::Play(){
	mRocks->SetLooped(true);
	mRocks->Play(0);
}
/*
* Pause the Sound
*/
void Rocks::Pause(){
	mRocks->Pause();
}
/*
* Do nothing only here as required by parent class
*/
void Rocks::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
/*
* Initialize the Emitter with zero vectors.
*/
void Rocks::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mRocks->GetSourceVoice()->GetVoiceDetails(&details);
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
void Rocks::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
/*
* Update the Emitters position vector
*/
void Rocks::UpdateEmitterPos(X3DAUDIO_VECTOR pos){
	mEmitter.Position = pos;
}
/*
* Update the Emitters velocity vector
*/
void Rocks::UpdateEmitterVelo(X3DAUDIO_VECTOR velo){
	mEmitter.Velocity = velo;
}
/*
* Return the XASound Source Voice
*/
IXAudio2SourceVoice* Rocks::getSourceVoice(){
	return mRocks->GetSourceVoice();
}