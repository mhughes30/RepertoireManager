

#include "SetBuilder.h"


//-------------------------------------------------------------//
//------------------- Set List Decorators ---------------------//
//-------------------------------------------------------------//


//----------------- SetSetListStruct -------------------//
void ISetMaker::SetSetListStruct(SetListData& songVec)
{ 
	m_setVect = songVec; 
}


//----------------- SetDuration -------------------//
void ISetMaker::SetDuration(uint16_t min)
{ 
	m_totalMin = min; 
}



//----------------- RandomSet -------------------//
RandomSet::RandomSet()
{
	m_randEng.seed(m_randDev());	// seed the engine on creation
}


//----------------- GetRandomIndex -------------------//
uint32_t RandomSet::GetRandomIndex(uint32_t lowRange, uint32_t highRange)
{
	// create a uniform distribution
	std::uniform_int_distribution<uint32_t> uniDist(lowRange, highRange);

	// sample from the distribution
	return uniDist(m_randEng);
}


//----------------- BuildSetList -------------------//
// returns true/false if building of the list succeded/failed
bool RandomSet::BuildSetList(void)
{
	bool didSucceed = true;
	std::unordered_set<uint32_t> indexSet;	// stores indexes 

	RepertoireManager* repMgr = RepertoireManager::GetInstance();

	const uint32_t lowerRange = 0;
	const uint32_t upperRange = static_cast<uint32_t>(repMgr->GetRepertoireSize());

	const uint16_t maxDurSec = m_totalMin * 60;
	uint16_t curDurSec = 0;

	while (curDurSec < maxDurSec)
	{
		// get the song at the given index
		uint32_t idx = this->GetRandomIndex(lowerRange, upperRange);

		// test if the index has already been added
		auto idxIter = std::find(indexSet.cbegin(), indexSet.cend(), idx);
		if (idxIter != indexSet.cend())
		{
			continue;
		}

		// add the index to the set
		indexSet.insert(idx);

		// Get the new song 
		auto curSongPtr = repMgr->cbegin() + idx;
		ISong* pSong = curSongPtr->get();

		// Add the new song and increment the set-list duration
		m_setVect.push_back(*curSongPtr);
		curDurSec += pSong->GetDuration();
	}

	return didSucceed;
}



