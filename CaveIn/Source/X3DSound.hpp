/********************************************************************
	Filename:	X3DSound.hpp
	Version: 	
	Created:	2013/03/26
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#ifndef _X3DSound_h_
#define _X3DSound_h_

#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>

namespace AllanMilne {
namespace Audio {

class X3DSound{
public:

	bool InitializeX3DAudio(XAUDIO2_DEVICE_DETAILS deviceDetails);
	bool InitializeListener();
	void Apply3D();

	X3DSound();
	~X3DSound();
private:
	X3DAUDIO_HANDLE mX3DInstance;
	X3DAUDIO_LISTENER mListener;
};

}
}
#endif // _X3DSound_h_