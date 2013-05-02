/***
for stereo input need matrix 4*4 first two how much of the left channel going to left and right the second two for right channel
****/
/*
	File:	Soundscape1.hpp
	Version:	0.1
	Date:	10th January 2013.
	Author:	Allan C. Milne.

	Requires:	IGameCore, XACore.

	Description:
	This is a concrete implementation of the IGameCore interface for the application logic.
	This is the concrete strategy class called  from the context WinCore class as part of a strategy pattern.
	*	See IGameCore.hpp for details of this interface.

	this application will eventually play a soundscape made up of a variety of sound elements.
	this version is the base definition of the class with no sound elements.
*/

#ifndef _SOUNDS_CUES_HPP_
#define _SOUNDS_CUES_HPP_

#include <xaudio2.h>
#include <X3DAudio.h>
#include "IGameCore.hpp"
using AllanMilne::IGameCore;
#include "AudioRenderable3D.hpp"
#include "Player.hpp"
#include "Rocks.hpp"
#include <vector>
using std::vector;

class XACore;
class XASound;
class Player;

class SoundCues : public IGameCore {
public:
	enum mTileTypes {Wall, Start, Finish, Good, Bad, Path};
	struct Tiles{
		mTileTypes tile;
		bool played;
	};

	//=== Implementation of the IGameCore interface.

	//--- create and initialize XACore and the sounds to be played.
	//--- since this game has no graphics the argument is not used.
	bool SetupGame (HWND aWindow);

	//--- process a single game frame.
	void ProcessGameFrame (const float deltaTime);

	//--- Release all XACore, XASound and other audio resources.
	void CleanupGame ();
	//=== Game specific behaviour.
	//--- Default constructor.
	SoundCues () 
	: mXACore (NULL), locationX(0), locationZ(0), mapSize(11), movementEnabled(true)
	{} // end constructor.

private:

	void UpdateSettings(AudioRenderable3D* audio);
	void Apply3D();
	void ClearArray();
	void SetupMap();
	inline mTileTypes CheckMap(int x, int y) {return mMap[x][y].tile;}
	bool CheckForwardTile(int x, int y);
	bool CheckMoveForward();
	void ChangeOrientation(int dir);
	void UpdateSoundTile();
	void PlaySoundTiles(int z, int x, X3DAUDIO_VECTOR pos);
	void StopAllSounds();
	bool InitRats();
	bool InitBats();
	bool InitBear();
	//--- the XAudio2 engine encapsulation.
	XACore *mXACore;
	XAUDIO2_FILTER_PARAMETERS mFParams;
	Player *mPlayer;
	X3DAUDIO_HANDLE mX3DInstance;
	vector<AudioRenderable3D*> mGoodSounds;
	vector<AudioRenderable3D*> mBadSounds;
	int mGoodIter, mBadIter;
	int locationX, locationZ, mapSize;
	bool movementEnabled;
	Tiles mMap[11][11]; 
}; // end Soundscape1 class.

#endif