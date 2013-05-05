/*
	File:	Soundscape1.cpp
	Version:	0.1
	Date:	9th January 2013.
	Author:	Allan C. Milne.

	Requires:	XACore.

	Description:
	This is the implementation of the Soundscape1 class members;
	this is the base version with no sounds yet added.

*/

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
//=== Implementation of the IGameCore interface.
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

	memset((void*)&mX3DInstance,0,sizeof(X3DAUDIO_HANDLE));
	DWORD channelMask = mXACore->GetDeviceDetails().OutputFormat.dwChannelMask;
	mXACore->GetChannelCount();
	X3DAudioInitialize(channelMask,X3DAUDIO_SPEED_OF_SOUND, mX3DInstance);

	mPlayer = new Player();
	mPlayer->InitializeListener();
	mAmbient = new Soundscape();
	InitSounds();
	SetupMap();
	return true;		// All has been setup without error.
} // end SetupGame function.
//--- process a single game frame.
void SoundCues::ProcessGameFrame (const float deltaTime){
	if(CheckStart() == false && CheckFinish() == false){
		mAmbient->ProcessGameFrame(deltaTime);
		mPlayer->Move(deltaTime);
		if(mPlayer->getTransition()){
			Apply3D();
		}
		if(mPlayer->getMoving() == false){
			movementEnabled = true;
		}
		if(movementEnabled){
			Move();
		}
		if(mPlayPath){
			mPath->RenderAudio(deltaTime);
		}
		if(mPlayGood){
			mGoodSounds.at(mGoodIter)->RenderAudio(deltaTime);
		}
		if(mPlayBad){
			mBadSounds.at(mBadIter)->RenderAudio(deltaTime);
		}
	}
} // end ProcessGameFrame function.
void SoundCues::Move(){
	if( (GetAsyncKeyState(VK_UP) & 0x0001) || (GetAsyncKeyState('W') & 0x0001) ){
		if(CheckMoveForward()){
			mPlayer->MoveForward();
			movementEnabled = false;
			mMap[locationZ][locationX].played = true;
			mBadIter=CheckIter(mBadIter, mBadSounds.size());
			mGoodIter=CheckIter(mGoodIter, mGoodSounds.size());
			StopAllSounds();
			UpdateSoundTile();
		}
	}else if( (GetAsyncKeyState(VK_LEFT) & 0x0001) || (GetAsyncKeyState('A') & 0x0001) ){
		ChangeOrientation(1);
		mPlayer->ShuffleLeft();
		Apply3D();
		movementEnabled = false;
	}else if((GetAsyncKeyState(VK_RIGHT) & 0x0001) || (GetAsyncKeyState('D') & 0x0001) ){
		ChangeOrientation(-1);
		mPlayer->ShuffleRight();
		Apply3D();
		movementEnabled = false;
	}else if((GetAsyncKeyState(VK_DOWN) & 0x0001) || (GetAsyncKeyState('S') & 0x0001) ){
		ChangeOrientation(2);
		mPlayer->ShuffleBack();
		Apply3D();
		movementEnabled = false;
	}
}
bool SoundCues::CheckStart(){
	if(mMap[locationZ][locationX].tile == tStart && mMap[locationZ][locationX].played == false){
		if(mStart->getFinished()){
			mMap[locationZ][locationX].played = true;
			mBadIter = rand()%mBadSounds.size();
			mGoodIter = rand()%mGoodSounds.size();
			UpdateSoundTile();
			mAmbient->SetupGame(mXACore);
		}
		mStart->Play();
		return true;
	}
	return false;
}
bool SoundCues::CheckFinish(){
	if(mMap[locationZ][locationX].tile == tFinish){
		if(mFinish->getFinished()){
			mFinished = true;
		}
		mFinish->Play();
	}
	return mFinished;
}
void SoundCues::UpdateSoundTile(){
	mPlayPath = false;
	mPlayGood = false;
	mPlayBad = false;
	if(mMap[locationZ-1][locationX].played == false){
		PlaySoundTiles(locationZ-1,locationX,mPlayer->getPlayerNorth());
	}
	if(mMap[locationZ+1][locationX].played == false){
		PlaySoundTiles(locationZ+1,locationX,mPlayer->getPlayerSouth());
	}
	if(mMap[locationZ][locationX-1].played == false){
		PlaySoundTiles(locationZ,locationX-1,mPlayer->getPlayerWest());
	}
	if(mMap[locationZ][locationX+1].played == false){
		PlaySoundTiles(locationZ,locationX+1,mPlayer->getPlayerEast());
	}
}
void SoundCues::PlaySoundTiles(int z, int x, X3DAUDIO_VECTOR pos){
	X3DAUDIO_VECTOR velo = {0.0f,0.0f,0.0f};
	switch (CheckMap(z,x)){
	case tFinish:
	case tGood:
		mGoodSounds.at(mGoodIter)->UpdateEmitter(pos,velo);
		UpdateSettings(mGoodSounds.at(mGoodIter));
		mGoodSounds.at(mGoodIter)->Play();
		mPlayGood = true;
		break;
	case tPath:
		mPath->UpdateEmitter(pos,velo);
		UpdateSettings(mPath);
		mPath->Play();
		mPlayPath = true;
		break;
	case tBad:
		mBadSounds.at(mBadIter)->UpdateEmitter(pos,velo);
		UpdateSettings(mBadSounds.at(mBadIter));
		mBadSounds.at(mBadIter)->Play();
		mPlayBad = true;
		break;
	case tWall:
		mWall->UpdateEmitter(pos,velo);
		UpdateSettings(mWall);
		break;
	}
}
bool SoundCues::CheckMoveForward(){
	X3DAUDIO_VECTOR velo = {0.0f,0.0f,0.0f};
	switch(PlayerOrientation){
	case North:
		if((locationZ-1)>=0){
			if(CheckForwardTile(locationZ-1,locationX, mPlayer->getPlayerNorth())){
				locationZ-=1;
				return true;
			}
		}
		break;
	case East:
		if((locationX+1)<mapSize){
			if(CheckForwardTile(locationZ,locationX+1, mPlayer->getPlayerEast())){
				locationX+=1;
				return true;
			}
		}
		break;
	case South:
		if((locationZ+1)<mapSize){
			if(CheckForwardTile(locationZ+1,locationX, mPlayer->getPlayerSouth())){
				locationZ+=1;
				return true;
			}
		}
		break;
	case West:
		if((locationX-1)>=0){
			if(CheckForwardTile(locationZ,locationX-1, mPlayer->getPlayerWest())){
				locationX-=1;
				return true;
			}
		}
		break;
	}
	return false;
}
void SoundCues::ChangeOrientation(int dir){
	if(dir == 1){
		switch(PlayerOrientation){
		case North: PlayerOrientation = West; break;
		case East: PlayerOrientation = North; break;
		case South: PlayerOrientation = East; break;
		case West: PlayerOrientation = South; break;
		}
	}else if(dir == -1){
		switch(PlayerOrientation){
		case North: PlayerOrientation = East; break;
		case East: PlayerOrientation = South; break;
		case South: PlayerOrientation = West; break;
		case West: PlayerOrientation = North; break;
		}
	}else if(dir == 2){
		switch(PlayerOrientation){
		case North: PlayerOrientation = South; break;
		case East: PlayerOrientation = West; break;
		case South: PlayerOrientation = North; break;
		case West: PlayerOrientation = East; break;
		}
	}
}
bool SoundCues::CheckForwardTile(int x, int y, X3DAUDIO_VECTOR pos){
	X3DAUDIO_VECTOR velo = {0.0f,0.0f,0.0f};
	switch(CheckMap(x,y)){
	case tPath:
	case tFinish:
	case tStart:
	case tGood: return true; break;
	case tWall: 
		mWall->UpdateEmitter(pos,velo);
		UpdateSettings(mWall);
		mWall->Play();
	case tBad: return false; break;
	default: return false; break;
	}
}
void SoundCues::Apply3D(){
	UpdateSettings(mPath);
	UpdateSettings(mWall);
	UpdateSettings(mGoodSounds.at(mGoodIter));
	UpdateSettings(mBadSounds.at(mBadIter));
}
void SoundCues::UpdateSettings(AudioRenderable3D* audio){
	X3DAudioCalculate(mX3DInstance, &mPlayer->getListener(), &audio->getEmitter(), 
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT ,
		audio->getDSPSettings());
	audio->getSourceVoice()->SetOutputMatrix(NULL, audio->getDSPSettings()->SrcChannelCount, audio->getDSPSettings()->DstChannelCount, audio->getDSPSettings()->pMatrixCoefficients);
	audio->getSourceVoice()->SetFrequencyRatio(audio->getDSPSettings()->DopplerFactor);
	XAUDIO2_FILTER_PARAMETERS FilterParams = {LowPassFilter, 2.0f*sinf(X3DAUDIO_PI/6.0f * audio->getDSPSettings()->LPFDirectCoefficient), 1.0f};
	audio->getSourceVoice()->SetFilterParameters(&FilterParams);
}
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
bool SoundCues::FinishedGame(){
	return mFinished;
}
void SoundCues::SetupMap(){
	ClearArray();
	SetUpOtherTiles();
	SetUpGoodTiles();
	SetUpBadTiles();
	SetUpPathTiles();
}
void SoundCues::ClearArray(){
	for(int i = 0; i < mapSize; i++){
		for (int j = 0; j < mapSize; j++){
			mMap[i][j].tile = tWall;
			mMap[i][j].played = false;
		}
	}
}
void SoundCues::SetUpOtherTiles(){
	locationX = 2;
	locationZ = 15;
	//init start and finish
	mMap[locationZ][locationX].tile = tStart;
	mMap[1][mapSize-2].tile = tFinish;
}
void SoundCues::SetUpGoodTiles(){
	//initialise good sounds.
	mMap[1][13].tile = tGood;
	mMap[2][12].tile = tGood;
	mMap[3][3].tile = tGood;
	mMap[3][10].tile = tGood;
	mMap[4][5].tile = tGood;
	mMap[4][9].tile = tGood;
	mMap[5][6].tile = tGood;
	mMap[7][2].tile = tGood;
	mMap[7][7].tile = tGood;
	mMap[7][8].tile = tGood;
	mMap[7][14].tile = tGood;
	mMap[8][3].tile = tGood;
	mMap[8][4].tile = tGood;
	mMap[8][10].tile = tGood;
	mMap[8][11].tile = tGood;
	mMap[9][15].tile = tGood;
	mMap[10][2].tile = tGood;
	mMap[10][11].tile = tGood;
	mMap[11][1].tile = tGood;
	mMap[11][6].tile = tGood;
	mMap[11][10].tile = tGood;
	mMap[12][3].tile = tGood;
	mMap[12][7].tile = tGood;
	mMap[12][11].tile = tGood;
	mMap[13][4].tile = tGood;
	mMap[13][8].tile = tGood;
	mMap[13][12].tile = tGood;
	mMap[14][3].tile = tGood;
	mMap[14][10].tile = tGood;
	mMap[14][11].tile = tGood;
	mMap[15][9].tile = tGood;
}
void SoundCues::SetUpBadTiles(){
	//initialise warning sounds
	mMap[1][11].tile = tBad;
	mMap[2][5].tile = tBad;
	mMap[3][1].tile = tBad;
	mMap[3][8].tile = tBad;
	mMap[4][12].tile = tBad;
	mMap[5][4].tile = tBad;
	mMap[6][4].tile = tBad;
	mMap[6][9].tile = tBad;
	mMap[6][11].tile = tBad;
	mMap[9][6].tile = tBad;
	mMap[9][8].tile = tBad;
	mMap[10][9].tile = tBad;
	mMap[11][8].tile = tBad;
	mMap[11][15].tile = tBad;
	mMap[12][5].tile = tBad;
	mMap[12][13].tile = tBad;
	mMap[13][1].tile = tBad;
	mMap[14][1].tile = tBad;
	mMap[15][4].tile = tBad;
	mMap[15][7].tile = tBad;
	mMap[15][12].tile = tBad;
}
void SoundCues::SetUpPathTiles(){
	//init path
	mMap[1][12].tile = tPath;
	mMap[1][14].tile = tPath;
	mMap[3][2].tile = tPath;
	mMap[3][4].tile = tPath;
	mMap[3][5].tile = tPath;
	mMap[3][9].tile = tPath;
	mMap[3][11].tile = tPath;
	mMap[3][12].tile = tPath;
	mMap[4][2].tile = tPath;
	mMap[5][2].tile = tPath;
	mMap[5][5].tile = tPath;
	mMap[5][7].tile = tPath;
	mMap[5][8].tile = tPath;
	mMap[5][9].tile = tPath;
	mMap[6][2].tile = tPath;
	mMap[7][4].tile = tPath;
	mMap[7][5].tile = tPath;
	mMap[7][6].tile = tPath;
	mMap[7][11].tile = tPath;
	mMap[7][12].tile = tPath;
	mMap[7][13].tile = tPath;
	mMap[7][15].tile = tPath;
	mMap[8][2].tile = tPath;
	mMap[8][8].tile = tPath;
	mMap[8][9].tile = tPath;
	mMap[8][15].tile = tPath;
	mMap[10][1].tile = tPath;
	mMap[10][3].tile = tPath;
	mMap[10][4].tile = tPath;
	mMap[10][5].tile = tPath;
	mMap[10][6].tile = tPath;
	mMap[10][10].tile = tPath;
	mMap[10][12].tile = tPath;
	mMap[10][13].tile = tPath;
	mMap[10][14].tile = tPath;
	mMap[10][15].tile = tPath;
	mMap[12][1].tile = tPath;
	mMap[12][2].tile = tPath;
	mMap[12][4].tile = tPath;
	mMap[12][6].tile = tPath;
	mMap[12][8].tile = tPath;
	mMap[12][10].tile = tPath;
	mMap[12][12].tile = tPath;
	mMap[14][2].tile = tPath;
	mMap[14][4].tile = tPath;
	mMap[14][8].tile = tPath;
	mMap[14][12].tile = tPath;
	mMap[15][8].tile = tPath;
	mMap[15][10].tile = tPath;
}
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
	}
	return true;
}
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
	mBadSounds.push_back((AudioRenderable3D*)bat);
	mBadSounds.push_back((AudioRenderable3D*)bat1);
	mBadSounds.push_back((AudioRenderable3D*)bat2);
	mBadSounds.push_back((AudioRenderable3D*)bat3);
	return true;
}
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
	mBadSounds.push_back((AudioRenderable3D*)rat);
	mBadSounds.push_back((AudioRenderable3D*)rat1);
	return true;
}
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
//--- Release all XACore resources.
//--- Note the order of destruction is important; XAudio2 destroys voices when the engine is destroyed, any calls to the voices AFTER this is an error, so any voice->DestroyVoice() should always be called before the engine is destroyed.
void SoundCues::CleanupGame (){
	mAmbient->CleanupGame();
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
	if (mXACore != NULL) {
		delete mXACore;
		mXACore = NULL;
	}
} // end CleanupGame function.
//=== end of code.