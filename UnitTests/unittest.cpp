#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\RepertoireManager\Song.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{		
	//--------- DURATION ----------//
	TEST_CLASS(DURATION)
	{
	public:
		
		TEST_METHOD(Construct)
		{
			std::string exp = "7:35";
			uint16_t time = 455;
			Duration dur(time);
			std::string res = dur.GetMinSecStr();
			Assert::AreEqual(res, exp, L"ctor1", LINE_INFO());

			Duration dur2(std::move(dur));
			res = dur2.GetMinSecStr();
			Assert::AreEqual(res, exp, L"ctor2", LINE_INFO());
		}
		TEST_METHOD(Assignment)
		{
			uint16_t time = 455;
			Duration dur(time);
			Duration dur2 = dur;
			//--- ==
			bool compare = (dur2 == dur);
			Assert::IsTrue(compare, L"assign1", LINE_INFO());

			//--- >
			dur2 = Duration(500);
			compare = (dur2 > dur);
			Assert::IsTrue(compare, L"assign2", LINE_INFO());
		}
	};

	//--------- GUITARCONFIG ----------//
	TEST_CLASS(GUITARCONFIG)
	{
	public:

		TEST_METHOD(Construct)
		{
			using G = GuitarConfig;
			GuitarConfig conf1(G::CAPO::ON_2, G::TUNING::STANDARD);
			std::string expTune = "E-A-D-G-B-E";
			uint16_t    expCapo = static_cast<uint16_t>(G::CAPO::ON_2);

			std::string actTune;
			conf1.GetTuning(actTune);
			bool compare = (actTune == expTune);
			Assert::IsTrue(compare, L"const1", LINE_INFO());

			uint16_t actCapo = static_cast<uint16_t>(conf1.GetCapoFret());
			compare = (expCapo == actCapo);
			Assert::IsTrue(compare, L"const2", LINE_INFO());
		}
	};

	//--------- HUMAN ----------//
	TEST_CLASS(HUMAN)
	{
	public:

		TEST_METHOD(Construct)
		{
			std::string aComp = "Antonio Lauro";
			Human a(aComp);
			std::string first = a.GetName(Human::NAME::FIRST);
			std::string last  = a.GetName(Human::NAME::LAST);
			bool isEqual = (first + " " + last) == aComp;
			Assert::IsTrue(isEqual, L"const1", LINE_INFO());

			a.SetName(Human::NAME::FIRST, "Mike");
			isEqual = ("Mike " + last) == "Mike Lauro";
			Assert::IsTrue(isEqual, L"const2", LINE_INFO());
		}
	};

}