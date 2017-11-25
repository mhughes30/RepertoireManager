#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\RepertoireManager\Song.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

__declspec(dllimport) Duration;


namespace UnitTests
{		
	TEST_CLASS(DURATION)
	{
	public:
		
		TEST_METHOD(Construct)
		{
			std::string exp = "7:35";
			uint16_t time = 455;
			Duration dur(time);
			const std::string res = dur.GetMinSecStr();

			Assert::AreEqual(res, exp, L"duration1", LINE_INFO());
		}

	};
}