#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "p2List.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

enum Audio_Flags
{
	MUSIC_AND_FX=-1,
	MUSIC,
	FX
};

class j1Audio : public j1Module
{
public:

	//--------INTERNAL CONTROL---------//
	//Constructor
	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	//Set the volume of the audio. 
	//For the flags use -1 for both channels, 0 for just music and 1 for just FX, or use the "Audio_Flags" enum as a value.
	void SetVolume(float volume,int flag=-1);

	//Adds a volume to the current volume value. 
	//For the flags use -1 for both channels, 0 for just music and 1 for just FX, or use the "Audio_Flags" enum as a value.
	void AddVolume(float volume, int flag = -1);

	//--------AUDIO---------//
	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:
	//--------AUDIO---------//
	//Pointer to the level's track
	_Mix_Music* music;
	//List to all sound effects
	p2List<Mix_Chunk*>	fx;

	float music_volume=0.0f;
	float fx_volume=0.0f;
};

#endif // __j1AUDIO_H__