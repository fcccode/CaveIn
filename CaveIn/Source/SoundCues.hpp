/********************************************************************
	Filename:	SoundCues.hpp
	Version: 	1.0
	Updated:	12/05/2013
	Author:		Jake  Morey implements Allan C. Milne IGameCore interface.
	
	Description:
	This is a concrete implementation of the IGameCore interface for the application logic.
	This is the concrete strategy class called  from the context WinCore class as part of a strategy pattern.
	*	See IGameCore.hpp for details of this interface.

	this application plays a soundscape made up of a variety of sound elements.
*********************************************************************/
#ifndef _SOUNDS_CUES_HPP_
#define _SOUNDS_CUES_HPP_

#include <xaudio2.h>
#include <X3DAudio.h>
#include <vector>
using std::vector;
#include "IGameCore.hpp"
using AllanMilne::IGameCore;
#include "AudioRenderable3D.hpp"
#include "Player.hpp"
#include "Path.hpp"
#include "Finish.hpp"
#include "Wall.hpp"
#include "Start.hpp"
#include "Soundscape.hpp"
#include "Walking.hpp"
#include "Shuffle.hpp"

class XACore;
class XASound;
class Player;

#define MAP_SIZE 11

class SoundCues : public IGameCore {
public:
	enum mTileTypes {tWall, tStart, tFinish, tGood, tBad, tPath};
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
	: mXACore (NULL), locationX(0), locationZ(0), mFinished(false), mAmbient(NULL)
	{} // end constructor.
private:
	inline mTileTypes CheckMap(int x, int y) {return mMap[x][y].tile;}
	//Functions for updating emitters from AudioRenderable3D classes.
	void UpdateSettings(AudioRenderable3D* audio);
	void Apply3D();
	//Checks the iterator isn't the same as last time.
	int CheckIter(int iter, int check);
	//Checks to see if the start sound has finished.
	bool CheckStart();
	//Checks to sett if the finished sound has finished.
	bool CheckFinish();
	//Holds key checks for movement.
	void Move();
	//Checks if the player is inside the array.
	bool CheckMoveForward();
	//checks the type of tile infront of the player.
	bool CheckForwardTile(int x, int y, X3DAUDIO_VECTOR pos);
	//change the orientation.
	void ChangeOrientationLeft();
	void ChangeOrientationBack();
	void ChangeOrientationRight();
	//update the sounds locations based upon the tiles
	void UpdateSoundTile();
	//start playing the sounds based upon the tiles surronding the player
	void SoundTiles(int z, int x, X3DAUDIO_VECTOR pos);
	//stop all playing sounds
	void StopAllSounds();
	//set up the tile map
	void SetupMap();
	void SetUpGoodTiles();
	void SetUpBadTiles();
	void SetUpPathTiles();
	void SetUpOtherTiles();
	void ClearArray();
	//initialise all sounds
	bool InitSounds();
	bool InitRats();
	bool InitBats();
	bool InitBear();
	bool InitOtherWarnings();
	bool InitGood();
	bool InitOther();
	bool InitWalk();
	//local variables
	X3DAUDIO_HANDLE mX3DInstance;
	XACore *mXACore;
	Soundscape *mAmbient;
	Player *mPlayer;
	//tile map array and indexs for the array
	Tiles mMap[MAP_SIZE][MAP_SIZE];
	int locationX, locationZ;
	//lists for both good and bad sounds, as well as integers to store positions in the vectors.
	vector<AudioRenderable3D*> mGoodSounds;
	vector<AudioRenderable3D*> mBadSounds;
	int mGoodIter, mBadIter;
	//3D sounds
	Path *mPath;
	Wall *mWall; 
	//AudioRenderable sounds
	Start *mStart;
	Finish *mFinish; 
	Walking *mWalking;
	Shuffle *mShuffle;
	//booleans for if certain types of sounds need playing
	bool mPlayPath;
	bool mPlayGood;
	bool mPlayBad;
	//finished the game variable
	bool mFinished; 
}; // end Soundcues class.

#endif