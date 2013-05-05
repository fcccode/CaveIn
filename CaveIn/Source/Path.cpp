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
#include "Path.hpp"

Path::Path(XACore *aCore)
	:mPath(NULL), mElapsedTime(0.0f)
{
	mPath = aCore->CreateSound("Sounds/Hey.wav");
}
Path::~Path(){
	if(mPath!=NULL){
		delete mPath;
		mPath = NULL;
	}
}
void Path::Play(){
	mPath->Play(0);
}
void Path::Pause(){
	mPath->Pause();
}

void Path::RenderAudio(const float deltaTime){
	static const float pauseTime	= 1.0f;
	if(!IsOk()){
		return;
	}
	if(mPath->IsPlaying() == false){
		mElapsedTime+=deltaTime;
		if(mElapsedTime>pauseTime){
			mElapsedTime = 0.0f;
			mPath->Play(0);
		}
	}
}
void Path::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mPath->GetSourceVoice()->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	X3DAUDIO_VECTOR tempVector = { 0.0f, 0.0f, 0.0f};
	mEmitter.Position = tempVector;
	mEmitter.Velocity = tempVector;

	mDSPSettings.SrcChannelCount = mEmitter.ChannelCount;
	mDSPSettings.DstChannelCount = xacore->GetChannelCount();
	mDSPSettings.pMatrixCoefficients = new FLOAT32[mDSPSettings.SrcChannelCount * mDSPSettings.DstChannelCount];
}
void Path::UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo){
	mEmitter.Position = pos;
	mEmitter.Velocity = velo;
}
IXAudio2SourceVoice* Path::getSourceVoice(){
	return mPath->GetSourceVoice();
}