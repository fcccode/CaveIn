/********************************************************************
	Filename:	Water.cpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	implements water.hpp.
	can be created so that it wil play either one of the water sounds.
*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;
#include "Water.hpp"
/*
* Constructor that initializes different sounds based upon integer flag passed to it.
*/
Water::Water(XACore *aCore, int sound)
	:mWater(NULL)
{
	switch(sound){
	case 0: mWater = aCore->CreateSound("Sounds/Warning/Water/Water.wav");
	case 1: mWater = aCore->CreateSound("Sounds/Warning/Water/Water1.wav");
	}
}
/*
 * Clean Up the class.
 */
Water::~Water(){
	if(mWater!=NULL){
		delete mWater;
		mWater = NULL;
	}
}
/*
* Set the sound to be looped.
*/
void Water::Play(){
	mWater->SetLooped(true);
	mWater->Play(0);
}
/*
* Pause the Sound
*/
void Water::Pause(){
	mWater->Pause();
}
/*
* Do nothing only here as required by parent class
*/
void Water::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
/*
* Initialize the Emitter with zero vectors.
*/
void Water::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mWater->GetSourceVoice()->GetVoiceDetails(&details);
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
void Water::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
/*
* Update the Emitters position vector
*/
void Water::UpdateEmitterPos(X3DAUDIO_VECTOR pos){
	mEmitter.Position = pos;
}
/*
* Update the Emitters velocity vector
*/
void Water::UpdateEmitterVelo(X3DAUDIO_VECTOR velo){
	mEmitter.Velocity = velo;
}
/*
* Return the XASound Source Voice
*/
IXAudio2SourceVoice* Water::getSourceVoice(){
	return mWater->GetSourceVoice();
}