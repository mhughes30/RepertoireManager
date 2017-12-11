
#include <chrono>
#include "TimeHelper.h"

// disable localtime warning
#pragma warning(disable:4996)

//-------------- GetYearMonthDay ----------------//
YearMonthDayTuple TimeHelper::GetYearMonthDay(void)
{
	auto   timeNow  = std::chrono::system_clock::now();
	time_t cTimeNow = std::chrono::system_clock::to_time_t(timeNow);
	auto cTm  = *localtime(&cTimeNow);

	std::string year  = std::to_string(cTm.tm_year + BASE_YEAR);
	std::string month = std::to_string(cTm.tm_mon + MONTH_OFFSET);
	std::string day   = std::to_string(cTm.tm_mday);

	return std::make_tuple(year, month, day);
}


//-------------- GetYear ----------------//
int TimeHelper::GetDate(TimeHelper::TYPE type)
{
	auto   timeNow = std::chrono::system_clock::now();
	time_t cTimeNow = std::chrono::system_clock::to_time_t(timeNow);
	auto cTm = *localtime(&cTimeNow);

	int date;
	switch (type)
	{
	case TYPE::DAY:
		date = cTm.tm_mday;
		break;
	case TYPE::MONTH:
		date = cTm.tm_mon + MONTH_OFFSET;
		break;
	case TYPE::YEAR:		
	default:
		date = cTm.tm_year + BASE_YEAR;
		break;
	}

	return date;
}
