/*
	file:	XACore.hpp
	Version:	1.1
	Date:	25th September 2012.
	Authors:	Stuart and Allan Milne.

	Namespace:	AllanMilne::Audio
	Exposes:	XACore
	Requires:	IXACore, XASound, PCMWave.

	Description:
	this is the concrete XACore class that encapsulates the core XAudio2 source voice functionality
	related to the XAudio2 processing engine and mastering voice.

	This is a concrete subclass of the IXACore interface (that defined only behaviour);
	here we define both behaviour and outline implementation.

	this class exposes basic XAudio2 functionality for creating an engine with mastering voice
	together with a factory method for creating XASound objects (encapsulating buffer and voice).
	Clients can subclass XACore exposing more sophisticated functionality as required;
	e.g. for managing XAudio 3-D and other client abstractions such as ambience.

*/

#ifndef __XACORE_HPP_1_1
#define __XACORE_HPP_1_1

#include <XAudio2.h>
#include <string>
using std::string;

#include "IXACore.hpp"
#include "XASound.hpp"
#include "PCMWave.hpp"

namespace AllanMilne {
namespace Audio {

class XACore : public virtual IXACore
{
public:

	//--- constructor method will initialise the engine and mastering voice.
	XACore ();
virtual 	~XACore ();

	//--- Access to the XAudio2 engine and mastering voice; return NULL if errors.
	inline IXAudio2 *GetEngine () const { return mXAEngine; }
	inline IXAudio2MasteringVoice *GetMasterVoice () const { return mMasteringVoice; }

	//--- Accessors for other global/device attributes.
	inline int GetChannelCount () const { return mChannelCount; }
	inline XAUDIO2_DEVICE_DETAILS& GetDeviceDetails () { return mDetails; }

	//--- Overloaded factory methods to create an XASound object; return NULL if error.
	//--- Provide a .wav file name.
	XASound *CreateSound (const string& aFilename);
	//--- Provide an encapsulated wave.
	XASound *CreateSound (PCMWave *aWave);


protected:		// rather than private to allow clients to define an extended version.
	IXAudio2 *mXAEngine;
	IXAudio2MasteringVoice *mMasteringVoice;

	int mChannelCount;			// No. of channels on audio device
	XAUDIO2_DEVICE_DETAILS mDetails;	// Audio device information

}; // end XACore interface.

} // end Audio namespace.
} // end AllanMilne namespace.

#endif