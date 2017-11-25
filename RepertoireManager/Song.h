
#pragma once
#ifndef SONG_H
#define SONG_H

#include <iostream>
#include <string>
#include <array>
#include <map>
#include <memory>

#include "archives.h"

#define ENUM_MAX	MAX

//-------------------------------------------------------//
//-------------------- Duration -------------------------//
//-------------------------------------------------------//
// Describes a pieces duration
class Duration
{
public:
	Duration() {}
	explicit Duration(uint16_t durationSec) : m_durationSec(durationSec)
	{
		ComputeMinSecStr();
	}
	virtual ~Duration() = default;

	Duration(const Duration& other) = default;
	Duration(Duration&& other)      = default;
	
	Duration& operator=(const Duration& other);
	Duration& operator=(Duration&& other);

	// comparison operators all baed on time
	friend bool operator==( const Duration& lhs, const Duration& rhs);
	friend bool operator<=(const Duration& lhs, const Duration& rhs);
	friend bool operator>=(const Duration& lhs, const Duration& rhs);
	friend bool operator>(const Duration& lhs, const Duration& rhs);
	friend bool operator<(const Duration& lhs, const Duration& rhs);

	friend std::ostream& operator<<(std::ostream& ostr, const Duration& dur);

	std::string GetMinSecStr(void) const;
	uint16_t    GetSeconds(void) const;

protected:
	virtual void ComputeMinSecStr(void);

	uint16_t	m_durationSec = 0;
	std::string m_minSecStr = "";

private:
	// boost serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_durationSec;
		ar & m_minSecStr;
	}
};


//-------------------------------------------------------//
//-------------------- GuitarConfig ---------------------//
//-------------------------------------------------------//
// Describes the configuration of a guitar (tuning, etc)
class GuitarConfig
{
public:
	// Enum of capo/cejilla positions on the guitar
	// MAX considers reasonable locations. 
	enum class CAPO : uint8_t
	{
		NONE = 0, 
		ON_1, ON_2, ON_3, ON_4, ON_5, 
		ON_6, ON_7, ON_8, ON_9, ENUM_MAX
	};

	// List of tunings typically used
	enum class TUNING : uint8_t
	{
		STANDARD = 0,
		DROP_D,
		CHOPI,
		CHOPI_MINOR,
		OPEN_D,
		OPEN_E,
		OPEN_G,
		FLY,
		ENUM_MAX
	};

	GuitarConfig() {};
	GuitarConfig(GuitarConfig::CAPO capoFret,  GuitarConfig::TUNING tuning)
	{
		SetCapoFret(capoFret);
		SetTuning(tuning);
	}

	friend bool operator==(const GuitarConfig& lhs, const GuitarConfig& rhs);

	virtual void SetCapoFret(GuitarConfig::CAPO capoFret);
	virtual CAPO GetCapoFret(void);

	virtual void	SetTuning(GuitarConfig::TUNING tuning);
	virtual TUNING	GetTuning(void);
	virtual void	GetTuning(std::string& tuning);
	
protected:
	CAPO	m_capoFret = CAPO::NONE;		// capo position
	TUNING	m_tuning   = TUNING::STANDARD;	// tuning used

private:
	// map of TUNING to its physical string-representation
	using TuningMap = std::map<TUNING, std::string>;
	static const TuningMap m_tuneMap;

	// boost serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_capoFret;
		ar & m_tuning;
	}
};


//-------------------------------------------------------//
//------------------------ Human ------------------------//
//-------------------------------------------------------//
// properties of a human
class Human
{
public:
	enum class NAME : uint8_t
	{
		FIRST,
		LAST,
		FULL,
		ENUM_MAX
	};

	Human() = default;
	// creates a human with the full name
	Human(const std::string& fullName)
	{
		SetName(NAME::FULL, fullName);
	}
	
	friend std::ostream& operator<<(std::ostream& ostr, const Human& hum);

	virtual void		SetName(Human::NAME nameType, std::string name);
	virtual std::string GetName(Human::NAME nameType) const;
	
protected:
	std::string m_lastName  = "";
	std::string m_firstName = "";

private:
	// boost serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_lastName;
		ar & m_firstName;
	}
};


//-------------------------------------------------------//
//---------------------- Songs --------------------------//
//-------------------------------------------------------//
// describes propeties of a composition/song

//--- Useful enum of guitar-only genres, describing mood of the pieces
enum class GENRE : uint8_t
{
	MISC = 0,
	SAD,
	MELANCHOLY,
	ROMANTIC,
	BRIGHT,
	FLAMENCO,
	HAWAIIAN,
	ENUM_MAX
};

//--- enum of instruments played in repertoire
enum class INSTRUMENT : uint8_t
{
	GUITAR,
	SITAR,
	UKULELE,
	ENUM_MAX
};

