/********************************************************************
	Filename:	Walking.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	
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
	void RenderAudio(const float deltaTime);
	void Play();
	void Pause();
	void Reset();
	bool getFinished();
	inline bool getStarted(){return mStarted;}
	inline bool IsOk() const {return (mWalking!=NULL);}
private:
	XASound *mWalking;
	bool mFinished;
	bool mStarted;
};

#endif 