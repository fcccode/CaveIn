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
	Rats(XACore *aCore, int sound);
	~Rats();
	
	void RenderAudio(const float deltaTime);
	bool IsOk() const;
private:
	bool mOk;
	int mPause;
	float mElapsedTime;
	XAUDIO2_BUFFER mRatData;
	IXAudio2SourceVoice **mRatVoices;
};

#endif