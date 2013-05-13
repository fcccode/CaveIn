/********************************************************************
	Filename:	Bats.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	AudioRenderable3D is the parent class.
	this class can be initialized to play one of five bat sounds.
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
	//inherited functions
	inline bool IsOk() const {return (mBat!=NULL);}
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
	XASound *mBat;
};

#endif 