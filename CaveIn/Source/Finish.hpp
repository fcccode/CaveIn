/********************************************************************
	Filename:	Bear.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _FINISH_HPP_
#define _FINISH_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable.hpp"

class XACore;
class XASound;

class Finish: public AudioRenderable
{
public:
	Finish(XACore *aCore);
	~Finish();
	void RenderAudio(const float deltaTime);
	bool IsOk() const;
	void Play();
	void Pause();
	XASound* getXASound() {return mFinish;}
	void InitializeEmitter(XACore *xacore);
	bool getFinished();
	IXAudio2SourceVoice* getSourceVoice();
private:
	XASound *mFinish;
	float mElapsedTime;
	float mVolumeAdjustment;
	bool mFinished;
	bool mStarted;
};

#endif 