//--- enum of different instrumentations
enum class INSTRUMENTATION : uint8_t
{
	SOLO,			// solo intsrument
	ENSEMBLE,		// ensemble; e.g. string-quintet
	BACKING_TRACK,	// playing with a prerecorded backing track
	DUET,			// duet with another instrument
	ENUM_MAX
};

//--- template for song types
class ISong
{
public:
	ISong() = default;
	ISong( const ISong& other)
	{
		m_year	   = other.m_year;
		m_composer = other.m_composer;
		m_title    = other.m_title;
		m_duration = other.m_duration;
		m_instType = other.m_instType;
		m_instTool = other.m_instTool;
	}
	virtual ~ISong() {}

	virtual void SetYear(uint16_t year);
	virtual void SetComposer(std::unique_ptr<Human>& hum);
	virtual void SetTitle(const std::string& title);
	virtual void SetDuration(std::unique_ptr<Duration>& dur);
	virtual void SetInstrumentation(INSTRUMENTATION inst);
	virtual void SetInstrument(INSTRUMENT instrument);

	virtual uint16_t		GetYear(void);
	virtual void			GetComposer(Human& hum);
	virtual std::string		GetTitle(void);
	virtual void			GetDuration(Duration& dur);
	virtual INSTRUMENTATION GetInstrumentation(void);
	virtual INSTRUMENT		GetInstrument(void);

protected:
	uint16_t		m_year = 1980;
	Human			m_composer = "";
	std::string		m_title = "";
	Duration		m_duration;
	INSTRUMENTATION m_instType = INSTRUMENTATION::SOLO;
	INSTRUMENT		m_instTool = INSTRUMENT::GUITAR;

private:
	// boost serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_year;
		ar & m_composer;
		ar & m_title;
		ar & m_duration;
		ar & m_instType;
		ar & m_instTool;
	}
};
BOOST_SERIALIZATION_ASSUME_ABSTRACT(ISong)

//--- guitar type 
class Guitar : public ISong
{
public:
	Guitar() = default;
	Guitar(Guitar&& guitar)
	{
		m_config   = guitar.m_config;
		m_genre	   = guitar.m_genre;
		m_year	   = guitar.m_year;
		m_title	   = guitar.m_title;
		m_composer = guitar.m_composer;
		m_duration = guitar.m_duration;
		m_instTool = guitar.m_instTool;
		m_instType = guitar.m_instType;
	}

	// equality comparisons for genre and composer
	virtual bool IsGenreEqual(const GENRE& other);
	virtual bool IsComposerEqual(const Human& other);
	virtual bool IsConfigEqual(const GuitarConfig& other);

	virtual void  SetGenre(GENRE genre);
	virtual GENRE GetGenre(void);
	virtual void  GetGuitarConfig(GuitarConfig& config);
	virtual void  SetGuitarConfig(const GuitarConfig& config);
	
	// all comparisons are based upon song duration, for filling a timeslot
	friend bool operator==(const Guitar& lhs, const Guitar& rhs);
	friend bool operator<=(const Guitar& lhs, const Guitar& rhs);
	friend bool operator>=(const Guitar& lhs, const Guitar& rhs);
	friend bool operator>(const Guitar& lhs, const Guitar& rhs);
	friend bool operator<(const Guitar& lhs, const Guitar& rhs);

protected:
	// initalize to no capo and standard tuning, and "misc" genre
	GuitarConfig m_config = { GuitarConfig::CAPO::NONE, GuitarConfig::TUNING::STANDARD };
	GENRE		 m_genre  = GENRE::MISC;

private:
	// boost serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		// add base class information
		ar & boost::serialization::base_object<ISong>(*this);
		ar & m_config;
		ar & m_genre;
	}
};

// TODO - add other types later - change to decorator pattern?


//-----------------------------------------------------------//
//--------------------- EnumToString ------------------------//
//-----------------------------------------------------------//

struct EnumToStr
{
	using GenreToStrMap = std::map<GENRE, std::string>;
	static const GenreToStrMap getStrFromGenre;

	using InstrumentToStrMap = std::map<INSTRUMENT, std::string>;
	static const InstrumentToStrMap getStrFromInstrument;

	using InstrumentationToStrMap = std::map<INSTRUMENTATION, std::string>;
	static const InstrumentationToStrMap getStrFromInstrumentation;

	using GuitarTuningToStrMap = std::map<GuitarConfig::TUNING, std::string>;
	static const GuitarTuningToStrMap getStrFromTuning;

	using GuitarCapoToStrMap = std::map<GuitarConfig::CAPO, std::string>;
	static const GuitarCapoToStrMap getStrFromCapo;
};



#endif  // !SONG_H

