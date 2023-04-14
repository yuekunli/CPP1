#include "LibHeaders.h"

namespace _1354_Construct_Target_Array_With_Multiple_Sums {

	using namespace std;

	class Solution
	{
	public:

		bool isPossible(vector<int>& target)
		{
			bool continueTest = false;
			for (auto& i : target)
			{
				if (i <= 0)
					return false;
				if (i > 1)
				{
					continueTest = true;
					break;
				}
			}

			if (!continueTest)
				return true;

			int sum = accumulate(target.begin(), target.end(), 0);
			auto maxi = max_element(target.begin(), target.end());
			int sumExcludeMax = sum - *maxi;
			int valueOfMaxiBefore = *maxi - sumExcludeMax;
			vector<int> v;
			copy(target.begin(), target.end(), back_inserter(v));
			v[(maxi - target.begin())] = valueOfMaxiBefore;
			return (isPossible(v));   // tail recursive
		}
	};

	class Solution2
	{
	public:

		bool isPossible(vector<int>& target)
		{
			int sum = accumulate(target.begin(), target.end(), 0);

			make_heap(target.begin(), target.end()); // create max heap, but the comp function it uses by default is the "less than" function

			while (target.size()>0)
			{
				if (target[0] <= 0)
					return false;

				if (target[0] == 1)
				{
					break;
				}

				int sumExcludeMax = sum - target[0];
			
				int valueofMaxBefore = target[0] - sumExcludeMax;

				if (valueofMaxBefore <= 0)
					return false;

				sum = target[0]; // what was the sum before we put that sum in the cell where current max is
			
				pop_heap(target.begin(), target.end());
			
				*(target.rbegin()) = valueofMaxBefore;
			
				push_heap(target.begin(), target.end());
			}

			if (find_if(target.begin(), target.end(), [](int& a) { return a != 1;}) != target.end())
				return false;

			return true;
		}
	};


	void Test_1354_Construct_Target_Array_With_Multiple_Sums()
	{
		Solution2 s;
		string s1;
		vector<int> target;
		while (true)
		{
			s1.clear();
			cout << "target: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			target.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(target));
			cout << boolalpha << s.isPossible(target) << '\n';
		}
	}
}