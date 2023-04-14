#include "LibHeaders.h"

namespace _2234_Max_Total_Beauty_Gardens {
	using namespace std;

	class Solution
	{

	public:

		long long maximumBeauty(vector<int>& flowers, long long newFlowers, int target, int full, int partial)
		{
			sort(flowers.begin(), flowers.end());
			auto it = lower_bound(flowers.begin(), flowers.end(), target);  // equal to or greater than target

			// corner case, every garden is already complete
			if (it == flowers.begin())
			{
				return flowers.size() * full;
			}

			int completeGardens = distance(it, flowers.end());

			int minimum = flowers[0];
			int current = flowers[0];
			int streak = 0;
			long long flowersNeeded = 0;
			long long flowersToSpend = newFlowers;
			for (int i = 0; i < flowers.size(); i++)
			{
				if (flowers[i] == current)
					streak++;
				else
				{
					int goal;
					if (flowers[i] < target)  // I didn't just run into a garden that is already complete
						goal = flowers[i];   // my goal is to fill all previous garden to this level
					else
						goal = target - 1;

					// can I increase the current minimum to the goal?
					if (flowersToSpend >= streak * (goal - current))
					{
						flowersToSpend -= streak * (goal - current);
						minimum = current = flowers[i];
						streak++;

						if (goal == target - 1)  // I just increased the minimum to 1 less than target, that's enough
							break;
					}
					else
					{
						// can I increase the current minimum by 1 or 2 or ...?
						int increase = flowersToSpend / streak;
						flowersToSpend -= increase * streak;
						minimum = minimum + increase;
						break;
					}
				}
			}

			int minimumEndIndex = streak - 1;

			long long totalMax = minimum * partial + completeGardens * full;

			it = prev(it); // it now points to the largest incomplete garden

			/*
			* What if I increased all the incomplete gardens to 1 less than the target?
			* for example: if target is 8
			*    7  7  7  7  7  8  8  8
			*                |
			*              "minimumEndIndex" and "it" would both point to this garden
			* 
			* or maybe the incomplete gardens are all increased to a certain level
			* (may not be exactly 1 less than the target), still "minimumEndIndex"
			* and "it" would both point to a garden just before the complete ones.
			*/

			int itIndex = distance(flowers.begin(), it);

			while (true)
			{
				int a; // flowers needed to make the garden, pointed to by "it", full

				if (minimumEndIndex < itIndex)
				{
					a = target - flowers[itIndex];
				}
				else
				{
					a = target - minimum;
				}

				if (flowersToSpend > a)
				{
					flowersToSpend -= a;
					completeGardens++;

					if (completeGardens == flowers.size())
						minimum = 0;  // this situation can happen if I have a lot flowers to spend
					                  // minimumEndIndex is always moved to left together with itIndex
					                  // so I never get a chance to adjust "minimum"
					                  // if every garden is brought to complete, there won't be any "minimum"
					
					int total = completeGardens * full + minimum * partial;
					if (total > totalMax) totalMax = total;
					if (itIndex==0)
						break;
					else
					{
						// minimumEndIndex and "it" point to the same garden
						// and I have enough flowers to spend to bring that garden to complete
						// I have to remove that garden from minimum streak
						if (itIndex == minimumEndIndex)
						{
							itIndex--; minimumEndIndex--;
						}
						else
							itIndex--;
					}
				}
				else
				{
					// I need to take an entire layer off minimums

					// some gardens aren't filled with any new flowers
					// they just meet the current minimum

					while (flowers[minimumEndIndex] == minimum)
					{
						if (minimumEndIndex > 0)
							minimumEndIndex--;
						else
						{
							minimumEndIndex--;
							break;
						}
					}
					if (minimumEndIndex < 0) break;

					flowersToSpend += (minimumEndIndex+1) * 1;
					minimum--;
				}
			}
			return totalMax;
		}
	};


	void Test_2234_Max_Total_Beauty_Gardens()
	{
		Solution solu;
		vector<int>flowers;
		vector<int> parameters(4);
		while (true)
		{
			string s;
			cout << "flowers: ";
			getline(cin, s);
			if (s.size() == 0) break;
			flowers.clear();
			stringstream ss(s);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(flowers));

			s.clear();
			cout << "new flowers,  target,  full,  partial: ";
			getline(cin, s);
			copy_n(istream_iterator<int>(cin), 4, parameters.begin());
			cout << solu.maximumBeauty(flowers, parameters[0], parameters[1], parameters[2], parameters[3]) << "\n";
		}
	}
}