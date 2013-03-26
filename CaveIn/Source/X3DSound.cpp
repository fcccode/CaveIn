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
#include "X3DSound.hpp"

namespace AllanMilne {
namespace Audio {

X3DSound::X3DSound(){
	memset((void*)&mX3DInstance,0,sizeof(X3DAUDIO_HANDLE));
	memset((void*)&mListener,0,sizeof(X3DAUDIO_LISTENER));
}
bool X3DSound::InitializeX3DAudio(XAUDIO2_DEVICE_DETAILS deviceDetails){
	DWORD channelMask;
	XAUDIO2_DEVICE_DETAILS voiceDetails = deviceDetails;
	channelMask = voiceDetails.OutputFormat.dwChannelMask;
	X3DAudioInitialize(channelMask,X3DAUDIO_SPEED_OF_SOUND, mX3DInstance);
	return true;
}
bool X3DSound::InitializeListener(){
	X3DAUDIO_VECTOR front = {0.0f, 0.0f, 1.0f};
	X3DAUDIO_VECTOR top = {0.0f, 0.0f, 1.0f};
	X3DAUDIO_VECTOR position = {0.0f, 0.0f, 1.0f};
	mListener.OrientFront = front;
	mListener.OrientTop = top;
	mListener.Position = position;
	return true;
}
void X3DSound::Apply3D(){

}

}
}