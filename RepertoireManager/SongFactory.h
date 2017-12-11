
#pragma once
#ifndef SONG_FACTORY_H
#define SONG_FACTORY_H

#if 0
#include "Song.h"

// TODO: use this later for generating set lists, and when more song-types are added
class SongFactory
{

private:
	SongFactory();
	SongFactory(const SongFactory&) {}
	SongFactory& operator=(const SongFactory&)
	{
		return *this;
	}

public:
	~SongFactory() {}

	static std::unique_ptr<ISong> GetSong(INSTRUMENT songType)
	{
		switch (songType)
		{
		case INSTRUMENT::GUITAR:
			//return 
			break;
		case INSTRUMENT::SITAR:
		case INSTRUMENT::UKULELE:
		default:
			return nullptr;
			break;
		}
	}
};

#endif

#endif
