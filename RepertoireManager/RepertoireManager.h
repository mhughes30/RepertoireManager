
#pragma once
#ifndef REPORTIORE_MANAGER_H
#define REPORTIORE_MANAGER_H

#include <list>
#include <vector>
#include "Song.h"
#include "archives.h"


enum class SONG_CAT : uint8_t
{
	COMPOSER = 0,
	TITLE,
	INSTRUMENT,
	DURATION,
	YEAR,
	INSTR_TYPE,
	GENRE,
	GUITAR_CAPO,
	GUITAR_TUNE,
	MAX
};


// Appends/Removes items to/from the repertoire
// Archives/Extracts the repertoire from permanent storage
// This is a Singleton
class RepertoireManager
{
private:
	// make all constructors/assignments private
	RepertoireManager() 
	{
	}
	RepertoireManager(const RepertoireManager& other) = default;
	RepertoireManager(RepertoireManager&& other)      = default;

	RepertoireManager& operator=(const RepertoireManager& other) = default;
	RepertoireManager& operator=(RepertoireManager&& other)		 = default;

	//--- contains all of the reperoire music
	std::vector<boost::shared_ptr<ISong>> m_repertoire;
	
	//--- filename to use
	//const std::string m_fileName = std::string(boost::archive::tmpdir()) + "\\songs.txt";
	const std::string m_fileName = ".\\Resources\\songs.txt";

	//--- used for getting heading labels
	using SongCatToStrMap = std::map<SONG_CAT, std::string>;
	static const SongCatToStrMap getStrFromSongCat;

	//--- boost serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		// must register all of the child types to make sure the archive 
		// contains the correct information.
		ar.register_type(static_cast<boost::shared_ptr<Guitar>>nullptr);
		// append the repertoire to the arhive
		ar & m_repertoire;
	}

	uint32_t m_totalDuration_min= 0;
public:
	~RepertoireManager() {};

	static RepertoireManager* m_instance;

	static RepertoireManager* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new RepertoireManager();
		}

		return m_instance;
	}
		
	void	 EraseSong(uint16_t index);			// erase song from the archive	
	void	 AppendSong(ISong& song);			// append song to the archive	
	void	 ReadRepertoireFromDisk(void);		// read the archive from disk	
	bool	 WriteRepertoireToDisk(void);		// write the archive to disk	
	size_t	 GetRepertoireSize(void);			// get the current archive size	
	uint32_t GetRepertoireDurationMin(void);	// get the current archive duration in Min

	// get a label to use for a heading, to format a display/file
	std::string GetHeadingLabel(SONG_CAT category);
	// extracts the record from "song", and places it into "songData" for string representation of all data
	void ExtractSongRecord( const boost::shared_ptr<ISong>& song, std::map<SONG_CAT, std::string>& songData );

	// iterators to the song repertoire
	std::vector<boost::shared_ptr<ISong>>::const_iterator cbegin();
	std::vector<boost::shared_ptr<ISong>>::const_iterator cend();
};



#endif

