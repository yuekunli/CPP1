#include "LibHeaders.h"

namespace _1815_Maximum_Number_Groups_Fresh_Donuts {

	using namespace std;

	class Solution
	{
		/*
		* What makes up the "state" of the execution/progression?
		* The number of groups in each super group.
		* If batch size is 5, there is a group of 3 and a group of 23, these two are just
		* two groups that have 3 extra people, so they are in the same super group.
		* As long as the size of each super group is the same, the state is the same.
		* Of course another factor that affect the progression is the remaining donuts
		* when the groups present a certain state.
		*/
		unsigned long long convertToState(vector<int>& superGroups, int batchSize)
		{
			/*
			*    overallState:    X X X X X    X X X X X  . . . .   X X X X X
			*                     \_______/    \_______/            \_______/
			*                         |                                |
			*                      number of groups                    number of groups
			*                      mod batch = batch-1                 mod batch = 1
			*/

			unsigned long long overallState = 0;
			unsigned long long countOfOneSuperGroup;

			for (int i = 1; i < batchSize; i++)
			{
				countOfOneSuperGroup = superGroups[i];
				countOfOneSuperGroup = countOfOneSuperGroup << ((i - 1)*5);
				overallState |= countOfOneSuperGroup;
			}
			return overallState;
		}

		unsigned long long modifyState(vector<int>& superGroups, int superGroupIndex, unsigned long long oldState)
		{
			unsigned long long newSuperGroupCount = superGroups[superGroupIndex];
			newSuperGroupCount = newSuperGroupCount << ((superGroupIndex - 1) * 5);
			unsigned long long newState = oldState;
			unsigned long long cleaner = 0b11111ull << ((superGroupIndex - 1) * 5);
			unsigned long long fullMask = (1ull << 40) - 1;  // must use "ull" suffix on integer literal to denote it is a unsigned long long
			cleaner = cleaner ^ fullMask;
			newState = oldState & cleaner;
			newState = newState |= newSuperGroupCount;
			return newState;
		}


		int recursiveSolve(unordered_map<unsigned long long, vector<int>>&rb, unsigned long long state, int remain, vector<int>&superGroups, int batchSize)
		{
			//if (all_of(superGroups.cbegin(), superGroups.cend(), [](int const& a) {return a==0;}))
			//{
			//	return 0;
			//}
			if (state == 0)
				return 0;

			if (rb.count(state) != 0 && rb[state][remain] != -1)
				return rb[state][remain];
	
			if (rb.count(state) == 0)
				rb[state].assign(batchSize, -1);

			int bestResult = 0;
			for (int i = 1; i < batchSize; i++)
			{
				if (superGroups[i] > 0)
				{
					superGroups[i]--;
					unsigned long long newState = modifyState(superGroups, i, state);
					int newRemain = 0;
					if (remain >= i)
						newRemain = remain - i;
					else
						newRemain = remain + batchSize - i;
					int result = recursiveSolve(rb, newState, newRemain, superGroups, batchSize) + (remain == 0? 1 : 0);
					superGroups[i]++;
					if (result > bestResult)
						bestResult = result;
				}
			}
			return bestResult;
		}

	public:

		int maxHappyGroups(int batchSize, vector<int>& groups)
		{
			vector<int>superGroups(batchSize, 0);
			for (int& n : groups)
			{
				int remain = n % batchSize;
				superGroups[remain]++;
			}
			int happyGroups = 0;
			happyGroups += superGroups[0];
			for (int i = 1; i <= batchSize / 2; i++)
			{
				int j = batchSize - i;
				if (i != j)
				{
					int m = min(superGroups[i], superGroups[j]);
					superGroups[i] -= m;
					superGroups[j] -= m;
					happyGroups += m;
				}
				else
				{
					int m = superGroups[i] / 2;
					superGroups[i] -= (m * 2);
					happyGroups += m;
				}
			}
			unsigned long long state = convertToState(superGroups, batchSize);
			
			if (state == 0)
				return happyGroups;

			unordered_map<unsigned long long, vector<int>> recordbook;
			return happyGroups + recursiveSolve(recordbook, state, 0, superGroups, batchSize);
		}
	};

	void Test_1815_Maximum_Number_Groups_Fresh_Donuts()
	{
		Solution s;

		vector<int>groups;
		int batchSize;
		string s1;

		while (true)
		{
			s1.clear();
			cout << "groups: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			groups.clear();
			stringstream ss(s1);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(groups));
			cout << "batch size: ";
			cin >> batchSize;
			cin.ignore();
			cout << s.maxHappyGroups(batchSize, groups) << '\n';
		}
	}
}