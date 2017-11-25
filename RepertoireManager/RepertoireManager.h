
#pragma once
#ifndef REPORTIORE_MANAGER_H
#define REPORTIORE_MANAGER_H

#include <list>
#include <vector>
#include "Song.h"
#include "archives.h"


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
	//std::vector<ISong*> m_repertoire;
	std::vector<boost::shared_ptr<ISong>> m_repertoire2;
	
	//--- filename to use
	//const std::string m_fileName = std::string(boost::archive::tmpdir()) + "\\songs.txt";
	const std::string m_fileName = ".\\Resources\\songs.txt";

	//--- boost serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		// must register all of the child types to make sure the archive 
		// contains the correct information.
		//ar.register_type(static_cast<Guitar*>nullptr);
		ar.register_type(static_cast<boost::shared_ptr<Guitar>>nullptr);
		// append the repertoire to the arhive
		ar & m_repertoire2;
	}
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

	// erase song from the archive
	void   EraseSong(uint16_t index);
	// append song to the archive
	void   AppendSong(ISong& song);
	// read the archive from disk
	void   ReadRepertoireFromDisk(void);
	// write the archive to disk
	void   WriteRepertoireToDisk(void);
	// get the current archive size
	size_t GetRepertoireSize(void);

	std::vector<boost::shared_ptr<ISong>>::const_iterator cbegin();
	std::vector<boost::shared_ptr<ISong>>::const_iterator cend();
};



#endif
