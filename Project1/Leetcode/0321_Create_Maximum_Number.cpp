#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#pragma warning(disable: 4267)
namespace _0321_Create_Maximum_Number {

	using namespace std;

	class Solution
	{
		/*
		* The value of this Solution is mainly to demonstrate how to create multi-dimensional array
		*/
	public:

		/*
		* This solution uses dynamic programming.
		* A 3-dimension table.
		* A cell in the table [i][j][p] means considering the subarray of nums1 from i to last
		* and the subarray of nums2 from j to last, what is the maximum number with p digits
		* can be produced by these two subarrays.
		* 
		* The idea is that if I want to find a 5-digit number, let me first focus on finding the
		* biggest number and use it as the 1st digit (counting from left) and then I need to find
		* the biggest 4-digit number the rest of the candidates can produce.
		*/

		/* for any one cell, I need to distinguish 3 cases: (1) never visited this cell
		* (2) visited but can't produce valid answer, (3) the valid answer is series of zeros
		* 
		* example: n1: ....... 7 ......
		* example: n2: ...7............
		* assume '7' is the max, does it matter which 7 I use first?
		* -- It doesn't matter if I have enough numbers behind these two 7's 
		* if I want a 3-digit number, no matter which 7 I use first,
		* I'm going to have two 7's as the first 2 digits, 
		* and I'll have the rest of each array produce one more digit
		* -- It does matter if I don't have enough numbers behind these two 7's
		* for example nums1: [3,9]
		*             nums2: [8,9], k = 3;
		* 
		* There are two 9's that are the biggest, but there is none left behind these two 9's.
		* If I take these two 9's as the first two digits, I won't have a 3rd digit.
		* And it's different as to which 9 I take first.
		* 
		* Example: n1: [6,7]
		*          n2: [6,8,4]  k = 5
		* 
		* I can't abandon any number. So my first choice is among the two 6's. And there is a difference
		* as to which 6 I pick first. I do have 3 behind these two 6's. They are enough to make up 3 digits
		* But because '6' is not the current greatest, so even if the rest numbers behind these two 6's
		* are enough, the results may be different as to which 6 I pick first.
		*/
		size_t len1;
		size_t len2;
		vector<int> maxN_Recur(vector<int>& n1, vector<int>& n2, int k, int idx1, int idx2, vector<int>*** t)
		{
			if (t[idx1][idx2][k][0] != -2)  // -2 means never visited    there are 4 index operators, but the table is 3-deminsion, each cell has a vector, hence 4 index operators
				return t[idx1][idx2][k];

			if (len1 - idx1 + len2 - idx2 < k)  // not enough digits to choose
			{
				t[idx1][idx2][k][0] = -1;
				return t[idx1][idx2][k];
			} // if both arrays are exhausted, it shouldn't pass this check

			//vector<int>const* answerForTheRest = NULL;
			vector<int>answerForTheRest;
			int maxLimit = 10;
			int maxIdx1 = -1;
			int maxIdx2 = -1;
			int max1 = -1;
			int max2 = -1;

			while (true)
			{
				// what if one of the two arrays is already exhausted
				for (int i = idx1; i < len1; i++)
				{
					if (n1[i] > max1 && n1[i] < maxLimit)
					{
						max1 = n1[i];
						maxIdx1 = i;
					}
				}
				for (int i = idx2; i < len2; i++)
				{
					if (n2[i] > max2 && n2[i] < maxLimit)
					{
						max2 = n2[i];
						maxIdx2 = i;
					}
				}

				// k == 1 case
				if (k == 1)
				{
					t[idx1][idx2][k][0] = max({max1, max2});
					return t[idx1][idx2][k];
				}

				if (max1 > max2)
				{
					// should try using n1's max first
					vector<int>const& a = maxN_Recur(n1, n2, k - 1, maxIdx1 + 1, idx2, t);
					if (a[0] != -1)
					{
						//answerForTheRest = &a;  // this won't work, a's life time is limited in this "if" block
													// answerForTheRest pointer will be valid, but object is destructed.
						answerForTheRest = a;
						break;
					}
					else
					{
						maxLimit = max1;
						max1 = -1;
						max2 = -1;
						maxIdx1 = -1;
						maxIdx2 = -1;
						continue;
					}
				}
				else if (max1 < max2)
				{
					// should try using n2's max first
					vector<int>const& a = maxN_Recur(n1, n2, k - 1, idx1, maxIdx2 + 1, t);
					if (a[0] != -1)
					{
						answerForTheRest = a;
						break;
					}
					else
					{
						maxLimit = max2;
						max1 = -1;
						max2 = -1;
						maxIdx1 = -1;
						maxIdx2 = -1;
						continue;
					}
				}
				else  // max1 == max2   try both
				{
					vector<int>const& a = maxN_Recur(n1, n2, k - 1, maxIdx1 + 1, idx2, t);
					vector<int>const& b = maxN_Recur(n1, n2, k - 1, idx1, maxIdx2 + 1, t);
					if (a[0] == -1 && b[0] != -1)
					{
						answerForTheRest = b;
						break;
					}
					if (a[0] != -1 && b[0] == -1)
					{
						answerForTheRest = a;
						break;
					}
					if (a[0] != -1 && b[0] != -1)
					{
						for (int i = 0; i < k - 1; i++)
						{
							if (a[i] > b[i])
							{
								answerForTheRest = a;
								break;
							}
							else if (a[i] < b[i])
							{
								answerForTheRest = b;
								break;
							}
						}
						answerForTheRest = a;
						break;
					}
					else
					{
						maxLimit = max2;
						max1 = -1;
						max2 = -1;
						maxIdx1 = -1;
						maxIdx2 = -1;
						continue;
					}
				}
			}

			vector<int> &answer = t[idx1][idx2][k];
			answer.clear();
			answer.push_back(max({max1, max2}));
			if (answerForTheRest.size()>0)
				answer.insert(answer.end(), answerForTheRest.begin(), answerForTheRest.end());

			return answer;

		}

