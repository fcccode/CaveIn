/********************************************************************
	Filename:	Bear.cpp
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
#include "Walking.hpp"

Walking::Walking(XACore *aCore)
	:mFinish(NULL), mElapsedTime(0.0f), mVolumeAdjustment(1.1f)
{
	mFinish = aCore->CreateSound("Sounds/Walking.wav");
}
Walking::~Walking()
{
	if(mFinish!=NULL){
		delete mFinish;
		mFinish = NULL;
	}
}
void Walking::Play()
{
	mFinish->Play(0);
}
void Walking::Pause(){
	mFinish->Pause();
}
inline bool Walking::IsOk() const {return (mFinish!=NULL);}
void Walking::RenderAudio(const float deltaTime)
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
		float volume = mFinish->GetVolume();
		if(volume<minVolume){
			mVolumeAdjustment = volumeUp;
		}else if(volume > maxVolume){
			mVolumeAdjustment = volumeDown;
		}
		mFinish->AdjustVolume(mVolumeAdjustment);
	}

	
}
IXAudio2SourceVoice* Walking::getSourceVoice(){
	return mFinish->GetSourceVoice();
}