/********************************************************************
	Filename:	Rats.cpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
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

Directions::Directions(XACore *aCore, int sound)
	:mGood(NULL), mElapsedTime(0.0f), mVolumeAdjustment(1.1f)
{
	switch(sound){
	case 0: mGood = aCore->CreateSound("Sounds/Good/ComeOn.wav"); break;
	case 1: mGood = aCore->CreateSound("Sounds/Good/ComeOverHere.wav"); break;
	case 2: mGood = aCore->CreateSound("Sounds/Good/HeyYou.wav"); break;
	case 3: mGood = aCore->CreateSound("Sounds/Good/Whistle.wav"); break;
	}
}
Directions::~Directions(){
	if(mGood!=NULL){
		delete mGood;
		mGood = NULL;
	}
}
void Directions::Play(){
	mGood->Play(0);
}
void Directions::Pause(){
	mGood->Pause();
}
void Directions::RenderAudio(const float deltaTime){
	static const float minVolume	= 0.1f;
	static const float maxVolume	= 1.0f;
	static const float volumeUp		= 1.25f;
	static const float volumeDown	= 0.8f;
	static const float pauseTime	= 1.0f;

	if(!IsOk()){
		return;
	}
	if(mGood->IsPlaying() == false){
		mElapsedTime+=deltaTime;
		if(mElapsedTime>pauseTime){
			mElapsedTime = 0.0f;
			mGood->Play(0);
		}
	}
}
void Directions::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mGood->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	X3DAUDIO_VECTOR tempVector = { 0.0f, 0.0f, 1.0f};
	mEmitter.Position = tempVector;
	mEmitter.Velocity = tempVector;

	mDSPSettings.SrcChannelCount = mEmitter.ChannelCount;
	mDSPSettings.DstChannelCount = xacore->GetChannelCount();
	mDSPSettings.pMatrixCoefficients = new FLOAT32[mDSPSettings.SrcChannelCount * mDSPSettings.DstChannelCount];
}
void Directions::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position.x += pos.x;
	mEmitter.Position.y += pos.y;
	mEmitter.Position.z += pos.z;
	mEmitter.Velocity.x += velo.x;
	mEmitter.Velocity.y += velo.y;
	mEmitter.Velocity.z += velo.z;
}
IXAudio2SourceVoice* Directions::getSourceVoice(){
	return mGood->GetSourceVoice();
}