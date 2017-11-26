// RepertoireManager.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
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
void RepertoireManager::WriteRepertoireToDisk(void)
{
	std::ofstream ofs(m_fileName);
	if (!ofs.good())
	{
		return;
	}
	boost::archive::text_oarchive outArchive(ofs);
	outArchive << m_repertoire;
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

