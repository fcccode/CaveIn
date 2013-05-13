/********************************************************************
	Filename:	Shuffle.cpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	used to play the shuffle sound effect.
	will keep playing until reset is called. 
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
/*
* Clean Up the class.
*/
Shuffle::~Shuffle(){
	if(mShuffle!=NULL){
		delete mShuffle;
		mShuffle = NULL;
	}
}
/*
* Play the sound on a loop .
*/
void Shuffle::Play(){
	mShuffle->Play(0);
	mShuffle->SetLooped(true);
	mStarted = true;
}
/*
* Check to see if the audio has finished playing.
*/
bool Shuffle::getFinished(){
	if(mShuffle->IsPlaying()==false && mStarted == true){
		mFinished = true;
	}
	return mFinished;
}
/*
* Pause the Sound
*/
void Shuffle::Pause(){
	mShuffle->Pause();
}
/*
* Reset the start and finished variables
* as well as stopping the audio playback
*/
void Shuffle::Reset(){
	mStarted = false;
	mFinished = false;
	mShuffle->Stop();
}
/*
* Do nothing only here as required by parent class
*/
void Shuffle::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
/*
* Return the XASound Source Voice
*/
IXAudio2SourceVoice* Shuffle::getSourceVoice(){
	return mShuffle->GetSourceVoice();
}