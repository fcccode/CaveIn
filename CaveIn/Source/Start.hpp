/********************************************************************
	Filename:	Start.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	Used to play the start sound.
	Also checks to see if the audio has finished playing.
	Inherits from AudioRenderable class.
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
	//functions from parent class
	void RenderAudio(const float deltaTime);
	inline bool IsOk() const {return (mStart!=NULL);}
	//play and pause functions
	void Play();
	void Pause();
	//if the sound has finished being played then return true.
	bool getFinished();
private:
	XASound *mStart;
	bool mFinished;
	bool mStarted;
};

#endif 