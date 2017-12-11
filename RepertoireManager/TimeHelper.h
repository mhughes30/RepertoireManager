#pragma once

#ifndef TIME_HELPER_H
#define TIME_HELPER_H

#include <tuple>
#include <string>


using YearMonthDayTuple = std::tuple<std::string, std::string, std::string>;

// performs ctime/chrono operations used throughout the project
class TimeHelper
{
public:
	enum class TYPE
	{
		YEAR = 0, MONTH, DAY
	};

	static YearMonthDayTuple GetYearMonthDay(void);
	static int GetDate(TimeHelper::TYPE type);

private:
	static const int BASE_YEAR    = 1900;	// ctime starts from 1900
	static const int MONTH_OFFSET = 1;		// month ranges from 0 to 11
};


#endif
