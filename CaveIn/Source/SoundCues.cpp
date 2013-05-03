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
//=== Implementation of the IGameCore interface.
namespace{
	enum Orientation{North, East, South, West};
	Orientation PlayerOrientation = North;
}
//--- create and initialize XACore.
//--- Since this game has no graphics the HWND argument is not used.
bool SoundCues::SetupGame (HWND aWindow)
{
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

	ClearArray();
	SetupMap();
	mBadIter = rand()%mBadSounds.size();
	mGoodIter = rand()%mGoodSounds.size();
	UpdateSoundTile();
	return true;		// All has been setup without error.
} // end SetupGame function.
//--- process a single game frame.
void SoundCues::ProcessGameFrame (const float deltaTime)
{
	bool positionChanged = false;
	bool changeSound = false;
	mPlayer->Move();
	if(mPlayer->getFinishedMoving() == false){
		movementEnabled = true;
	}else{
		positionChanged = true;
	}
	if(movementEnabled){
		if( (GetAsyncKeyState(VK_UP) & 0x0001) || (GetAsyncKeyState('W') & 0x0001) ){
			if(CheckMoveForward()){
				mPlayer->MoveForward();
				movementEnabled = false;
				positionChanged = true;
				changeSound = true;
				mMap[locationZ][locationX].played = true;
				StopAllSounds();
			}
		}
		if( (GetAsyncKeyState(VK_LEFT) & 0x0001) || (GetAsyncKeyState('A') & 0x0001) ){
			ChangeOrientation(1);
			mPlayer->ShuffleLeft();
			positionChanged = true;
			movementEnabled = false;
		}
		if((GetAsyncKeyState(VK_RIGHT) & 0x0001) || (GetAsyncKeyState('D') & 0x0001) ){
			ChangeOrientation(-1);
			mPlayer->ShuffleRight();
			positionChanged = true;
			movementEnabled = false;
		}
		if((GetAsyncKeyState(VK_DOWN) & 0x0001) || (GetAsyncKeyState('S') & 0x0001) ){
			ChangeOrientation(2);
			mPlayer->ShuffleBack();
			positionChanged = true;
			movementEnabled = false;
		}
	}
	if(changeSound){
		mBadIter = rand()%mBadSounds.size();
		mGoodIter = rand()%mGoodSounds.size();
	}
	if(positionChanged){
		UpdateSoundTile();
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
} // end ProcessGameFrame function.
void SoundCues::UpdateSoundTile(){
	mPlayPath = false;
	mPlayGood = false;
	mPlayBad = false;
	X3DAUDIO_VECTOR pos = mPlayer->getListener().Position;
	if(mMap[locationZ-1][locationX].played == false){
		pos.z-=1;
		PlaySoundTiles(locationZ-1,locationX,pos);
		pos.z+=1;
	}
	if(mMap[locationZ+1][locationX].played == false){
		pos.z+=1;
		PlaySoundTiles(locationZ+1,locationX,pos);
		pos.z-=1;
	}
	if(mMap[locationZ][locationX-1].played == false){
		pos.x-=1;
		PlaySoundTiles(locationZ,locationX-1,pos);
		pos.x+=1;
	}
	if(mMap[locationZ][locationX+1].played == false){
		pos.x+=1;
		PlaySoundTiles(locationZ,locationX+1,pos);
		pos.x-=1;
	}
}
void SoundCues::PlaySoundTiles(int z, int x, X3DAUDIO_VECTOR pos){
	X3DAUDIO_VECTOR velo = {0.0f,0.0f,0.0f};
	switch (CheckMap(z,x)){
	case tGood:
		mGoodSounds.at(mGoodIter)->UpdateEmitter(pos,velo);
		UpdateSettings(mGoodSounds.at(mGoodIter));
		mGoodSounds.at(mGoodIter)->Play();
		mPlayGood = true;
		break;
	case tFinish:
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
	}
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
bool SoundCues::CheckMoveForward(){
	X3DAUDIO_VECTOR velo = {0.0f,0.0f,0.0f};
	X3DAUDIO_VECTOR pos = mPlayer->getListener().Position;
	switch(PlayerOrientation){
	case North:
		if((locationZ-1)>0){
			if(CheckForwardTile(locationZ-1,locationX)){
				locationZ-=1;
				return true;
			}
		}else if((locationZ-1)==0){
			pos.z = float(locationZ-1);
			mWall->UpdateEmitter(pos,velo);
			UpdateSettings((AudioRenderable3D*)mWall);
			mWall->Play();
		}
		break;
	case East:
		if((locationX+1)<mapSize){
			if(CheckForwardTile(locationZ,locationX+1)){
				locationX+=1;
				return true;
			}
		}else if((locationX+1)==mapSize){
			pos.x = float(locationX+1);
			mWall->UpdateEmitter(pos,velo);
			UpdateSettings((AudioRenderable3D*)mWall);
			mWall->Play();
		}
		break;
	case South:
		if((locationZ+1)<mapSize){
			if(CheckForwardTile(locationZ+1,locationX)){
				locationZ+=1;
				return true;
			}
		}else if((locationZ+1)==mapSize){
			pos.z = float(locationZ+1);
			mWall->UpdateEmitter(pos,velo);
			UpdateSettings((AudioRenderable3D*)mWall);
			mWall->Play();
		}
		break;
	case West:
		if((locationX-1)>0){
			if(CheckForwardTile(locationZ,locationX-1)){
				locationX-=1;
				return true;
			}
		}else if((locationX-1)==0){
			pos.x = float(locationX-1);
			mWall->UpdateEmitter(pos,velo);
			UpdateSettings((AudioRenderable3D*)mWall);
			mWall->Play();
		}
		break;
	}
	return false;
}
//--- Release all XACore resources.
//--- Note the order of destruction is important; XAudio2 destroys voices when the engine is destroyed, any calls to the voices AFTER this is an error, so any voice->DestroyVoice() should always be called before the engine is destroyed.
void SoundCues::CleanupGame ()
{
	vector<AudioRenderable3D*>::const_iterator iter;
	for(iter = mGoodSounds.begin(); iter!=mGoodSounds.end(); ++iter){
		delete *iter;
	}
	mGoodSounds.clear();
	for(iter = mBadSounds.begin(); iter!=mBadSounds.end(); ++iter){
		delete *iter;
	}
	mBadSounds.clear();
	if (mXACore != NULL) {
		delete mXACore;
		mXACore = NULL;
	}
} // end CleanupGame function.

void SoundCues::Apply3D(){
	vector<AudioRenderable3D*>::const_iterator iter;
	UpdateSettings((AudioRenderable3D*)mPath);
	UpdateSettings((AudioRenderable3D*)mWall);
	for(iter=mGoodSounds.begin(); iter!=mGoodSounds.end(); ++iter){
		UpdateSettings((*iter));
	}
	for(iter=mBadSounds.begin(); iter!=mBadSounds.end(); ++iter){
		UpdateSettings((*iter));
	}
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
void SoundCues::UpdateSettings(AudioRenderable3D* audio){
	//deep copy vs shallow copy
	X3DAUDIO_DSP_SETTINGS* settings = audio->getDSPSettings();
	X3DAudioCalculate(mX3DInstance, &mPlayer->getListener(), &audio->getEmitter(), 
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT ,
		settings);
	audio->getSourceVoice()->SetOutputMatrix(NULL, settings->SrcChannelCount, settings->DstChannelCount, settings->pMatrixCoefficients);
	audio->getSourceVoice()->SetFrequencyRatio(settings->DopplerFactor);
	XAUDIO2_FILTER_PARAMETERS FilterParams = {LowPassFilter, 2.0f*sinf(X3DAUDIO_PI/6.0f * settings->LPFDirectCoefficient), 1.0f};
	audio->getSourceVoice()->SetFilterParameters(&FilterParams);
}
void SoundCues::ClearArray(){
	for(int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			mMap[i][j].tile = tWall;
			mMap[i][j].played = false;
		}
	}
}
void SoundCues::SetupMap(){
	locationX = 5;
	locationZ = 9;
	//initialise warning sounds
	mMap[1][2].tile = tBad;
	mMap[1][6].tile = tBad;
	mMap[2][1].tile = tBad;
	mMap[2][7].tile = tBad;
	mMap[4][3].tile = tBad;
	mMap[4][5].tile = tBad;
	mMap[4][9].tile = tBad;
	mMap[4][9].tile = tBad;
	mMap[5][2].tile = tBad;
	mMap[5][3].tile = tBad;
	mMap[5][6].tile = tBad;
	mMap[5][7].tile = tBad;
	mMap[8][4].tile = tBad;
	mMap[9][2].tile = tBad;
	mMap[9][7].tile = tBad;
	//initialise good sounds.
	mMap[1][4].tile = tGood;
	mMap[2][3].tile = tGood;
	mMap[2][5].tile = tGood;
	mMap[2][9].tile = tGood;
	mMap[3][2].tile = tGood;
	mMap[3][6].tile = tGood;
	mMap[3][8].tile = tGood;
	mMap[4][1].tile = tGood;
	mMap[6][4].tile = tGood;
	mMap[6][6].tile = tGood;
	mMap[7][1].tile = tGood;
	mMap[7][3].tile = tGood;
	mMap[7][7].tile = tGood;
	mMap[8][2].tile = tGood;
	mMap[8][6].tile = tGood;
	//init start and finish
	mMap[9][5].tile = tStart;
	mMap[1][9].tile = tFinish;
	//init path
	mMap[1][3].tile = tPath;
	mMap[1][5].tile = tPath;
	mMap[3][1].tile = tPath;
	mMap[3][3].tile = tPath;
	mMap[3][5].tile = tPath;
	mMap[3][7].tile = tPath;
	mMap[3][9].tile = tPath;
	mMap[5][1].tile = tPath;
	mMap[6][1].tile = tPath;
	mMap[6][3].tile = tPath;
	mMap[6][5].tile = tPath;
	mMap[6][7].tile = tPath;
	mMap[8][1].tile = tPath;
	mMap[8][3].tile = tPath;
	mMap[8][5].tile = tPath;
	mMap[8][7].tile = tPath;
}
bool SoundCues::CheckForwardTile(int x, int y){
	X3DAUDIO_VECTOR velo = {0.0f,0.0f,0.0f};
	X3DAUDIO_VECTOR pos = mPlayer->getListener().Position;
	switch(CheckMap(x,y)){
	case tPath:
	case tFinish:
	case tStart:
	case tGood: return true; break;
	case tWall: 
		mWall->UpdateEmitter(pos,velo);
		UpdateSettings((AudioRenderable3D*)mWall);
		mWall->Play();
	case tBad: return false; break;
	default: return false; break;
	}
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
//=== end of code.