		vector<int> maxNumber(vector<int>& n1, vector<int>& n2, int k)
		{
			len1 = n1.size();
			len2 = n2.size();

			vector<int>*** t = new vector<int>**[len1+1];
			for (int i = 0; i <= len1; i++)
			{
				t[i] = new vector<int>*[len2+1];
				for (int j = 0; j <= len2; j++)
				{
					t[i][j] = new vector<int>[k+1];
					for (int p = 0; p <= k; p++)
						t[i][j][p].push_back(-2);
				}
			}

			vector<int> answer = maxN_Recur(n1, n2, k, 0, 0, t);

			for (int i = 0; i < len1; i++)
			{
				for (int j = 0; j < len2; j++)
				{
					delete[] t[i][j];
				}
				delete[] t[i];
			}
			delete[] t;

			return answer;
		}
	};

	/*
	* Look at a subarray of nums1 and nums2
	* nums1: .......i...x..       look at the subarray starting at i
	* nums2: ...........j..y..    look at the subarray starting at j
	* 
	* say for example, I want to find the maximum 2 digits number from these two subarrays.
	* I found 'x' and 'y' in each array.
	* 
	* Now I want to find maximum 5 digits number from these two subarrays.
	* Is it always equivalent to finding the maximum 3 digits number from the subarrays starting at 'x' and 'y',
	* and just combine 'x', 'y' with that 3 digits number, then I get a maximum 5 digits number?
	* 
	* In subarrays starting from 'i' and 'j', I get a maximum m-digit number, the last digit I picked from nums1
	* is 'x' and the last digit I picked from nums2 is 'y'.
	* Now I want to get maximum n-digit number (n > m) from subarrays starting from 'i' and 'j', I just look for
	* n-m digits from subarrays starting from 'x' and 'y'.
	* 
	* This is wrong idea.
	* 
	* nums1: 3, 9, 1
	* nums2: 5, 8, 2
	* 
	* maximum 2-digit number: 98
	* maximum 5-digit number, I can't look for another 3 digits from subarrays starting from '9' and '8'
	*/


