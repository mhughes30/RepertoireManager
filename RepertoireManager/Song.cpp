
#include "stdafx.h"
#include "Song.h"


//----------------------------------------------------------//
//---------------------- Duration --------------------------//
//----------------------------------------------------------//

//------------ operator overloads ------------//
std::ostream& operator<<(std::ostream& ostr, const Duration& dur)
{
	ostr << dur.GetMinSecStr();
	return ostr;
}

bool operator==(const Duration& lhs, const Duration& rhs)
{
	return (lhs.GetSeconds() == rhs.GetSeconds());
}

bool operator<=(const Duration& lhs, const Duration& rhs)
{
	return (lhs.GetSeconds() <= rhs.GetSeconds());
}

bool operator>=(const Duration& lhs, const Duration& rhs)
{
	return (lhs.GetSeconds() >= rhs.GetSeconds());
}

bool operator>(const Duration& lhs, const Duration& rhs)
{
	return (lhs.GetSeconds() > rhs.GetSeconds());
}

bool operator<(const Duration& lhs, const Duration& rhs)
{
	return (lhs.GetSeconds() < rhs.GetSeconds());
}

//------------ copy assignment ------------//
Duration& Duration::operator=(const Duration& dur)
{
	if (this != &dur)
	{
		this->m_durationSec = dur.m_durationSec;
		this->m_minSecStr = dur.m_minSecStr;
	}
	return *this;
}

//------------ move assignment ------------//
Duration& Duration::operator=(Duration&& dur)
{
	if (this != &dur)
	{
		this->m_durationSec = dur.m_durationSec;
		this->m_minSecStr   = dur.m_minSecStr;
	}
	return *this;
}

//------------ GetMinSecStr ------------//
std::string Duration::GetMinSecStr(void) const
{
	return this->m_minSecStr;
}

uint16_t Duration::GetSeconds(void) const
{
	return this->m_durationSec;
}

//------------ ComputeMinSecStr ------------//
void Duration::ComputeMinSecStr(void)
{
	constexpr uint16_t SEC_PER_MIN = 60;
	auto numMin = m_durationSec / SEC_PER_MIN;
	auto numSec = m_durationSec % SEC_PER_MIN;
	m_minSecStr = std::to_string(numMin) + ":" + std::to_string(numSec);
}


//----------------------------------------------------------//
//-------------------- GuitarConfig ------------------------//
//----------------------------------------------------------//

bool operator==(const GuitarConfig& lhs, const GuitarConfig& rhs)
{
	return (lhs.m_capoFret == rhs.m_capoFret) && (lhs.m_tuning == rhs.m_tuning);
}

//------------ const TuningMap ------------//
const GuitarConfig::TuningMap GuitarConfig::m_tuneMap = []
{
	TuningMap map = 
	{
		{ TUNING::STANDARD,		"E-A-D-G-B-E" },
		{ TUNING::DROP_D,		"D-A-D-G-B-E" },
		{ TUNING::CHOPI,		"E-G#-B-G#-B-E" },
		{ TUNING::CHOPI_MINOR,	"E-G-B-G-B-E" },
		{ TUNING::OPEN_D,		"D-A-D-F#-A-D" },
		{ TUNING::OPEN_E,		"E-B-E-G#-B-E" },
		{ TUNING::OPEN_G,		"D-G-D-G-B-D" },
		{ TUNING::FLY,			"B-E-B-E-B-E" },
	};

	return map;
}();

//------------ Get/Set CapoFret ------------//
void GuitarConfig::SetCapoFret( GuitarConfig::CAPO capoFret )
{
	if (capoFret < GuitarConfig::CAPO::MAX)
		this->m_capoFret = capoFret;
}

GuitarConfig::CAPO GuitarConfig::GetCapoFret(void)
{
	return this->m_capoFret;
}

//------------ Get/Set Tuning ------------//
void GuitarConfig::SetTuning( GuitarConfig::TUNING tuning )
{
	if (tuning < GuitarConfig::TUNING::MAX)
		this->m_tuning = tuning;
}

GuitarConfig::TUNING GuitarConfig::GetTuning(void)
{
	return m_tuning;
}

void GuitarConfig::GetTuning(std::string& tuning)
{
	tuning = m_tuneMap.at(m_tuning);
}


