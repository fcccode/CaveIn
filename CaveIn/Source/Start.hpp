/********************************************************************
	Filename:	Bear.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _START_HPP_
#define _START_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable.hpp"

class XACore;
class XASound;

class Start: public AudioRenderable
{
public:
	Start(XACore *aCore);
	~Start();
	void RenderAudio(const float deltaTime);
	bool IsOk() const;
	void Play();
	void Pause();
	XASound* getXASound() {return mStart;}
	void InitializeEmitter(XACore *xacore);
	bool getFinished();
	IXAudio2SourceVoice* getSourceVoice();
private:
	XASound *mStart;
	float mElapsedTime;
	float mVolumeAdjustment;
	bool mFinished;
	bool mStarted;
};

#endif 