/*
	file:	Frogs.hpp
	Version:	1.1
	Date:	6th 2013.
	Author:	allan c. Milne.

	Exposes:	Frogs.
	Requires:	AudioRenderable, XACore.

	Description:
	Defines an AudioRenderable class that will play the sound of frogs croaking in harmony.
	this will be played at random time intervals.
	Uses frequency adjustment to implement the croaking harmonies.

	The implementation uses XAudio2 buffer and source voices directly.
*/

#ifndef __FROGS_HPP_1_1__
#define __FROGS_HPP_1_1__

#include<XAudio2.h>

#include "AudioRenderable.hpp"

class XACore;

class AmbientBats : public AudioRenderable
{
public:
	//--- called every game frame to check if random time has passed and frogs should play.
	//--- Parameter is time in seconds since last call.
	void RenderAudio (const float deltaTime);

	//--- Indicate if object has been created correctly.
	bool IsOk() const;

	//--- constructor loads and sets up the sound elements.
	AmbientBats(XACore *aCore);
	virtual ~AmbientBats();

private:
	bool mOk;							// indicates if sound and voices have been set up correctly.
	int mPause;							// number of seconds to pause between playing the frogs.
	float mElapsedTime;					// Time since frogs last played.
	XAUDIO2_BUFFER mFrogData;			// the sound sample data for the base frogs croak.
	IXAudio2SourceVoice **mBatVoices;	// array of pointer to source voices for different harmonies.

};	// end Frogs class.

#endif