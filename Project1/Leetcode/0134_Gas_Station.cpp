#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<numeric>

namespace _0134_Gas_Station {

	using namespace std;

	class Solution
	{
	public:
		int canCompleteCircuit(vector<int>& gas, vector<int>& cost)
		{
			size_t len = gas.size();
			vector<int>supplyConsumeDiff(len);

			for (size_t i = 0; i < len; i++)
			{
				supplyConsumeDiff[i] = gas[i] - cost[i];
			}
			int sum = accumulate(supplyConsumeDiff.begin(), supplyConsumeDiff.end(), 0);
			if (sum < 0)
				return -1;
			
			for (size_t i = 1; i < len; i++)
			{
				supplyConsumeDiff[i] = supplyConsumeDiff[i - 1] + supplyConsumeDiff[i]; // why does this rolling sum method work?
			}

			auto it = min_element(supplyConsumeDiff.begin(), supplyConsumeDiff.end());
			if (it != supplyConsumeDiff.end() - 1)
				return (int)distance(supplyConsumeDiff.begin(), it) + 1;
			else
				return 0;
		}
	};


	/*
	* A    B    C     D     E     F     G
	* 
	* I try to start my journey from station A. I get to E, pump gas at E, while I'm on my way to F, I run out gas.
	* The accumulation of gas when I arrive at E (before pumping gas at E) is always positive. It's impossible that the gas
	* tank dropped below zero when I'm at B or C or D. Obviously, I can't start at A, note that I can just try starting at F.
	* I don't need to try starting at B, C, D, or E. Why?
	* Because the gas accumulation is always positive or at least zero. If I try starting at D, I don't even have the accumulation
	* from A to D. I can't get pass F even when I have the accumulation from A to D, let alone not having it.
	* So if I start at A and run out of gas between E and F, none of the stations between A and E (inclusively) is viable.
	* This is why when I have loopedback and if I still don't have a viable option, there is no solution.
	*/


	class Solution2 // this is accepted by leetcode. And this is also O(N)
	{
	public:
		int canCompleteCircuit(vector<int>& gas, vector<int>& cost)
		{
			size_t len = gas.size();
			vector<int>supplyConsumeDiff(len);
			// supplyConsumeDiff[x] means the net effect of adding gas at station x and spending gas traveling to station x+1
			// but not adding gas at station x+1. Basically if I start at station x. supplyConsumeDiff[x] + supplyConsumeDiff[x+1] +
			// supplyConsumeDiff[x+2] + ... + supplyConsumeDiff[y] means the gas I have in my tank after arriving at station y+1
			// but not adding gas at station y+1;

			for (size_t i = 0; i < len; i++)
			{
				supplyConsumeDiff[i] = gas[i] - cost[i];
			}
			int sum = accumulate(supplyConsumeDiff.begin(), supplyConsumeDiff.end(), 0);
			if (sum < 0)
				return -1;

			size_t start = 0;
			bool loopedback = false;
			while (true)
			{
				int total = 0;
				size_t i = start;

				while (true)
				{
					total += supplyConsumeDiff[i];
					if (total < 0)
					{
						if (i < len - 1)
						{
							if (loopedback)
								return -1;
							else
							    start = i + 1;
						}
						break;
					}
					else
					{
						if (i < len - 1)
							i++;
						else
						{
							i = 0;
							loopedback = true;
						}
					}
					if (i == start)
						return (int)start;
				}
			}
		}
	};


	void Test_0134_Gas_Station()
	{
		Solution2 s;
		string s1;
		vector<int>gas;
		vector<int>cost;
		vector<int>tmp;
		while (true)
		{
			s1.clear();
			cout << "gas and cost: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			tmp.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(tmp));
			size_t sz = tmp.size();
			sz /= 2;
			gas.assign(sz, 0);
			cost.assign(sz, 0);
			for (size_t i = 0; i < sz; i++)
			{
				gas[i] = tmp[i];
				cost[i] = tmp[i + sz];
			}
			cout << s.canCompleteCircuit(gas, cost) << "\n\n";
		}
	}
}