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
	Bat *bat = new Bat(mXACore, 0);
	bat->InitializeEmitter(mXACore);
	if(!bat->IsOk()){
		MessageBox(NULL,"Error loading bird.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete mRain;
		delete bat;
		return false;
	}
	Bat *bat1 = new Bat(mXACore, 1);
	bat1->InitializeEmitter(mXACore);
	if(!bat1->IsOk()){
		MessageBox(NULL,"Error loading bird.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete mRain;
		delete bat;
		delete bat1;
		return false;
	}
	Bat *bat2 = new Bat(mXACore, 2);
	bat2->InitializeEmitter(mXACore);
	if(!bat2->IsOk()){
		MessageBox(NULL,"Error loading bird.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete mRain;
		delete bat;
		delete bat1;
		delete bat2;
		return false;
	}
	Bat *bat3 = new Bat(mXACore, 3);
	bat3->InitializeEmitter(mXACore);
	if(!bat3->IsOk()){
		MessageBox(NULL,"Error loading bird.wav",TEXT("SetupGame()-FAILED"),MB_OK|MB_ICONERROR);
		delete mRain;
		delete bat;
		delete bat1;
		delete bat2;
		delete bat3;
		return false;
	}
	mBadSounds.push_back((AudioRenderable3D*)bat);
	mBadSounds.push_back((AudioRenderable3D*)bat1);
	mBadSounds.push_back((AudioRenderable3D*)bat2);
	mBadSounds.push_back((AudioRenderable3D*)bat3);
	ClearArray();
	SetupMap();
	mBadIter = rand()%mBadSounds.size();
	mBadSounds.at(mBadIter)->Play();
	//mGoodIter = rand()%mGoodSounds.size();
	//mGoodSounds.at(mGoodIter)->Play();
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
		//mGoodIter = rand()%mGoodSounds.size();
	}
	if(positionChanged){
		//mGoodSounds.at(mGoodIter)->Play();
		
		UpdateSoundTile();
	}
} // end ProcessGameFrame function.
void SoundCues::UpdateSoundTile(){
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
	case Good:
		//mGoodSounds.at(mGoodIter)->UpdateEmitter(pos,velo);
		//UpdateSettings(mGoodSounds.at(mGoodIter));
		break;
	case Finish:
		//mGoodSounds.at(mGoodIter)->UpdateEmitter(pos,velo);
		//UpdateSettings(mGoodSounds.at(mGoodIter));
		break;
	case Path:
		//path sounds
		break;
	case Bad:
		mBadSounds.at(mBadIter)->UpdateEmitter(pos,velo);
		UpdateSettings(mBadSounds.at(mBadIter));
		mBadSounds.at(mBadIter)->Play();
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
	switch(PlayerOrientation){
	case North:
		if((locationZ-1)>0){
			if(CheckForwardTile(locationZ-1,locationX)){
				locationZ-=1;
				return true;
			}
		}else if((locationZ-1)==0){
			//play wall sound
		}
		break;
	case East:
		if((locationX+1)<mapSize){
			if(CheckForwardTile(locationZ,locationX+1)){
				locationX+=1;
				return true;
			}
		}else if((locationX+1)==mapSize){
			//play wall sound
		}
		break;
	case South:
		if((locationZ+1)<mapSize){
			if(CheckForwardTile(locationZ+1,locationX)){
				locationZ+=1;
				return true;
			}else if((locationZ+1)==mapSize){
				//play wall sound
			}
		}
		break;
	case West:
		if((locationX-1)>0){
			if(CheckForwardTile(locationZ,locationX-1)){
				locationX-=1;
				return true;
			}else if((locationX-1)==0){
				//play wall sound
			}
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
	for(iter=mGoodSounds.begin(); iter!=mGoodSounds.end(); ++iter){
		UpdateSettings((*iter));
	}
	for(iter=mBadSounds.begin(); iter!=mBadSounds.end(); ++iter){
		UpdateSettings((*iter));
	}
}
void SoundCues::StopAllSounds(){
	vector<AudioRenderable3D*>::const_iterator iter;
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
			mMap[i][j].tile = Wall;
			mMap[i][j].played = false;
		}
	}
}
void SoundCues::SetupMap(){
	locationX = 5;
	locationZ = 9;
	//initialise warning sounds
	mMap[1][2].tile = Bad;
	mMap[1][6].tile = Bad;
	mMap[2][1].tile = Bad;
	mMap[2][7].tile = Bad;
	mMap[4][3].tile = Bad;
	mMap[4][5].tile = Bad;
	mMap[4][9].tile = Bad;
	mMap[4][9].tile = Bad;
	mMap[5][2].tile = Bad;
	mMap[5][3].tile = Bad;
	mMap[5][6].tile = Bad;
	mMap[5][7].tile = Bad;
	mMap[8][4].tile = Bad;
	mMap[9][2].tile = Bad;
	mMap[9][7].tile = Bad;
	//initialise good sounds.
	mMap[1][4].tile = Good;
	mMap[2][3].tile = Good;
	mMap[2][5].tile = Good;
	mMap[2][9].tile = Good;
	mMap[3][2].tile = Good;
	mMap[3][6].tile = Good;
	mMap[3][8].tile = Good;
	mMap[4][1].tile = Good;
	mMap[6][4].tile = Good;
	mMap[6][6].tile = Good;
	mMap[7][1].tile = Good;
	mMap[7][3].tile = Good;
	mMap[7][7].tile = Good;
	mMap[8][2].tile = Good;
	mMap[8][6].tile = Good;
	//init start and finish
	mMap[9][5].tile = Start;
	mMap[1][9].tile = Finish;
	//init path
	mMap[1][3].tile = Path;
	mMap[1][5].tile = Path;
	mMap[3][1].tile = Path;
	mMap[3][3].tile = Path;
	mMap[3][5].tile = Path;
	mMap[3][7].tile = Path;
	mMap[3][9].tile = Path;
	mMap[5][1].tile = Path;
	mMap[6][1].tile = Path;
	mMap[6][3].tile = Path;
	mMap[6][5].tile = Path;
	mMap[6][7].tile = Path;
	mMap[8][1].tile = Path;
	mMap[8][3].tile = Path;
	mMap[8][5].tile = Path;
	mMap[8][7].tile = Path;
}
bool SoundCues::CheckForwardTile(int x, int y){
	switch(CheckMap(x,y)){
	case Path:
	case Finish:
	case Start:
	case Good: return true; break;
	case Wall: //play wall sound
	case Bad: return false; break;
	default: return false; break;
	}
}
//=== end of code.