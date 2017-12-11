
#include <chrono>
#include "SetBuilder.h"
#include "TimeHelper.h"

//-------------------------------------------------------------//
//--------------------- Set List Builder ----------------------//
//-------------------------------------------------------------//
#define TAB "\t"

//----------------- CreateTitle -------------------//
void SetListHtmlWriter::CreateTitle(const std::string& titleStr)
{
	m_file << "<head>" << "\n";
	m_file << "<title>" << "\n";
	m_file << titleStr << "\n";
	m_file << "</title>" << "\n";
	m_file << "</head>" << "\n";
}

//----------------- CreateTableTitle -------------------//
void SetListHtmlWriter::CreateTableTitle(const std::string& titleStr, uint16_t duration)
{
	m_file << "<tr align=\"center\">" << "\n";
	m_file << "<th colspan=\"6\">" << "<BR><H3>" << titleStr <<  " - " <<  std::to_string(duration) << " min" << "</H3>";
	m_file << "</th>" << "\n";
	m_file << "</tr>" << "\n";
}

//----------------- WriteSetListToHtml -------------------//
bool SetListHtmlWriter::WriteSetListToHtml(SetListConstIter begin, SetListConstIter end, uint16_t duration)
{
	if (false == m_file.is_open())
	{
		return false;
	}

	auto timeTuple = TimeHelper::GetYearMonthDay();

	std::string year  = std::get<static_cast<int>(TimeHelper::TYPE::YEAR)>(timeTuple);
	std::string month = std::get<static_cast<int>(TimeHelper::TYPE::MONTH)>(timeTuple);
	std::string day   = std::get<static_cast<int>(TimeHelper::TYPE::DAY)>(timeTuple);
	std::string monDayYear = month + "/" + day + "/" + year;

	RepertoireManager* repMgr = RepertoireManager::GetInstance();

	std::string title = "Guitar Set List: " + monDayYear;

	m_file << "<html>" << "\n";
	this->CreateTitle(title);


	m_file << "<body>" << "\n";
	m_file << "<table width=\"50%\" border=\"5\" cellpadding=\"3\" cellspacing= \"3\">" << "\n";

	this->CreateTableTitle(title, duration);
	this->CreateHeadings();

	for (auto iter = begin; iter != end; ++iter)
	{
		this->AddTableRecord(*iter);
	}
	
	m_file << "</table>" << "\n";
	m_file << "</body>" << "\n";		
	m_file << "</html>" << "\n";

	m_file.close();

	return true;
}

//----------------- CreateHeadings -------------------//
void SetListHtmlWriter::CreateHeadings(void)
{
	RepertoireManager* repMgr = RepertoireManager::GetInstance();

	m_file << "<tr align=\"center\">" << "\n";
	m_file << TAB << "<th>" << repMgr->GetHeadingLabel(SONG_CAT::COMPOSER)		<< "</th>" << "\n";
	m_file << TAB << "<th>" << repMgr->GetHeadingLabel(SONG_CAT::TITLE)			<< "</th>" << "\n";
	m_file << TAB << "<th>" << repMgr->GetHeadingLabel(SONG_CAT::DURATION)		<< "</th>" << "\n";
	m_file << TAB << "<th>" << repMgr->GetHeadingLabel(SONG_CAT::YEAR)			<< "</th>" << "\n";
	m_file << TAB << "<th>" << repMgr->GetHeadingLabel(SONG_CAT::GUITAR_CAPO)	<< "</th>" << "\n";
	m_file << TAB << "<th>" << repMgr->GetHeadingLabel(SONG_CAT::GUITAR_TUNE)	<< "</th>" << "\n";
	m_file << "</tr>" << "\n";
}


//----------------- AddTableRecord -------------------//
void SetListHtmlWriter::AddTableRecord(boost::shared_ptr<ISong> pSong)
{
	Guitar* song = dynamic_cast<Guitar*>(pSong.get());

	GuitarConfig conf;
	song->GetGuitarConfig(conf);
	uint16_t capo = static_cast<uint16_t>(conf.GetCapoFret());
	std::string tuning;
	conf.GetTuning(tuning);
	Duration dur;
	song->GetDuration(dur);

	m_file << "<tr align=\"center\">" << "\n";
	m_file << TAB << "<td>" << song->GetComposer()	<< "</td>" << "\n";
	m_file << TAB << "<td>" << song->GetTitle()		<< "</td>" << "\n";
	m_file << TAB << "<td>" << dur.GetMinSecStr()	<< "</td>" << "\n";
	m_file << TAB << "<td>" << song->GetYear()		<< "</td>" << "\n";
	m_file << TAB << "<td>" << capo					<< "</td>" << "\n";
	m_file << TAB << "<td>" << tuning				<< "</td>" << "\n";
	m_file << "</tr>" << "\n";
}


//-------------------------------------------------------------//
//----------------------- ISetMaker ---------------------------//
//-------------------------------------------------------------//

//----------------- SetSetListStruct -------------------//
void ISetMaker::SetSetListStruct(SetListData& songVec)
{ 
	m_setVect = &songVec; 
}


//----------------- SetDuration -------------------//
void ISetMaker::SetDuration(uint16_t& min)
{ 
	m_totalMin = &min; 
}


//-------------------------------------------------------------//
//----------------------- RandomSet ---------------------------//
//-------------------------------------------------------------//

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

//----------------- IsRepSizeSufficient -------------------//
bool RandomSet::IsRepSizeSufficient(void)
{
	// the repertoire size must be at least this much larger than the requested set list
	const float BUFF_FACTOR = 1.2f;

	RepertoireManager* repMgr = RepertoireManager::GetInstance();

	float minRepSizeMin = BUFF_FACTOR * static_cast<float>(*m_totalMin);
	float actRepSizeMin = static_cast<float>(repMgr->GetRepertoireDurationMin());

	if (actRepSizeMin <= minRepSizeMin)
		return false;

	return true;
}

//----------------- BuildSetList -------------------//
// returns true/false if building of the list succeded/failed
bool RandomSet::BuildSetList(void)
{
	std::unordered_set<uint32_t> indexSet;	// stores indexes to songs 

	RepertoireManager* repMgr = RepertoireManager::GetInstance();

	if (this->IsRepSizeSufficient() == false)
	{
		return false;
	}

	const uint32_t lowerRange = 0;
	const uint32_t upperRange = static_cast<uint32_t>(repMgr->GetRepertoireSize());

	const uint16_t maxDurSec = *m_totalMin * 60;
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
		m_setVect->push_back(*curSongPtr);
		curDurSec += pSong->GetDuration();
	}

	return true;
}


//-------------------------------------------------------------//
//------------------------ FullSet ----------------------------//
//-------------------------------------------------------------//

//----------------- BuildSetList -------------------//
// returns true/false if building of the list succeded/failed
bool FullSet::BuildSetList(void)
{
	RepertoireManager* repMgr = RepertoireManager::GetInstance();

	*m_totalMin = 0;	// set to 0, since this will be reset to the full repertoire duration

	for (auto curSong = repMgr->cbegin(); curSong != repMgr->cend(); ++curSong)
	{
		m_setVect->push_back(*curSong);
		*m_totalMin += curSong->get()->GetDuration();	// returns seconds
	}
	*m_totalMin /= 60;

	return true;
}



