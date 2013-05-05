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
	:mWalking(NULL), mFinished(false), mStarted(false)
{
	mWalking = aCore->CreateSound("Sounds/Footsteps.wav");
}
Walking::~Walking(){
	if(mWalking!=NULL){
		delete mWalking;
		mWalking = NULL;
	}
}
void Walking::Play(){
	if(mWalking->IsPlaying() == false && mStarted == false){
		mWalking->Play(0);
		mStarted = true;
	}
}
bool Walking::getFinished(){
	if(mWalking->IsPlaying()==false && mStarted == true){
		mFinished = true;
	}
	return mFinished;
}
void Walking::Pause(){
	mWalking->Pause();
}
void Walking::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
void Walking::Reset(){
	mStarted = false;
	mFinished = false;
}
IXAudio2SourceVoice* Walking::getSourceVoice(){
	return mWalking->GetSourceVoice();
}