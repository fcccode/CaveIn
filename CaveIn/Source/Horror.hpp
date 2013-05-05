/*
	file:	Bird.hpp
	Version:	1.1
	Date:	6th March 2013.
	Author:	allan c. Milne.

	Exposes:	Bird.
	Requires:	AudioRenderable, XACore, XASound.

	Description:
	Defines an AudioRenderable class that will play a bird chirp moving towards and away from the listener.
	A XASound object is used to model the bird sound sample.
	Uses volume adjustment to provide the perception of movement.

	The constructor sets up the XASound object.
	Play() starts the sound playing.
	AudioRender(...) implements the dynamic movement.
*/

#ifndef __HORROR_HPP_1_1__
#define __HORROR_HPP_1_1__

#include "AudioRenderable.hpp"

class XACore;
class XASound;

class Horror : public AudioRenderable
{
public:
	//--- Called every game frame to perform the audio animation of the bird's movement.
	//--- Parameter is time in seconds since last call.
	void RenderAudio (const float deltaTime);

	//--- indicates if the object has been created correctly.
	bool IsOk () const;

	//--- constructor sets up the XASound object.
	Horror (XACore *aCore);
	virtual ~Horror();

	//--- Starts the bird sound, does not initiate any movement.
	void Play();

private:
	XASound *mHorror;					// the bird sound.
	float mElapsedTime;				// time since last bird movement applied.
	float mVolumeAdjustment;		// amount of volume to apply for one movement, is changed to indicate "bouncing" of the sound away/towards the listener.
	float mPauseTime;
};	// end Bird class.

#endif