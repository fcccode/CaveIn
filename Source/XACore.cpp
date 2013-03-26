/*
	file:	XACore.cpp
	Version:	1.1
	Date:	25th September 2012.
	Authors:	Stuart and Allan Milne.

	Namespace:	AllanMilne::Audio
	Exposes:	XACore implementation.
	Requires:	XACore, WaveFileManager, XASound, PCMWave.

	Description:
	this is the implementation of the concrete XACore class that encapsulates the core XAudio2 source voice functionality
	related to the XAudio2 processing engine and mastering voice.
	See XACore.hpp for class declaration.

	this implementation requires to use the concrete XASound and PCMWave classes
	since we are instantiating objects of these classes and require access to their constructors.

	The CreateSound() implementation uses the WaveFileManager singleton class to load .wav files.

*/

#include <windows.h>
#include <XAudio2.h>
#include<string>
using std::string;

#include "XACore.hpp"
#include "WaveFileManager.hpp"
#include "XASound.hpp"
#include "PCMWave.hpp"

namespace AllanMilne {
namespace Audio {

//--- Constructor method; instantiates XAudio2 engine and mastering cvoice.
XACore::XACore () 
{
	HRESULT hr;		// use to catch XAudio2 function call results.
	CoInitializeEx( NULL, COINIT_MULTITHREADED );
	hr = XAudio2Create (&mXAEngine, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED (hr))
	{
		CoUninitialize();
		mXAEngine = NULL;
		mMasteringVoice = NULL;
		return;
	}
	mXAEngine->StartEngine();
	// Retrieve information about the audio device.
	// Note the Windows 8 SdK distribution of XAudio2 requires an alternative approach to retrieving this information.
	mXAEngine->GetDeviceDetails (0, &mDetails); 
	mChannelCount = mDetails.OutputFormat.Format.nChannels;

	hr = mXAEngine->CreateMasteringVoice (&mMasteringVoice);
	if (FAILED (hr))
	{
		CoUninitialize();
		mMasteringVoice = NULL;
		return;
	}
} // end XACore constructor method.

//--- destructor method.
//--- Note only the engine is a COM object and therefore should be released; other elements are under control of the engine.
XACore::~XACore ()
{
	if (mXAEngine != NULL)
		mXAEngine->StopEngine ();
	if (mMasteringVoice != NULL)
		mMasteringVoice->DestroyVoice ();
	if (mXAEngine != NULL) {
		mXAEngine->Release();
		mXAEngine = NULL;
	}
	CoUninitialize();
} // end destructor method.

//=== Overloaded factory methods to create an XASound object; return NULL if error.
//=== Note the implementation is cascaded down the function definitions; from file -> wave buffer.

//--- Provide a .wav file name.
//--- Uses the wave file manager singleton class that caches loaded .wav files.
XASound *XACore::CreateSound (const string &aFileName) 
{
	//Load the audio data from a .wav file; return if unsuccessful.
	PCMWave *waveBuffer = WaveFileManager::GetInstance().LoadWave (aFileName);
	return CreateSound (waveBuffer);
} // end CreateSound from file.

//--- Provide an encapsulated wave buffer.
XASound *XACore::CreateSound (PCMWave *aWave) 
{
	// guard against invalid wave format.
	if (aWave->GetStatus() != PCMWave::OK) { return NULL; }

	// copy windows wave format struct from the PCMWave struct field.
	WAVEFORMATEX wFmt;
	memset ((void*)&wFmt, 0, sizeof (WAVEFORMATEX));
	memcpy_s ((void*)&wFmt, sizeof (WaveFmt), (void*)&(aWave->GetWaveFormat()), sizeof (WaveFmt));
	// Create the source voice for the specified wave format; return NULL if failed.
	IXAudio2SourceVoice *source;
	// Called with only 2 arguments the source voice will be routed directly to the mastering voice.
	HRESULT hr = mXAEngine->CreateSourceVoice (&source, &wFmt, XAUDIO2_VOICE_USEFILTER);
	if( FAILED( hr ) ) return NULL;

	// Create and initialise an XAudio2 buffer struct from the PCMWave object.
	XAUDIO2_BUFFER buffer;
	memset ((void*)&buffer, 0, sizeof (XAUDIO2_BUFFER));
	buffer.AudioBytes = aWave->GetDataSize ();
	buffer.pAudioData = (BYTE*)(aWave->GetWaveData ());
	//--- Following listed here for reference - have already been set to 0 via the memset above.
	buffer.Flags = 0;			// almost always 0; =XAUDIO2_END_OF_STREAM to suppress some debug warnings re buffer starvation.
	buffer.PlayBegin = 0;		// First sample in the buffer that should be played.
	buffer.PlayLength = 0;		// Number of samples to play; 0=entire buffer (begin must also be 0).
	buffer.LoopBegin = 0;		// First sample to be looped; must be <(PlayBegin+PlayLength); can be <PlayBegin.
	buffer.LoopLength = 0;		// Number of samples in loop; =0 indicates entire sample; PlayBegin > (LoopBegin+LoopLength) < PlayBegin+PlayLength).
	buffer.LoopCount = 0;		// Number of times to loop; =XAUDIO2_LOOP_INFINITE to loop forever; if 0 then LoopBegin and LoopLength must be 0.
	buffer.pContext = NULL;		// context to be passed to the client in callbacks.

	// return the XASound object created from the source boice and the XAudio2 buffer.
	return new XASound (source, buffer );
} // end CreateSound from wave buffer.

} // end Audio namespace.
} // end AllanMilne namespace.