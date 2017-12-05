#pragma once
#ifndef SET_BUILDER_H
#define SET_BUILDER_H

#include <memory>
#include <vector>
#include <random>
#include <unordered_set>
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
		//m_builder.SetIterators( begin(), end() );
	}
	
	// external iterators to the set list
	SetListConstIter cbegin() { return m_setList.cbegin(); }
	SetListConstIter cend() { return m_setList.cend(); }
	
	void	 SetTotalDuration(uint16_t min) { m_totalDuration_min = min; }
	uint16_t GetTotalDuration( void ) { return m_totalDuration_min; }
	void	 BuildSetList(void) { m_builder.BuildSetList() };

private:
	songT		m_builder;
	uint16_t	m_totalDuration_min;
	SetListData m_setList;

	// internal iterators to the set list
	//SetListIter begin() { return m_setList.begin(); }
	//SetListIter end() { return m_setList.end(); }
};


//-------------------------------------------------------------//
//------------------- Set List Decorators ---------------------//
//-------------------------------------------------------------//

//------ ISetMaker -------//
// songT type
class ISetMaker
{
public:
	virtual void SetSetListStruct(SetListData& songVec);
	virtual void SetDuration(uint16_t min);
	// returns true if build succeeded; false otherwise
	virtual bool BuildSetList(void) = 0;
protected:
	uint16_t	m_totalMin = 0;
	SetListData m_setVect;
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
};


#endif


