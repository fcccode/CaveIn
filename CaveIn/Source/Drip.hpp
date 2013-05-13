/********************************************************************
	Filename:	Drip.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	this class is heavily based upon the Frogs.cpp class.
	Defines an AudioRenderable class that will play the sound of water drops multiple times.
	this will be played at random time intervals.
	Uses frequency adjustment to implement the variations in audio.

	The implementation uses XAudio2 buffer and source voices directly.
*********************************************************************/
#ifndef __DRIP_HPP_1_1__
#define __DRIP_HPP_1_1__

#include<XAudio2.h>

#include "AudioRenderable.hpp"

class XACore;

class Drip : public AudioRenderable
{
public:
	//--- called every game frame to check if random time has passed and frogs should play.
	//--- Parameter is time in seconds since last call.
	void RenderAudio (const float deltaTime);

	//--- Indicate if object has been created correctly.
	inline bool IsOk() const { return mOk; }

	//--- constructor loads and sets up the sound elements.
	Drip(XACore *aCore);
	virtual ~Drip();

private:
	bool mOk;							// indicates if sound and voices have been set up correctly.
	float mPause;							// number of seconds to pause between playing the drops.
	float mElapsedTime;					// Time since drops last played.
	XAUDIO2_BUFFER mDripData;			// the sound sample data for the base water drop.
	IXAudio2SourceVoice **mDripVoices;	// array of pointer to source voices
	int mAmountDrips;
};
#endif