	class Solution2
	{
		vector<int> getMaxKFromOneArray(vector<int>& nums, int k)  // this function itself can become a good standalond exercise.
		{
			vector<int> a;
			size_t len = nums.size();
			
			for (int i = 0; i < len; i++)
			{
				if (a.size() == 0)
					a.push_back(nums[i]);
				else
				{
					while (a.size() > 0 && *a.rbegin() < nums[i])
					{
						// if a's size is 5 and if I remove last of 'a', I'll have 4 left in 'a'
						// I'll add in nums[i], 
						// if i is 15, and len is 18, I have 18 - 15 -1 = 2 left in nums after nums[i]
						// so if I remove the last of 'a' and add nums[i] (a's size doesn't change),
						// at most I can collect:  a.size() + (18 - 15 - 1)
						if (a.size()  + len - i - 1 >= k)
						{
							a.pop_back();
						}
						else
						{
							break;
						}
					}
					a.push_back(nums[i]);
				}
			}
			if (a.size() > k)
			{
				auto it = a.begin();
				advance(it, k);
				a.erase(it, a.end());
			}
			return a;
		}

		/*
		* I must guarantee one number is bigger than the other when I actually do the merge
		* for example: nums1: 6 7
		*              nums2: 6 8 4
		* The correct answer should be 6 8 6 7 4
		* 
		* If I just randomly pick the first '6', I can get a wrong answer:
		* 
		* nums1: 6 7
		* nums2: 6 8 4
		* 
		* pick '6' from nums1
		* 
		* nums1: 7
		* nums2: 6 8 4
		* 
		* merged: 6
		* 
		* now I'm going to pick '7' from nums1 because 7 > 6
		* 
		* nums1:
		* nums2: 6 8 4
		* 
		* merged: 6 7
		* 
		* ....
		* 
		* nums1:
		* nums2:
		* 
		* merged: 6 7 6 8 4
		* 
		* This is different than merge sort's merge, because numbers in each vector are not sorted here.
		* 
		*/
		void mergeTwo(vector<int>const& nums1, vector<int>const& nums2, vector<int>& merged)
		{
			size_t len1 = nums1.size();
			size_t len2 = nums2.size();

			int compResult = compare(nums1, nums2);
			if (compResult == -1)
			{
				mergeTwoGuaranteeNums1Bigger(nums2, nums1, merged);
			}
			else
				mergeTwoGuaranteeNums1Bigger(nums1, nums2, merged);
		}

		void mergeTwoGuaranteeNums1Bigger(vector<int>const& nums1, vector<int>const& nums2, vector<int>& merged)
		{
			size_t len1 = nums1.size();
			size_t len2 = nums2.size();

			//std::merge(nums1.begin(), nums1.end(), nums2.begin(), nums2.end(), merged.begin());  // can't use built-in merge, sequence not ordered
			int i = 0, j = 0; auto it = merged.begin();
			while (i < len1 && j < len2)
			{
				if (nums1[i] >= nums2[j])
				{
					*it = nums1[i];
					it = next(it);
					i++;
				}
				else
				{
					*it = nums2[j];
					it = next(it);
					j++;
				}
			}
			if (i < len1)
				copy(nums1.begin() + i, nums1.end(), it);
			else if (j < len2)
				copy(nums2.begin() + j, nums2.end(), it);
		}

		int compare(vector<int>const & nums1, vector<int>const & nums2)  // nums1 < nums2: -1;   nums1 == nums2: 0;   nums1 > nums2: 1
		{
			bool result = lexicographical_compare(nums1.begin(), nums1.end(), nums2.begin(), nums2.end());
			if (result)
				return -1;
			else
			{
				bool result2 = lexicographical_compare(nums2.begin(), nums2.end(), nums1.begin(), nums1.end());
				if (result2)
					return 1;
				else
					return 0;
			}
		}

