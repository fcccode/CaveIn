/********************************************************************
	Filename:	Walking.cpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	implements walking.hpp.
	plays walking sound once, when finished return false, then reset all variables.
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
/*
* Clean Up the class.
*/
Walking::~Walking(){
	if(mWalking!=NULL){
		delete mWalking;
		mWalking = NULL;
	}
}
/*
* If not already playing then play the audio.
*/

void Walking::Play(){
	if(mWalking->IsPlaying() == false && mStarted == false){
		mWalking->Play(0);
		mStarted = true;
	}
}
/*
* Check to see if the audio has finished playing.
*/

bool Walking::getFinished(){
	if(mWalking->IsPlaying()==false && mStarted == true){
		mFinished = true;
	}
	return mFinished;
}
/*
* Pause the Sound
*/
void Walking::Pause(){
	mWalking->Pause();
}
/*
* Do nothing only here as required by parent class
*/
void Walking::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
/*
* Reset start and finish variables.
*/
void Walking::Reset(){
	mStarted = false;
	mFinished = false;
}
/*
* Return the XASound Source Voice
*/
IXAudio2SourceVoice* Walking::getSourceVoice(){
	return mWalking->GetSourceVoice();
}