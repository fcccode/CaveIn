/********************************************************************
	Filename:	Directions.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	this class inherits from the virtual class AudioRenderable3D.
	it is used to play a good sound. when creating the class a flag
	is passed that states when sound this class should be initialized with.
*********************************************************************/
#ifndef _DIRECTIONS_HPP_
#define _DIRECTIONS_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable3D.hpp"

class XACore;
class XASound;

class Directions: public AudioRenderable3D
{
public:
	Directions(XACore *aCore, int sound);
	~Directions();
	//functions for controling the audio.
	void RenderAudio(const float deltaTime);
	void Play();
	void Pause();
	inline bool IsOk() const {return (mGood!=NULL);}
	//functions relating to the emitter.
	void InitializeEmitter(XACore *xacore);
	inline X3DAUDIO_EMITTER getEmitter(){ return mEmitter;}
	inline X3DAUDIO_DSP_SETTINGS* getDSPSettings(){return &mDSPSettings;}
	void UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo);
	void UpdateEmitterPos(X3DAUDIO_VECTOR pos);
	void UpdateEmitterVelo(X3DAUDIO_VECTOR velo);
	IXAudio2SourceVoice* getSourceVoice();
private:
	XASound *mGood;
	float mElapsedTime;
	float mVolumeAdjustment;
	float mPaused;
};

#endif 