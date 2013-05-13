/********************************************************************
	Filename:	SoundCues.cpp
	Version: 	1.0
	Updated:	12/05/2013
	Author:		Jake  Morey implements Allan C. Milne IGameCore interface.

	Description:
	This is the implementation of the SoundsCues class members;
	*	See SoundCues.hpp for details of this interface.
	this application plays a soundscape made up of a variety of sound elements.
	*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include <vector>
using std::vector;
#include "XACore.hpp"
using AllanMilne::Audio::XACore;
#include "XASound.hpp"
using AllanMilne::Audio::XASound;
#include "Player.hpp"
using AllanMilne::Audio::Player;
#include "Bats.hpp"
#include "Rats.hpp"
#include "Bear.hpp"
#include "Water.hpp"
#include "Rocks.hpp"
#include "Path.hpp"
#include "Wall.hpp"
#include "Directions.hpp"
#include "Soundcues.hpp"
#include "Finish.hpp"
#include "Start.hpp"
#include "Walking.hpp"
#include "Shuffle.hpp"
/*
* Namespace for local enumerated type orientation
*/
namespace{
	enum Orientation{North, East, South, West};
	Orientation PlayerOrientation = North;
}
//--- create and initialize XACore.
//--- Since this game has no graphics the HWND argument is not used.
bool SoundCues::SetupGame (HWND aWindow){
	mPlayPath = false;
	// set up the XAudio2 engine and mastering voice; check if ok.
	mXACore = new XACore ();
	if (mXACore->GetEngine() == NULL || mXACore->GetMasterVoice() == NULL) {
		return false;
	}
	//create the X3DInstance
	memset((void*)&mX3DInstance,0,sizeof(X3DAUDIO_HANDLE));
	DWORD channelMask = mXACore->GetDeviceDetails().OutputFormat.dwChannelMask;
	mXACore->GetChannelCount();
	X3DAudioInitialize(channelMask,X3DAUDIO_SPEED_OF_SOUND, mX3DInstance);
	//initialize the player and X3DAudio Listener
	mPlayer = new Player();
	mPlayer->InitializeListener();
	//create the soundscape class
	mAmbient = new Soundscape();
	//initialize all sounds
	InitSounds();
	//initialize the tile map array
	SetupMap();
	return true;		// All has been setup without error.
} // end SetupGame function.
//--- process a single game frame.
void SoundCues::ProcessGameFrame (const float deltaTime){
	//checks if start and finish sounds are playing
	//if not do other functions
	if(CheckStart() == false && CheckFinish() == false){
		//process ambient sounds
		mAmbient->ProcessGameFrame(deltaTime);
		//move player
		mPlayer->Move(deltaTime);
		//if the player is rotating update the emitters
		//if it has stopped and the shuffle sound is still play stop playing
		if(mPlayer->getTransition()){
			Apply3D();
		}else if(mShuffle->getStarted() == true){
			mShuffle->Reset();
		}
		//if the player is not moving
		//check to see if it was recently walking or shuffling
		if(mPlayer->getMoving() == false){
			//if neither walking or turning recently then just move
			//if just finished playing the walking sounds then update the sounds that need to be played and reset the walking sound
			//if just finished shuffling the reset shuffling sound and move
			if(mWalking->getStarted()== false && mShuffle->getStarted() == false){
				Move();
			}else if(mWalking->getStarted() == true && mWalking->getFinished() == true){
				mMap[locationZ][locationX].played = true;
				mBadIter=CheckIter(mBadIter, mBadSounds.size());
				mGoodIter=CheckIter(mGoodIter, mGoodSounds.size());
				StopAllSounds();
				UpdateSoundTile();
				mWalking->Reset();
				Move();
			}else if(mShuffle->getStarted() == true && mShuffle->getFinished()==true){
				mShuffle->Reset();
				Move();
			}
		}
		//if true then play path sound
		if(mPlayPath){
			mPath->RenderAudio(deltaTime);
		}
		//if true then play a sound from the good list
		if(mPlayGood){
			mGoodSounds.at(mGoodIter)->RenderAudio(deltaTime);
		}
		//if true then play a sound from the bas list
		if(mPlayBad){
			mBadSounds.at(mBadIter)->RenderAudio(deltaTime);
		}
	}
} // end ProcessGameFrame function.
/*
* Depending on the keypress do a different task.
*/
void SoundCues::Move(){
	//if keypress was UP or W then try to move forward
	//if LEFT or A then rotate left, if Right or D then rotate right
	//if BACK or S then rotate 180 degrees
	if( (GetAsyncKeyState(VK_UP) & 0x0001) || (GetAsyncKeyState('W') & 0x0001) ){
		if(CheckMoveForward()){
			mPlayer->MoveForward();
			mWalking->Play();
		}
	}else if( (GetAsyncKeyState(VK_LEFT) & 0x0001) || (GetAsyncKeyState('A') & 0x0001) ){
		ChangeOrientationLeft();
	}else if((GetAsyncKeyState(VK_RIGHT) & 0x0001) || (GetAsyncKeyState('D') & 0x0001) ){
		ChangeOrientationRight();
	}else if((GetAsyncKeyState(VK_DOWN) & 0x0001) || (GetAsyncKeyState('S') & 0x0001) ){
		ChangeOrientationBack();
	}
}
/*
* Checks to see the start sound has finished being played.
*/
bool SoundCues::CheckStart(){
	//if the current element in the array is start and it hasn't been played 
	//check that the start sound hasn't finished playing, if it hasn't do nothing
	//if it has then update the tiles around the player and init the ambient sounds.
	if(mMap[locationZ][locationX].tile == tStart && mMap[locationZ][locationX].played == false){
		if(mStart->getFinished()){
			mMap[locationZ][locationX].played = true;
			mBadIter = rand()%mBadSounds.size();
			mGoodIter = rand()%mGoodSounds.size();
			UpdateSoundTile();
			//try init ambient sounds if can't quit
			if(mAmbient->SetupGame(mXACore)==false){
				mFinished = true;
				PostQuitMessage(0);
			}
		}
		mStart->Play();
		return true;
	}
	return false;
}
/*
* Checks to see the finish sound has finished being played.
*/
bool SoundCues::CheckFinish(){
	//checks if the current position is the finished tile
	//if so then check the sounf has finished play
	//if this is also true the post a quit message
	if(mMap[locationZ][locationX].tile == tFinish){
		if(mFinish->getFinished()){
			mFinished = true;
			PostQuitMessage(0);
		}
		mFinish->Play();
	}
	return mFinished;
}
/*
* Checks the tiles around the player for tiles that have not been played before.
*/
void SoundCues::UpdateSoundTile(){
	//set the play variables to false
	mPlayPath = false;
	mPlayGood = false;
	mPlayBad = false;
	//check all four elements around the player to see what tiles havnt been played
	if(mMap[locationZ-1][locationX].played == false){
		SoundTiles(locationZ-1,locationX,mPlayer->getPlayerNorth());
	}
	if(mMap[locationZ+1][locationX].played == false){
		SoundTiles(locationZ+1,locationX,mPlayer->getPlayerSouth());
	}
	if(mMap[locationZ][locationX-1].played == false){
		SoundTiles(locationZ,locationX-1,mPlayer->getPlayerWest());
	}
	if(mMap[locationZ][locationX+1].played == false){
		SoundTiles(locationZ,locationX+1,mPlayer->getPlayerEast());
	}
}
/*
* Update the emitter positions of the correct sound in relation to the player.
*/
void SoundCues::SoundTiles(int z, int x, X3DAUDIO_VECTOR pos){
	switch (CheckMap(z,x)){
	case tFinish:
	case tGood:
		//update the position of the good sound and play for the first time
		mGoodSounds.at(mGoodIter)->UpdateEmitterPos(pos);
		UpdateSettings(mGoodSounds.at(mGoodIter));
		mGoodSounds.at(mGoodIter)->Play();
		mPlayGood = true;
		break;
	case tPath:
		//update the position of the path sound and play for the first time
		mPath->UpdateEmitterPos(pos);
		UpdateSettings(mPath);
		mPath->Play();
		mPlayPath = true;
		break;
	case tBad:
		//update the position of the bas sound and play for the first time
		mBadSounds.at(mBadIter)->UpdateEmitterPos(pos);
		UpdateSettings(mBadSounds.at(mBadIter));
		mBadSounds.at(mBadIter)->Play();
		mPlayBad = true;
		break;
	case tWall:
		//update the position of the wall
		mWall->UpdateEmitterPos(pos);
		UpdateSettings(mWall);
		break;
	}
}
/*
* Checks to make sure the player won't leave the array when moving forward.
* This is based upon the orientation of the player.
* Returns true if the player can move forward.
*/
bool SoundCues::CheckMoveForward(){
	switch(PlayerOrientation){
	case North:
		if((locationZ-1)>=0){
			//if inside the array check if the user can move forward
			//if they can set the current tile to be a wall and move forward
			if(CheckForwardTile(locationZ-1,locationX, mPlayer->getPlayerNorth())){
				mMap[locationZ][locationX].tile=tWall;
				locationZ-=1;
				return true;
			}
		}
		break;
	case East:
		if((locationX+1)<MAP_SIZE){
			//if inside the array check if the user can move forward
			//if they can set the current tile to be a wall and move forward
			if(CheckForwardTile(locationZ,locationX+1, mPlayer->getPlayerEast())){
				mMap[locationZ][locationX].tile=tWall;
				locationX+=1;
				return true;
			}
		}
		break;
	case South:
		if((locationZ+1)<MAP_SIZE){
			//if inside the array check if the user can move forward
			//if they can set the current tile to be a wall and move forward
			if(CheckForwardTile(locationZ+1,locationX, mPlayer->getPlayerSouth())){
				mMap[locationZ][locationX].tile=tWall;
				locationZ+=1;
				return true;
			}
		}
		break;
	case West:
		if((locationX-1)>=0){
			//if inside the array check if the user can move forward
			//if they can set the current tile to be a wall and move forward
			if(CheckForwardTile(locationZ,locationX-1, mPlayer->getPlayerWest())){
				mMap[locationZ][locationX].tile=tWall;
				locationX-=1;
				return true;
			}
		}
		break;
	}
	return false;
}
/*
* Change the orientation variable anti-clockwise degrees
*/
void SoundCues::ChangeOrientationLeft(){
	switch(PlayerOrientation){
	case North: PlayerOrientation = West; break;
	case East: PlayerOrientation = North; break;
	case South: PlayerOrientation = East; break;
	case West: PlayerOrientation = South; break;
	}
	//move the player and update the sounds
	mPlayer->ShuffleLeft();
	Apply3D();
	mShuffle->Play();
}
/*
* Change the orientation variable clockwise 90 degrees
*/
void SoundCues::ChangeOrientationRight(){
	switch(PlayerOrientation){
	case North: PlayerOrientation = East; break;
	case East: PlayerOrientation = South; break;
	case South: PlayerOrientation = West; break;
	case West: PlayerOrientation = North; break;
	}
	//move the player and update the sounds
	mPlayer->ShuffleRight();
	Apply3D();
	mShuffle->Play();
}
/*
* Change the orientation variable 180 degreed
*/
void SoundCues::ChangeOrientationBack(){
	switch(PlayerOrientation){
	case North: PlayerOrientation = South; break;
	case East: PlayerOrientation = West; break;
	case South: PlayerOrientation = North; break;
	case West: PlayerOrientation = East; break;
	}
	//move the player and update the sounds
	mPlayer->ShuffleBack();
	Apply3D();
	mShuffle->Play();
}
/*
* Check the type of the tile in front of the player.
* If it is not a wall or a bad sound the return true.
* else return false. if a wall also play the wall sound.
*/
bool SoundCues::CheckForwardTile(int x, int y, X3DAUDIO_VECTOR pos){
	switch(CheckMap(x,y)){
	case tPath:
	case tFinish:
	case tStart:
	case tGood: return true; break;
	case tWall: 
		mWall->UpdateEmitterPos(pos);
		UpdateSettings(mWall);
		mWall->Play();
	case tBad: return false; break;
	default: return false; break;
	}
}
/*
* Apply 3D to the Path and Wall objects. 
* As well as the currently playing good and bad sounds
*/
void SoundCues::Apply3D(){
	UpdateSettings(mPath);
	UpdateSettings(mWall);
	UpdateSettings(mGoodSounds.at(mGoodIter));
	UpdateSettings(mBadSounds.at(mBadIter));
}
/*
* Update the emitter & dsp settings of a chosen AudioRenderable3D object via the use of X3DAudio Calculate
*/
void SoundCues::UpdateSettings(AudioRenderable3D* audio){
	X3DAudioCalculate(mX3DInstance, &mPlayer->getListener(), &audio->getEmitter(), 
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT ,
		audio->getDSPSettings());
	audio->getSourceVoice()->SetOutputMatrix(NULL, audio->getDSPSettings()->SrcChannelCount, audio->getDSPSettings()->DstChannelCount, audio->getDSPSettings()->pMatrixCoefficients);
	audio->getSourceVoice()->SetFrequencyRatio(audio->getDSPSettings()->DopplerFactor);
	XAUDIO2_FILTER_PARAMETERS FilterParams = {LowPassFilter, 2.0f*sinf(X3DAUDIO_PI/6.0f * audio->getDSPSettings()->LPFDirectCoefficient), 1.0f};
	audio->getSourceVoice()->SetFilterParameters(&FilterParams);
}
/*
* Randomize a value between 0 and size.
* Then check this is not the same as the previous value.
* If it is increment if smaller than size else decrement. 
*/
int SoundCues::CheckIter(int check, int size){
	int iter = rand()%size;
	if(iter == check){
		if(iter+1 < size){
			iter++;
		}else{
			iter--;
		}
	}
	return iter;
}
/*
* Stop all 3D sounds from playing.
*/
void SoundCues::StopAllSounds(){
	vector<AudioRenderable3D*>::const_iterator iter;
	mWall->Pause();
	mPath->Pause();
	for(iter=mGoodSounds.begin(); iter!=mGoodSounds.end(); ++iter){
		(*iter)->Pause();
	}
	for(iter=mBadSounds.begin(); iter!=mBadSounds.end(); ++iter){
		(*iter)->Pause();
	}
}
/*
* Sets up the tile map array.
* Class all individual functions.
*/
void SoundCues::SetupMap(){
	ClearArray();
	SetUpOtherTiles();
	SetUpGoodTiles();
	SetUpBadTiles();
	SetUpPathTiles();
}
/*
* Set all elements in the array to be Wall tiles.
* Set played to be false.
*/
void SoundCues::ClearArray(){
	for(int i = 0; i < MAP_SIZE; i++){
		for (int j = 0; j < MAP_SIZE; j++){
			mMap[i][j].tile = tWall;
			mMap[i][j].played = false;
		}
	}
}
/*
* Initializes the locations of the start and finish tiles.
*/
void SoundCues::SetUpOtherTiles(){
	locationX = 2;
	locationZ = 9;
	mMap[locationZ][locationX].tile = tStart;
	mMap[1][9].tile = tFinish;
}
/*
* Initializes the locations of the good tiles.
*/
void SoundCues::SetUpGoodTiles(){
	mMap[3][2].tile = tGood;
	mMap[3][8].tile = tGood;

	mMap[4][6].tile = tGood;
	mMap[4][8].tile = tGood;
	
	mMap[5][1].tile = tGood;
	mMap[5][7].tile = tGood;
	
	mMap[6][3].tile = tGood;
	mMap[6][7].tile = tGood;
	
	mMap[7][4].tile = tGood;
	mMap[7][9].tile = tGood;
	
	mMap[8][3].tile = tGood;
	mMap[8][8].tile = tGood;
}
/*
* Initializes the locations of the bad tiles.
*/
void SoundCues::SetUpBadTiles(){
	mMap[1][7].tile = tBad;

	mMap[2][6].tile = tBad;

	mMap[5][5].tile = tBad;

	mMap[6][5].tile = tBad;

	mMap[7][1].tile = tBad;

	mMap[8][1].tile = tBad;
	mMap[8][6].tile = tBad;

	mMap[9][4].tile = tBad;
	mMap[9][9].tile = tBad;
}
/*
* Initializes the locations of the path tiles.
*/
void SoundCues::SetUpPathTiles(){
	mMap[1][8].tile = tPath;

	mMap[2][8].tile = tPath;

	mMap[3][1].tile = tPath;
	mMap[3][3].tile = tPath;
	mMap[3][4].tile = tPath;
	mMap[3][5].tile = tPath;
	mMap[3][6].tile = tPath;

	mMap[4][1].tile = tPath;
	mMap[4][9].tile = tPath;

	mMap[5][6].tile = tPath;
	mMap[5][9].tile = tPath;

	mMap[6][1].tile = tPath;
	mMap[6][2].tile = tPath;
	mMap[6][4].tile = tPath;
	mMap[6][9].tile = tPath;

	mMap[7][7].tile = tPath;

	mMap[8][2].tile = tPath;
	mMap[8][4].tile = tPath;
	mMap[8][7].tile = tPath;
	mMap[8][9].tile = tPath;

}
/*
* Initializes all sounds.
* Returns false if there is an error.
*/
bool SoundCues::InitSounds(){
	if(!InitRats()){
		return false;
	}else if(!InitBats()){
		return false;
	}else if(!InitBear()){
		return false;
	}else if(!InitOtherWarnings()){
		return false;
	}else if(!InitGood()){
		return false;
	}else if(!InitOther()){
		return false;
	}else if(!InitWalk()){
		return false;
	}
	return true;
}
/*
* Initializes all bat sounds.
* Returns false if there is an error.
*/
bool SoundCues::InitBats(){
	Bat *bat = new Bat(mXACore, 0);
	if(!bat->IsOk()){
		MessageBox(NULL,"Error loading bat.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete bat;
		return false;
	}
	bat->InitializeEmitter(mXACore);
	Bat *bat1 = new Bat(mXACore, 1);
	if(!bat1->IsOk()){
		MessageBox(NULL,"Error loading bat1.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete bat;
		delete bat1;
		return false;
	}
	bat1->InitializeEmitter(mXACore);
	Bat *bat2 = new Bat(mXACore, 2);
	if(!bat2->IsOk()){
		MessageBox(NULL,"Error loading bat2.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete bat;
		delete bat1;
		delete bat2;
		return false;
	}
	bat2->InitializeEmitter(mXACore);
	Bat *bat3 = new Bat(mXACore, 3);
	if(!bat3->IsOk()){
		MessageBox(NULL,"Error loading bat3.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete bat;
		delete bat1;
		delete bat2;
		delete bat3;
		return false;
	}
	bat3->InitializeEmitter(mXACore);
	Bat *bat4 = new Bat(mXACore, 4);
	if(!bat4->IsOk()){
		MessageBox(NULL,"Error loading bat4.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete bat;
		delete bat1;
		delete bat2;
		delete bat3;
		delete bat4;
		return false;
	}
	bat4->InitializeEmitter(mXACore);
	mBadSounds.push_back((AudioRenderable3D*)bat);
	mBadSounds.push_back((AudioRenderable3D*)bat1);
	mBadSounds.push_back((AudioRenderable3D*)bat2);
	mBadSounds.push_back((AudioRenderable3D*)bat3);
	mBadSounds.push_back((AudioRenderable3D*)bat4);
	return true;
}
/*
* Initializes all rat sounds.
* Returns false if there is an error.
*/
bool SoundCues::InitRats(){
	Rat *rat = new Rat(mXACore, 0);
	if(!rat->IsOk()){
		MessageBox(NULL,"Error loading rat.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete rat;
		return false;
	}
	rat->InitializeEmitter(mXACore);
	Rat *rat1 = new Rat(mXACore, 1);
	if(!rat1->IsOk()){
		MessageBox(NULL,"Error loading rat1.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete rat;
		delete rat1;
		return false;
	}
	rat1->InitializeEmitter(mXACore);
	Rat *rat2 = new Rat(mXACore, 2);
	if(!rat2->IsOk()){
		MessageBox(NULL,"Error loading rat1.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete rat;
		delete rat1;
		delete rat2;
		return false;
	}
	rat1->InitializeEmitter(mXACore);
	mBadSounds.push_back((AudioRenderable3D*)rat);
	mBadSounds.push_back((AudioRenderable3D*)rat1);
	return true;
}
/*
* Initializes all bear sounds.
* Returns false if there is an error.
*/
bool SoundCues::InitBear(){
	Bear *bear = new Bear(mXACore, 0);
	if(!bear->IsOk()){
		MessageBox(NULL,"Error loading bear.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete bear;
		return false;
	}
	bear->InitializeEmitter(mXACore);
	Bear *bear1 = new Bear(mXACore, 1);
	if(!bear1->IsOk()){
		MessageBox(NULL,"Error loading bear1.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete bear;
		delete bear1;
		return false;
	}
	bear1->InitializeEmitter(mXACore);
	mBadSounds.push_back((AudioRenderable3D*)bear);
	mBadSounds.push_back((AudioRenderable3D*)bear1);
	return true;
}
/*
* Initializes all rock and water sounds.
* Returns false if there is an error.
*/
bool SoundCues::InitOtherWarnings(){
	Rocks *rocks = new Rocks(mXACore);
	if(!rocks->IsOk()){
		MessageBox(NULL,"Error loading rocks.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete rocks;
		return false;
	}
	rocks->InitializeEmitter(mXACore);
	Water *water = new Water(mXACore,0);
	if(!water->IsOk()){
		MessageBox(NULL,"Error loading water.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete rocks;		
		delete water;
		return false;
	}
	water->InitializeEmitter(mXACore);
	Water *water1 = new Water(mXACore,0);
	if(!water1->IsOk()){
		MessageBox(NULL,"Error loading water.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete rocks;		
		delete water;
		delete water1;
		return false;
	}
	water1->InitializeEmitter(mXACore);
	mBadSounds.push_back((AudioRenderable3D*)water);
	mBadSounds.push_back((AudioRenderable3D*)water1);
	mBadSounds.push_back((AudioRenderable3D*)rocks);
	return true;
}
/*
* Initializes path, walk, start, and finish sounds.
* Returns false if there is an error.
*/
bool SoundCues::InitOther(){
	mPath = new Path(mXACore);
	if(!mPath->IsOk()){
		MessageBox(NULL,"Error loading bat.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete mPath;
		return false;
	}
	mPath->InitializeEmitter(mXACore);
	mWall = new Wall(mXACore);
	if(!mWall->IsOk()){
		MessageBox(NULL,"Error loading bat1.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete mPath;
		delete mWall;
		return false;
	}
	mWall->InitializeEmitter(mXACore);
	mFinish = new Finish(mXACore);
	if(!mFinish->IsOk()){
		MessageBox(NULL,"Error loading bat1.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete mPath;
		delete mWall;
		delete mFinish;
		return false;
	}
	mStart = new Start(mXACore);
	if(!mStart->IsOk()){
		MessageBox(NULL,"Error loading bat1.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete mPath;
		delete mWall;
		delete mFinish;
		delete mStart;
		return false;
	}
	return true;
}
/*
* Initializes all good sounds.
* Returns false if there is an error.
*/
bool SoundCues::InitGood(){
	Directions *good = new Directions(mXACore, 0);
	if(!good->IsOk()){
		MessageBox(NULL,"Error loading bat.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete good;
		return false;
	}
	good->InitializeEmitter(mXACore);
	Directions *good1 = new Directions(mXACore, 1);
	if(!good1->IsOk()){
		MessageBox(NULL,"Error loading bat1.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete good;
		delete good1;
		return false;
	}
	good1->InitializeEmitter(mXACore);
	Directions *good2 = new Directions(mXACore, 2);
	if(!good2->IsOk()){
		MessageBox(NULL,"Error loading bat2.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete good;
		delete good1;
		delete good2;
		return false;
	}
	good2->InitializeEmitter(mXACore);
	Directions *good3 = new Directions(mXACore, 3);
	if(!good3->IsOk()){
		MessageBox(NULL,"Error loading bat3.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete good;
		delete good1;
		delete good2;
		delete good3;
		return false;
	}
	good3->InitializeEmitter(mXACore);
	mGoodSounds.push_back((AudioRenderable3D*)good);
	mGoodSounds.push_back((AudioRenderable3D*)good1);
	mGoodSounds.push_back((AudioRenderable3D*)good2);
	mGoodSounds.push_back((AudioRenderable3D*)good3);
	return true;
}
/*
* Initializes walking and shuffling sounds.
* Returns false if there is an error.
*/
bool SoundCues::InitWalk(){
	mWalking = new Walking(mXACore);
	if(!mWalking->IsOk()){
		MessageBox(NULL,"Error loading bat.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete mWalking;
		return false;
	}
	mShuffle = new Shuffle(mXACore);
	if(!mShuffle->IsOk()){
		MessageBox(NULL,"Error loading bat1.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete mWalking;
		delete mShuffle;
		return false;
	}
	return true;
}
//--- Release all XACore resources.
//--- Note the order of destruction is important; XAudio2 destroys voices when the engine is destroyed, any calls to the voices AFTER this is an error, so any voice->DestroyVoice() should always be called before the engine is destroyed.
void SoundCues::CleanupGame (){
	vector<AudioRenderable3D*>::const_iterator iter;
	for(iter = mGoodSounds.begin(); iter!=mGoodSounds.end(); ++iter){
		delete *iter;
	}
	mGoodSounds.clear();
	for(iter = mBadSounds.begin(); iter!=mBadSounds.end(); ++iter){
		delete *iter;
	}
	mBadSounds.clear();
	delete mFinish;
	delete mPath;
	delete mWall;
	delete mStart;
	delete mWalking;
	delete mShuffle;
	mAmbient->CleanupGame();
	if (mXACore != NULL) {
		delete mXACore;
		mXACore = NULL;
	}
} // end CleanupGame function.
//=== end of code.