//----------------------------------------------------------//
//----------------------- Human ----------------------------//
//----------------------------------------------------------//
void Human::SetName(Human::NAME nameType, std::string name)
{
	if (nameType < Human::NAME::MAX)
	{
		switch (nameType)
		{
		case Human::NAME::FIRST:
			m_firstName = name;
			break;
		case Human::NAME::LAST:
			m_lastName = name;
			break;
		case Human::NAME::FULL:
		{
			size_t pos  = name.find(" ");
			m_firstName = name.substr(0, pos);
			m_lastName  = name.substr(pos+1);
			break;
		}
		default:
			break;
		}
	}
}

std::string Human::GetName(Human::NAME nameType) const
{
	std::string name = "";

	switch (nameType)
	{
	case Human::NAME::FIRST:
		name = m_firstName;
		break;
	case Human::NAME::LAST:
	default:
		name = m_lastName;
		break;
	case Human::NAME::FULL:
		name = m_firstName + " " + m_lastName;
		break;
	}

	return name;
}

std::ostream& operator<<(std::ostream& ostr, const Human& hum)
{
	ostr << hum.GetName(Human::NAME::LAST);
	return ostr;
}


//----------------------------------------------------------//
//------------------------ Song ----------------------------//
//----------------------------------------------------------//

//---------------------- ISong -----------------------//

//------------ Get/Set Title ------------//
void ISong::SetTitle(const std::string& title)
{
	m_title = title;
}

std::string ISong::GetTitle(void)
{
	return m_title;
}

//------------ GetSet Year ------------//
void ISong::SetYear(uint16_t year)
{
	m_year = year;
}

uint16_t ISong::GetYear(void)
{
	return m_year;
}

//------------ Get/Set Composer ------------//
void ISong::SetComposer(std::unique_ptr<Human>& hum)
{
	m_composer = *(hum.get());
}

void ISong::GetComposer(Human& hum)
{
	hum = m_composer;
}

std::string	ISong::GetComposer(void)
{
	return m_composer.GetName(Human::NAME::LAST);
}

//------------ Get/Set Duration ------------//
void ISong::SetDuration(std::unique_ptr<Duration>& dur)
{
	m_duration = *(dur.get());
}

uint16_t ISong::GetDuration(void)
{
	return m_duration.GetSeconds();
}

void ISong::GetDuration(Duration& dur)
{
	dur = m_duration;
}

//------------ SetInstrumentation ------------//
void ISong::SetInstrumentation(INSTRUMENTATION inst)
{
	m_instType = inst;
}

INSTRUMENTATION ISong::GetInstrumentation(void)
{
	return m_instType;
}

//------------ Get/Set Instrument ------------//
void ISong::SetInstrument(INSTRUMENT instrument)
{
	m_instTool = instrument;
}

INSTRUMENT ISong::GetInstrument(void)
{
	return m_instTool;
}


//---------------------- Guitar -----------------------//

//----------- Get/Set Genre -------------//
void Guitar::SetGenre(GENRE genre)
{
	m_genre = genre;
}

GENRE Guitar::GetGenre(void)
{
	return m_genre;
}

//----------- Get/Set GuitarConfig -------------//
void Guitar::GetGuitarConfig(GuitarConfig& config)
{
	config = m_config;
}

void Guitar::SetGuitarConfig(const GuitarConfig& config)
{
	m_config = config;
}

//------------ IsGenreEqual ------------//
bool Guitar::IsGenreEqual(const GENRE& other)
{
	return (m_genre == other);
}

//------------ IsComposerEqual ------------//
bool Guitar::IsComposerEqual(const Human& other)
{
	return ( m_composer.GetName(Human::NAME::FULL) == other.GetName(Human::NAME::FULL) );
}

//------------ IsConfigEqual ------------//
bool Guitar::IsConfigEqual(const GuitarConfig& other)
{
	return (m_config == other);
}

//----------- Comparison Operators ------------//
// all comparisons are based upon song duration
bool operator==(const Guitar& lhs, const Guitar& rhs)
{
	return (lhs.m_duration == rhs.m_duration);
}

bool operator<=(const Guitar& lhs, const Guitar& rhs)
{
	return (lhs.m_duration <= rhs.m_duration);
}

bool operator>=(const Guitar& lhs, const Guitar& rhs)
{
	return (lhs.m_duration >= rhs.m_duration);
}

