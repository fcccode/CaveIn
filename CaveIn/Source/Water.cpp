/********************************************************************
	Filename:	Rocks.cpp
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
#include "Water.hpp"

Water::Water(XACore *aCore, int sound)
	:mWater(NULL), mElapsedTime(0.0f), mVolumeAdjustment(1.1f)
{
	switch(sound){
	case 0: mWater = aCore->CreateSound("Sounds/Warning/Water/Water.wav");
	case 1: mWater = aCore->CreateSound("Sounds/Warning/Water/Water1.wav");
	}
}
Water::~Water()
{
	if(mWater!=NULL){
		delete mWater;
		mWater = NULL;
	}
}
void Water::Play()
{
	mWater->SetLooped(true);
	mWater->Play(0);
}
void Water::Pause(){
	mWater->Pause();
}
inline bool Water::IsOk() const {return (mWater!=NULL);}

void Water::RenderAudio(const float deltaTime)
{
	static const float minVolume	= 0.1f;
	static const float maxVolume	= 1.0f;
	static const float volumeUp		= 1.25f;
	static const float volumeDown	= 0.8f;
	static const float pauseTime	= 1.0f;

	if(!IsOk()){
		return;
	}
	mElapsedTime+=deltaTime;
	if(mElapsedTime>pauseTime){
		mElapsedTime = 0.0f;
		float volume = mWater->GetVolume();
		if(volume<minVolume){
			mVolumeAdjustment = volumeUp;
		}else if(volume > maxVolume){
			mVolumeAdjustment = volumeDown;
		}
		mWater->AdjustVolume(mVolumeAdjustment);
	}

	
}
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
void Water::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position.x += pos.x;
	mEmitter.Position.y += pos.y;
	mEmitter.Position.z += pos.z;
	mEmitter.Velocity.x += velo.x;
	mEmitter.Velocity.y += velo.y;
	mEmitter.Velocity.z += velo.z;
}
IXAudio2SourceVoice* Water::getSourceVoice(){
	return mWater->GetSourceVoice();
}