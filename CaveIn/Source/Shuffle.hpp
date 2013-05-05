/********************************************************************
	Filename:	Bear.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _SHUFFLE_HPP_
#define _SHUFFLE_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable.hpp"

class XACore;
class XASound;

class Shuffle: public AudioRenderable
{
public:
	Shuffle(XACore *aCore);
	~Shuffle();
	void RenderAudio(const float deltaTime);
	void Play();
	void Pause();
	XASound* getXASound() {return mShuffle;}
	IXAudio2SourceVoice* getSourceVoice();
	bool getFinished();
	inline bool IsOk() const {return (mShuffle!=NULL);}
private:
	XASound *mShuffle;
	float mElapsedTime;
	float mVolumeAdjustment;
	bool mFinished;
	bool mStarted;
};

#endif 