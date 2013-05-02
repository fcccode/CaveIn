/********************************************************************
	Filename:	Bats.cpp
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
#include "Bats.hpp"

Bat::Bat(XACore *aCore, int sound)
	:mBat(NULL), mElapsedTime(0.0f), mVolumeAdjustment(1.1f)
{
	switch(sound){
	case 0: mBat = aCore->CreateSound("Sounds/Warnings/Bats/Bats.wav"); break;
	case 1: mBat = aCore->CreateSound("Sounds/Warnings/Bats/Bats1.wav"); break;
	case 2: mBat = aCore->CreateSound("Sounds/Warnings/Bats/Bats2.wav"); break;
	case 3: mBat = aCore->CreateSound("Sounds/Warnings/Bats/Bats3.wav"); break;
	case 4: mBat = aCore->CreateSound("Sounds/Warnings/Bats/Bats4.wav"); break;
	}
}
Bat::~Bat()
{
	if(mBat!=NULL){
		delete mBat;
		mBat = NULL;
	}
}
void Bat::Play()
{
	mBat->SetLooped(true);
	mBat->Play(0);
}
inline bool Bat::IsOk() const {return (mBat!=NULL);}

void Bat::RenderAudio(const float deltaTime)
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
		float volume = mBat->GetVolume();
		if(volume<minVolume){
			mVolumeAdjustment = volumeUp;
		}else if(volume > maxVolume){
			mVolumeAdjustment = volumeDown;
		}
		mBat->AdjustVolume(mVolumeAdjustment);
	}

	
}
