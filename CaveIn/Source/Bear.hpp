/********************************************************************
	Filename:	Bear.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _BEAR_HPP_
#define _BEAR_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable3D.hpp"

class XACore;
class XASound;

class Bear: public AudioRenderable3D
{
public:
	Bear(XACore *aCore, int sound);
	~Bear();
	void RenderAudio(const float deltaTime);
	bool IsOk() const;
	void Play();
	void Pause();
	XASound* getXASound() {return mBear;}
	void InitializeEmitter(XACore *xacore);
	inline X3DAUDIO_EMITTER getEmitter(){ return mEmitter;}
	inline X3DAUDIO_DSP_SETTINGS* getDSPSettings(){return &mDSPSettings;}
	IXAudio2SourceVoice* getSourceVoice();
	void UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo);
private:
	XASound *mBear;
	float mElapsedTime;
	float mVolumeAdjustment;
};

#endif 