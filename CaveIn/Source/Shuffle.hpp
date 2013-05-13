/********************************************************************
	Filename:	Shuffle.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	this function inherits from the AudioRenderable class.
	it has functionaility to check if the audio has started playing
	as well as functionality that checks it the audio has finished.
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
	//functions from the parent class
	void RenderAudio(const float deltaTime);
	inline bool IsOk() const {return (mShuffle!=NULL);}
	//functions for controlling the audio
	void Play();
	void Pause();
	void Reset();
	//function that check if the audio has started playing and if it has finished playing
	inline bool getStarted(){return mStarted;}
	bool getFinished();
private:
	XASound *mShuffle;
	bool mFinished;
	bool mStarted;
};

#endif 