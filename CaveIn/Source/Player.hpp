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

	inline X3DAUDIO_LISTENER getListener() {return mListener;}

	Player();
	~Player();
private:
	X3DAUDIO_LISTENER mListener;
};

}
}
#endif // _X3DSound_h_