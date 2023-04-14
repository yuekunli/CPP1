#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<unordered_map>
#include<numeric>
#include<algorithm>

namespace _0805_Split_Array_With_Same_Average {

	using namespace std;

	class Solution // this solution works, but it's slow, the bottleneck is running the backtrack part too many times.
	{

		/*
		(1). find the overall average, for example, it's 4 + 10/15
		(2). find the greatest common divisor(gcd) between 10 and 15, it's 5
		(3). divide both 10 and 15 by that gcd, I get 2 and 3
		(4). so the overall average is really 4 + 2/3
		(5). If it can be divided into two and both have 4 + 2/3 as their average
		     The average must be exact, it can't be fractional with small difference
	    (6). If one set can have exact same average, the other set must have the exact average too
		(7). the question is now converted to whether I can find a subset with 3 elements that sum up to 14
		     or whether I can find a subset with 6 elements that sum up to 28
			 or whether I can find a subset with 9 elements that sum up to 42
			 ...
		(8). Use back track to solve this converted problem
		*/

		/*
		* Some additional explanation:
		* If the input has 42 numbers, and assume the average is 15/7.
		* Imagine all 42 numbers are 15/7. I'm now splitting these 42 numbers into two groups.
		* For example, if I put 10 of them in one group and 32 in the other, the sum of the 1st
		* group would be (15/7 * 10) which is a fractional number. But in reality, each group's
		* sum must be integer. So I can only divide them into group of 7 and 35 or 14 and 28
		* or 21 and 21.
		* Why can I imagine every number is 15/7? because it demands that each group has the same
		* average. And if I put two groups together the combined average is still the same.
		* So if I do the reverse, to split a big group into two smaller ones and both smaller
		* groups have the same average, the original big group must have the same average.
		*/

	public:

		bool sumWithCount_Recur(vector<int>& nums, int targetSum, int currentSum, size_t targetCount, size_t currentCount, size_t index)
		{
			if (currentSum == targetSum && currentCount == targetCount)
				return true;

			if (currentSum != targetSum && currentCount == targetCount)
				return false;

			if (index >= nums.size())
				return false;

			if (sumWithCount_Recur(nums, targetSum, currentSum+nums[index], targetCount, currentCount+1, index+1))  // take this number
				return true;
			else
				return sumWithCount_Recur(nums, targetSum, currentSum, targetCount, currentCount, index+1);  // skip this number
		}

		bool sumWithCount(vector<int>& nums, int sum, size_t count)
		{
			size_t len = nums.size();
			if (count > len / 2)
			{
				int sumTotal = accumulate(nums.begin(), nums.end(), 0);
				return sumWithCount(nums, sumTotal - sum, len - count);
			}
			return sumWithCount_Recur(nums, sum, 0/*currentSum*/, count, 0/*currentCount*/, 0/*index*/);
		}

		int euclid(int a, int b)
		{
			if (b == 0)
				return a;
			else
				return euclid(b, a % b);
		}

		bool splitArraySameAverage(vector<int>& nums)
		{
			int len = (int)nums.size();
			int sumTotal = accumulate(nums.begin(), nums.end(), 0);
			int quo = sumTotal / len;
			int rem = sumTotal % len;

			/*
			if sumTotal is divisible by len for example 24 / 8 = 3
			quo = 3, rem = 0, len=8
			euclid will return 8 as gcd
			after dividing by gcd:
			rem = 0
			len = 1
			*/

			int gcd = euclid(rem, len);
			if (gcd > 1)
			{
				rem /= gcd;
				len /= gcd;
			}

			for (int i = 1; i * len <= nums.size()/2; i++)  //
			{
				/*
				* need to be non-empty, so i starts from 1.
				* when i*len is greater than half of size, I don't need to check again.
				* if size is 42 and len is 7, when I check group of 7, it's the same as checking group of 35.
				* The array can be equally divided, for example 42 --> 21 + 21. So I need "less than or equal to"
				*/

				/*
				 * example: quo=4, rem=2, len=3, overall average 4+(2/3) = 14/3
				 * i=1: find 3-element set, sum is 14;  set size: 3*1, sum: (4*3+2)*1
				 * i=2: find 6-element set, sum is 28;  set size: 3*2, sum: (4*3+2)*2
				 */
				if (sumWithCount(nums, (quo * len + rem) * i, len * i))
					return true;
			}
			return false;
		}
	};



	class Solution2
	{
		// n: numerator, d: denominator
		// compare n1/d1 and n2/d2
		int compare(int n1, int d1, int n2, int d2)
		{
			if (n1 * d2 < n2 * d1)
				return -1;
			else if (n1 * d2 == n2 * d1)
				return 0;
			else
				return 1;
		}

		int euclid(int a, int b)
		{
			if (b == 0)
				return a;
			else
				return euclid(b, a % b);
		}

		int inputLength;  // len is used in calculating average, using int is more convenient

