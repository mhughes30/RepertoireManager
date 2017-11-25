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
	std::cout << "reading to disk...\n";
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
	std::cout << "writing to disk...\n";
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

/*
int main()
{
	ISong mySong;
	std::unique_ptr<Human> man(new Human("Antonio Lauro"));
	mySong.SetComposer(man);
	std::unique_ptr<Duration> dur(new Duration(420));
	mySong.SetDuration(dur);
	mySong.SetInstrument(INSTRUMENT::GUITAR);
	mySong.SetInstrumentation(INSTRUMENTATION::BACKING_TRACK);
	mySong.SetTitle("El Nino");


	Guitar myG(mySong);

	RepertoireManager* mgr = RepertoireManager::GetInstance();

	//mgr->AppendSong(myG);
	
	mgr->ReadRepertoireFromDisk();
	auto itr = mgr->cbegin();
	ISong* m2 = *itr;
	std::cout << "read: " << m2->GetYear();

//	mgr->WriteRepertoireToDisk();
	
		
	int z;
	std::cin >> z;

    return 0;
}
*/
