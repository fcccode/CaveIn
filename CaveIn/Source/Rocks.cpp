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
#include "Rocks.hpp"

Rocks::Rocks(XACore *aCore)
	:mRocks(NULL)
{
	mRocks = aCore->CreateSound("Sounds/Warning/Rocks.wav");
}
Rocks::~Rocks(){
	if(mRocks!=NULL){
		delete mRocks;
		mRocks = NULL;
	}
}
void Rocks::Play(){
	mRocks->SetLooped(true);
	mRocks->Play(0);
}
void Rocks::Pause(){
	mRocks->Pause();
}

void Rocks::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
void Rocks::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mRocks->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	X3DAUDIO_VECTOR tempVector = { 0.0f, 0.0f, 1.0f};
	mEmitter.Position = tempVector;
	mEmitter.Velocity = tempVector;

	mDSPSettings.SrcChannelCount = mEmitter.ChannelCount;
	mDSPSettings.DstChannelCount = xacore->GetChannelCount();
	mDSPSettings.pMatrixCoefficients = new FLOAT32[mDSPSettings.SrcChannelCount * mDSPSettings.DstChannelCount];
}
void Rocks::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
void Rocks::UpdateEmitterPos(X3DAUDIO_VECTOR pos){
	mEmitter.Position = pos;
}
void Rocks::UpdateEmitterVelo(X3DAUDIO_VECTOR velo){
	mEmitter.Velocity = velo;
}
IXAudio2SourceVoice* Rocks::getSourceVoice(){
	return mRocks->GetSourceVoice();
}