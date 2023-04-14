#include "LibHeaders.h"

namespace _1879_Minimum_XOR_Sum_Two_Arrays {
	using namespace std;

	class Solution
	{
		/*
		* nums1 is fixed, nums2 can be re-arranged.
		* examine numbers in nums1 one by one, for each number under examination, I need to pick a number
		* from nums2 (among the numbers that are still available) to pair with the number in nums1.
		* The state of the execution/progress is determined by two factors:
		* (1) which number in nums1 is under examination
		* (2) which numbers in nums2 are still available
		* The 2nd dimension is represented by a bit mask, constraints: 1 <= n <= 14.
		* O(2^14) different situations for the 2nd dimension, that is a lot memory.
		* I try to use a hash map to store the 2nd dimension information in hope to save some memory,
		* but it probably won't achieve so, because every possible bitmask is going to be
		* tested as I recursively go deeper and deeper. Eventually I'll have just 1 number left available
		* in nums2 and the last number in nums1 is under examination. The only number left available in
		* nums2 is also the last one in nums2. So the bitmask is going to look like: 1 1 1 . . . 1 1 1 0
		* Every possible bitmask needs an entry in the hash map so maybe using a 2-D array is more
		* memory efficient after all.
		*/

		int maskMax;
		int n;
		int solve(unordered_map<int, vector<int>>& r, vector<int>&nums1, vector<int>&nums2, int nums1Index, int nums2Mask)
		{
			if (nums1Index == n)
				return 0;
			if (r.count(nums2Mask) == 1)
			{
				if (r[nums2Mask][nums1Index] != -1)
					return r[nums2Mask][nums1Index];
			}

			int minimumSum = numeric_limits<int>::max();
			int base = 1 << (n - 1);
			int oldNums2Mask = nums2Mask;
			for (int i = 0; i < n; i++)
			{
				if (!(nums2Mask & (base >> i)))
				{
					int resultForCurrentNumber = nums1[nums1Index] ^ nums2[i];
					nums2Mask |= (base >> i);
					int resultForTheRest = solve(r, nums1, nums2, nums1Index + 1, nums2Mask);
					if (minimumSum > resultForCurrentNumber + resultForTheRest)
						minimumSum = resultForCurrentNumber + resultForTheRest;
					nums2Mask = oldNums2Mask;
				}
			}
			if (r.count(nums2Mask) == 0)
			{
				r[nums2Mask] = vector<int>(n, -1);
			}
			r[nums2Mask][nums1Index] = minimumSum;
			return minimumSum;
		}
	public:
		int minimumXORSum(vector<int>& nums1, vector<int>& nums2)
		{
			unordered_map<int, vector<int>> recordbook;
			n = nums1.size();
			maskMax = 1 << n;
			return solve(recordbook, nums1, nums2, 0/*index in nums1, which number is under consideration*/, 0/*mask in nums1, every number is available*/);
		}
	};

	void Test_1879_Minimum_XOR_Sum_Two_Arrays()
	{
		Solution s;
		string s1;
		vector<int>nums1;
		vector<int>nums2;
		vector<int>tmp;
		while (true)
		{
			s1.clear();
			cout << "two arrays: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			tmp.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(tmp));
			int sz = tmp.size() / 2;
			nums1.assign(sz, 0);
			nums2.assign(sz, 0);
			copy(tmp.begin(), tmp.begin() + sz, nums1.begin());
			copy(tmp.begin() + sz, tmp.end(), nums2.begin());
			cout << s.minimumXORSum(nums1, nums2) << "\n\n";
		}
	}
}