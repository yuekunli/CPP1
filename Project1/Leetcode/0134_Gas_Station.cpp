#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<numeric>
#pragma warning(disable:4244 4267)

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
				return distance(supplyConsumeDiff.begin(), it) + 1;
			else
				return 0;

		}
	};

	class Solution2 // this is accepted by leetcode.
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
						return start;
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