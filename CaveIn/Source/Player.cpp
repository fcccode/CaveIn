/********************************************************************
	Filename:	X3DSound.cpp
	Version: 	
	Created:	26/03/2013
	
	Author:		Jake Morey
	
	Description:
	
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
	// Calculate value to normalize (1,0,1) type 45 degree cardinal points.
	const float coordValue = 1.0f / sqrt(2.0f);
	const float coordValue1 = 0.9239f;
	const float coordValue2 = 0.3827f;

	//=== vectors are normalised.
	X3DAUDIO_VECTOR compassVectors[16] = {
		{0.0f, 0.0f, 1.0f}, {coordValue2, 0.0f, coordValue1}, {coordValue, 0.0f, coordValue}, {coordValue1, 0.0f, coordValue2},
		{1.0f, 0.0f, 0.0f}, {coordValue1, 0.0f, -coordValue2}, {coordValue, 0.0f, -coordValue}, {coordValue2, 0.0f, -coordValue1},
		{0.0f, 0.0f, -1.0f}, {-coordValue2, 0.0f, -coordValue1}, {-coordValue, 0.0f, -coordValue}, {-coordValue1, 0.0f, -coordValue2},
		{-1.0f, 0.0f, 0.0f}, {-coordValue1, 0.0f, coordValue2}, {-coordValue, 0.0f, coordValue}, {-coordValue2, 0.0f, -coordValue1}
	};
} // end anonymous namespace.


Player::Player(){
	scale = 5.0f;
	mElapsedTime = 0.0f;
	moving = false;
	transition = false;
	memset((void*)&mListener,0,sizeof(X3DAUDIO_LISTENER));
}
bool Player::InitializeListener(){
	X3DAUDIO_VECTOR top = {0.0f, 1.0f, 0.0f};
	X3DAUDIO_VECTOR position = {0.0f, 0.0f, 0.0f};
	mListener.OrientFront = compassVectors[int(currentDir)];
	mListener.OrientTop = top;
	mListener.Position = position;
	return true;
}
void Player::Move(const float deltaTime){
	if(transition == true){
		RotateRound(deltaTime);
	}
}
void Player::RotateRound(const float deltaTime){
	static const float pauseTime	= 1.0f;

	mElapsedTime+=deltaTime;
	if(nextDir == currentDir){
		transition = false;
		moving = false;
	}else if(mElapsedTime>pauseTime){
		mElapsedTime = 0.0f;
		int endPoint = int(nextDir);
		int currentPoint = int(currentDir);
		if(endPoint==12 && currentPoint == 0){
			currentDir = Orientation(15);
		}else if(endPoint<currentPoint){
			currentDir = Orientation(currentPoint-1);
		}else if(endPoint==0 && currentPoint == 15){
			currentDir = Orientation(0);
		}else if(endPoint>currentPoint){
			currentDir = Orientation(currentPoint+1);
		}
		mListener.OrientFront = compassVectors[int(currentDir)];
	}
}
bool Player::MoveForward(){
	switch(currentDir){
	case N:
		mListener.Position.z+=scale;
		return true;
	case E:
		mListener.Position.x+=scale;
		return true;
	case S:
		mListener.Position.z-=scale;
		return true;
	case W:
		mListener.Position.x-=scale;
		return true;
	default: return false;
	}
}
bool Player::ShuffleLeft(){
	switch(currentDir){
	case N: 
		nextDir = W;
		transition = true;
		moving = true;
		return true;
	case E:
		nextDir = N;
		transition = true;
		moving = true;
		return true;
	case S:
		nextDir = E;
		transition = true;
		moving = true;
		return true;
	case W:
		nextDir = S;
		transition = true;
		moving = true;
		return true;
	default: return false;
	}
}
bool Player::ShuffleRight(){
	switch(currentDir){
	case N:
		nextDir = E;
		transition = true;
		moving = true;
		return true;
	case E:
		nextDir = S;
		transition = true;
		moving = true;
		return true;
	case S:
		nextDir = W;
		transition = true;
		moving = true;
		return true;
	case W:
		nextDir = N;
		transition = true;
		moving = true;
		return true;
	default: return false;
	}
}
bool Player::ShuffleBack(){
	switch(currentDir){
	case N: 
		nextDir = S;
		transition = true;
		moving = true;
		return true;
	case E:
		nextDir = W;
		transition = true;
		moving = true;
		return true;
	case S:
		nextDir = N;
		transition = true;
		moving = true;
		return true;
	case W:
		nextDir = E;
		transition = true;
		moving = true;
		return true;
	default: return false;
	}
}
X3DAUDIO_VECTOR Player::getPlayerNorth(){
	X3DAUDIO_VECTOR pos = {mListener.Position.x,mListener.Position.y,mListener.Position.z+scale};
	return pos;
}
X3DAUDIO_VECTOR Player::getPlayerEast(){
	X3DAUDIO_VECTOR pos = {mListener.Position.x+scale,mListener.Position.y,mListener.Position.z};
	return pos;
}
X3DAUDIO_VECTOR Player::getPlayerSouth(){
	X3DAUDIO_VECTOR pos = {mListener.Position.x,mListener.Position.y,mListener.Position.z-scale};
	return pos;
}
X3DAUDIO_VECTOR Player::getPlayerWest(){
	X3DAUDIO_VECTOR pos = {mListener.Position.x-scale,mListener.Position.y,mListener.Position.z};
	return pos;
}
}
}