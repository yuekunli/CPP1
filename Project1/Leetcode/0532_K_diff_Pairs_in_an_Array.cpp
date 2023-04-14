#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<unordered_set>
#include<map>
#include<set>
#include<functional>
#include<algorithm>

namespace _0532_K_diff_Pairs_in_an_Array {

	using namespace std;

	/*
	* There is an error in problem statement:
	* | nums[i] - nums[j] | = k
	* 
	* There is an error in Example 1 too:
	* "There are two 2-diff pairs in the array, (3,1) and (3,5)
	*/

	/*
	* The tricky part of this question is to avoid duplicate.
	* Let's say when I examine a number, I only consider the numbers behind it in the original input.
	* Even so, I still can't avoid duplicate completely.
	* for example, nums: [.... 3, .. 5, .... 3, .... 5, ....]  k = 2
	* When I examine the first '3', I'm going to find two 5's. I can avoid counting (3,5) twice.
	* But when I examine the second '3', I'm going to find the second '5'.
	* So when I finish examining a number, I need to remember I have seen it so that I skip a number with the same value later.
	*/


	class Solution
	{
	public:

		struct A
		{
			int value;
			int index;
		};

		static void lookForPair(A& a, vector<A>& v, int& total, map<int,vector<int>>&m, int k)
		{
			pair<int, int> pairs[2];
			int foundPairs = 0;
			int target = a.value + k;

			auto itLo = lower_bound(v.begin(), v.end(), A{ target, 0 }, [](A const& x, A const& y) {return x.value < y.value; });
			auto itUp = upper_bound(v.begin(), v.end(), A{ target, 0 }, [](A const& x, A const& y) {return x.value < y.value; });

			if (itLo != v.end())
			{
				if ((*itLo).value > target)
					return;
				else  // (*itLo).value == target
				{
					// there are elements that are equal to target

					vector<A>::iterator lastEqual;

					if (itUp != v.end()) // there is upper bound
					{
						lastEqual = itUp - 1;
					}
					else // no upper bound
					{
						lastEqual = v.end() - 1;
					}


					if (distance(itLo, lastEqual) + 1 == 1)
					{
						// there is only one equal to target
						if (k != 0)
						{
							foundPairs = 1;
							if (a.index < (*itLo).index)
							{
								pairs[0].first = a.value;
								pairs[0].second = target;
							}
							else
							{
								pairs[0].first = target;
								pairs[0].second = a.value;
							}
						}
						else
						{
							// diff is 0, and there is only one such element, it's the element itself
						}
					}
					else
					{
						// there are two or more equal to target
						if (k == 0)
						{
							foundPairs = 1;
							pairs[0].first = a.value;
							pairs[0].second = target;
						}
						else
						{
							if (clamp(a.index, (*itLo).index, (*lastEqual).index) == a.index)
							{
								foundPairs = 2;
								pairs[0].first = pairs[1].second = a.value;
								pairs[0].second = pairs[1].first = target;
							}
							else if (a.index < (*itLo).index)
							{
								foundPairs = 1;
								pairs[0].first = a.value;
								pairs[0].second = target;
							}
							else
							{
								foundPairs = 1;
								pairs[0].first = target;
								pairs[0].second = target;
							}
						}
					}
				}
			}
			for (int i = 0; i < foundPairs; i++)
			{
				vector<int>& v = m[pairs[i].first];
				if (find(v.begin(), v.end(), pairs[i].second) == v.end())
				{
					total++;
					v.emplace_back(pairs[i].second);
				}
			}
		}

		int findPairs(vector<int>& nums, int k)
		{
			map<int, vector<int>> alreadyFoundPairs;
			vector<A> v;
			int index = -1;
			transform(nums.begin(), nums.end(), back_inserter(v), [&](int const& n) {
				index++;
				return A{ n, index };
				});
			std::sort(v.begin(), v.end(), [](A&a,A&b) {
				if (a.value != b.value)
					return a.value < b.value;
				else
					return a.index < b.index;
				});

			int total = 0;
			for_each(v.begin(), v.end(), std::bind(lookForPair, placeholders::_1, std::ref(v), std::ref(total), std::ref(alreadyFoundPairs), k));

			return total;
		}
	};


