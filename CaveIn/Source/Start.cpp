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
	:mStart(NULL), mFinished(false), mStarted(false)
{
	mStart = aCore->CreateSound("Sounds/CaveIn.wav");
}
Start::~Start(){
	if(mStart!=NULL){
		delete mStart;
		mStart = NULL;
	}
}
void Start::Play(){	
	if(mStart->IsPlaying() == false && mStarted == false){
		mStart->Play(0);
		mStarted = true;
	}
}
void Start::Pause(){
	mStart->Pause();
}
bool Start::getFinished(){
	if(mStart->IsPlaying()==false && mStarted == true){
		mFinished = true;
	}
	return mFinished;
}
void Start::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
IXAudio2SourceVoice* Start::getSourceVoice(){
	return mStart->GetSourceVoice();
}