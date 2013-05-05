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
	:mShuffle(NULL), mFinished(false), mStarted(false)
{
	mShuffle = aCore->CreateSound("Sounds/Shuffle.wav");
}
Shuffle::~Shuffle(){
	if(mShuffle!=NULL){
		delete mShuffle;
		mShuffle = NULL;
	}
}
void Shuffle::Play(){
	if(mShuffle->IsPlaying() == false && mStarted == false){
		mShuffle->Play(0);
		mStarted = true;
	}
}
bool Shuffle::getFinished(){
	if(mShuffle->IsPlaying()==false && mStarted == true){
		mFinished = true;
	}
	return mFinished;
}
void Shuffle::Pause(){
	mShuffle->Pause();
}
void Shuffle::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
IXAudio2SourceVoice* Shuffle::getSourceVoice(){
	return mShuffle->GetSourceVoice();
}