	class Solution2
	{
		/*
		* sort the numbers by their value but attach their indexes into the sorted array.
		* Examine numbers one by one in the original input array. Calculate the target I want to find.
		* If number under examination is 7 and k is 4, the target can be 3 or 11.
		* Do binary search in the sorted array, find all 3's, only count the ones that are behind the 7 in the original input array.
		* And only count 1 pair even if there are multiple such 3's. Do the same for 11.
		*/
		/*
		* The basic principles of this solution is the same as the previous one.
		* This one is just cleaner and simpler.
		*/

	public:

		struct A
		{
			int value;
			int index;
		};

		struct AComp
		{
			using is_transparent = void;

			bool operator()(A const& a, A const& b) const
			{
				return a.value < b.value;
			}

			bool operator()(A const& a, int b) const
			{
				return a.value < b;
			}

			bool operator()(int a, A const& b) const
			{
				return a < b.value;
			}
		};

		void lookForPair(int n, int index, int target, vector<A>& v, int& total)
		{
			auto itLo = lower_bound(v.begin(), v.end(), target, AComp{});
			auto itUp = upper_bound(v.begin(), v.end(), target, AComp{});

			if (itLo != v.end())
			{
				if ((*itLo).value > target)
					return;
				else  // (*itLo).value == target
				{
					// there are elements that are equal to target

					vector<A>::iterator lastEqual;

					if (itUp != v.end()) // there is an upper bound
					{
						lastEqual = prev(itUp);
					}
					else // no upper bound
					{
						lastEqual = prev(v.end());
					}

					// There may be multiple valus equal to target
					// but I only care if a target's index in the original input is greater than this number's index
					// and as long as there is one such target, I can count 1 more valid pair.
					// Must not count more than 1.

					auto it = itLo;
					while (it <= lastEqual)
					{
						if (it->index > index)
						{
							total++;
							break;
						}
						it = next(it);
					}
				}
			}
		}

		int findPairs(vector<int>& nums, int k)
		{
			vector<A> sortedNumsWithIndex;
			
			set<int> alreadySeen;

			int index = -1;
			transform(nums.begin(), nums.end(), back_inserter(sortedNumsWithIndex),
				[&](int const& n)
				{
					index++;
					return A{ n, index };
				});
			std::sort(sortedNumsWithIndex.begin(), sortedNumsWithIndex.end(), AComp{});

			int validPairsCount = 0;
			
			for (int i = 0; i < nums.size(); i++)
			{
				if (alreadySeen.find(nums[i]) != alreadySeen.end())
					continue;

				int target = nums[i] + k;
				lookForPair(nums[i], i, target, sortedNumsWithIndex, validPairsCount);
				
				if (k != 0)
				{
					// if k is 0, running the 2nd time will cause duplicate, because the target will be the same.
					target = nums[i] - k;
					lookForPair(nums[i], i, target, sortedNumsWithIndex, validPairsCount);
				}
				
				alreadySeen.insert(nums[i]);
			}
			return validPairsCount;
		}
	};

	/*
	* The first two solutions use sorting. But Why don't I just use hash map?
	*/
	/*
	* Two corner cases:
	* input: [1, 3, 1, 5, 4]   answer: 1
	* input: [1, 1, 1, 1, 1]   answer: 1
	*/
	class Solution3
	{
	public:
		int findPairs(vector<int>& nums, int k)
		{
			unordered_set<int> m;
			unordered_set<int> samePairWhenKZero;
			int total = 0;
			for (int const& c : nums)
			{
				if (k == 0)
				{
					if (m.count(c) == 1 && samePairWhenKZero.count(c) == 0)
					{
						total++;
						samePairWhenKZero.insert(c);
					}
					else
						m.insert(c);
				}
				else
				{
					if (m.count(c) == 0)
					{
						int pairing = c - k;
						if (m.count(pairing) == 1)
							total++;
						pairing = c + k;
						if (m.count(pairing) == 1)
							total++;

						m.insert(c);
					}
				}
			}
			return total;
		}
	};

	void Test_0532_K_diff_Pairs_in_an_Array()
	{
		Solution3 s;

		vector<int> nums;
		string arrayInString;
		int k;
		while (true)
		{
			nums.clear();
			arrayInString.clear();
			cout << "array:";
			getline(cin,arrayInString);
			if (arrayInString.size() == 0) break;
			std::istringstream ss(arrayInString);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << "k:";
			cin >> k;
			cin.ignore();
			cout << s.findPairs(nums, k) << '\n';
		}
	}
}