/********************************************************************
	Filename:	Player.cpp
	Version: 	1.0
	Updated:	13/05/2013
	
	Author:		Jake Morey
	
	Description:
	this class describes the movement of the player in 3D space. 
	it stores the X3DAudio Listener and updates this when moving.
*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "Player.hpp"
#include <math.h>
namespace AllanMilne {
namespace Audio {

//=== use anonymous namespace to define array of compass point orientation vectors scoped for this file.
namespace {
	enum Orientation{N, NNE, NE, ENE, E, ESE, SE, SSE, S, SSW, SW, WSW, W, WNW, NW, NNW};
	Orientation currentDir = N;
	Orientation nextDir = N;
	// Calculate value to normalize vectors for increments of 22.5 degrees
	const float coordValue = 1.0f / sqrt(2.0f);
	const float coordValue1 = 0.9239f;
	const float coordValue2 = 0.3827f;

	//=== vectors are normalized.
	X3DAUDIO_VECTOR compassVectors[16] = {
		{0.0f, 0.0f, 1.0f}, {coordValue2, 0.0f, coordValue1}, {coordValue, 0.0f, coordValue}, {coordValue1, 0.0f, coordValue2},
		{1.0f, 0.0f, 0.0f}, {coordValue1, 0.0f, -coordValue2}, {coordValue, 0.0f, -coordValue}, {coordValue2, 0.0f, -coordValue1},
		{0.0f, 0.0f, -1.0f}, {-coordValue2, 0.0f, -coordValue1}, {-coordValue, 0.0f, -coordValue}, {-coordValue1, 0.0f, -coordValue2},
		{-1.0f, 0.0f, 0.0f}, {-coordValue1, 0.0f, coordValue2}, {-coordValue, 0.0f, coordValue}, {-coordValue2, 0.0f, -coordValue1}
	};
} // end anonymous namespace.

Player::Player(){
	mScale = 10.0f;
	mElapsedTime = 0.0f;
	mMoving = false;
	mTransition = false;
	memset((void*)&mListener,0,sizeof(X3DAUDIO_LISTENER));
}
/*
* Initialize the listener at the zero vector, facing north. 
*/
bool Player::InitializeListener(){
	X3DAUDIO_VECTOR top = {0.0f, 1.0f, 0.0f};
	X3DAUDIO_VECTOR position = {0.0f, 0.0f, 0.0f};
	mListener.OrientFront = compassVectors[int(currentDir)];
	mListener.OrientTop = top;
	mListener.Position = position;
	return true;
}
/*
* Call rotate function if need to change orientation
*/
void Player::Move(const float deltaTime){
	if(mTransition == true){
		RotateRound(deltaTime);
	}
}
/*
*	Rotate through the different orientation types.
*/
void Player::RotateRound(const float deltaTime){
	static const float pauseTime	= 1.0f;
	//if the current direction is equal to the direction the user wanted to be facing then reset variables
	//else check to see if a period of time has passed. this elapsed time is need because there
	// is a delay between the playing of the sounds and if there wasn't a delay here you couldn't hear 
	//the change in direction take effect
	mElapsedTime+=deltaTime;
	if(nextDir == currentDir){
		mTransition = false;
		mMoving = false;
	}else if(mElapsedTime>pauseTime){
		mElapsedTime = 0.0f;
		//convert to ints so the variables can be evaluated
		int endPoint = int(nextDir);
		int currentPoint = int(currentDir);
		//if the current position is north and the player needs to be facing west
		//automatically jump to NNW, the same applies if at NNW and the player needs to get to N
		//jump to north.
		//else depending on where the player needs to face increment or decrement the variable
		//afterwards recast the variables as an Orientation enumerated type. 
		if(endPoint==12 && currentPoint == 0){
			currentDir = Orientation(15);
		}else if(endPoint<currentPoint){
			currentDir = Orientation(currentPoint-1);
		}else if(endPoint==0 && currentPoint == 15){
			currentDir = Orientation(0);
		}else if(endPoint>currentPoint){
			currentDir = Orientation(currentPoint+1);
		}
		//set the listener to be facing the right way
		mListener.OrientFront = compassVectors[int(currentDir)];
	}
}
/*
* Move the listener forward in 3D space by the amount of the variable.
*/
bool Player::MoveForward(){
	switch(currentDir){
	case N:
		mListener.Position.z+=mScale;
		return true;
	case E:
		mListener.Position.x+=mScale;
		return true;
	case S:
		mListener.Position.z-=mScale;
		return true;
	case W:
		mListener.Position.x-=mScale;
		return true;
	default: return false;
	}
}
/*
* Set the direction the player wants to be facing 90 degrees left from the current direction.
* also set movement variables to true.
*/
bool Player::ShuffleLeft(){
	switch(currentDir){
	case N: 
		nextDir = W;
		mTransition = true;
		mMoving = true;
		return true;
	case E:
		nextDir = N;
		mTransition = true;
		mMoving = true;
		return true;
	case S:
		nextDir = E;
		mTransition = true;
		mMoving = true;
		return true;
	case W:
		nextDir = S;
		mTransition = true;
		mMoving = true;
		return true;
	default: return false;
	}
}
/*
* Set the direction the player wants to be facing 90 degrees right from the current direction.
* also set movement variables to true.
*/
bool Player::ShuffleRight(){
	switch(currentDir){
	case N:
		nextDir = E;
		mTransition = true;
		mMoving = true;
		return true;
	case E:
		nextDir = S;
		mTransition = true;
		mMoving = true;
		return true;
	case S:
		nextDir = W;
		mTransition = true;
		mMoving = true;
		return true;
	case W:
		nextDir = N;
		mTransition = true;
		mMoving = true;
		return true;
	default: return false;
	}
}
/*
* Set the direction the player wants to be facing 180 degrees from the current direction.
* also set movement variables to true.
*/
bool Player::ShuffleBack(){
	switch(currentDir){
	case N: 
		nextDir = S;
		mTransition = true;
		mMoving = true;
		return true;
	case E:
		nextDir = W;
		mTransition = true;
		mMoving = true;
		return true;
	case S:
		nextDir = N;
		mTransition = true;
		mMoving = true;
		return true;
	case W:
		nextDir = E;
		mTransition = true;
		mMoving = true;
		return true;
	default: return false;
	}
}
/*
* Return a vector with the scale factor added to the z component on the listener position vector.
*/
X3DAUDIO_VECTOR Player::getPlayerNorth(){
	X3DAUDIO_VECTOR pos = {mListener.Position.x,mListener.Position.y,mListener.Position.z+mScale};
	return pos;
}
/*
* Return a vector with the scale factor added to the x component on the listener position vector.
*/
X3DAUDIO_VECTOR Player::getPlayerEast(){
	X3DAUDIO_VECTOR pos = {mListener.Position.x+mScale,mListener.Position.y,mListener.Position.z};
	return pos;
}
/*
* Return a vector with the scale factor subtracted to the z component on the listener position vector.
*/
X3DAUDIO_VECTOR Player::getPlayerSouth(){
	X3DAUDIO_VECTOR pos = {mListener.Position.x,mListener.Position.y,mListener.Position.z-mScale};
	return pos;
}
/*
* Return a vector with the scale factor subtracter to the x component on the listener position vector.
*/
X3DAUDIO_VECTOR Player::getPlayerWest(){
	X3DAUDIO_VECTOR pos = {mListener.Position.x-mScale,mListener.Position.y,mListener.Position.z};
	return pos;
}
}
}