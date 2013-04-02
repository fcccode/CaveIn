/********************************************************************
	Filename:	Rats.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _RATS_HPP_
#define _RATS_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable.hpp"
class XACore;

class Rats: virtual AudioRenderable
{
public:
	void RenderAudio(const float deltaTime);
	bool IsOk() const;
	void UpdateEmitter(X3DAUDIO_LISTENER player);
	Rats(XACore *aCore);
	virtual ~Rats();

private:
	bool mOk;
	int mPause;
	float mElapsedTime;
	XAUDIO2_BUFFER mRatData;
	IXAudio2SourceVoice **mRatVoices;
};

#endif