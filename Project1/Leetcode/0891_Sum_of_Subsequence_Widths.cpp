#include "LibHeaders.h"

namespace _0891_Sum_of_Subsequence_Widths {

	using namespace std;

	class Solution
	{
		/*
		* If the sorted array is something like this:  a, b, c, d, e, f, g
		* Let's say I take a 3-element subsequence from the sorted array, I get "adg".
		* These 3 elements must be in the original input in *some* order. If I take
		* all possible subsequences from the original input, this sequence "adg"
		* will be picked out sooner or later. Since I'm just considering the max
		* and min in a subsequence, so considering subsequences in the sorted array
		* is just as good as considering subsequences in the original input.
		* Now the sorted array is this: a, b, c, d, e, f, g
		*                                  |           |
		* If I pick 'b' and 'f' as the min and max of a subsequence, how many possible
		* subsequences can I form? Basically pick elements in between 'b' and 'f'.
		* How many ways to pick? If there are 3 elements in between, then I have 2^3 ways
		* to pick. All these subsequences have the same width.
		* So fix the min, fix the max (to the right of min), count the elements in between
		* (for example there are k elements in between) the same width will repeat 2^k times.
		*/
	public:
		int sumSubseqWidths(vector<int>& nums)
		{
			int base = 1'000'000'007;
			sort(nums.begin(), nums.end());
			int len = nums.size();
			int s = 0;
			for (int L = 1; L <= len - 1; L++) // number of hops from segment start to segment end, 1-element subsequence has 0 hop, but 1-element subsequence's width=0
			{
				for (int i = 0; i <= len - L - 1; i++)   // len = 9;   0,1,2,3,4,5,6,7,8  if L=3, the maximum of i is 5
				{
					int j = i + L;
					int width = nums[j] - nums[i];
					if (width > 0)
					{
						int between = L - 1;
						int count = pow(2, between);
						s += (width * count);
						s %= base;
					}
				}
			}
			return s;
		}
	};

	void Test_0891_Sum_of_Subsequence_Widths()
	{
		vector<int> nums;
		string numsInString;
		Solution s;
		while (true)
		{
			nums.clear();
			numsInString.clear();
			cout << "array:";
			getline(cin, numsInString);
			if (numsInString.size() == 0) break;
			istringstream ss(numsInString);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << s.sumSubseqWidths(nums) << '\n';
		}
	}
}