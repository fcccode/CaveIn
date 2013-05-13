/********************************************************************
	Filename:	AudioRenderable3D.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	A virtual class with all functions being pure virtual functions.
	Also has two variables mEmitter and mDSPSettings this variables 
	are used by every child class to play 3D sounds.
	
*********************************************************************/
#ifndef _AUDIO_RENDERABLE_3D_
#define _AUDIO_RENDERABLE_3D_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "XASound.hpp"
class XACore;
class XASound;

class AudioRenderable3D
{
public:
	//virtual classes for initializing the emitter and updating it
	virtual void InitializeEmitter(XACore *xacore)=0;
	virtual X3DAUDIO_EMITTER getEmitter()=0;
	virtual X3DAUDIO_DSP_SETTINGS* getDSPSettings()=0;
	virtual IXAudio2SourceVoice* getSourceVoice() = 0;
	virtual void UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo) = 0;
	virtual void UpdateEmitterPos(X3DAUDIO_VECTOR pos) = 0;
	virtual void UpdateEmitterVelo(X3DAUDIO_VECTOR velo) = 0;
	//virtual functions for playing audio
	virtual void RenderAudio(const float deltaTime) = 0;
	virtual bool IsOk() const = 0;
	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual ~AudioRenderable3D(){}
	//constructor that sets a chunk of memeory for the emitter and dsp settings
	AudioRenderable3D(){
		memset((void*)&mDSPSettings,0,sizeof(X3DAUDIO_DSP_SETTINGS));
		memset((void*)&mEmitter,0,sizeof(X3DAUDIO_EMITTER));
	}
protected:
	X3DAUDIO_EMITTER mEmitter;
	X3DAUDIO_DSP_SETTINGS mDSPSettings;
};
#endif