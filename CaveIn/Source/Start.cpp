/********************************************************************
	Filename:	Start.cpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	used to store the XASound for the start sound effect. 
	Implements start.hpp
	once the audio has finished playing return a variable.
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
/*
* Clean Up the class.
*/

Start::~Start(){
	if(mStart!=NULL){
		delete mStart;
		mStart = NULL;
	}
}
/*
* If not already playing then play the audio.
*/
void Start::Play(){	
	if(mStart->IsPlaying() == false && mStarted == false){
		mStart->Play(0);
		mStarted = true;
	}
}
/*
* Pause the Sound
*/

void Start::Pause(){
	mStart->Pause();
}
/*
* Check to see if the audio has finished playing.
*/
bool Start::getFinished(){
	if(mStart->IsPlaying()==false && mStarted == true){
		mFinished = true;
	}
	return mFinished;
}
/*
* Do nothing only here as required by parent class
*/
void Start::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}