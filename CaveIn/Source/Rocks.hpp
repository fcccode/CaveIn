/********************************************************************
	Filename:	Rocks.h
	Version: 	
	Created:	2013/03/12
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _Rocks_h_
#define _Rocks_h_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "AudioRenderable3D.hpp"

class XACore;
class XASound;

class Rocks: public AudioRenderable3D
{
public:
	Rocks(void);
	~Rocks(void);
	void RenderAudio(const float deltaTime);
	bool IsOk() const;
	void Play();
	Rocks(XACore *aCore, int sound);
	void InitializeEmitter(XACore *xacore);
	inline X3DAUDIO_EMITTER getEmitter(){ return mEmitter;}
	inline X3DAUDIO_DSP_SETTINGS* getDSPSettings(){return &mDSPSettings;}
	IXAudio2SourceVoice* getSourceVoice();
	void UpdateEmitter(X3DAUDIO_VECTOR pos, X3DAUDIO_VECTOR velo);
private:
	XASound *mRock;
	float mElapsedTime;
	float mVolumeAdjustment;
};

#endif // _Rocks_h_

