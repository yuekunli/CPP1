#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <random>
#include <functional>
#pragma warning(disable: 4267 4244)


namespace _0042_Trapping_Rain_Water {

	using namespace std;

	class Solution
	{
		/*
		* If the peak in every segment is somewhat in the middle
		* run time is O(n*logn)
		*/
	public:

		int trappedAmountInSegment(vector<int>& h, int start, int end)
		{
			if (start == end - 1) return 0;

			int lower = min(h[start], h[end]);

			bool concave = true;
			int sum = 0;  // volumn occupied by the black blocks
			int hiInBetween = 0;
			int hiIndex = -1;

			for (int i = start + 1; i <= end - 1; i++)
			{
				if (h[i] > lower)
				{
					concave = false;
					//break;
				}

				sum += h[i];
				if (h[i] > hiInBetween)
				{
					hiInBetween = h[i];
					hiIndex = i;
				}
			}
		
			if (concave)
				return (end - start - 1) * lower - sum;

		
			int r1, r2;
			if (hiIndex >= start + 2)
				r1 = trappedAmountInSegment(h, start, hiIndex);
			else
				r1 = 0;

			if (end >= hiIndex + 2)
				r2 = trappedAmountInSegment(h, hiIndex, end);
			else
				r2 = 0;

			return r1 + r2;
		}

		int trap(vector<int>& height)
		{
			return trappedAmountInSegment(height, 0, height.size() - 1);
		}
	};



	class Solution2
	{
		/*
		* Keep track of downward progressions, when I start climbing an upward progression,
		* match up to the previous downward progression.
		* run time: O(n)
		*/
	public:
		int trap(vector<int>& height)
		{
			int len = height.size();
			if (len == 1) return 0;

			// find the first peak
			// I probably don't need to look for the first peak after all, if there is an upward build-up
			// at the beginning, if I start from the main loop, I'll look for the previous downward
			// progression, but there isn't one, so it won't make a difference.
			int firstPeak = -1;
			for (int i = 0; i <= len-2; i++)
			{
				if (height[i] > height[i + 1])
				{
					firstPeak = i;
					break;
				}
			}
		
			if (firstPeak == -1 || firstPeak == len - 2)
				return 0;

			vector<int> downwardProgression;
			bool upOrDown = true;
			int waterlineHeight = 0;
			int sum = 0;

			for (int i = firstPeak; i < len; i++)
			{
				if (i == firstPeak || height[i - 1] > height[i])
				{
					if (i != firstPeak && upOrDown)  // just turned downward, the previous block is a local peak
						downwardProgression.emplace_back(i - 1);
					
					downwardProgression.emplace_back(i); // if 'i' is first peak, I'm certainly turning downward, and the downward starts at this column, not the previous one.
					waterlineHeight = 0;
					upOrDown = false;
					continue;
				}

				if (height[i - 1] == height[i])
				{
					if (upOrDown)
					{  }// don't do anything?
					else
						*downwardProgression.rbegin() = i;
				}

				if (height[i - 1] < height[i])
				{
					upOrDown = true;
					if (downwardProgression.size() > 0)
					{
						auto it = downwardProgression.rbegin();
						if (height[*it] > height[i])
						{
							sum += (i - (*it) - 1) * (height[i] - waterlineHeight);
							waterlineHeight = height[i];
						}
						else
						{
							while (it != downwardProgression.rend() && height[*it] <= height[i])
							{
								sum += (i - (*it) - 1)* (height[*it] - waterlineHeight);
								waterlineHeight = height[*it];
								it = next(it);
							}
							if (it != downwardProgression.rend())
							{
								sum += (i - (*it) - 1) * (height[i] - waterlineHeight);
								waterlineHeight = height[i];
							}
							downwardProgression.erase(it.base(), downwardProgression.end());
						}
					}
				}
			}
			return sum;
		}
	};


	class Solution3
	{
		/*
		* This is just a further clean-up and tidy-up of Solution2
		*/
	public:
		int trap(vector<int>& height)
		{
			int len = height.size();

			vector<int> downwardProgression;
			bool upOrDown = true;    // true: in upward trend;   false: in downward trend
			int waterlineHeight = 0;
			int sum = 0;

			downwardProgression.push_back(0);
			upOrDown = false;

			for (int i = 1; i < len; i++)
			{
				if (height[i - 1] > height[i])
				{
					if (upOrDown)  // just turned downward, the previous column is a local peak
						downwardProgression.emplace_back(i - 1);

					downwardProgression.emplace_back(i);
					waterlineHeight = 0;
					upOrDown = false;
					continue;
				}

				if (height[i - 1] == height[i])
				{
					if (upOrDown)
					{
					}// don't do anything?
					else
						*downwardProgression.rbegin() = i;
				}

				if (height[i - 1] < height[i])
				{
					upOrDown = true;
					if (downwardProgression.size() > 0)
					{
						auto it = downwardProgression.rbegin();

						while (it != downwardProgression.rend() && height[*it] <= height[i])
						{
							sum += (i - (*it) - 1) * (height[*it] - waterlineHeight);
							waterlineHeight = height[*it];
							it = next(it);
						}
						if (it != downwardProgression.rend())
						{
							sum += (i - (*it) - 1) * (height[i] - waterlineHeight);
							waterlineHeight = height[i];
						}
						downwardProgression.erase(it.base(), downwardProgression.end());
					}
				}
			}
			return sum;
		}
	};


