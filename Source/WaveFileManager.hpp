/*
	file:	WaveFileManager.hpp
	Version:	1.0
	Date:	25th September 2012.
	Authors:	Stuart & Allan Milne.

	Namespace:	AllanMilne::Audio
	Exposes: WaveFileManager.
	Requires:	PCMWave.

	Description:
	Caches PCMWave objects representing raw audio data loaded from .wav files.

	When a .wav file load is requested first the manager checks if this file has already been loaded;
	* if already loaded it returns the cached PCMWave object, 
	* if not the data is loaded and cached for any further requests.

	Note this implementation is NOT thread safe.

	Exposed and implemented as a singleton pattern.
*/

#ifndef __WAVEFILEMANAGER_HPP_1_0
#define __WAVEFILEMANAGER_HPP_1_0

#include <string>
#include <map>
using std::string;
using std::map;

#include "PCMWave.hpp"

namespace AllanMilne {
namespace Audio {

class WaveFileManager 
{
public:

	//--- Get the singleton instance - use lazy evaluation.
	static WaveFileManager& GetInstance (); 

	//--- Delete manager instance, will call instance destructor and hence all PCMWave destructors.
	static void Delete ();
	
	//--- Retrieve the PCMWave object for the named file; loads it into the cache if not already loaded.
	PCMWave *LoadWave (const string &aFileName);

	//--- delete named PCMWave object from cache; returns false if named file not loaded into cache.
	//--- NB will cause destructor of the relevant PCMWave object to be called and hence all associated sound sample data will be invalid.
	bool UnloadWave (const string &aFileName);

	//--- Returns true if named file is in the cache; false otherwise.
	bool IsLoaded (const string &aFileName);


private:
	// the singleton instance.
	static WaveFileManager *mInstance;

	// Constructor & destructor are hidden to implement singleton patern.
	WaveFileManager ();
	~WaveFileManager ();

	// the cache storing the loaded .wav file representations.
	map<string, PCMWave*> mWaveFileCache;

}; // end WaveFileManager class.

} // end Audio namespace.
} // end AllanMilne namespace.

#endif