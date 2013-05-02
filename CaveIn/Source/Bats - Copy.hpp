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
#include "AudioRenderable.hpp"

class XACore;
class XASound;

class Bat: virtual AudioRenderable
{
public:
	void RenderAudio(const float deltaTime);
	bool IsOk() const;
	void Play();
	void InitializeEmitter(XACore *xacore);
	void UpdateEmitter(X3DAUDIO_LISTENER player);
	inline X3DAUDIO_EMITTER getEmitter(){ return mEmitter;}
	inline X3DAUDIO_DSP_SETTINGS getDSPSettings(){return mDSPSettings;}
	IXAudio2SourceVoice* getSourceVoice();
	Bat(XACore *aCore);
	~Bat();
private:
	XASound *mBat;
	float mElapsedTime;
	float mVolumeAdjustment;
};

#endif