/********************************************************************
	Filename:	Finish.cpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	used to play finish sound. implements finished.hpp.
	once the sound has finished return a variable.
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
/*
* Clean Up the class.
*/
Finish::~Finish()
{
	if(mFinish!=NULL){
		delete mFinish;
		mFinish = NULL;
	}
}
/*
* If not already playing then play the audio.
*/
void Finish::Play(){
	if(mFinish->IsPlaying() == false && mStarted == false){
		mFinish->Play(0);
		mStarted = true;
	}
}
/*
* Pause the Sound
*/
void Finish::Pause(){
	mFinish->Pause();
}
/*
* Check to see if the audio has finished playing.
*/
bool Finish::getFinished(){
	if(mFinish->IsPlaying()==false && mStarted == true){
		mFinished = true;
	}
	return mFinished;
}
/*
* Do nothing only here as required by parent class
*/
void Finish::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}