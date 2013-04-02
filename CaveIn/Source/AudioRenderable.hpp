/********************************************************************
	Filename:	AudioRenderable.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _AUDIO_RENDERABLE_
#define _AUDIO_RENDERABLE_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>

class AudioRenderable
{
public:
	virtual void RenderAudio(const float deltaTime) = 0;
	virtual bool IsOk() const = 0;
	virtual void UpdateEmitter(X3DAUDIO_LISTENER player)=0;
	virtual ~AudioRenderable(){}
	AudioRenderable(){
		memset((void*)&mDSPSettings,0,sizeof(X3DAUDIO_DSP_SETTINGS));
		memset((void*)&mEmitter,0,sizeof(X3DAUDIO_EMITTER));
	}
protected:
	X3DAUDIO_EMITTER mEmitter;
	X3DAUDIO_DSP_SETTINGS mDSPSettings;
};
#endif