		bool backtrackSolve(vector<int>&nums, int numeratorOfOverallAverage, int denominatorOfOverallAverage, int currentNumerator, int currentDenominator, int index)
		{
			if (index == inputLength)
				return false;

			if (currentDenominator > inputLength / 2)
				return false;

			int compareResult = compare(numeratorOfOverallAverage, denominatorOfOverallAverage, currentNumerator + nums[index], currentDenominator + 1);

			if (compareResult == 0)
			{
				// I found a way to make a subarray's average equal to overall average
				// but make sure this subarray is not just the whole array
				if (currentDenominator + 1 == inputLength)
					return false;
				return true;
			}
			else if (compareResult < 0)
				return false;
			else
			{
				if (backtrackSolve(nums, numeratorOfOverallAverage, denominatorOfOverallAverage, currentNumerator + nums[index], currentDenominator + 1, index + 1))
					return true;
				else
					return backtrackSolve(nums, numeratorOfOverallAverage, denominatorOfOverallAverage, currentNumerator, currentDenominator, index+1);
			}
		}

	public:

		bool splitArraySameAverage(vector<int>& nums)
		{
			int denominator = inputLength = (int)nums.size();

			if (inputLength <= 1)
				return false;

			int sumTotal = accumulate(nums.begin(), nums.end(), 0);
			int quo = sumTotal / denominator;
			int rem = sumTotal % denominator;

			/*
			if sumTotal is divisible by len for example 24 / 8 = 3
			quo = 3, rem = 0, len=8
			euclid will return 8 as gcd
			after dividing by gcd:
			rem = 0
			len = 1
			*/

			int gcd = euclid(rem, denominator);
			if (gcd > 1)
			{
				rem /= gcd;
				denominator /= gcd;
			}

			sort(nums.begin(), nums.end());

			return backtrackSolve(nums, quo * denominator + rem, denominator, 0, 0, 0);
		}
	};



	class Solution3
	{
		// n: numerator, d: denominator
		// compare n1/d1 and n2/d2
		int compare(int n1, int d1, int n2, int d2)
		{
			if (n1 * d2 < n2 * d1)
				return -1;
			else if (n1 * d2 == n2 * d1)
				return 0;
			else
				return 1;
		}

		int euclid(int a, int b)
		{
			if (b == 0)
				return a;
			else
				return euclid(b, a % b);
		}

		size_t inputLength;  // is used in calculating average, using int is more convenient
		int numeratorOfOverallAverage;
		int denominatorOfOverallAverage;

		bool backtrackSolve(unordered_map<int, vector<vector<signed char>>>&m, vector<int>& nums, int currentNumerator, int currentDenominator, size_t index)
		{
			if (index == inputLength)
				return false;

			if (currentDenominator > inputLength / 2)
				return false;

			if (m.count(currentNumerator) != 0)
			{
				if (m[currentNumerator][currentDenominator][index] != -1)
					return false;
			}

			int compareResult = compare(numeratorOfOverallAverage, denominatorOfOverallAverage, currentNumerator + nums[index], currentDenominator + 1);

			if (compareResult == 0)
			{
				// I found a way to make a subarray's average equal to overall average
				// but make sure this subarray is not just the whole array
				if (currentDenominator + 1 == inputLength)
					goto RETURN_FALSE;

				return true;
			}
			else if (compareResult < 0)
				goto RETURN_FALSE;
			else
			{
				bool result = backtrackSolve(m, nums, currentNumerator + nums[index], currentDenominator + 1, index + 1);
				if (result)
					return true;
				else
				{
					result = backtrackSolve(m, nums, currentNumerator, currentDenominator, index + 1);
					if (result)
						return true;
					else
						goto RETURN_FALSE;
				}
			}

		RETURN_FALSE:
			if (m.count(currentNumerator) > 0)
			{
				m[currentNumerator][currentDenominator][index] = 0;
			}
			else
			{
				m[currentNumerator] = vector<vector<signed char>>(inputLength/2+1, vector<signed char>(inputLength, -1));
				m[currentNumerator][currentDenominator][index] = 0;	
			}
			return false;
		}

	public:

		bool splitArraySameAverage(vector<int>& nums)
		{
			inputLength = nums.size();
			int denominator = (int)inputLength;
			if (inputLength <= 1)
				return false;

			int sumTotal = accumulate(nums.begin(), nums.end(), 0);
			int quo = sumTotal / denominator;
			int rem = sumTotal % denominator;

			/*
			if sumTotal is divisible by len for example 24 / 8 = 3
			quo = 3, rem = 0, len=8
			euclid will return 8 as gcd
			after dividing by gcd:
			rem = 0
			len = 1
			*/

			int gcd = euclid(rem, denominator);
			if (gcd > 1)
			{
				rem /= gcd;
				denominator /= gcd;
			}

			sort(nums.begin(), nums.end());

			numeratorOfOverallAverage = quo * denominator + rem;
			denominatorOfOverallAverage = denominator;

			unordered_map<int, vector<vector<signed char>>> m;
			return backtrackSolve(m, nums, 0, 0, 0);
		}
	};



	/*
	Test these:

	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30

	60 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30

	41 8467 6334 6500 9169 5724 1478 9358 6962 4464 5705 8145 3281 6827 9961 491 2995 1942 4827 5436 2391 4604 3902 153 292 2382 7421 8716 9718 9895
	
	*/


	void Test_0805_Split_Array_With_Same_Average()
	{
		vector<int> nums;
		string arrayInString;
		Solution3 s;

		while (true)
		{
			arrayInString.clear();
			cout << "Input Array: ";
			getline(cin, arrayInString);
			if (arrayInString.size() == 0) break;
			nums.clear();
			std::istringstream ss(arrayInString);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << boolalpha << s.splitArraySameAverage(nums) << '\n';
		}
	}
}