	class Solution4
	{
		/*
		* This is a more generalized solution.
		* Don't really need to explicitly remember whether I'm in the upward trend or downward.
		* As long as a new column is shorter or equal to the previous one, put it in the 
		* "unaccounted" group. As long as a new column is higher than the previous one,
		* Try to calculate if this one and the previous one can trap any water, and then
		* try the one before that and so on, until run into an even taller column.
		*/
	public:
		int trap(vector<int>& height)
		{
			size_t len = height.size();

			vector<size_t> unaccountedColumnIndex{ 0 };

			int trappedWaterTotal = 0;

			for (size_t i = 1; i < height.size(); i++)
			{
				auto it = unaccountedColumnIndex.rbegin();
				if (height[*it] >= height[i])
				{
					unaccountedColumnIndex.emplace_back(i);
				}
				else
				{
					size_t toErase = unaccountedColumnIndex.size() - 1;
					int waterLine = 0;
					while (it != unaccountedColumnIndex.rend() && height[*it] <= height[i])
					{
						int trappedWater = (i - *it - 1) * (height[*it] - waterLine);
						trappedWaterTotal += trappedWater;
						waterLine = height[*it];
						toErase--;
						it = next(it);
					}
					if (it != unaccountedColumnIndex.rend())
					{
						int trappedWater = (i - *it - 1) * (height[i] - waterLine);
						trappedWaterTotal += trappedWater;
						unaccountedColumnIndex.erase(unaccountedColumnIndex.begin() + toErase + 1, unaccountedColumnIndex.end());
						unaccountedColumnIndex.emplace_back(i);
					}
					else
					{
						unaccountedColumnIndex = vector<size_t>();
						unaccountedColumnIndex.emplace_back(i);
					}
				}
			}
			return trappedWaterTotal;
		}
	};


	class AutoTest
	{
		random_device rd;
		mt19937 gen;
		uniform_int_distribution<> uid;
		Solution  s1;
		Solution3 s3;
		vector<int> v;

		void generateArray()
		{
			//v.resize(20);
			generate(v.begin(), v.end(), bind(uid, ref(gen)));
		}

		bool runOneTest()
		{
			generateArray();

			int a1 = s1.trap(v);
			int a3 = s3.trap(v);

			if (a1 != a3)
			{
				for (int& i : v)
					cout << i << ' ';
				cout << '\n';
				cout << "a1: " << a1 << "     a2: " << a3 << '\n';
				return false;
			}
			return true;
		}

	public:
		AutoTest() : gen(rd()), uid(0, 15), v(14) {}

		void run(int numberOfTests)
		{
			for (int i = 0; i < numberOfTests; i++)
				if (!runOneTest())
					return;
			cout << "finished all pass\n";
		}
	};

	/*
	* Test cases to consider:
	*
	*  examples on leetcode:
	* 
	*  0  1  0  2  1  0  1  3  2  1  2  1 
	*  4  2  0  3  2  5
	*
	*  down up down up (the peak in the middle is shorter)
	* 
	*  *                 *
	*  * *        *      *
	*  * * *    * * *  * *
	*  * * *  * * * *  * *
	* 
	*  10  9  8  7  6  5  4  2  4  6  7  6  5  4  1  0  4  7  9  10  11
	* 
	*  answer should be: 86
	* 
	* 
	*/

	void Test_0042_Trapping_Rain_Water()
	{
		Solution4 s;

		int option;
		while (true)
		{
			cout << "1: manual test;  2: auto test;  0: exit  >  ";
			cin >> option;
			if (option == 0) break;
			if (option == 1)
			{
				string s1;
				vector<int> heights;
				cin.ignore();
				cout << "heights: ";
				getline(cin, s1);
				stringstream ss(s1);
				copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(heights));
				cout << s.trap(heights) << '\n';
			}
			if (option == 2)
			{
				AutoTest ats;
				ats.run(1'000'000);
			}
		}
	}
}