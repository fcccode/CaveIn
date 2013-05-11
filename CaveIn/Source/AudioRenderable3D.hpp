/********************************************************************
	Filename:	AudioRenderable.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
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
	virtual void RenderAudio(const float deltaTime) = 0;
	virtual bool IsOk() const = 0;
	virtual void InitializeEmitter(XACore *xacore)=0;
	virtual XASound* getXASound() = 0;
	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual ~AudioRenderable3D(){}
	virtual X3DAUDIO_EMITTER getEmitter()=0;
	virtual X3DAUDIO_DSP_SETTINGS* getDSPSettings()=0;
	virtual IXAudio2SourceVoice* getSourceVoice() = 0;
	virtual void UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo) = 0;
	virtual void UpdateEmitterPos(X3DAUDIO_VECTOR pos) = 0;
	virtual void UpdateEmitterVelo(X3DAUDIO_VECTOR velo) = 0;
	AudioRenderable3D(){
		memset((void*)&mDSPSettings,0,sizeof(X3DAUDIO_DSP_SETTINGS));
		memset((void*)&mEmitter,0,sizeof(X3DAUDIO_EMITTER));
		memset((void*)&mCone,0,sizeof(X3DAUDIO_CONE));
	}
protected:
	X3DAUDIO_EMITTER mEmitter;
	X3DAUDIO_DSP_SETTINGS mDSPSettings;
	X3DAUDIO_CONE mCone;
};
#endif