#include "LibHeaders.h"

namespace _0300_Longest_Increasing_Subsequence {

	using namespace std;

	class Solution1  // This is the solution I worked out
	{
		/*
		* This solution fails to return corrent answer on this input:
		* 
		* input: 75 44 26 38 16 24 9 25 59 29 62 70
		* 
        * longest increasing subseq: 16 24 25 29 62 70
		*/

	public:

		int lengthOfLIS(vector<int> const & nums)
		{
			int len = nums.size();

			map<int, int> m; // for each value in nums, the length of the longest increasing subseq ending at that value
			m[nums[0]] = 1;

			int maxSoFar = 1;
			
			for (int i = 1; i < len; i++)
			{
				auto lowIt = m.lower_bound(nums[i]);
				if (lowIt == m.begin() && (lowIt->first != nums[i]))
				{
					m[nums[i]] = 1;
					continue;
				}
				else if (lowIt == m.begin())
				{
					continue;
				}

				if (lowIt == m.end())
				{
					m[nums[i]] = maxSoFar+1;
					maxSoFar++;
					continue;
				}

				auto after = lowIt;
				auto pre = prev(lowIt);

				if (pre->second >= after->second)
				{
					int newValue = pre->second + 1;
					m[nums[i]] = newValue;
					if (maxSoFar < newValue)
						maxSoFar = newValue;
					continue;
				}

				if (pre == m.begin())
				{
					int newVal = pre->second + 1;
					m[nums[i]] = newVal;
					if (maxSoFar < newVal)
						maxSoFar = newVal;
					continue;
				}

				auto oneToTheRight = pre;
				
				auto it = prev(oneToTheRight);
				while (it != m.begin())
				{
					if (it->second >= oneToTheRight->second)
					{
						int newVal = it->second + 1;
						m[nums[i]] = newVal;
						if (maxSoFar < newVal)
							maxSoFar = newVal;
						break;
					}
					oneToTheRight = it;
					it = prev(oneToTheRight);
				}
				if (it == m.begin() && it->second < oneToTheRight->second)
				{
					m[nums[i]] = pre->second + 1;
					if (maxSoFar < pre->second + 1)
						maxSoFar = pre->second + 1;
				}
			}
			return maxSoFar;
		}
	};


	class Solution2
	{
	public:

		int lengthOfLIS(vector<int> const& nums)
		{
			vector<int>subseqEndings; 
					// the smallest ending value of 1-element, 2-element, 3-element, ... n-element
			        // increasing subsequence
			        // for example, if there are some (more than 1) 3-element subsequences that are increaseing
					// what is the smallest ending of those 3-element subsequences
					// The smallest ending of those 3-element subsequences is stored in cell#2
					// because index starts at 0
			
			int len = nums.size();
			
			subseqEndings.push_back(nums[0]);
			
			for (int i = 1; i < len; i++)
			{
				auto lowIt = lower_bound(subseqEndings.begin(), subseqEndings.end(), nums[i]);
				
				/*
				if (lowIt == subseqEndings.begin())
				{
					if (nums[i] < subseqEndings[0])
						subseqEndings[0] = nums[i];
					continue;
				}
				*/

				if (lowIt == subseqEndings.end())
				{
					subseqEndings.push_back(nums[i]);
					continue;
				}

				// found the lower bound anywhere in the array:
				//        4  5    6  7 .... (indexes)
				// ...... a  b    c  d ....
				//             |  |
				//             |  +-- lower bound
				//             +-- potential insertion point
				// 
				// 6-element increasing subsequence could have a smaller ending now
				// Extra information, not just the ending of 6-element increasing subsequence
				// is being changed here, actually the entire 6-element increasing subsequence
				// may be changed, because the old subsequence's second last element may not
				// be smaller than nums[i]. But if I look at the 5-element increasing subsequence
				// and add nums[i] to that, then I get a new 6-element increasing subsequence.

				* lowIt = nums[i];


			}
			return subseqEndings.size();
		}

