/********************************************************************
	Filename:	Finish.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	inherits from the AudioRendable class. 
	this class is used to play the finished sound.
	this class haves functionality to check to see if the audio is finished playing. 
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
	//functions from the parent class
	void RenderAudio(const float deltaTime);
	inline bool IsOk() const {return (mFinish!=NULL);}
	//functions for playing and pausing audio
	void Play();
	void Pause();
	//function checks to see if the audio is finished playing
	bool getFinished();
private:
	XASound *mFinish;
	bool mFinished;
	bool mStarted;
};

#endif 