	public:
		vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k)
		{
			size_t len1 = nums1.size();
			size_t len2 = nums2.size();
			vector<int> maxSoFar;
			for (int takeFrom1 = 0; takeFrom1 <= k; takeFrom1++)
			{
				if (len1 < takeFrom1)
					break;
				int takeFrom2 = k - takeFrom1;
				if (takeFrom2 > len2)
					continue;

				vector<int>const & a1 = getMaxKFromOneArray(nums1, takeFrom1);
				vector<int>const & a2 = getMaxKFromOneArray(nums2, takeFrom2);
				vector<int>merged(k);
				mergeTwo(a1, a2, merged);

				if (compare(maxSoFar, merged) == -1)   // looks like I don't need to make "compare" distinguish equal case
					maxSoFar = merged;
			}
			return maxSoFar;
		}
	};


	class Solution3
	{
		/*
		* This solution is a good exercise and a good idea, but there is no caching, so time limit exceeded
		*/
		size_t len1;
		size_t len2;
		int goalLength;
		void backtrack_solve(vector<int>& nums1, vector<int>& nums2, size_t i1, size_t i2, int digitsToTake, vector<int>& maxSoFar, vector<int>&temporary)
		{
			if (digitsToTake == 0)
			{
				maxSoFar = temporary;
				return;
			}
			if (i1 == len1 && i2 == len2 && digitsToTake > 0)
			{
				temporary = maxSoFar;
				return;
			}
			size_t index = goalLength - digitsToTake;
			/*
			* For example, the goal is to find 8 digits. The original input 'k' is 8.
			* I need to find 3 more digits at this point, "digitsToTake" is 3.
			* max so far:  0   1   2   3   4   5   6   7   <-- indices of 8 digits
			*                                  |
			*                                looking for a better choice for this digit.
			*/

			// look for a digit from nums1 that is better than what "maxSoFar" has.
			for (size_t i = i1; i < len1; i++)
			{
				if (nums1[i] > temporary[index])
				{
					temporary[index] = nums1[i];
					fill(temporary.begin() + index + 1, temporary.end(), 0);
					backtrack_solve(nums1, nums2, i + 1, i2, digitsToTake - 1, maxSoFar, temporary);
				}
				else if (nums1[i] == temporary[index])
				{
					backtrack_solve(nums1, nums2, i + 1, i2, digitsToTake - 1, maxSoFar, temporary);
				}
			}
			for (size_t i = i2; i < len2; i++)
			{
				if (nums2[i] > temporary[index])
				{
					temporary[index] = nums2[i];
					fill(temporary.begin() + index + 1, temporary.end(), 0);
					backtrack_solve(nums1, nums2, i1, i + 1, digitsToTake - 1, maxSoFar, temporary);
				}
				else if (nums2[i] == temporary[index])
				{
					backtrack_solve(nums1, nums2, i1, i + 1, digitsToTake - 1, maxSoFar, temporary);
				}
			}
		}

	public:
		vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k)
		{
			len1 = nums1.size();
			len2 = nums2.size();
			goalLength = k;
			vector<int> maxSoFar(k, 0);
			vector<int> temporary(k, 0);

			backtrack_solve(nums1, nums2, 0, 0, k, maxSoFar, temporary);
			return maxSoFar;
		}
	};

	/*
	* 1 <= m, n <= 500
	* both input vectors can be hundreds elements long, and 'k' can be hundreds.
	* r[x][y][300] cannot be just one integer number with 300 digits.
	* So each entry of the record book cannot be just one integer, it has to be a vector.
	*/
	class Solution4  // Memory limit exceeded
	{
		size_t len1;
		size_t len2;
	public:

		string solve(vector<vector<vector<string>>>& r, vector<int>& nums1, vector<int>& nums2, size_t i1, size_t i2, int digitsToPick)
		{	
			string result, theRest;

			int maxDigit1;
			size_t maxDigitIndex1;
			int maxDigit2;
			size_t maxDigitIndex2;

			if (i1 == len1 && i2 != len2)
			{
				maxDigit1 = -1;
				maxDigitIndex1 = -1;
				maxDigit2 = nums2[i2];
				maxDigitIndex2 = i2;
			}
			else if (i1 != len1 && i2 == len2)
			{
				maxDigit1 = nums1[i1];
				maxDigitIndex1 = i1;
				maxDigit2 = -1;
				maxDigitIndex2 = -1;
			}
			else
			{
				maxDigit1 = nums1[i1];
				maxDigitIndex1 = i1;
				maxDigit2 = nums2[i2];
				maxDigitIndex2 = i2;
			}

			if (r[i1][i2][digitsToPick] != "#")
				return r[i1][i2][digitsToPick];

			for (size_t i = i1; i < len1; i++)
			{
				if (len1 - i - 1 + len2 - i2 >= digitsToPick - 1)
				{
					if (nums1[i] > maxDigit1)  // I should break out whenever I see a '9'
					{
						maxDigit1 = nums1[i];
						maxDigitIndex1 = i;
					}
				}
				else
					break;
			}
			for (size_t i = i2; i < len2; i++)
			{
				if (len2 - i - 1 + len1 - i1 >= digitsToPick - 1)
				{
					if (nums2[i] > maxDigit2)
					{
						maxDigit2 = nums2[i];
						maxDigitIndex2 = i;
					}
				}
				else
					break;
			}

			if (maxDigit1 > maxDigit2)
			{
				if (digitsToPick > 1)
				{
					theRest = solve(r, nums1, nums2, maxDigitIndex1 + 1, i2, digitsToPick - 1);
					
					result = to_string(maxDigit1) + theRest;
				}
				else
					result = to_string(maxDigit1);
			}
			else if (maxDigit1 < maxDigit2)
			{
				if (digitsToPick > 1)
				{
					theRest = solve(r, nums1, nums2, i1, maxDigitIndex2 + 1, digitsToPick - 1);
					
					result = to_string(maxDigit2) + theRest;
				}
				else
					result = to_string(maxDigit2);
			}
			else
			{
				if (digitsToPick > 1)
				{
					string theRest1 = solve(r, nums1, nums2, maxDigitIndex1 + 1, i2, digitsToPick - 1);
					string theRest2 = solve(r, nums1, nums2, i1, maxDigitIndex2 + 1, digitsToPick - 1);
					
					int cmp = theRest1.compare(theRest2);
					if (cmp < 0)
					{
						result = to_string(maxDigit1) + theRest2;
					}
					else
					{
						result = to_string(maxDigit1) + theRest1;
					}
				}
				else
				{
					result = to_string(maxDigit1);
				}
			}
			r[i1][i2][digitsToPick] = result;
			return result;
		}

		vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k)
		{
			if (all_of(nums1.begin(), nums1.end(), [](int& a) {return a == 0; })
				&& all_of(nums2.begin(), nums2.end(), [](int& a) {return a == 0; }))
			{
				return vector<int>{0}; // return initializer_list({ 0 }); <== Visual Studio can compile this, but leetcode can't
			}

			len1 = nums1.size();
			len2 = nums2.size();
			vector<vector<vector<string>>> r(len1+1, vector<vector<string>>(len2+1, vector<string>(k+1, "#")));
			string s = solve(r, nums1, nums2, 0, 0, k);
			vector<int>answer(s.size());
			for (size_t i = 0; i < s.size(); i++)
			{
				answer[i] = s[i] - '0';
			}
			return answer;
		}
	};


	class Solution5 // not tested.   What if I don't use record book at all?
	{
		size_t len1;
		size_t len2;
	public:

		string solve(vector<int>& nums1, vector<int>& nums2, size_t i1, size_t i2, int digitsToPick)
		{
			string result, theRest;

			int maxDigit1;
			size_t maxDigitIndex1;
			int maxDigit2;
			size_t maxDigitIndex2;
			size_t i1Max;
			size_t i2Max;
			
			if (i1 == len1 && i2 != len2)
			{
				maxDigit1 = -1;
				maxDigitIndex1 = -1;
				maxDigit2 = nums2[i2];
				maxDigitIndex2 = i2;
			}
			else if (i1 != len1 && i2 == len2)
			{
				maxDigit1 = nums1[i1];
				maxDigitIndex1 = i1;
				maxDigit2 = -1;
				maxDigitIndex2 = -1;
			}
			else
			{
				maxDigit1 = nums1[i1];
				maxDigitIndex1 = i1;
				maxDigit2 = nums2[i2];
				maxDigitIndex2 = i2;
			}

			i1Max = len1 + len2 - i2 - digitsToPick;
			for (size_t i = i1; i < i1Max; i++)
			{
				if (nums1[i] > maxDigit1)
				{
					maxDigit1 = nums1[i];
					maxDigitIndex1 = i;
					if (nums1[i] == 9)
						break;
				}
			}
			i2Max = len2 + len1 - i1 - digitsToPick;
			for (size_t i = i2; i < i2Max; i++)
			{
				if (nums2[i] > maxDigit2)
				{
					maxDigit2 = nums2[i];
					maxDigitIndex2 = i;
					if (nums2[i] == 9)
						break;
				}
			}

			if (maxDigit1 > maxDigit2)
			{

				if (digitsToPick > 1)
				{
					theRest = solve(nums1, nums2, maxDigitIndex1 + 1, i2, digitsToPick - 1);

					return to_string(maxDigit1) + theRest;
				}
				else
					return to_string(maxDigit1);
			}
			else if (maxDigit1 < maxDigit2)
			{

				if (digitsToPick > 1)
				{
					theRest = solve(nums1, nums2, i1, maxDigitIndex2 + 1, digitsToPick - 1);

					return to_string(maxDigit2) + theRest;
				}
				else
					return to_string(maxDigit2);
			}
			else
			{
				if (digitsToPick > 1)
				{
					if (maxDigitIndex1 == i1Max && maxDigitIndex2 == i2Max)
					{
					
						/*
						* I can't just compare the next digits.
						* for example. nums1: 3, 9;   nums2: 8, 9   k = 3;
						* max digits in both arrays are '9', but they don't have "next" digits.
						* 
						*/

						if (maxDigitIndex1 != len1 - 1 && maxDigitIndex2 != len2 - 1)
						{
							if (nums1[maxDigitIndex1 + 1] > maxDigit2 && nums1[maxDigitIndex1 + 1] > nums2[maxDigitIndex2 + 1])
							{
								theRest = solve(nums1, nums2, maxDigitIndex1 + 1, i2, digitsToPick - 1);

								return to_string(maxDigit1) + theRest;
							}
							if (nums2[maxDigitIndex2 + 1] > maxDigit1 && nums2[maxDigitIndex2 + 1] > nums1[maxDigitIndex1 + 1])
							{
								theRest = solve(nums1, nums2, i1, maxDigitIndex2 + 1, digitsToPick - 1);

								return to_string(maxDigit2) + theRest;
							}
						}
						else if (maxDigitIndex1 == len1 - 1 && maxDigitIndex2 != len2 - 1)
						{
							theRest = solve(nums1, nums2, i1, maxDigitIndex2 + 1, digitsToPick - 1);

							return to_string(maxDigit2) + theRest;
						}
						else if (maxDigitIndex1 != len1 - 1 && maxDigitIndex2 == len2 - 1)
						{
							theRest = solve(nums1, nums2, maxDigitIndex1 + 1, i2, digitsToPick - 1);

							return to_string(maxDigit1) + theRest;
						}
					}

					theRest = solve(nums1, nums2, maxDigitIndex1 + 1, i2, digitsToPick - 1);

					return to_string(maxDigit1) + theRest;
				}
				else
				{
					return to_string(maxDigit1);
				}
			}
		}

		vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k)
		{
			if (all_of(nums1.begin(), nums1.end(), [](int& a) {return a == 0; })
				&& all_of(nums2.begin(), nums2.end(), [](int& a) {return a == 0; }))
			{
				return vector<int>{0}; // return initializer_list({ 0 }); <== Visual Studio can compile this, but leetcode can't
			}

			len1 = nums1.size();
			len2 = nums2.size();
			string s = solve(nums1, nums2, 0, 0, k);
			vector<int>answer(s.size());
			for (size_t i = 0; i < s.size(); i++)
			{
				answer[i] = s[i] - '0';
			}
			return answer;
		}
	};



	void Test_0321_Create_Maximum_Number()
	{
		Solution5 solu;

		while (true)
		{
			vector<int>nums1;
			vector<int>nums2;
			int k;
			string s;
			cout << "nums1: ";
			getline(cin, s);
			if (s.size() == 0) break;
			stringstream ss1(s);
			copy(istream_iterator<int>(ss1), istream_iterator<int>(), back_inserter(nums1));
			s.clear();
			cout << "nums2: ";
			getline(cin, s);
			stringstream ss2(s);
			copy(istream_iterator<int>(ss2), istream_iterator<int>(), back_inserter(nums2));
			cout << "k: ";
			cin >> k;
			cin.ignore(); // ignore the newline character so that next test can run properly

			vector<int>const& answer = solu.maxNumber(nums1, nums2, k);
			for (int const& x : answer)
				cout << x << "";
			cout << '\n';
		}
	}
}