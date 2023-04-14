#include<iostream>
#include<string>
#include<sstream>
#include<vector>

namespace _0915_Partition_Array_into_Disjoint_Intervals {

	using namespace std;

	class Solution
	{
	public:

		int partitionDisjoint(vector<int>& nums)
		{
			size_t len = nums.size();
			if (len == 2)
				return 1;  // guaranteed a valid answer

			size_t potentialLeftEnd = 0;  // index of the end of left
			int maxOfLeft = nums[0];
			int maxSinceLeftEnd = nums[1];
			for (size_t i = 1; i < len; i++)
			{
				if (nums[i] >= maxOfLeft)
				{
					if (maxSinceLeftEnd < nums[i])
					{
						maxSinceLeftEnd = nums[i];
					}
				}
				else
				{
					potentialLeftEnd = i;
					if (maxOfLeft < maxSinceLeftEnd)
						maxOfLeft = maxSinceLeftEnd;
					if (i < len - 1)
						maxSinceLeftEnd = nums[i + 1];

				}
			}
			return (int)potentialLeftEnd + 1;
		}
	};



	void Test_0915_Partition_Array_into_Disjoint_Intervals()
	{
		vector<int> nums;
		string numsInString;
		int test;
		char p[] = "abc";
		Solution s;
		while (true)
		{
			cout << "test?";
			cin >> test;
			if (test == 0)break;
			nums.clear();
			numsInString.clear();
			cin.getline(p, 1);
			cout << "array: ";
			getline(cin, numsInString);
			istringstream ss(numsInString);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << s.partitionDisjoint(nums) << '\n';
		}
	}

}