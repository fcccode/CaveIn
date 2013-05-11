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
#include "Bear.hpp"

Bear::Bear(XACore *aCore, int sound)
	:mBear(NULL)
{
	switch(sound){
	case 0: mBear = aCore->CreateSound("Sounds/Warning/Bear/BearRoar.wav"); break;
	case 1: mBear = aCore->CreateSound("Sounds/Warning/Bear/BearRoar1.wav"); break;
	}
}
Bear::~Bear(){
	if(mBear!=NULL){
		delete mBear;
		mBear = NULL;
	}
}
void Bear::Play(){
	mBear->SetLooped(true);
	mBear->Play(0);
}
void Bear::Pause(){
	mBear->Pause();
}

void Bear::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
void Bear::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mBear->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	X3DAUDIO_VECTOR tempVector = { 0.0f, 0.0f, 1.0f};
	mEmitter.Position = tempVector;
	mEmitter.Velocity = tempVector;

	mDSPSettings.SrcChannelCount = mEmitter.ChannelCount;
	mDSPSettings.DstChannelCount = xacore->GetChannelCount();
	mDSPSettings.pMatrixCoefficients = new FLOAT32[mDSPSettings.SrcChannelCount * mDSPSettings.DstChannelCount];
}
void Bear::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
void Bear::UpdateEmitterPos(X3DAUDIO_VECTOR pos){
	mEmitter.Position = pos;
}
void Bear::UpdateEmitterVelo(X3DAUDIO_VECTOR velo){
	mEmitter.Velocity = velo;
}
IXAudio2SourceVoice* Bear::getSourceVoice(){
	return mBear->GetSourceVoice();
}