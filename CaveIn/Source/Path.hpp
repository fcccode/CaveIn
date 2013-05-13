/********************************************************************
	Filename:	Path.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	this class inherits from AudioRenderable3D.
	this class is used to play the audio for the path tiles.
	will keep playing the sound after a period of time.
*********************************************************************/
#ifndef _PATH_HPP_
#define _PATH_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable3D.hpp"

class XACore;
class XASound;

class Path: public AudioRenderable3D
{
public:
	Path(XACore *aCore);
	~Path();
	//render audio is used to play the audio again after a period time
	void RenderAudio(const float deltaTime);
	inline bool IsOk() const {return (mPath!=NULL);}
	//play and pause functions
	void Play();
	void Pause();
	//emitter functions
	void InitializeEmitter(XACore *xacore);
	inline X3DAUDIO_EMITTER getEmitter(){ return mEmitter;}
	inline X3DAUDIO_DSP_SETTINGS* getDSPSettings(){return &mDSPSettings;}
	void UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo);
	void UpdateEmitterPos(X3DAUDIO_VECTOR pos);
	void UpdateEmitterVelo(X3DAUDIO_VECTOR velo);
	IXAudio2SourceVoice* getSourceVoice();
private:
	XASound *mPath;
	float mElapsedTime;
};

#endif 