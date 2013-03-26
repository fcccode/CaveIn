/*
	file:	IXACore.hpp
	Version:	1.1
	Date:	25th September 2012.
	Authors:	Stuart and Allan Milne.

	Namespace:	AllanMilne::Audio
	Exposes:	IXACore
	Requires:	XASound, PCMWave.

	Description:
	this is a design for the XACore class that will encapsulate the core XAudio2 functionality
	related to the XAudio2 processing engine and mastering voice.

	The design is presented as an interface (abstract class - pure virtual functions) defining only behaviour.
	See the XACore class for implementation details.

	this INTERFACE exposes only basic XAudio2 functionality for creating an engine with mastering voice
	together with a factory method for creating XASound objects (encapsulating buffer and voice).
	Clients can subclass THIS INTERFACE exposing more sophisticated functionality as required;
	e.g. for managing XAudio 3-D and other client abstractions such as ambience.

*/

#ifndef __IXACORE_HPP_1_1
#define __IXACORE_HPP_1_1

#include <XAudio2.h>
#include <string>
using std::string;

#include "XASound.hpp"
#include "PCMWave.hpp"

namespace AllanMilne {
namespace Audio {

class IXACore
{
public:

	//--- Access to the XAudio2 engine and mastering voice; return NULL if errors.
	virtual IXAudio2 *GetEngine () const = 0;
	virtual IXAudio2MasteringVoice *GetMasterVoice () const = 0;

	//--- Accessors for other global/device attributes.
	virtual int GetChannelCount () const =0;
	virtual XAUDIO2_DEVICE_DETAILS& GetDeviceDetails () = 0;

	//--- Overloaded factory methods to create an XASound object; return NULL if error.
	//--- Provide a .wav file name.
	virtual XASound *CreateSound (const string &aFileName) = 0;
	//--- Provide an encapsulated wave.
	virtual XASound *CreateSound (PCMWave *aWave) = 0;

}; // end IXACore interface.

} // end Audio namespace.
} // end AllanMilne namespace.

#endif