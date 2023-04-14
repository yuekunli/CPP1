#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<random>
#include<numeric>
#include<bitset>
#include<functional>

namespace _0910_Smallest_Range_II {

	using namespace std;

	class Solution
	{
	public:

		/*
		I may not need the average of the entire array, maybe I just need the average between min and max
		The error of this solution is that when an elements is changed (by either +k or -k), if it was the
		min or max, then min (or max) is also changed
		*/
		int smallestRangeII(vector<int>& nums, int k)
		{
			int s = accumulate(nums.begin(), nums.end(), 0);
			size_t len = nums.size();

			//int quo = s / len;
			//int rem = s % len;
			double avg = (double)s / (double)len;

			int mini = numeric_limits<int>::max();
			int maxi = 0;

			vector<int> withinRadius;

			for_each(nums.begin(), nums.end(), 
				[&](int const & i) 
				{
					if ((double)i <= avg - (double)k)
					{
						// if the array has only 1 element, I need to update both mini and maxi
						if (mini > i + k)
							mini = i + k;
						if (maxi < i + k)
							maxi = i + k;
					}
					else if ((double)i >= avg + (double)k)
					{
						if (mini > i - k)
							mini = i - k;
						if (maxi < i - k)
							maxi = i - k;
					}
					else
					{
						withinRadius.emplace_back(i);
					}
				});

			if (withinRadius.size() == nums.size())
			{
				// everything is with in the radius
				auto [minIter, maxIter] = minmax_element(nums.begin(), nums.end());
				mini = *minIter;
				maxi = *maxIter;
			}

			for_each(withinRadius.begin(), withinRadius.end(),
				[&](int const& i)
				{
					if (i + k <= maxi || i - k >= mini)
					{
						// no nothing
					}
					else if (mini - (i - k) >= (i + k) - maxi)
					{
						maxi = i + k;
					}
					else if (mini - (i - k) < (i + k) - maxi)
					{
						mini = i - k;
					}
				});
			return maxi - mini;
		}
	};


	class Solution2
	{
	public:

		pair<int, vector<int>> smallestRangeII(vector<int>& nums, int k)
		{
			auto [minIter, maxIter] = minmax_element(nums.begin(), nums.end());
			int mini = *minIter;
			int maxi = *maxIter;
			float avg = ((float)mini + (float)maxi) / 2.0f;
			mini = numeric_limits<int>::max();
			maxi = 0;
			
			vector<int> answer;
			for_each(nums.begin(), nums.end(),
				[&](int& n)
				{
					if ((float)n <= avg)
					{
						if (maxi < n + k)
							maxi = n + k;
						if (mini > n + k)
							mini = n + k;
						answer.emplace_back(n + k);
					}
					else
					{
						if (maxi < n - k)
							maxi = n - k;
						if (mini > n - k)
							mini = n - k;
						answer.emplace_back(n - k);
					}

				});
			return { maxi - mini, answer };
		}
	};


	class Solution3  // accepted
	{
	public:

		/*
		* if every element is moved toward the same direction, the score will just be the same as it is when
		* none is moved. Move every element to the right (+k), "fold" down one by one from the maximum, see
		* if the score changes at any point of this process.
		* The key is that every element must be moved, either +k or -k, none can stay put.
		* Therefore if I move every element to the right (+k), the best chance to lower the score is by
		* folding down bigger elements one by one.
		*/
		int smallestRangeII(vector<int>& nums, int k)
		{
			sort(nums.begin(), nums.end());
			size_t len = nums.size();
			int score = nums[len - 1] - nums[0];
			int maxFoldedDown = nums[len - 1] - k;
			for_each(nums.begin(), nums.end(),
				[&](int& n)
				{
					n += k;
				});
			
			for (int i = (int)len - 1; i >= 0; i--)
			{
				if (i != 0)
				{
					if (nums[i] - 2 * k >= nums[0])
					{
						int potentialLowerScore = max(maxFoldedDown, nums[i - 1]) - nums[0];
						                            //\___________/  \________/
						                            //      |            |
						                            //      |            +-- maximum among all the numbers that have not been folded down
						                            //      +----------------maximum among all the numbers that have been folded down
						if (score > potentialLowerScore)
						{
							score = potentialLowerScore;
						}
					}
					else
					{
						int potentialLowerScore = max(maxFoldedDown, nums[i - 1]) - (nums[i] - 2 * k);
						if (score > potentialLowerScore)
						{
							score = potentialLowerScore;
						}
					}

					if (maxFoldedDown > nums[i - 1])
					{
						/*
						* if at some point maxFoldedDown is greater than nums[i-1], which means
						* max(maxFoldedDown, nums[i-1]) is going to yield maxFoldedDown, the subsequent iterations
						* are going to yield the same result because nums[i-1] is going to be smaller and smaller.
						* On the other hand, folding down nums[i] is going to yield smaller and smaller value.
						* so the potentialLowerScore won't become smaller.
						*/
						break;
					}
				}
				else
				{
					/*
					now I'm trying to fold down nums[0], because I have tried folding down every other one
					now if I do fold down nums[0], I'll end up folding down every one
					this is equivalent to folding down every one at the beginning
					the score is just the same as not folding any one
					then score will just be the same as "score" initial value
					if any of those "potential lower scores" is really lower, the score in this case won't matter
					*/
				}
			}
			return score;
		}
	};


