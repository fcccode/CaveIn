/********************************************************************
	Filename:	Bats.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _BAT_HPP_
#define _BAT_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable3D.hpp"

class XACore;
class XASound;

class Bat: public AudioRenderable3D
{
public:
	Bat(XACore *aCore, int sound);
	~Bat();
	void RenderAudio(const float deltaTime);
	bool IsOk() const;
	void Play();
	void Pause();
	XASound* getXASound() {return mBat;}
	void InitializeEmitter(XACore *xacore);
	inline X3DAUDIO_EMITTER getEmitter(){ return mEmitter;}
	inline X3DAUDIO_DSP_SETTINGS* getDSPSettings(){return &mDSPSettings;}
	IXAudio2SourceVoice* getSourceVoice();
	void UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo);
private:
	XASound *mBat;
	float mElapsedTime;
	float mVolumeAdjustment;
};

#endif 