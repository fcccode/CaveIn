/********************************************************************
	Filename:	Rats.cpp
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
#include "Rats.hpp"

Rat::Rat(XACore *aCore, int sound)
	:mRat(NULL)
{
	switch(sound){
	case 0: mRat = aCore->CreateSound("Sounds/Warning/Rats/Rats.wav"); break;
	case 1: mRat = aCore->CreateSound("Sounds/Warning/Rats/Rats1.wav"); break;
	}
}
Rat::~Rat(){
	if(mRat!=NULL){
		delete mRat;
		mRat = NULL;
	}
}
void Rat::Play(){
	mRat->SetLooped(true);
	mRat->Play(0);
}
void Rat::Pause(){
	mRat->Pause();
}
void Rat::RenderAudio(const float deltaTime){
	if(!IsOk()){
		return;
	}
}
void Rat::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mRat->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	X3DAUDIO_VECTOR tempVector = { 0.0f, 0.0f, 1.0f};
	mEmitter.Position = tempVector;
	mEmitter.Velocity = tempVector;

	mDSPSettings.SrcChannelCount = mEmitter.ChannelCount;
	mDSPSettings.DstChannelCount = xacore->GetChannelCount();
	mDSPSettings.pMatrixCoefficients = new FLOAT32[mDSPSettings.SrcChannelCount * mDSPSettings.DstChannelCount];
}
void Rat::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
IXAudio2SourceVoice* Rat::getSourceVoice(){
	return mRat->GetSourceVoice();
}