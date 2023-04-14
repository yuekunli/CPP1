#include "LibHeaders.h"

namespace _1419_Minimum_Number_of_Frogs_Croaking {

	using namespace std;

	class Solution
	{
	public:

		/*
		* Each frog operates like a state machine (automatum)
		*/

		int minNumberOfFrogs(string croakOfFrogs)
		{
			unordered_map<char, int> m;
			m['c'] = 1;
			m['r'] = 2;
			m['o'] = 3;
			m['a'] = 4;
			m['k'] = 5;
			vector<int> frogsStates;
			for (char const& c : croakOfFrogs)
			{
				int nextState = m[c];
				int i;
				for (i = 0; i < frogsStates.size(); i++)
				{
					if (frogsStates[i] == nextState - 1)
					{
						frogsStates[i] = (frogsStates[i] + 1) % 5;
						break;
					}
				}
				if (i == frogsStates.size() && nextState == 1)
					frogsStates.push_back(1);
				if (i == frogsStates.size() && nextState != 1)
					return -1;
			}
			if (all_of(frogsStates.begin(), frogsStates.end(),
				[](int const & n)
				{return n == 0; })
				)
				return frogsStates.size();

			return -1;
		}
	};



	class Solution2
	{
		// this solution is better because its time cost doesn't depend on
		// the total number of frogs necessary to croak out the string
	public:

		int minNumberOfFrogs(string croakOfFrogs)
		{
			unordered_map<char, int> m;
			m['c'] = 0;  // the index into the "occurrence" array
			m['r'] = 1;
			m['o'] = 2;
			m['a'] = 3;
			m['k'] = 4;

			int occurrence[5] = { 0,0,0,0,0 };

			// at any time the number of 'c' must be greater than or equal to the number of 'r',
			// 'r' must be greater than or equal to 'o', so on so forth

			int totalFrogs = 0;
			int idleFrogs = 0;
			for (char const& c : croakOfFrogs)
			{
				int index = m[c];
				if (index == 0)
				{
					if (idleFrogs > 0)
					{
						idleFrogs--;
						occurrence[0]++;
					}
					else
					{
						totalFrogs++;
						occurrence[0]++;
					}
				}
				else if (index != 4)
				{
					if (occurrence[index - 1] == occurrence[index])
						return -1;
					else
						occurrence[index]++;
				}
				else
				{
					idleFrogs++;
					//for_each(begin(occurrence), end(occurrence),
					//	[](int & n)
					//	{n--; });
				}
			}
			return totalFrogs == idleFrogs ? totalFrogs : -1;
		}

	};

	void Test_1419_Minimum_Number_of_Frogs_Croaking()
	{
		string input;
		Solution2 s;
		while (true)
		{
			input.clear();
			cout << "input: ";
			getline(cin, input);
			if (input.size() == 0) break;
			cout << s.minNumberOfFrogs(input) << '\n';
		}
	}
}