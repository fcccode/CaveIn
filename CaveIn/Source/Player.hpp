/********************************************************************
	Filename:	Player.hpp
	Version: 	
	Created:	2013/03/26
	
	Author:		Jake Morey
	
	Description:
	this class describes the movement of the player in 3D space. 
	it stores the X3DAudio Listener and updates this when moving.
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
	Player();
	~Player();
	//initialize listener
	bool InitializeListener();
	//functions for moving around 3D space
	void Move(const float deltaTime);
	bool MoveForward();
	bool ShuffleLeft();
	bool ShuffleRight();
	bool ShuffleBack();
	void RotateRound(const float deltaTime);
	//inline functions that return variables
	inline bool getMoving() {return mMoving;}
	inline bool getTransition() {return mTransition;}
	inline X3DAUDIO_LISTENER getListener() {return mListener;}
	//functions that return vectors for positions in 3D space
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
#endif