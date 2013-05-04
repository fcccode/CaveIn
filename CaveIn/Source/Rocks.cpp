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
#include "Rocks.hpp"

Rocks::Rocks(XACore *aCore)
	:mRocks(NULL), mElapsedTime(0.0f), mVolumeAdjustment(1.1f)
{
	mRocks = aCore->CreateSound("Sounds/Warning/Rocks.wav");
}
Rocks::~Rocks()
{
	if(mRocks!=NULL){
		delete mRocks;
		mRocks = NULL;
	}
}
void Rocks::Play()
{
	mRocks->SetLooped(true);
	mRocks->Play(0);
}
void Rocks::Pause(){
	mRocks->Pause();
}
inline bool Rocks::IsOk() const {return (mRocks!=NULL);}

void Rocks::RenderAudio(const float deltaTime)
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
		float volume = mRocks->GetVolume();
		if(volume<minVolume){
			mVolumeAdjustment = volumeUp;
		}else if(volume > maxVolume){
			mVolumeAdjustment = volumeDown;
		}
		mRocks->AdjustVolume(mVolumeAdjustment);
	}

	
}
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
void Rocks::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
IXAudio2SourceVoice* Rocks::getSourceVoice(){
	return mRocks->GetSourceVoice();
}