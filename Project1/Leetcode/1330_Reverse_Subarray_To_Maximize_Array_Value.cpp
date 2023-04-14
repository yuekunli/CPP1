#include "LibHeaders.h"

namespace _1330_Reverse_Subarray_To_Maximize_Array_Value {

using namespace std;

class Solution
{
	/*
	* if the input array is:  a  b  c  d  e  f  g  h  i  j  k  l  m  n  p  q
	*                                       |              |
	* If I decide to reverse [f,j].
	* The only things that matter are the values of 'e', 'f', 'j', and 'k'.
	* The order of these 4 numbers on x-axis has a few different scenarios, but
	* if I consider |e-f| + |j-k|  and |e-j| + |f-k|, there aren't too many scenarios
	* basically if they are aligned like this:
	* e  f  j  k
	*   (|e-j| + |f-k|) - (|e-f| + |j-k|) = 2 * distance between f and j
	* find the bigger number of every pair, find the smaller number of every pair.
	* among all the bigger numbers, find the smallest. among all the smaller numbers, find the biggest.
	* This is equivalent to saying find the smallest 'f' and the biggest 'j' so that
	* 2 * distance between f and j becomes biggest.
	*/

public:

	int maxValueAfterReverse(vector<int>& nums)
	{
		if (nums.size() == 1)
			return abs(nums[0]);

		if (nums.size() == 2)
			return abs(nums[0] - nums[1]);

		int currentValue = inner_product(nums.begin(), (nums.end() - 1), (nums.begin() + 1), 0, plus<int>{}, [](int& a, int& b)
		{
			return abs(a - b);
		});

		int maxDiffIfReverseWithLeftEnd = 0;
		int maxDiffIfReverseWithRightEnd = 0;
		int lowestRightEnd = max(nums[0], nums[1]);
		int highestLeftEnd = min(nums[0], nums[1]);

		for (int i = 1; i <= nums.size() - 2; i++)
		{
			int diffIfReverseWithLeftEnd = abs(nums[0] - nums[i + 1]) - abs(nums[i] - nums[i + 1]);
			if (diffIfReverseWithLeftEnd > maxDiffIfReverseWithLeftEnd)
				maxDiffIfReverseWithLeftEnd = diffIfReverseWithLeftEnd;

			int diffIfReverseWithRightEnd = abs(nums[i-1] - *(nums.end()-1)) - abs(nums[i-1] - nums[i]);
			if (diffIfReverseWithRightEnd > maxDiffIfReverseWithRightEnd)
				maxDiffIfReverseWithRightEnd = diffIfReverseWithRightEnd;

			int rightEndofThisInterval = max(nums[i], nums[i + 1]);
			int leftEndofThisInterval = min(nums[i], nums[i + 1]);

			if (rightEndofThisInterval < lowestRightEnd)
				lowestRightEnd = rightEndofThisInterval;

			if (leftEndofThisInterval > highestLeftEnd)
				highestLeftEnd = leftEndofThisInterval;
		}

		int diffByReverseMiddle = 0;
		if (lowestRightEnd < highestLeftEnd)
			diffByReverseMiddle = (highestLeftEnd - lowestRightEnd) * 2;

		return currentValue + max({ maxDiffIfReverseWithLeftEnd, maxDiffIfReverseWithRightEnd, diffByReverseMiddle });
	}
};


/*
* Test cases:
*   input                 answer
*   2 3 1 5 4             10      2 5 1 3 4
*   2 4 9 24 2 1 10       68      2 4 9 2 24 1 10
*   2 -3 -1 -5 -4         17
*   2 -3 1 -5 4           27
*   5 10 7 15             21      5 15 10 7
*/
void Test_1330_Reverse_Subarray_To_Maximize_Array_Value()
{
	Solution s;

	while (true)
	{
		string s1;
		vector<int> nums;
		cout << "nums: ";
		getline(cin, s1);
		if (s1.size() == 0) break;
		stringstream ss(s1);
		copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
		cout << s.maxValueAfterReverse(nums) << '\n';
	}
}
}