bool operator>(const Guitar& lhs, const Guitar& rhs)
{
	return (lhs.m_duration > rhs.m_duration);
}

bool operator<(const Guitar& lhs, const Guitar& rhs)
{
	return (lhs.m_duration < rhs.m_duration);
}


//-----------------------------------------------------------//
//--------------------- EnumToString ------------------------//
//-----------------------------------------------------------//

//------------ GetStrFromCapo ------------//
const EnumToStr::GuitarCapoToStrMap EnumToStr::getStrFromCapo = []
{
	EnumToStr::GuitarCapoToStrMap map =
	{
		{ GuitarConfig::CAPO::NONE,		"0" },
		{ GuitarConfig::CAPO::ON_1,		"1" },
		{ GuitarConfig::CAPO::ON_2,		"2" },
		{ GuitarConfig::CAPO::ON_3,		"3" },
		{ GuitarConfig::CAPO::ON_4,		"4" },
		{ GuitarConfig::CAPO::ON_5,		"5" },
		{ GuitarConfig::CAPO::ON_6,		"6" },
		{ GuitarConfig::CAPO::ON_7,		"7" },
		{ GuitarConfig::CAPO::ON_8,		"8" },
		{ GuitarConfig::CAPO::ON_9,		"9" },
		{ GuitarConfig::CAPO::ENUM_MAX,	"Capo" },
	};
	return map;
}();

//------------ GetStrFromTuning ------------//
const EnumToStr::GuitarTuningToStrMap EnumToStr::getStrFromTuning = []
{
	EnumToStr::GuitarTuningToStrMap map =
	{
		{ GuitarConfig::TUNING::STANDARD,		"STANDARD" },
		{ GuitarConfig::TUNING::DROP_D,			"DROP_D" },
		{ GuitarConfig::TUNING::CHOPI,			"CHOPI" },
		{ GuitarConfig::TUNING::CHOPI_MINOR,	"CHOPI_MINOR" },
		{ GuitarConfig::TUNING::OPEN_D,			"OPEN_D" },
		{ GuitarConfig::TUNING::OPEN_E,			"OPEN_E" },
		{ GuitarConfig::TUNING::OPEN_G,			"OPEN_G" },
		{ GuitarConfig::TUNING::FLY,			"FLY" },
		{ GuitarConfig::TUNING::ENUM_MAX,		"Tuning" },
	};
	return map;
}();

//------------ GetStrFromInstrumentation ------------//
const EnumToStr::InstrumentationToStrMap EnumToStr::getStrFromInstrumentation = []
{
	EnumToStr::InstrumentationToStrMap map =
	{
		{ INSTRUMENTATION::SOLO,			"SOLO" },
		{ INSTRUMENTATION::ENSEMBLE,		"ENSEMBLE" },
		{ INSTRUMENTATION::BACKING_TRACK,	"BACKING_TRACK" },
		{ INSTRUMENTATION::DUET,			"DUET" },
		{ INSTRUMENTATION::ENUM_MAX,		"Instrumentation" },
	};
	return map;
}();

//------------ GetStrFromInstrument ------------//
const EnumToStr::InstrumentToStrMap EnumToStr::getStrFromInstrument = []
{
	EnumToStr::InstrumentToStrMap map =
	{
		{ INSTRUMENT::GUITAR,	"GUITAR" },
		{ INSTRUMENT::SITAR,	"SITAR" },
		{ INSTRUMENT::UKULELE,	"UKULELE" },
		{ INSTRUMENT::ENUM_MAX,	"Instrument" },
	};
	return map;
}();

//------------ getStrFromGenre ------------//
 const EnumToStr::GenreToStrMap EnumToStr::getStrFromGenre = []
{
	EnumToStr::GenreToStrMap map =
	{
		{ GENRE::MISC,			"MISC" },
		{ GENRE::SAD,			"SAD" },
		{ GENRE::MELANCHOLY,	"MELANCHOLY" },
		{ GENRE::ROMANTIC,		"ROMANTIC" },
		{ GENRE::BRIGHT,		"BRIGHT" },
		{ GENRE::FLAMENCO,		"FLAMENCO" },
		{ GENRE::HAWAIIAN,		"HAWAIIAN" },
		{ GENRE::ENUM_MAX,		"Genre" },
	};
	return map;
}();



