/********************************************************************
	Filename:	Rocks.cpp
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
#include "Wall.hpp"

Wall::Wall(XACore *aCore)
	:mWall(NULL)
{
	mWall = aCore->CreateSound("Sounds/Hurt.wav");
}
/* 
* Clean Up the class.
*/
Wall::~Wall(){
	if(mWall!=NULL){
		delete mWall;
		mWall = NULL;
	}
}
/*
* Play the Sound once.
*/
void Wall::Play(){
	mWall->Play(0);
}
/*
* Pause the Sound
*/
void Wall::Pause(){
	mWall->Pause();
}
/*
* Do nothing only here as required by parent class
*/
void Wall::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
/*
* Initialize the Emitter with zero vectors.
*/
void Wall::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mWall->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	X3DAUDIO_VECTOR tempVector = { 0.0f, 0.0f, 0.0f};
	mEmitter.Position = tempVector;
	mEmitter.Velocity = tempVector;

	mDSPSettings.SrcChannelCount = mEmitter.ChannelCount;
	mDSPSettings.DstChannelCount = xacore->GetChannelCount();
	mDSPSettings.pMatrixCoefficients = new FLOAT32[mDSPSettings.SrcChannelCount * mDSPSettings.DstChannelCount];
}
/*
 * Update the Emitters position and velocity vectors
 */
void Wall::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
/*
* Update the Emitters position vector
*/
void Wall::UpdateEmitterPos(X3DAUDIO_VECTOR pos){
	mEmitter.Position = pos;
}
/*
* Update the Emitters velocity vector
*/
void Wall::UpdateEmitterVelo(X3DAUDIO_VECTOR velo){
	mEmitter.Velocity = velo;
}
/*
* Return the XASound Source Voice
*/
IXAudio2SourceVoice* Wall::getSourceVoice(){
	return mWall->GetSourceVoice();
}