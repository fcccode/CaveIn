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
#include "Wall.hpp"

Wall::Wall(XACore *aCore)
	:mWall(NULL), mElapsedTime(0.0f), mVolumeAdjustment(1.1f)
{
	mWall = aCore->CreateSound("Sounds/Hurt.wav");
}
Wall::~Wall()
{
	if(mWall!=NULL){
		delete mWall;
		mWall = NULL;
	}
}
void Wall::Play()
{
	mWall->Play(0);
}
void Wall::Pause(){
	mWall->Pause();
}
inline bool Wall::IsOk() const {return (mWall!=NULL);}

void Wall::RenderAudio(const float deltaTime)
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
		float volume = mWall->GetVolume();
		if(volume<minVolume){
			mVolumeAdjustment = volumeUp;
		}else if(volume > maxVolume){
			mVolumeAdjustment = volumeDown;
		}
		mWall->AdjustVolume(mVolumeAdjustment);
	}

	
}
void Wall::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mWall->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	X3DAUDIO_VECTOR tempVector = { 0.0f, 0.0f, 1.0f};
	mEmitter.Position = tempVector;
	mEmitter.Velocity = tempVector;

	mDSPSettings.SrcChannelCount = mEmitter.ChannelCount;
	mDSPSettings.DstChannelCount = xacore->GetChannelCount();
	mDSPSettings.pMatrixCoefficients = new FLOAT32[mDSPSettings.SrcChannelCount * mDSPSettings.DstChannelCount];
}
void Wall::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position.x += pos.x;
	mEmitter.Position.y += pos.y;
	mEmitter.Position.z += pos.z;
	mEmitter.Velocity.x += velo.x;
	mEmitter.Velocity.y += velo.y;
	mEmitter.Velocity.z += velo.z;
}
IXAudio2SourceVoice* Wall::getSourceVoice(){
	return mWall->GetSourceVoice();
}