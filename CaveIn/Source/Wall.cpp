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
Wall::~Wall(){
	if(mWall!=NULL){
		delete mWall;
		mWall = NULL;
	}
}
void Wall::Play(){
	mWall->Play(0);
}
void Wall::Pause(){
	mWall->Pause();
}
void Wall::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
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
void Wall::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
void Wall::UpdateEmitterPos(X3DAUDIO_VECTOR pos){
	mEmitter.Position = pos;
}
void Wall::UpdateEmitterVelo(X3DAUDIO_VECTOR velo){
	mEmitter.Velocity = velo;
}
IXAudio2SourceVoice* Wall::getSourceVoice(){
	return mWall->GetSourceVoice();
}