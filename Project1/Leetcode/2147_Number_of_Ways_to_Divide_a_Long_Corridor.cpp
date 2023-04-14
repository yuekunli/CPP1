#include "LibHeaders.h"

namespace _2147_Number_of_Ways_to_Divide_a_Long_Corridor {

using namespace std;

class Solution
{
public:

	// ..... 5  6  7  8  9 ....
	//       S  P  P  P  S
	// if a pair is {5,9} it means there is a seat at each of 5 and 9
	// there are 4 gaps, 4 choices to install barrier
	int countPossibilities(vector<pair<int, int>>& v)
	{
		int total = 1;
		for (auto& p : v)
		{
			int choicesInThisPair = p.second - p.first;
			total *= choicesInThisPair;
			total %= 1'000'000'007;
		}
		return total;
	}


	int numberOfWays(string corridor)
	{
		vector<pair<int, int>> possibleInstallPositions;
		int index = 0;
		bool foundSeat1 = false;
		bool foundSeat2 = false;
		int seat2Index = -1;
		for (auto& c : corridor)
		{
			if (c == 'P')
			{
				index++;
				continue;
			}
			if (c == 'S')
			{
				if (!foundSeat1)
				{
					foundSeat1 = true;
					index++;
					continue;
				}
				if (foundSeat1 && !foundSeat2)
				{
					seat2Index = index;
					foundSeat2 = true;
					index++;
					continue;
				}
				if (foundSeat1 && foundSeat2)
				{
					possibleInstallPositions.emplace_back(seat2Index, index); // if there are just 1 pair of seats, I'll never add to "possibleInstallPositions"
					index++;
					foundSeat1 = true;
					foundSeat2 = false;
					continue;
				}
			}
		}
		if (!(foundSeat1 && foundSeat2))  // total seat count is odd
			return 0;
		if (possibleInstallPositions.size() == 0 && !foundSeat1 && !foundSeat2) // there is no seat at all
			return 0;

		int answer = countPossibilities(possibleInstallPositions); // this will return 1 if "possibleInstallPosition" is empty, which means there is only 1 pair

		return answer;
	}
};


void Test_2147_Number_of_Ways_to_Divide_a_Long_Corridor()
{
	Solution s;
	string corridor;
	while (true)
	{
		cout << "corridor: ";
		getline(cin, corridor);
		if (corridor.size() == 0) break;
		cout << s.numberOfWays(corridor) << '\n';
	}
}
}