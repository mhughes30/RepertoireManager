#pragma once
#ifndef SET_BUILDER_H
#define SET_BUILDER_H

#include <memory>
#include <vector>
#include <random>
#include <unordered_set>
#include <fstream>
#include <stdint.h>

#include "Song.h"
#include "RepertoireManager.h"


// short-hand for these iterators
using SetListConstIter = std::vector<boost::shared_ptr<ISong>>::const_iterator;
using SetListIter	   = std::vector<boost::shared_ptr<ISong>>::iterator;
using SetListData	   = std::vector<boost::shared_ptr<ISong>>;


//---------------------------------------------------------//
//--------------------- SetListBuilder --------------------//
//---------------------------------------------------------//
// Decorator class with a songT decorator. 
// songT is intended to be an implementation of ISetMaker
template<typename songT>
class SetListBuilder
{
public:
	SetListBuilder() = delete;

	// must set targetDuration upon creation
	SetListBuilder( uint16_t targetDuration_min ) : m_totalDuration_min(targetDuration_min)
	{
		m_builder.SetDuration( m_totalDuration_min );
		m_builder.SetSetListStruct(m_setList);
	}
	
	// external iterators to the set list
	SetListConstIter cbegin() { return m_setList.cbegin(); }
	SetListConstIter cend() { return m_setList.cend(); }
	
	void	 SetTotalDuration(uint16_t min) { m_totalDuration_min = min; }
	uint16_t GetTotalDuration( void ) { return m_totalDuration_min; }
	void	 BuildSetList(void) { m_builder.BuildSetList(); }
	
private:
	songT		m_builder;
	uint16_t	m_totalDuration_min;
	SetListData m_setList;
};


//-------------------------------------------------------------//
//--------------------- SetListHtmlWriter----------------------//
//-------------------------------------------------------------//
class SetListHtmlWriter
{
public:
	SetListHtmlWriter() = delete;
	SetListHtmlWriter(std::string fileName) : m_fileName(fileName), m_file(fileName)
	{}

	bool WriteSetListToHtml(SetListConstIter begin, SetListConstIter end, uint16_t duration = 0);

private:
	void CreateTitle(const std::string& titleStr);
	void CreateTableTitle(const std::string& titleStr, uint16_t duration);

	void CreateHeadings(void);
	void AddTableRecord(boost::shared_ptr<ISong> pSong);

	std::ofstream m_file;
	std::string m_fileName;
};


//-------------------------------------------------------------//
//------------------- Set List Decorators ---------------------//
//-------------------------------------------------------------//
// use as template songT param in SetListBuilder

//------ ISetMaker -------//
// intended to be an interface
class ISetMaker
{
public:
	virtual ~ISetMaker() {};
	virtual void SetSetListStruct(SetListData& songVec);
	virtual void SetDuration(uint16_t& min);
	// returns true if build succeeded; false otherwise
	virtual bool BuildSetList(void) { return false; }
protected:
	uint16_t*	 m_totalMin;
	SetListData* m_setVect;
};


//------ RandomSet -------//
// Generates a Random Set List
// meets the set-list duration criteria
class RandomSet : public ISetMaker
{
public:
	RandomSet();
	~RandomSet() = default;
	bool BuildSetList(void);	
private:
	std::random_device	m_randDev;	// provides seed for m_randEng
	std::mt19937		m_randEng;	// randum number engine (MT)

	uint32_t GetRandomIndex(uint32_t lowRange, uint32_t highRange);
	bool     IsRepSizeSufficient(void);
};


//------ FullSet -------//
// Generates a Set list from all of the music available
class FullSet : public ISetMaker
{
public:
	FullSet() = default;
	~FullSet() = default;
	bool BuildSetList(void);
private:
};


#endif


