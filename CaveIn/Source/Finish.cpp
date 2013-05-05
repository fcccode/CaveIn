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
#include "Finish.hpp"

Finish::Finish(XACore *aCore)
	:mFinish(NULL), mElapsedTime(0.0f), mVolumeAdjustment(1.1f), mFinished(false), mStarted(false)
{
	mFinish = aCore->CreateSound("Sounds/Finish.wav");
}
Finish::~Finish()
{
	if(mFinish!=NULL){
		delete mFinish;
		mFinish = NULL;
	}
}
void Finish::Play()
{
	if(mFinish->IsPlaying() == false && mStarted == false){
		mFinish->Play(0);
		mStarted = true;
	}
}
void Finish::Pause(){
	mFinish->Pause();
}
inline bool Finish::IsOk() const {return (mFinish!=NULL);}
bool Finish::getFinished(){
	if(mFinish->IsPlaying()==false && mStarted == true){
		mFinished = true;
	}
	return mFinished;
}
void Finish::RenderAudio(const float deltaTime)
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
IXAudio2SourceVoice* Finish::getSourceVoice(){
	return mFinish->GetSourceVoice();
}