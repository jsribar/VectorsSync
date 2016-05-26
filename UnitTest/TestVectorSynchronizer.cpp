#include "stdafx.h"
#include "CppUnitTest.h"
#include "../VectorSync/VectorSynchronizer.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest
{		
	struct StepLogger
	{
		~StepLogger()
		{
			cerr << "InsertFunctionCalls: " << InsertFunctionCalls << endl;
			cerr << "RemoveFunctionCalls: " << RemoveFunctionCalls << endl;
			cerr << "MoveFunctionCalls: " << MoveFunctionCalls << endl << endl;
		}
		void InsertFunction(string value, int position)
		{
			++InsertFunctionCalls;
		}

		void RemoveFunction(int position)
		{
			++RemoveFunctionCalls;
		}

		void MoveFunction(int from, int to)
		{
			++MoveFunctionCalls;
		}

		int InsertFunctionCalls{ 0 };
		int RemoveFunctionCalls{ 0 };
		int MoveFunctionCalls{ 0 };
	};


	TEST_CLASS(VectorSynchronizerTestTest1)
	{

		using StringVectorSynchronizer = VectorSynchronizer<string, StepLogger>;

	public:
		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsItemsReorderedAccordingToTarget)
		{
			vector<string> initial{ "Zagreb", "Wien", "Paris", "Roma", "London" };
			vector<string> targeted{ "Paris", "Zagreb", "London", "Wien", "Roma" };
			StepLogger sl;
			StringVectorSynchronizer vs(initial, sl);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}

		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsItemsWithAdditionalItemsReorderedAccordingToTarget)
		{
			vector<string> initial{ "Zagreb", "Wien", "Paris" };
			vector<string> targeted{ "Paris", "Zagreb", "London", "Wien", "Roma" };
			StepLogger sl;
			StringVectorSynchronizer vs(initial, sl);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}

		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsItemsExcludingRemovedItemsReorderedAccordingToTarget)
		{
			vector<string> initial{ "Zagreb", "Wien", "Paris", "London", "Roma" };
			vector<string> targeted{ "Roma", "London", "Paris" };
			StepLogger sl;
			StringVectorSynchronizer vs(initial, sl);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}

		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsItemsReorderedAccordingToTargetForCompletelyDifferentLists)
		{
			vector<string> initial{ "Zagreb", "Wien", "Paris", "London", "Roma" };
			vector<string> targeted{ "Madrid", "Lisboa", "Dublin", "Oslo", "Stockholm" };
			StepLogger sl;
			StringVectorSynchronizer vs(initial, sl);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}

		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsItemsReorderedAccordingToTargetForEmptyLists)
		{
			vector<string> initial;
			vector<string> targeted{ "Madrid", "Lisboa", "Dublin", "Oslo", "Stockholm" };
			StepLogger sl;
			StringVectorSynchronizer vs(initial, sl);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}

		TEST_METHOD(VectorSynchronizer_Synchronize_ReturnsEmptyVectorIfTargetIsEmpty)
		{
			vector<string> initial{ "Madrid", "Lisboa", "Dublin", "Oslo", "Stockholm" };
			vector<string> targeted;
			StepLogger sl;
			StringVectorSynchronizer vs(initial, sl);
			Assert::IsTrue(vs.SynchronizeToDestination(targeted) == targeted);
		}
	};
}