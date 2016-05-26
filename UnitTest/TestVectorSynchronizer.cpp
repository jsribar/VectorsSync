#include "stdafx.h"
#include "CppUnitTest.h"
#include "../VectorSync/VectorsSynchronizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest
{		
	TEST_CLASS(VectorSynchronizerTestTest1)
	{
	public:
		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsItemsReorderedAccordingToTarget)
		{
			vector<string> initial{ "Zagreb", "Wien", "Paris", "Roma", "London" };
			vector<string> targeted{ "Paris", "Zagreb", "London", "Wien", "Roma" };
			VectorsSynchronizer<string> vs(initial);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}

		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsItemsWithAdditionalItemsReorderedAccordingToTarget)
		{
			vector<string> initial{ "Zagreb", "Wien", "Paris" };
			vector<string> targeted{ "Paris", "Zagreb", "London", "Wien", "Roma" };
			VectorsSynchronizer<string> vs(initial);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}

		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsItemsExcludingRemovedItemsReorderedAccordingToTarget)
		{
			vector<string> initial{ "Zagreb", "Wien", "Paris", "London", "Roma" };
			vector<string> targeted{ "Paris", "London", "Roma" };
			VectorsSynchronizer<string> vs(initial);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}

		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsItemsReorderedAccordingToTargetForCompletelyDifferentLists)
		{
			vector<string> initial{ "Zagreb", "Wien", "Paris", "London", "Roma" };
			vector<string> targeted{ "Madrid", "Lisboa", "Dublin", "Oslo", "Stockholm" };
			VectorsSynchronizer<string> vs(initial);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}

		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsItemsReorderedAccordingToTargetForEmptyLists)
		{
			vector<string> initial;
			vector<string> targeted{ "Madrid", "Lisboa", "Dublin", "Oslo", "Stockholm" };
			VectorsSynchronizer<string> vs(initial);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}

		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsEmptyVectorIfTargetIsEmpty)
		{
			vector<string> initial{ "Madrid", "Lisboa", "Dublin", "Oslo", "Stockholm" };
			vector<string> targeted;
			VectorsSynchronizer<string> vs(initial);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}
	};
}