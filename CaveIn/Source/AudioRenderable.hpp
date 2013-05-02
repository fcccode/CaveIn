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
class XACore;

class AudioRenderable
{
public:
	virtual void RenderAudio(const float deltaTime) = 0;
	virtual bool IsOk() const = 0;
	virtual ~AudioRenderable(){}
};
#endif