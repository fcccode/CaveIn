/********************************************************************
	Filename:	Horror.hpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	this class inherits from the AudioRenderable class.
	plays the audio after a random period of time.
	influenced by Birds.cpp
*********************************************************************/

#ifndef __HORROR_HPP_1_1__
#define __HORROR_HPP_1_1__

#include "AudioRenderable.hpp"

class XACore;
class XASound;

class Horror : public AudioRenderable
{
public:
	//--- Called every game frame to check if the audio can be played again.
	//--- Parameter is time in seconds since last call.
	void RenderAudio (const float deltaTime);

	//--- indicates if the object has been created correctly.
	inline bool IsOk () const { return (mHorror != NULL); }

	//--- constructor sets up the XASound object.
	Horror (XACore *aCore);
	virtual ~Horror();

	//--- Starts the horror sound
	void Play();

private:
	XASound *mHorror;				// the horror sound.
	float mElapsedTime;				// time since last horror movement applied.	
	float mPauseTime;
};	// end class.

#endif