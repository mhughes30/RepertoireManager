
#pragma once
#ifndef ARHIVES_H
#define ARCHIVE_H

// Contains files related to boos serialization and archiving

// includes for serializing and storing objects to file
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
// disable a warning associated with usage of Boost serialzer
#pragma warning(disable:4996)
#include <boost/archive/tmpdir.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>

#endif