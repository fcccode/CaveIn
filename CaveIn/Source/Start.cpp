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
#include "Start.hpp"

Start::Start(XACore *aCore)
	:mStart(NULL), mElapsedTime(0.0f), mVolumeAdjustment(1.1f), mFinished(false), mStarted(false)
{
	mStart = aCore->CreateSound("Sounds/CaveIn.wav");
}
Start::~Start()
{
	if(mStart!=NULL){
		delete mStart;
		mStart = NULL;
	}
}
void Start::Play()
{
	if(mStart->IsPlaying() == false && mStarted == false){
		mStart->Play(0);
		mStarted = true;
	}
}
void Start::Pause(){
	mStart->Pause();
}
inline bool Start::IsOk() const {return (mStart!=NULL);}
bool Start::getFinished(){
	if(mStart->IsPlaying()==false && mStarted == true){
		mFinished = true;
	}
	return mFinished;
}
void Start::RenderAudio(const float deltaTime)
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
		float volume = mStart->GetVolume();
		if(volume<minVolume){
			mVolumeAdjustment = volumeUp;
		}else if(volume > maxVolume){
			mVolumeAdjustment = volumeDown;
		}
		mStart->AdjustVolume(mVolumeAdjustment);
	}

	
}
IXAudio2SourceVoice* Start::getSourceVoice(){
	return mStart->GetSourceVoice();
}