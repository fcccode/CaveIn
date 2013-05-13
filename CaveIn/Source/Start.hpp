/********************************************************************
	Filename:	Start.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
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
	void Play();
	void Pause();
	bool getFinished();
	inline bool IsOk() const {return (mStart!=NULL);}
private:
	XASound *mStart;
	bool mFinished;
	bool mStarted;
};

#endif 