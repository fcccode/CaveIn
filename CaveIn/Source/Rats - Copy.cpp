/********************************************************************
	Filename:	Rats.cpp
	Version: 	
	Created:	02/04/2013
	
	Author:		Jake Morey
	
	Description:
	
*********************************************************************/
#include <time.h>

#define XAUDIO2_HELPER_FUNCTIONS
#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>

#include "XACore.hpp"
#include "WaveFileManager.hpp"
#include "PCMWave.hpp"
#include "WaveFmt.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::WaveFileManager;
using AllanMilne::Audio::PCMWave;
using AllanMilne::Audio::WaveFmt;
#include "Rats.hpp"

bool LoadFrogsFile(XAUDIO2_BUFFER *aBuffer, WAVEFORMATEX *aFormat);
int GetRandomPause();

Rats::Rats(XACore *aCore)
	:mOk(false), mPause(0), mElapsedTime(0.0f), mRatVoices(NULL)
{
	WAVEFORMATEX wFmt;
	if(!LoadFrogsFile(&mRatData,&wFmt)){
		return;
	}

	mRatVoices = new IXAudio2SourceVoice*[3];
	IXAudio2 *engine = aCore->GetEngine();
	if(engine == NULL){
		return;
	}
	HRESULT hr = 0;
	for(int i = 0; i < 3; i++){
		hr = hr & engine->CreateSourceVoice(&(mRatVoices[i]), &wFmt);
	}
	if(FAILED(hr)){
		delete[] mRatVoices;
		return;
	}

	float frequencyRatio;
	mRatVoices[0]->GetFrequencyRatio(&frequencyRatio);
	float semitones = XAudio2FrequencyRatioToSemitones(frequencyRatio);
	mRatVoices[1]->SetFrequencyRatio(XAudio2SemitonesToFrequencyRatio(semitones+4));
	mRatVoices[2]->SetFrequencyRatio(XAudio2SemitonesToFrequencyRatio(semitones+7));
	for(int i = 0; i<3; ++i){
		mRatVoices[i]->Start();
	}

	srand(time(NULL));
	mPause = GetRandomPause();

	mOk = true;
}
Rats::~Rats()
{
	if(mRatVoices!=NULL){
		for(int i = 0; i< 3; ++i){
			if(mRatVoices[i]!=NULL){
				mRatVoices[i]->Stop();
				mRatVoices[i]->FlushSourceBuffers();
				mRatVoices[i]->DestroyVoice();
			}
		}
		delete [] mRatVoices;
	}
}


//--- called every game frame to check if random time has passed and frogs should play.
//--- Parameter is time in seconds since last call.
void Rats::RenderAudio (const float deltaTime)
{
	// Guard against invalid sound elements.
	if (!IsOk()) return;
	mElapsedTime += deltaTime;
	// do nothing if random pause time has not passed.
	if (mElapsedTime < mPause) return;
	// play the harmonised frog sounds.
	for (int i=0; i<3; ++i) {
		mRatVoices[i]->SubmitSourceBuffer(&mRatData);
	}
	// Restart timing for the next play.
	mPause = GetRandomPause();
	mElapsedTime = 0.0f;
} // end Renderaudio function.

//--- Indicate if object has been created correctly.
inline bool Rats::IsOk() const { return mOk; }


//=== private helper functions.

//--- Load wave file and assign XAudio2 buffer and wave format struct to reference parameters.
//--- returns false if any error in loading .wav file.
bool LoadFrogsFile (XAUDIO2_BUFFER *aBuffer, WAVEFORMATEX *aFormat)
{
	PCMWave *wave = WaveFileManager::GetInstance().LoadWave ("Sounds/frogs.wav");
	// If the file was not loaded correctly then can go no further.
	if (wave->GetStatus() != PCMWave::OK) { return false; }
	// Initialise the XAudio2 buffer struct from the PCMWave object.
	memset ((void*)aBuffer, 0, sizeof (XAUDIO2_BUFFER));
	aBuffer->AudioBytes = wave->GetDataSize ();
	aBuffer->pAudioData = (BYTE*)(wave->GetWaveData ());
	// copy windows wave format struct from the PCMWave struct field.
	memset ((void*)aFormat, 0, sizeof (WAVEFORMATEX));
	memcpy_s ((void*)aFormat, sizeof (WaveFmt), (void*)&(wave->GetWaveFormat()), sizeof (WaveFmt));
	return true;
} // end LoadFrogsFile function.

//--- Get a random pause time between frogs playing; units are seconds.
int GetRandomPause ()
{
	// Time range to use.
	static const int minPause = 7;
	static const int range = 10;

	return rand() % range + minPause;
} // end GetRandomPause function.

void Rats::UpdateEmitter(X3DAUDIO_LISTENER player){

}
void Rats::InitializeEmitter(XACore *xacore){
	XAUDIO2_VOICE_DETAILS details;
	mRatVoices[0]->GetVoiceDetails(&details);
	mEmitter.ChannelCount = details.InputChannels;
	mEmitter.CurveDistanceScaler = 1.0f;
	X3DAUDIO_VECTOR tempVector = { 1.0f, 0.0f, 0.0f};
	mEmitter.Position = tempVector;
	tempVector.x = 0.0f;
	mEmitter.Velocity = tempVector;

	mDSPSettings.SrcChannelCount = mEmitter.ChannelCount;
	mDSPSettings.DstChannelCount = xacore->GetChannelCount();
	mDSPSettings.pMatrixCoefficients = new FLOAT32[mDSPSettings.SrcChannelCount * mDSPSettings.DstChannelCount];
}
//=== end of code.