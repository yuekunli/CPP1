#include "LibHeaders.h"

namespace _2141_Maximum_Running_Time_of_N_Computers {

using namespace std;

class Solution
{

	// n = 2, batteries = [3, 3, 3]
	// 3 3 3
	// 2 2 3
	// 1 2 2
	// 1 1 1
	// 0 0 1
	// always use the two fullest batteries

	// 10, 8, 7, 5,  |  5, 4, 3, 3, 3,

	// distribute the sum of extra batteries onto the first n batteries so that the first n numbers are as
	// equally distributed as possible.

    // 10, 8, 7, 5     extra sum = 18
	// 10, 8, 7, 7     extra sum = 16
	// 10, 8, 8, 8     extra sum = 14
	// 10, 10, 10, 10  extra sum = 8
	// 12, 12, 12, 12  extra sum = 0
	// as if I have 4 batteries each of which can run 12 minutes.
	//
	// actual execution:
	// 10, 8, 7, 5, | 5, 4, 3, 3, 3
	// 1 minute
	// 9, 7, 6, 4  |  5, 4, 3, 3, 3
	// swap
	// 9, 7, 6, 5 | 4, 4, 3, 3, 3
	// 2 minutes
	// 7, 5, 4, 3 | 4, 4, 3, 3, 3
	// swap
	// 7, 5, 4, 4, | 4, 3, 3, 3, 3
	// 1 minute
	// 6, 4, 3, 3, | 4, 3, 3, 3, 3
	// swap
	// 6, 4, 4, 3  | 3, 3, 3, 3, 3
	// 1 minute
	// 5, 3, 3, 2  | 3, 3, 3, 3, 3
	// swap
	// 5, 3, 3, 3  | 3, 3, 3, 3, 2
	// 1 minute
	// 4, 2, 2, 2  | 3, 3, 3, 3, 2
	// swap
	// 4, 3, 3, 3  | 3, 2, 2, 2, 2
	// 1 minute
	// 3, 2, 2, 2  | 3, 2, 2, 2, 2
	// swap
	// 3, 3, 2, 2  | 2, 2, 2, 2, 2
	// 1 minute
	// 2, 2, 1, 1  | 2, 2, 2, 2, 2
	// swap
	// 2, 2, 2, 2  | 2, 2, 2, 1, 1
	// 1 minute
	// 1, 1, 1, 1  | 2, 2, 2, 1, 1
	// swap
	// 2, 2, 2, 1  | 1, 1, 1, 1, 1
	// 1 minute
	// 1, 1, 1, 0  | 1, 1, 1, 1, 1
	// swap
	// 1  1, 1, 1  | 1, 1, 1, 1, 0
	// 1 minute
	// 0, 0, 0, 0  | 1, 1, 1, 1, 0
	// swap
	// 1, 1, 1, 1  | 0, 0, 0, 0, 0
	// 1 minute
	// 0, 0, 0, 0  | 0, 0, 0, 0, 0





	// Does this strategy depends on whether the extra batteries are distributed or concentrated?

	// for example: 5 computers:  10, 9, 6, 6, 6,   extra battery sum = 4

	// Are these two cases different?

    // 10, 9, 6, 6, 6  |  4
	// 10, 9, 6, 6, 6  |  1, 1, 1, 1


	// 10, 9, 6, 6, 6  | 4
	// 7, 6, 3, 3, 3   | 4      3 minutes
	// 7, 6, 4, 3, 3   | 3      swap
	// 6, 5, 3, 2, 2   | 3      1 minute
	// 6, 5, 3, 3, 2   | 2      swap
	// 5, 4, 2, 2, 1   | 2      1 minute
	// 5, 4, 2, 2, 2   | 1      swap
	// 3, 2, 0, 0, 0   | 1      2 minute
	//                          7 minutes total

	// 10, 9, 6, 6, 6  | 1, 1, 1, 1
	// 4, 3, 0, 0, 0, 0 | 1, 1, 1, 1  6 minutes
	// 4, 3, 1, 1, 1, 1 | 0, 0, 0, 0  swap
	// 3, 2, 0, 0, 0, 0 | 0, 0, 0, 0  1 minute
	//                                7 total minutes

	/*
	* Obviously, I can't just add up all batteries and divide them by n.
	* For example, 2 computers, 3 batteries: [6, 1, 1].
	* Apparently I can only run both computers simultaneously for 2 minutes and
	* I have to leave a battery with 4-minute worth of power.
	*/

public:

	long long maxRunTime(int n, vector<int>& b)   // b: batteries
	{
		sort(b.begin(), b.end(), greater<int>{});
		
		if (n == b.size())
			return *(b.rbegin());

		int ebs = 0; // extra batteries sum
		ebs = accumulate((b.begin() + n), b.end(), 0);

		int clb = b[n-1]; // life time of current lowest battery

		for (int i = n-1; i >= 0; i--)
		{
			if (b[i] > clb)
			{
				// can I bring the batteries to the right of 'i' to the same value as b[i]?
				int lowBatteryCount = n - 1 - i;
				int diff = b[i] - clb;
				if (ebs >= lowBatteryCount * diff)
				{
					ebs -= lowBatteryCount * diff;
					clb = b[i];
					if (ebs == 0)
						return clb;
				}
				else
				{
					// I can't bring all the low batteries to the same level as b[i]
					// what is the best I can do?
					if (ebs >= lowBatteryCount)
					{
						return clb + (ebs / lowBatteryCount);
					}
					else
						return clb;
				}
			}
		}
		// out of the loop, supposedly, every battery is brought to the same level as b[0]
		// but can I do even better?
		if (ebs >= n)
		{
			return clb + (ebs / n);
		}
		return clb;
	}
};


void Test_2141_Maximum_Running_Time_of_N_Computers()
{
	Solution s;
	vector<int>batteries;
	int n;
	while (true)
	{
		string s1;
		cout << "batteries: ";
		getline(cin, s1);
		if (s1.size() == 0) break;
		stringstream ss(s1);
		batteries.clear();
		copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(batteries));
		cout << "computers: ";
		cin >> n;
		cin.ignore();
		cout << s.maxRunTime(n, batteries) << '\n';
	}
}

}