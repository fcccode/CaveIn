/********************************************************************
	Filename:	Rats.hpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description: 
	Describes Rat class which inherits from AudioRenderable3D.
	this class is used to play one of the rat sounds depending on the 
	flag set when being created
*********************************************************************/
#ifndef _RAT_HPP_
#define _RAT_HPP_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable3D.hpp"

class XACore;
class XASound;

class Rat: public AudioRenderable3D
{
public:
	Rat(XACore *aCore, int sound);
	~Rat();
	//inherited functions
	inline bool IsOk() const {return (mRat!=NULL);}
	void RenderAudio(const float deltaTime);
	//play and pause functions
	void Play();
	void Pause();
	//emitter and dsp setting classes
	void InitializeEmitter(XACore *xacore);
	inline X3DAUDIO_EMITTER getEmitter(){ return mEmitter;}
	inline X3DAUDIO_DSP_SETTINGS* getDSPSettings(){return &mDSPSettings;}
	void UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo);
	void UpdateEmitterPos(X3DAUDIO_VECTOR pos);
	void UpdateEmitterVelo(X3DAUDIO_VECTOR velo);
	IXAudio2SourceVoice* getSourceVoice();
private:
	XASound *mRat;
};

#endif 