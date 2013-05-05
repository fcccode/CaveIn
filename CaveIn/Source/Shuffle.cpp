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
#include "Shuffle.hpp"

Shuffle::Shuffle(XACore *aCore)
	:mShuffle(NULL), mElapsedTime(0.0f), mVolumeAdjustment(1.1f)
{
	mShuffle = aCore->CreateSound("Sounds/Shuffle.wav");
}
Shuffle::~Shuffle()
{
	if(mShuffle!=NULL){
		delete mShuffle;
		mShuffle = NULL;
	}
}
void Shuffle::Play()
{
	mShuffle->Play(0);
}
void Shuffle::Pause(){
	mShuffle->Pause();
}
inline bool Shuffle::IsOk() const {return (mShuffle!=NULL);}
void Shuffle::RenderAudio(const float deltaTime)
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
		float volume = mShuffle->GetVolume();
		if(volume<minVolume){
			mVolumeAdjustment = volumeUp;
		}else if(volume > maxVolume){
			mVolumeAdjustment = volumeDown;
		}
		mShuffle->AdjustVolume(mVolumeAdjustment);
	}

	
}
IXAudio2SourceVoice* Shuffle::getSourceVoice(){
	return mShuffle->GetSourceVoice();
}