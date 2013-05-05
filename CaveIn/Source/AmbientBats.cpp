/*
	file:	Frogs.cpp
	Version:	1.0
	Date:	11th January 2013.
	Author:	allan c. Milne.

	Exposes:	Frogs.
	Requires:	Sounds/frogs.wav, XACore, WaveFileManager, PCMWave.

	Description:
	Implements the AudioRenderable Frogs class that will play the sound of frogs croaking in harmony.
	this will be played at random time intervals.
	Uses frequency adjustment to implement the croaking harmonies.

	The implementation uses XAudio2 buffer and source voices directly.
	It uses the WaveFileManager singleton to load the frogs sound sample data.

	Note XAUDIO2_HELPER_FUNCTIONS must be defined in the project preprocessor properties.
*/

#include <time.h>

// following define required in order to use the semitones/frequency ratio helper functions.
#define XAUDIO2_HELPER_FUNCTIONS
#include<XAudio2.h>

#include "XACore.hpp"
#include "WaveFileManager.hpp"
#include "PCMWave.hpp"
#include "WaveFmt.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::WaveFileManager;
using AllanMilne::Audio::PCMWave;
using AllanMilne::Audio::WaveFmt;

#include "AmbientBats.hpp"

//--- internal helper function prototypes.
bool LoadFrogsFile (XAUDIO2_BUFFER *aBuffer, WAVEFORMATEX *aFormat);
int GetRandomPause ();


//--- called every game frame to check if random time has passed and frogs should play.
//--- Parameter is time in seconds since last call.
void AmbientBats::RenderAudio (const float deltaTime)
{
	// Guard against invalid sound elements.
	if (!IsOk()) return;
	mElapsedTime += deltaTime;
	// do nothing if random pause time has not passed.
	if (mElapsedTime < mPause) return;
	// play the harmonised frog sounds.
	for (int i=0; i<3; ++i) {
		mBatVoices[i]->SubmitSourceBuffer(&mFrogData);
	}
	// Restart timing for the next play.
	mPause = GetRandomPause();
		mElapsedTime = 0.0f;
} // end Renderaudio function.

//--- Indicate if object has been created correctly.
inline bool AmbientBats::IsOk() const { return mOk; }

//--- constructor loads and sets up the sound elements.
//--- Each step is checked and if not ok the constructor returns, with MOk being false.
	AmbientBats::AmbientBats(XACore *aCore)
	: mOk (false), mPause (0), mElapsedTime (0.0f), mBatVoices (NULL)
{
	// load the frogs.wav file.
	WAVEFORMATEX wFmt;
	if (!LoadFrogsFile (&mFrogData, &wFmt)) return;		// file not loaded correctly.

	// Create the array of source voices for the harmonised frogs.
	mBatVoices = new IXAudio2SourceVoice*[3];
	IXAudio2 *engine = aCore->GetEngine();
	if (engine == NULL) return;		// XAudio2 engine was not created correctly.
	HRESULT hr = 0;
	for (int i=0; i<3; ++i)
	{
		// with 2 parameters the source voices are all routed to the mastering voice.
		hr= hr & engine->CreateSourceVoice (&(mBatVoices[i]), &wFmt );
	}
	if( FAILED( hr ) ) 
	{
		delete [] mBatVoices;
		return;
	}
	// Now set up frog harmonies.
	float frequencyRatio;
	mBatVoices[0]->GetFrequencyRatio(&frequencyRatio);
	float semitones = XAudio2FrequencyRatioToSemitones (frequencyRatio);
	mBatVoices[1]->SetFrequencyRatio(XAudio2SemitonesToFrequencyRatio (semitones+4));
	mBatVoices[2]->SetFrequencyRatio(XAudio2SemitonesToFrequencyRatio(semitones+7));
	// Add these voices to the audio processing graph.
	for (int i=0; i<3; ++i) {
		mBatVoices[i]->Start();
	}
	
	// See the random number genrator and get the first random pause time..
	srand (time (NULL));
	mPause = GetRandomPause();

	// Everything has been created correctly.
	mOk = true;
} // end Frogs constructor function.

//--- release the frogs audio resources.
//--- Note we must not delete the source voices here; they are owned by the XAudio2 engine and will be deleted by it.
AmbientBats::~AmbientBats()
{
	if (mBatVoices != NULL) {
		for (int i=0; i<3; ++i) {
			if (mBatVoices[i]!= NULL) {
				mBatVoices[i]->Stop();
				mBatVoices[i]->FlushSourceBuffers();
				mBatVoices[i]->DestroyVoice();
			}
		} // end for each source voice.
		delete [] mBatVoices;
	} // end if have an array.
} // end Frogs destructor function.


//=== private helper functions.

//--- Load wave file and assign XAudio2 buffer and wave format struct to reference parameters.
//--- returns false if any error in loading .wav file.
bool LoadFrogsFile (XAUDIO2_BUFFER *aBuffer, WAVEFORMATEX *aFormat)
{
	PCMWave *wave = WaveFileManager::GetInstance().LoadWave ("Sounds/Atmosphere/Bats.wav");
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

//=== end of code.