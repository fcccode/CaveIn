/********************************************************************
	Filename:	Good.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _GOOD_HPP_
#define _GOOD_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable3D.hpp"

class XACore;
class XASound;

class Directions: public AudioRenderable3D
{
public:
	Directions(XACore *aCore, int sound);
	~Directions();
	void RenderAudio(const float deltaTime);
	void Play();
	void Pause();
	XASound* getXASound() {return mGood;}
	void InitializeEmitter(XACore *xacore);

	inline X3DAUDIO_EMITTER getEmitter(){ return mEmitter;}
	inline X3DAUDIO_DSP_SETTINGS* getDSPSettings(){return &mDSPSettings;}
	inline bool IsOk() const {return (mGood!=NULL);}

	IXAudio2SourceVoice* getSourceVoice();
	void UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo);
	void UpdateEmitterPos(X3DAUDIO_VECTOR pos);
	void UpdateEmitterVelo(X3DAUDIO_VECTOR velo);
private:
	XASound *mGood;
	float mElapsedTime;
	float mVolumeAdjustment;
};

#endif 