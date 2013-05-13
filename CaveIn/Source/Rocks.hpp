/********************************************************************
	Filename:	Rocks.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	this class can be implemented to play one of the many rock sounds.
	this class inherits from AudioRenderable3D
*********************************************************************/
#ifndef _ROCKS_HPP_
#define _ROCKS_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable3D.hpp"

class XACore;
class XASound;

class Rocks: public AudioRenderable3D
{
public:
	Rocks(XACore *aCore);
	~Rocks();
	//inherited functions
	inline bool IsOk() const {return (mRocks!=NULL);}
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
	XASound *mRocks;
};

#endif 