	class Solution4  // Since Solution3 is accepted, I didn't materialize this idea.
	{
		/*
		* Let's assume there are odd number of elements in the input so that there is a single median.
		* It feels that all that matter are the maximum, minimum, and the median of the input array.
		* 
		* What can I do to these 3 numbers?
		* For each one of these 3, there are 2 choices, +k or -k.
		* So there are 2^3 = 8 possibilities.
		* But some of them are trivial to check, for example all +k or all -k.
		* But what is a systematic way to check these 3 elements?
		*/

	public:
		int samllestRangeII(vector<int>& nums, int k)
		{
			auto [minIter, maxIter] = minmax_element(nums.begin(), nums.end());
			int mini = *minIter;
			int maxi = *maxIter;

			size_t len = nums.size();

			if (len % 2 == 1)
			{
				// all that matter are min, max, and median

				auto midIt = nums.begin() + (len - 1) / 2;
				nth_element(nums.begin(), midIt, nums.end());
				int mid = *midIt;
				
			}
			else
			{
				// all that matter are min, max, lower_median, and upper_median
			}

			return 0;
		}
	};


	static pair<int, vector<int>> brute_force(vector<int>& nums, int k)
	{
		size_t len = nums.size();
		int last = (int)pow(2, len);
		int minScore = numeric_limits<int>::max();
		vector<int>answer(len);
		for (int i = 1; i <= last; i++)
		{
			bitset<32>bits(i - 1); // the bit at index 0 is not the left-most bit, but rather the least-significant bit, which is right-most
			int maxi = 0;
			int mini = numeric_limits<int>::max();
			int index = 0;
			vector<int>potentialAnswer(len);
			for_each(nums.begin(), nums.end(),
				[&](int& n)
				{
					// n + (-2 * bits[index] * k) + k
					// bits[index] == 0  ==>  n + k;
					// bits[index] == 1  ==>  n - k
					int new_n = n + (-2 * bits[index] * k) + k;
					if (mini > new_n)
						mini = new_n;
					if (maxi < new_n)
						maxi = new_n;
					potentialAnswer[index] = new_n;
					index++;
				});
			if (minScore > (maxi - mini))
			{
				minScore = maxi - mini;
				copy(potentialAnswer.begin(), potentialAnswer.end(), answer.begin());
			}
		}
		return { minScore, answer };
	}


	static void automated_test()
	{
		int cases = 5; // number of test cases
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(0, 50);
		vector<int> nums(10);
		Solution3 s;
		
		for (int i = 1; i <= cases; i++)
		{
			cout << "\n\ntest case " << i << "\n\n";
			
			generate(nums.begin(), nums.end(), bind(dist, ref(gen)));
			int k = gen() % 60;
			for (int& n : nums)
				cout << n << ' ';
			cout << "\n\nk: " << k << '\n';


			auto [score, arr] = brute_force(nums, k);
			
			//auto [algo_score, algo_arr] = s.smallestRangeII(nums, k);
			int algo_score = s.smallestRangeII(nums, k);
			
			if (score != algo_score)
			{
				cout << "\n\nERROR !!!!!!!!!!\n\n";
				cout << "brute force answer: " << score << '\n';
				for (int& n : arr)
					cout << n << ' ';

				cout << "\nalgorithm answer: " << algo_score << '\n';
				/*
				for (int& n : algo_arr)
					cout << n << ' ';
				*/
				cout << '\n';
				break;
			}
			else
			{
				cout << "\n\nPASS ------  min score: " << score << "\n\n";
				for (int& n : arr)
					cout << n << ' ';
				cout << "\n\n";
			}
		}
	}


	void Test_0910_Smallest_Range_II()
	{
		int test;
		int k;
		vector<int> nums;
		string numsInString;

		Solution3 s;

		while (true)
		{
			cout << "manual test (1), automated test (2), or no test (0)? ";
			cin >> test;
			if (test == 0) break;
			if (test == 2)
			{
				automated_test();
			}
			else if (test==1)
			{
				nums.clear();
				numsInString.clear();
				cout << "k: ";
				cin >> k;
				cin.ignore();
				cout << "array: ";
				getline(cin, numsInString);
				istringstream ss(numsInString);
				copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
				//auto [score, arr] = s.smallestRangeII(nums, k);
				int score = s.smallestRangeII(nums, k);
				cout << score << '\n';
				//for (int& n : arr)
				//	cout << n << ' ';
				cout << '\n';
				//cout << brute_force(nums, k) << '\n';
			}
		}
	}
}