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
	void InitializeEmitter(XACore *xacore);
	void UpdateEmitter(X3DAUDIO_LISTENER player);
	Rats(XACore *aCore);
	~Rats();
	inline X3DAUDIO_EMITTER getEmitter(){ return mEmitter;}
	inline X3DAUDIO_DSP_SETTINGS getDSPSettings(){return mDSPSettings;}
	inline IXAudio2SourceVoice* getSourceVoice(){return mRatVoices[0];}
private:
	bool mOk;
	int mPause;
	float mElapsedTime;
	XAUDIO2_BUFFER mRatData;
	IXAudio2SourceVoice **mRatVoices;
};

#endif