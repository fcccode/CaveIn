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

namespace AllanMilne {
namespace Audio {

Player::Player(){
	
	memset((void*)&mListener,0,sizeof(X3DAUDIO_LISTENER));
}
bool Player::InitializeListener(){
	X3DAUDIO_VECTOR front = {0.0f, 0.0f, 1.0f};
	X3DAUDIO_VECTOR top = {0.0f, 0.0f, 1.0f};
	X3DAUDIO_VECTOR position = {0.0f, 0.0f, 1.0f};
	mListener.OrientFront = front;
	mListener.OrientTop = top;
	mListener.Position = position;
	return true;
}

}
}