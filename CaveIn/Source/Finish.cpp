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
	:mFinish(NULL), mFinished(false), mStarted(false)
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
void Finish::Play(){
	if(mFinish->IsPlaying() == false && mStarted == false){
		mFinish->Play(0);
		mStarted = true;
	}
}
void Finish::Pause(){
	mFinish->Pause();
}
bool Finish::getFinished(){
	if(mFinish->IsPlaying()==false && mStarted == true){
		mFinished = true;
	}
	return mFinished;
}
void Finish::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
IXAudio2SourceVoice* Finish::getSourceVoice(){
	return mFinish->GetSourceVoice();
}