#include "LibHeaders.h"

namespace _1862_Sum_of_Floored_Pairs {

	using namespace std;

	class Solution
	{
	
	public:

		int sumOfFlooredPairs(vector<int>& nums)
		{
			int len = nums.size();
			
			sort(nums.begin(), nums.end());
			int total = 0;
			for (int i = 0; i < len; i++)
			{
				int multiple = 1;
				while (true)
				{
					int upperLimit = nums[i] * (multiple+1);
					int lowerLimit = nums[i] * multiple;
					auto itHi = lower_bound(nums.begin(), nums.end(), upperLimit);
					auto itLo = lower_bound(nums.begin(), nums.end(), lowerLimit);
					bool jumpOut = false;

					// if one segment is missing, itHi and itLo can both point to end()
					// or the starting number in the next segment, if both are out, I break out.
					// If both are point to the next segment, I increment multiple and continue.
					// but it's also possible that this is the last segment so itLo is inside and itHi is out
					// in that case, I should finish the calculation for this segment then break out.
					// for example I'm examing '7',        ..... 27, 36, 38, ....
					// multiple is '4', entire [28, 34] segment is missing, itHi and itLo
					// will both point to "36"

					if (itHi == nums.end() && itLo == nums.end())
						break;
					else if (itHi == nums.end())
						jumpOut = true;
					else if (*itHi >= upperLimit && *itLo >= upperLimit)
					{
						multiple++;
						continue;
					}
						
					itHi = itHi - 1;
					int count = itHi - itLo + 1;
					total += count * multiple;
					if (jumpOut)
						break;
					else
						multiple++;
				}
			}
			return total;
		}
	};

	void Test_1862_Sum_of_Floored_Pairs()
	{
		Solution s;
		string s1;
		vector<int> nums;
		while (true)
		{
			s1.clear();
			cout << "nums: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			nums.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << s.sumOfFlooredPairs(nums) << '\n';
		}
	}
}