		vector<int> longestIncreaseSubseq_AnswerReconstruct(vector<int> const & nums)
		{
			vector<vector<int>> subseqs_indexes;
			vector<int> endings;

			int len = nums.size();
			endings.push_back(nums[0]);
			subseqs_indexes.push_back({ 0 });

			for (int i = 1; i < len; i++)
			{
				auto lowIt = lower_bound(endings.begin(), endings.end(), nums[i]);

				if (lowIt == endings.end())
				{
					endings.push_back(nums[i]);
					subseqs_indexes.push_back(*(prev(subseqs_indexes.end()))); // for example if I'm recognizing a new 5-element subsequence
					                                                           // let me first copy the indexes for the 4-element subsequence
					(*prev(subseqs_indexes.end())).push_back(i);               // then add this new element's index on top of that
					continue;
				}


				/*
				* endings:
				*       4   5   6   7
				* ..... a   b   c   d .....
				*             | |
				*             | +--- lower bound
				*             +--- insertion point 
				*  
				* I'm examining 'i in nums, 'i' will be the new ending of a 6-element increasing subseq
				* What is this new 6-element subseq based upon?
				* It's based exactly upon the current 5-element increasing subseq, because 'i' is just greater
				* than the ending of 5-element increasing subseq.
				* So the indexes of the new 6-element increasing subseq is just the 5-element subseq's indexes
				* plus 'i's index.
				*/

				(*lowIt) = nums[i];
				int d = distance(endings.begin(), lowIt);
				auto a = subseqs_indexes.begin();
				advance(a, d);  // the offset of 'a' in subseqs_indexes is equal to the offset of lowIt in endings
				
				a->clear();
				if (a == subseqs_indexes.begin())
				{
					a->push_back({ i });
					continue;
				}
				auto pre = prev(a);
				*a = *pre;  // replace with the previous subsequence's indexes
				a->push_back(i); // add on this new element's index


				/*
				* Question: If I'm just upding the 6th index of the 6-element subsequence,
				* why do I clear these indexes and copy from 5-element subsequence's index again?
				* Why don't I just update 6-element's index?
				* 
				* for example: 5 6 1 2 3 4
				* the progression of the subseqs_indexes vector will be like this:
				* 
				* 5
				* ------------
				* 
				* 5   5
				*     6
				* 
				* ------------
				* 
				* 1   5
				*     6     update the index for 1-element subsequence
				* 
				* ------------
				* 
				* 1   5
				*     2           Now this is already wrong, I want to update the index for 2-element subsequence
				*                 if I don't copy from 1-element subsequence's indexes, I end up having this
				* ------------
				*/

			}
			return *(prev(subseqs_indexes.end()));
		}
	};


	class AutoTest
	{
	public:
		random_device rd;
		mt19937 gen{ rd() };   // when initializing variables outside functions, curly braces must be used.
		uniform_int_distribution<> dist{ 1, 100 };
		Solution2 s;

		vector<int> generateArray()
		{
			vector<int>nums(12);
			generate(nums.begin(), nums.end(), bind(dist, ref(gen)));
			return nums;
		}


		bool isStrictlyIncrease(vector<int>& arr)
		{
			auto it = adjacent_find(arr.begin(), arr.end(), 
				[](int const & a, int const & b) {
					return a >= b;
				});
			return it == arr.end();
		}

	    vector<int> lengthOfLIS_bruteForce(vector<int> const & nums)
		{
			int len = nums.size();
			int last = static_cast<int>(pow(2, len));
			int longestLen = 1;
			vector<int> answer;
			for (int i = 0; i < last; i++)
			{
				bitset<32> bi(i);
				vector<int> subseq;
				for (int j = 0; j < len; j++)
				{
					if (bi[len-j-1] == 1)
						subseq.emplace_back(nums[j]);
				}
				if (subseq.size()>longestLen && isStrictlyIncrease(subseq))
				{
					longestLen = subseq.size();
					answer = subseq;
				}
				subseq.clear();
			}
			return answer;
		}

		bool runOneTest()
		{
			vector<int> const& nums = generateArray();

			vector<int> const& bruteAnswer = lengthOfLIS_bruteForce(nums);
			
			vector<int> const& fast_answer_elements_indexes = s.longestIncreaseSubseq_AnswerReconstruct(nums);
			vector<int> fast_answer;
			
			fast_answer.resize(fast_answer_elements_indexes.size());
			transform(fast_answer_elements_indexes.begin(), fast_answer_elements_indexes.end(), fast_answer.begin(),
				[&](auto& index)
				{return nums[index]; });

			
			for (auto const& i : nums)
				cout << i << ' ';
			cout << "\n";

			for (auto const& i : bruteAnswer)
				cout << i << ' ';
			cout << "\n";
			for (auto const& i : fast_answer)
				cout << i << ' ';
			cout << "\n--------------------------------------\n";
			
			if (bruteAnswer.size() != fast_answer.size())
			{
				return false;
			}

			return true;
		}

		void runTests()
		{
			for (int i = 0; i < 10; i++)
				if (!runOneTest())
				{
					cout << "ERROR !!!\n";
					break;
				}

			cout << "all pass\n";
		}
	};

	void Test_0300_Longest_Increasing_Subsequence()
	{
		int test;
		vector<int> nums;
		string numsInString;
		
		Solution2 s;
		AutoTest ate;
		while (true)
		{
			cout << "run test? (1) auto (2) manual (0) quit ";
			cin >> test;
			if (test == 0) break;
			if (test == 2)
			{
				nums.clear();
				numsInString.clear();
				cin.ignore();
				cout << "array: ";
				getline(cin, numsInString);
				istringstream ss(numsInString);
				copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
				cout << s.lengthOfLIS(nums) << "\n\n";
			}
			else if (test == 1)
			{
				ate.runTests();
			}
		}
	}
}