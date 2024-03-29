// RepertoireManager.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <boost/lexical_cast.hpp>
#include "RepertoireManager.h"
BOOST_CLASS_EXPORT(Guitar)

// singleton instance inialized to null
RepertoireManager* RepertoireManager::m_instance = nullptr;


//--------------- EraseSong ---------------//
void RepertoireManager::EraseSong(uint16_t index)
{
	auto loc = m_repertoire.cbegin() + index;
	m_repertoire.erase(loc);
}

//--------------- AppendSong ---------------//
void RepertoireManager::AppendSong(ISong& song)
{	
	// get correct type
	Guitar& aGuitar = dynamic_cast<Guitar&>(song);
	// created shared pointer for mem mgmt, and use move constructor
	boost::shared_ptr<Guitar> pSong( new Guitar(std::move(aGuitar)) );
	m_repertoire.push_back(pSong);
}

//--------------- ReadRepertoireFromDisk ---------------//
void RepertoireManager::ReadRepertoireFromDisk(void)
{
	std::ifstream ifs(m_fileName);
	bool isGood = ifs.good();
	if (!isGood)
	{
		return;
	}
	boost::archive::text_iarchive inArchive(ifs);
	inArchive >> m_repertoire;
}

//--------------- WriteRepertoireToDisk ---------------//
// writes the repertoire to a file on disk
// returns true/false on success/failure
bool RepertoireManager::WriteRepertoireToDisk(void)
{
	std::ofstream ofs(m_fileName);
	if (!ofs.good())
	{
		return false;
	}
	boost::archive::text_oarchive outArchive(ofs);
	outArchive << m_repertoire;

	return true;
}

//--------------- const iterators ---------------//
std::vector<boost::shared_ptr<ISong>>::const_iterator RepertoireManager::cbegin()
{
	return m_repertoire.cbegin();
}

std::vector<boost::shared_ptr<ISong>>::const_iterator RepertoireManager::cend()
{
	return m_repertoire.cend();
}

//--------------- GetRepertoireSize ---------------//
size_t RepertoireManager::GetRepertoireSize(void)
{
	return m_repertoire.size();
}

//--------------- GetRepertoireDurationMin ---------------//
// TODO: change this so the duration is computed once at read, and updated from there
uint32_t RepertoireManager::GetRepertoireDurationMin(void)
{
	m_totalDuration_min = 0;
	auto iter = cbegin();
	for (; iter != cend(); ++iter)
	{
		m_totalDuration_min += iter->get()->GetDuration();
	}

	return m_totalDuration_min;
}


//--------------- GetHeadingLabel ---------------//
std::string RepertoireManager::GetHeadingLabel(SONG_CAT category)
{
	if (category > SONG_CAT::MAX)
	{
		return getStrFromSongCat.at(SONG_CAT::MAX);
	}
	return getStrFromSongCat.at(category);
}

const RepertoireManager::SongCatToStrMap RepertoireManager::getStrFromSongCat = []
{
	RepertoireManager::SongCatToStrMap map =
	{
		{ SONG_CAT::COMPOSER,		"Composer" },
		{ SONG_CAT::TITLE,			"Title" },
		{ SONG_CAT::INSTRUMENT,		"Instrument" },
		{ SONG_CAT::DURATION,		"Duration" },
		{ SONG_CAT::YEAR,			"Year" },
		{ SONG_CAT::INSTR_TYPE,		"Type" },
		{ SONG_CAT::GENRE,			"Genre" },
		{ SONG_CAT::GUITAR_CAPO,	"Capo" },
		{ SONG_CAT::GUITAR_TUNE,	"Tuning" },
		{ SONG_CAT::MAX,			"idx" },
	};
	return map;
}();

//--------------- ExtractSongRecord ---------------//
void RepertoireManager::ExtractSongRecord( const boost::shared_ptr<ISong>& songPtr, std::map<SONG_CAT, std::string>& songData )
{
	ISong* song = songPtr.get();
	Guitar* curSong = static_cast<Guitar*>(song);
	// composer
	Human hum;
	curSong->GetComposer(hum);
	songData[SONG_CAT::COMPOSER] = hum.GetName(Human::NAME::FULL).c_str();
	// Title
	songData[SONG_CAT::TITLE] = curSong->GetTitle().c_str();
	// Instrument
	songData[SONG_CAT::INSTRUMENT] = EnumToStr::getStrFromInstrument.at(curSong->GetInstrument()).c_str();
	// Duration
	Duration dur;
	curSong->GetDuration(dur);
	songData[SONG_CAT::DURATION] = dur.GetMinSecStr().c_str();
	// year
	songData[SONG_CAT::YEAR] = boost::lexical_cast<std::string>(curSong->GetYear());
	// Instrumentation Type
	songData[SONG_CAT::INSTR_TYPE] = EnumToStr::getStrFromInstrumentation.at(curSong->GetInstrumentation()).c_str();
	// genre
	songData[SONG_CAT::GENRE] = EnumToStr::getStrFromGenre.at(curSong->GetGenre()).c_str();
	// capo and tuning
	GuitarConfig gc;
	curSong->GetGuitarConfig(gc);
	songData[SONG_CAT::GUITAR_CAPO] = EnumToStr::getStrFromCapo.at(gc.GetCapoFret()).c_str();
	songData[SONG_CAT::GUITAR_TUNE] = EnumToStr::getStrFromTuning.at(gc.GetTuning()).c_str();
}

