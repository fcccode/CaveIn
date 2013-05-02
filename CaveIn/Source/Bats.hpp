/********************************************************************
	Filename:	Bats.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _BAT_HPP_
#define _BAT_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable.hpp"

class XACore;
class XASound;

class Bat: virtual AudioRenderable
{
public:
	void RenderAudio(const float deltaTime);
	bool IsOk() const;
	void Play();
	Bat(XACore *aCore, int sound);
	~Bat();
private:
	XASound *mBat;
	float mElapsedTime;
	float mVolumeAdjustment;
};

#endif