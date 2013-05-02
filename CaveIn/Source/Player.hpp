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
	void Move();
	inline bool getFinishedMoving() {return moving;}
	inline X3DAUDIO_LISTENER getListener() {return mListener;}
	bool UpdateSettings(X3DAUDIO_HANDLE instance, X3DAUDIO_EMITTER& emitter, X3DAUDIO_DSP_SETTINGS& settings);
	Player();
	~Player();
	void RotateRound();
private:
	X3DAUDIO_LISTENER mListener;
	bool transition;
	bool moving;
	float scale;
};

}
}
#endif // _X3DSound_h_