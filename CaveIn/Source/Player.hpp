/********************************************************************
	Filename:	Player.hpp
	Version: 	
	Created:	2013/03/26
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _Player_h_
#define _Player_h_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>

namespace AllanMilne {
namespace Audio {

class Player{
public:
	bool InitializeListener();
	bool MoveForward();
	bool ShuffleLeft();
	bool ShuffleRight();
	bool ShuffleBack();
	void Move(const float deltaTime);
	inline bool getMoving() {return mMoving;}
	inline bool getTransition() {return mTransition;}
	inline X3DAUDIO_LISTENER getListener() {return mListener;}
	bool UpdateSettings(X3DAUDIO_HANDLE instance, X3DAUDIO_EMITTER& emitter, X3DAUDIO_DSP_SETTINGS& settings);
	Player();
	~Player();
	void RotateRound(const float deltaTime);
	X3DAUDIO_VECTOR getPlayerNorth();
	X3DAUDIO_VECTOR getPlayerEast();
	X3DAUDIO_VECTOR getPlayerSouth();
	X3DAUDIO_VECTOR getPlayerWest();
private:
	X3DAUDIO_LISTENER mListener;
	bool mTransition;
	bool mMoving;
	float mElapsedTime;
	float mScale;
};

}
}
#endif // _X3DSound_h_