/********************************************************************
	Filename:	Water.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	this class inherits from AudioRenderable3D to play a chosen water sound.
*********************************************************************/
#ifndef _WATER_HPP_
#define _WATER_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable3D.hpp"

class XACore;
class XASound;

class Water: public AudioRenderable3D
{
public:
	Water(XACore *aCore, int sound);
	~Water();
	//inherited functions
	inline bool IsOk() const {return (mWater!=NULL);}
	void RenderAudio(const float deltaTime);
	//play and pause functions
	void Play();
	void Pause();
	//emitter and dsp setting classes
	void InitializeEmitter(XACore *xacore);
	inline X3DAUDIO_EMITTER getEmitter(){ return mEmitter;}
	inline X3DAUDIO_DSP_SETTINGS* getDSPSettings(){return &mDSPSettings;}
	void UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo);
	void UpdateEmitterPos(X3DAUDIO_VECTOR pos);
	void UpdateEmitterVelo(X3DAUDIO_VECTOR velo);
	IXAudio2SourceVoice* getSourceVoice();
private:
	XASound *mWater;
};

#endif 