/********************************************************************
	Filename:	Walking.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	Class is for playing walking sound. 
	has functionality for checking if the sound is finished playing.
	Inherits from AudioRenderable
*********************************************************************/
#ifndef _WALKING_HPP_
#define _WALKING_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable.hpp"

class XACore;
class XASound;

class Walking: public AudioRenderable
{
public:
	Walking(XACore *aCore);
	~Walking();
	//functions from the parent class
	void RenderAudio(const float deltaTime);
	inline bool IsOk() const {return (mWalking!=NULL);}
	//used to play, pause, and reset the audio of the class.
	void Play();
	void Pause();
	void Reset();
	//functions to check if the sound has started being played and if it has finished being played.
	bool getFinished();
	inline bool getStarted(){return mStarted;}
private:
	XASound *mWalking;
	bool mFinished;
	bool mStarted;
};

#endif 