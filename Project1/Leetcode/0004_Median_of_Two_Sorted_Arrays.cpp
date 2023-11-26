#include <iostream>
#include <vector>


namespace _0004_Median_of_Two_Sorted_Arrays {

	using namespace std;

	class Solution
	{
	public:
		virtual double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) = 0;
	};

	class Solution2 : public Solution
	{
	public:
		double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2)
		{
			if (nums1.size() > nums2.size())
				return getMedian(nums2, nums1);
			else
				return getMedian(nums1, nums2);
		}

		/*
		 *   v1:  1, 2, 4, 6, 9
		 *   v2:  3, 5, 7, 8, 11
		 *
		 * I want the 8th overall. target is 8
		 *
		 * v1.size is 5
		 * v1_start is 0
		 * v1_end is 4
		 * take half of v1   i1 is (4-0)/2 = 2 (index 2)    c1 is 2-0+1 = 3     take 3 elements from v1
		 * then I take 5 elements from v2   c2 is 8-3=5
		 */
		double getMedian(std::vector<int>& v1, std::vector<int>& v2)  // caller makes sure v1 is shorter than v2
		{
			bool odd = (v1.size() + v2.size()) % 2 == 1;
			size_t target;
			if (odd)
			{
				target = (v1.size() + v2.size()) / 2 + 1;
			}
			else
			{
				target = (v1.size() + v2.size()) / 2;
			}

			size_t v1_start = 0;              // start and end of the searching range
			size_t v1_end = v1.size() - 1;

			while (true)
			{
				size_t i1 = (v1_end - v1_start) / 2 + v1_start;  // median index or the lower median index, include this in the first half
				size_t c1 = i1 + 1;  // number of elements I'm taking from v1, either exactly half (of the range denoted by [v1_start, v1_end]) or 1 fewer than the other half
				size_t c2 = target - c1;
				size_t i2 = c2 - 1;

				if (c2 == 0)
				{
					if (odd)
					{
						// if v1+v2 length is odd, and v1 is guaranteed to be equal or shorter than v2,
						// v1 and v2 can be, for example, 5 and 6, there is no way I don't take any element
						// from v2, if I want to get to the median.
					}
					else
					{
						// this is the case where v1.size() == v2.size() and v1 is entirely less than v2
						// and i1 is pointing at the last of v1, which means I'm taking the entire v1
						if (v1[i1] <= v2[0])
						{
							return (double)(v1[i1] + v2[0]) / 2.0;
						}
					}
				}

				// if v1 is guaranteed to be shorter, even if v1 is entirely preceeding v2,
				// usually I still have to take a few from v2
				// corner case when c2 will be 0:
				// v1 and v2 are equally long, for example, both have 5 elements
				// I need the 5th and 6th
				// When look for 5th, if v1 entirely preceeds v2, I take 5 from v1 and none from v2

				if (i1 < v1.size() - 1 && v2[i2] > v1[i1 + 1])  // I took too many from v2, and too few from v1, need more from v1.
				{
					v1_start = i1 + 1;
				}
				else if (i2 < v2.size() - 1 && v1[i1] > v2[i2 + 1])  // I took too many from v1
				{
					// if I'm already just taking 1 from v1, how is the case taking-none-from-v1 represented? p1 will be invalid
					v1_end = i1 - 1;
				}
				else if (i1 == v1.size() - 1)  // the case taking-all-from-v1 can be represented
				{
					return odd ? ((double)std::max(v1[i1], v2[i2])) : ((double)(std::max(v1[i1], v2[i2]) + v2[i2 + 1]) / 2.0);
				}
				/*
				* It's impossible to take all of v2 given that v2 is longer.
				* If v2 is really longer, v2 has more than half of the elements, I must not take all of v2.
				* What if v1 and v2 are equally long and v1 is entirely greater than v2?
				* Or more generally speaking, what if v1 is shorter and entirely greater than v2, so that
				* the answer is to take some of v2 and none of v1. Where is this case handled?
				* Therefore this implementation is not 100% correct. The while loop doesn't have a terminating condition.
				* It entirely relies on the logics inside it to stop the loop.
				* If say v1 is 4-element long, and v2 is 10-element long, and v1 is entirely greater than v2.
				* At some point I'll have v1_end < v1_start, (basically I'll keep moving v1_end to the left, but since
				* the correct answer is to take none from v1, as long as v1_start <= v1_end, I won't get the answer I want)
				*/
				else
				{
					if (odd)
					{
						return std::max(v1[i1], v2[i2]);
					}
					else
					{
						return (double)(std::max(v1[i1+1], v2[i2 + 1]) + std::min(v1[i1 + 1], v2[i2 + 1])) / 2.0;
					}
				}
			}
		}
	};

	class Solution3 : public Solution
	{
		// the same principles as Solution2 just better handling of corner cases (one array
		// entirely preceeding the other), deal with the corner case up front.



		double getMedian(std::vector<int>& v1, std::vector<int>& v2)
		{
			bool odd = (v1.size() + v2.size()) % 2 == 1;
			size_t target;
			if (odd)
			{
				target = (v1.size() + v2.size()) / 2 + 1;
			}
			else
			{
				target = (v1.size() + v2.size()) / 2;
			}

			if (*(v1.rbegin()) <= *(v2.begin()))
			{
				if (v1.size() == v2.size())
				{
					return (double)(*(v1.rbegin()) + *(v2.begin())) / 2.0;
				}
				if (v1.size() < v2.size())
				{
					size_t t = target - v1.size();  // if I want 4 elements from v2, I need begin()+3
					if (odd)
					{
						return (double)*(v2.begin() + t - 1);
					}
					else
					{
						return (double)(*(v2.begin() + t - 1) + *(v2.begin() + t)) / 2.0;
					}
				}
			}
			if (*(v1.begin()) >= *(v2.rbegin()))
			{
				if (v1.size() == v2.size())
				{
					return (double)(*(v1.begin()) + *(v2.rbegin())) / 2.0;
				}
				if (v1.size() < v2.size())
				{
					if (odd)
					{
						return (double)*(v2.begin() + target - 1);
					}
					else
					{
						return (double)(*(v2.begin() + target - 1) + *(v2.begin() + target)) / 2.0;
					}
				}
			}

			size_t v1_start = 0;              // start and end of the searching range
			size_t v1_end = v1.size() - 1;

			while (true)
			{
				/*
				* If neither of v1 or v2 is entirely preceding or trailing the other,
				* is it still possible that I have to take none from either one of the two?
				* --- Yes, it's still possible
				* 1, 2, 3, 4, 5, 6, 7, 8, 9,                  14, 15     ---  v2
				*                             10, 11, 12, 13             ---  v1
				* 
				* v1 is shorter, both are sorted, median is '8', I need to take 8 from v2 and none from v1.
				* 
				* So this solution is still not 100% correct, the while loop doesn't have a terminating
				* condition, I'll keep binary searching in v1 but won't get an answer until
				* v1_start and v1_end cross each other, I still won't stop the loop, but at that point,
				* i1 will be out of v1's range, and indexing to i1 will crash.
				*/
				size_t i1 = (v1_end - v1_start) / 2 + v1_start;  // median index or the lower median index, include this in the first half
				size_t c1 = i1 + 1;  // number of elements I'm taking from v1, either exactly half (of the range denoted by [v1_start, v1_end]) or 1 fewer than the other half
				size_t c2 = target - c1;
				size_t i2 = c2 - 1;

				if ((i1 + 1) < v1.size() && v2[i2] > v1[i1 + 1])  // I took too many from v2
				{
					v1_start = i1 + 1;
				}
				else if ((i2 + 1) < v2.size() && v1[i1] > v2[i2 + 1])  // I took too many from v1
				{
					v1_end = i1 - 1;
				}
				else
				{
					if (odd)
					{
						return (double)std::max(v1[i1], v2[i2]);
					}
					else
					{
						return (double)(std::max(v1[i1], v2[i2]) + std::min(v1[i1 + 1], v2[i2 + 1])) / 2.0;
					}
				}
			}
		}


	public:

		double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2)
		{
			if (nums1.size() > nums2.size())
				return getMedian(nums2, nums1);
			else
				return getMedian(nums1, nums2);
		}

	};


	/*
	* A difference between this binary search and regular binary search is that
	* in regular binary search, start and end can both move toward the middle and
	* go across each other which essentially means there is no answer.
	* In this binary search, "v1_start" starts at 0, as long as "v1_start" moves
	* toward the middle once, it means I need to take some from v1 at least.
	* "v1_start" and "v1_end" won't end up crossing each other and leaves me not
	* knowing how many to take from v1.
	* The only way "v1_start" and "v1_end" go across each other is that "v1_end" keeps
	* moving to the left and eventually goes over "v1_start", and "v1_start" never moves.
	* This means I must not take any element from v1.
	* "v1_start" can also keep moving to the right and eventually crosses "v1_end", this
	* doesn't mean a failure though, it just means I need to take entire v1.
	*/

	class Solution4 : public Solution
	{
		double getMedian(std::vector<int>& v1, std::vector<int>& v2)
		{
			bool odd = (v1.size() + v2.size()) % 2 == 1;
			size_t target;
			if (odd)
			{
				target = (v1.size() + v2.size()) / 2 + 1;
			}
			else
			{
				target = (v1.size() + v2.size()) / 2;
			}

			size_t v1_start = 0;              // start and end of the searching range
			size_t v1_end = v1.size() - 1;

			size_t i1 = 0;
			size_t c1 = 0;
			size_t c2 = 0;
			size_t i2 = 0;

			/*
			* There is no way to not do special handling.
			* If I don't check special cases before the while loop, I have to consider
			* the case where v1 is entirely precede v2 and the two are equally long.
			* a b c d e | f g h j k
			*    v1          v2
			* target is 5
			* right answer is c1 = 5 and c2 = 0,
			* but if I calculate i2, which is c2 -1, I get invalid value
			*/
			while (true)
			{
				i1 = (v1_end - v1_start) / 2 + v1_start;
				c1 = i1 + 1;
				c2 = target - c1;
				i2 = c2 - 1;

				if (c2 == 0)
				{
					// only possibility for c2 = 0 is
					//  a b c d e | f g h j k
					//      v1          v2
					break;
				}

				if ((i1 + 1) < v1.size() && v2[i2] > v1[i1 + 1])  // I took too many from v2
				{
					v1_start = i1 + 1;
					if (v1_start >= v1.size())
					{
						i1 = v1.size() - 1; // I want to take more from v1, but this is the most I can get from v1
						c1 = i1 + 1;
						c2 = target - c1;
						i2 = c2 - 1;
						break;
					}
				}
				else if ((i2 + 1) < v2.size() && v1[i1] > v2[i2 + 1])  // I took too many from v1
				{
					// I'm about to do i1 - 1, because i1 is size_t, result willl neven be negative, can't compare the result to 0, must compare i1 to 0 before the subtraction
					if (i1 == 0) // taking 1 from v1, but it's still too many, so I have to take none from v1
					{
						c1 = 0;
						c2 = target;
						i2 = c2 - 1;
						break;
					}
					v1_end = i1 - 1;
				}
				else
				{
					// i1 and i2 are the correct values now
					break;
				}
			}

			if (c2 == 0)
			{
				if (odd)
				{
					return (double)v1[i1];
				}
				else
				{
					return (double)(v1[i1] + v2[0]) / 2.0;
				}
			}

			if (c1 > 0)
			{
				// I'm taking at least 1 from v1, so 'i1' is a valid index into v1
				if (odd)
				{
					return (double)std::max(v1[i1], v2[i2]);
				}
				else
				{
					return (double)(std::max(v1[i1], v2[i2]) + std::min(v1[i1 + 1], v2[i2 + 1])) / 2.0;
				}
			}
			else
			{
				// I'm not taking any of v1
				if (odd)
				{
					return (double)*(v2.begin() + target - 1);
				}
				else
				{
					return (double)(*(v2.begin() + target - 1) + *(v2.begin() + target)) / 2.0;
					// Does this test break this solution here? -- Yes it does.
					// target is 3, but I need the 3rd and 4th elements, when only considering target
					// I don't need to take any from v1, but the next of target comes from v1.
					// v2  1 2 3     6
					// v1        4 5 
				}
			}
		}

	public:

		double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2)
		{
			if (nums1.size() > nums2.size())
				return getMedian(nums2, nums1);
			else
				return getMedian(nums1, nums2);
		}
	};



	class Solution5 : public Solution
	{
		double getMedian(std::vector<int>& v1, std::vector<int>& v2)
		{
			bool odd = (v1.size() + v2.size()) % 2 == 1;
			size_t target;
			if (odd)
			{
				target = (v1.size() + v2.size()) / 2 + 1;
			}
			else
			{
				target = (v1.size() + v2.size()) / 2;
			}

			size_t v1_start = 0;              // start and end of the searching range
			size_t v1_end = v1.size() - 1;

			size_t i1 = 0;
			size_t c1 = 0;
			size_t c2 = 0;
			size_t i2 = 0;

			while (true)
			{
				i1 = (v1_end - v1_start) / 2 + v1_start;
				c1 = i1 + 1;
				c2 = target - c1;
				i2 = c2 - 1;

				if (c2 == 0)
				{
					// only possibility for c2 = 0 is
					//  a b c d e | f g h j k
					//      v1          v2
					break;
				}

				if ((i1 + 1) < v1.size() && v2[i2] > v1[i1 + 1])  // I took too many from v2
				{
					if (i1 == v1.size() - 1)
					{
						// need to make sure v1_start is valid after doing i1+1
						// only case where i1+1 becomes invalid is when i1 is already the last element in v1
						// but if i1 is already the last one, it won't even come in this "if" clause.
						// it won't pass the i1 +1 < v1.size() check
						// on the other hand, if I'm already taking all v1, and v1 is guaranteed to be equal or shorter,
						// it's impossible that I'm still not getting the solution
					}
					v1_start = i1 + 1;
				}
				else if ((i2 + 1) < v2.size() && v1[i1] > v2[i2 + 1])  // I took too many from v1
				{
					// i1 is size_t (unsigned), must compare with 0, instead of doing subtraction
					if (i1 == 0)
					{
						c1 = 0;
						c2 = target;
						i2 = c2 - 1;
						break;
					}
					v1_end = i1 - 1;
				}
				else
				{
					// i1 and i2 are the correct values now
					break;
				}
			}

			if (c2 == 0)
			{
				// only possibility for c2 = 0 is
				//  a b c d e | f g h j k
				//      v1          v2
				// don't even need to check "odd"

				return (double)(v1[i1] + v2[0]) / 2.0;
			}

			if (c1 == 0)
			{
				// I'm not taking any of v1, but this is to find "target",
				// If overall count is even, I need target and the next of target,
				// and this "next of target" can very likely come from v1
				if (odd)
				{
					return (double)v2[i2];
				}
				else
				{
					if (i2 < v2.size() - 1)
					{
						return ((double) (v2[i2] + std::min(v2[i2 + 1], v1[0]))) / 2.0;
					}
					else
					{
						// i2 points at the last of v2
						//  a b c d e | f g h j k
						//      v2          v1
						return (double)(v2[i2] + v1[0]) / 2.0;
					}
				}
			}

			// now is the general case, taking some from both v1 and v2
			if (odd)
			{
				return (double)std::max(v1[i1], v2[i2]);
			}
			else
			{
				// v1            3   4   5
				// v2    1   2                6   7   8   9   10
				//                       |
				//                      target
				// i1 + 1 is invalid index into v1

				if (i1 == v1.size() - 1)
				{
					return (double)(std::max(v1[i1], v2[i2]) + v2[i2 + 1]) / 2.0;
				}
				else if (i2 == v2.size() - 1) // this is the c1 == 0 case, which is handled above
				{
					throw runtime_error("Not expected execution path");
				}
				else
					return ((double)(std::max(v1[i1], v2[i2]) + std::min(v1[i1 + 1], v2[i2 + 1]))) / 2.0;
			}
		}

	public:

		double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2)
		{
			if (nums1.size() > nums2.size())
				return getMedian(nums2, nums1);
			else
				return getMedian(nums1, nums2);
		}
	};

	static void Test1(Solution *s)
	{
		// 1 2 3 4 5 6 7 8 9 11     ---   5.5
		std::vector<int> nums1 { 1, 2, 4, 6, 9 };
		std::vector<int> nums2 { 3, 5, 7, 8, 11 };

		auto result = s->findMedianSortedArrays(nums1, nums2);

		std::cout << result << ((result == 5.5) ? "  Pass" : "  Fail") << '\n';
	}

	static void Test2(Solution *s)
	{
		// 1  2  4  6  7  8  11  14  18  19  21   ---  8
		std::vector<int> nums1 { 1, 2, 4, 6 };
		std::vector<int> nums2 { 7, 8, 11, 14, 18, 19, 21 };

		auto result = s->findMedianSortedArrays(nums1, nums2);

		std::cout << result << ((result == 8.0) ? "  Pass" : "  Fail") << '\n';
	}

	static void Test3(Solution *s)
	{
		// 7, 8, 11, 14, 18, 19, 21, 33, 37, 38, 42, 48    --- 20
		std::vector<int> nums1 { 33, 37, 38, 42, 48 };
		std::vector<int> nums2 { 7, 8, 11, 14, 18, 19, 21 };

		auto result = s->findMedianSortedArrays(nums1, nums2);

		std::cout << result << ((result == 20.0) ? "  Pass" : "  Fail") << '\n';
	}

	static void Test4(Solution* s)
	{
		// 3, 4, 5, 6, 7, 8, 11, 12, 13, 14
		std::vector<int> nums1 { 3, 4, 5, 6, 7 };
		std::vector<int> nums2 { 8, 11, 12, 13, 14 };

		auto result = s->findMedianSortedArrays(nums1, nums2);

		std::cout << result << ((result == 7.5) ? "  Pass" : "  Fail") << '\n';
	}

	static void Test5(Solution *s)
	{
		std::vector<int> nums1 { 3, 4, 5, 6, 7 };

		auto result = s->findMedianSortedArrays(nums1, nums1);

		std::cout << result << ((result == 5.0) ? "  Pass" : "  Fail") << '\n';
	}

	static void Test6(Solution *s) // This test case proves that Solution2 doesn't work, and in fact, it crashes.
	{
		vector<int> nums1{ 11, 12, 13, 14 };
		vector<int> nums2{ 1, 2, 3, 4, 5 };
		
		double result = s->findMedianSortedArrays(nums1, nums2);
		cout << result << ((result == 5.0) ? "  Pass" : "  Fail") << '\n';
	}

	static void Test7(Solution *s) // this test case proves that Solution3 doesn't work, and it can crash.
	{
		vector<int> nums1{ 10, 11, 12, 13 };
		vector<int> nums2{ 1,2,3,4,5,6,7,8,9,14,15 };
		
		double result = s->findMedianSortedArrays(nums1, nums2);

		cout << result << ((result == 8.0) ? "  Pass" : "  Fail") << '\n';
	}

	static void Test8(Solution* s) // this test case proves taht Solution4 is wrong
	{
		vector<int> nums1{ 4, 5 };
		vector<int> nums2{ 1,2,3,6 };

		double result = s->findMedianSortedArrays(nums1, nums2);

		cout << result << ((result == 3.5) ? "  Pass" : "  Fail") << '\n';
	}

	void Test_0004_Median_of_Two_Sorted_Arrays()
	{
		Solution* solu;

		int whichSolution = 0;

		cout << "Choose a solution: 2, 3, 4, 5 >  ";
		cin >> whichSolution;

		switch (whichSolution)
		{
		case 2:
			solu = new Solution2();
			break;
		case 3:
			solu = new Solution3();
			break;
		case 4:
			solu = new Solution4();
			break;
		case 5:
			solu = new Solution5();
			break;
		default:
			throw runtime_error("invalid solution number");
		}

		int testCase = 0;
		while (true)
		{
			std::cout << "choose a test case:  0: autotest, -1: exit, 1--8 test cases   ";
			std::cin >> testCase;
			switch (testCase)
			{
			case -1:
				goto done;
			case 0:
				cout << "auto test is missing";
				goto done;
			case 1:
				Test1(solu);
				break;
			case 2:
				Test2(solu);
				break;
			case 3:
				Test3(solu);
				break;
			case 4:
				Test4(solu);
				break;
			case 5:
				Test5(solu);
				break;
			case 6:
				Test6(solu);
				break;
			case 7:
				Test7(solu);
				break;
			case 8:
				Test8(solu);
				break;
			}
		}
		done:
		delete solu;
	}
}