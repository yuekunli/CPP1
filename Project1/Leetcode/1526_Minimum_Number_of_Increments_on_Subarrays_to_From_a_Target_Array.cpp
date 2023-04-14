#include "LibHeaders.h"

namespace _1526_Minimum_Increments_Subarrays_to_Target_Array {

	using namespace std;

	class Solution1
	{
	public:

		int solvePeaksVallies(vector<int>& p, vector<int>& v)
		{
			if (p.size() != v.size() + 1)
				throw runtime_error("peaks and vallies not match 2");
			if (p.size() == 1)
				return p[0];

			auto minOfV = min_element(v.begin(), v.end());

			vector<int>peaks;
			vector<int>vallies;

			int heightOfMinValley = *minOfV;
			int offset = distance(v.begin(), minOfV);

			/*
			*  p0     p1     p2     p3     p4     p5     p6
			*      v0     v1     v2     v3     v4     v5
			*
			*  if v1 is the minimum of vallies
			*
			*
			*/

			transform(p.begin(), p.begin() + offset + 1, back_inserter(peaks),
				[&](int const& a)
				{return a - heightOfMinValley; });

			transform(v.begin(), v.begin() + offset, back_inserter(vallies),
				[&](int const& a)
				{return a - heightOfMinValley; });

			int result_left = solvePeaksVallies(peaks, vallies);

			peaks.clear();
			vallies.clear();

			transform(p.begin() + offset + 1, p.end(), back_inserter(peaks),
				[&](int const& a)
				{return a - heightOfMinValley; });

			transform(v.begin() + offset + 1, v.end(), back_inserter(vallies),
				[&](int const& a)
				{return a - heightOfMinValley; });

			int result_right = solvePeaksVallies(peaks, vallies);

			return heightOfMinValley + result_left + result_right;
		}

		int minNumberOperations(vector<int>& target)
		{
			int len = target.size();
			if (len == 1) return target[0];
			if (len == 2) return max({ target[0], target[1] });
			bool newMount = true;
			bool ascOrDesc = target[1] >= target[0] ? true : false;
			vector<int> peaks;
			vector<int> vallies;
			int high = ascOrDesc ? target[1] : target[0];
			int low = ascOrDesc ? 0 : target[1];
			for (int i = 2; i < len; i++)
			{
				if (target[i] >= target[i - 1] && ascOrDesc)
				{
					high = target[i];
					continue;
				}
				if (target[i] < target[i - 1] && !ascOrDesc)
				{
					low = target[i];
					continue;
				}
				if (target[i] >= target[i - 1] && !ascOrDesc)
				{
					vallies.push_back(low);
					high = target[i];
					ascOrDesc = true;
					continue;
				}
				if (target[i] < target[i - 1] && ascOrDesc)
				{
					peaks.push_back(high);
					low = target[i];
					ascOrDesc = false;
					continue;
				}
			}
			if (ascOrDesc)
				peaks.push_back(high);

			if (peaks.size() != vallies.size() + 1)
				throw runtime_error("peaks and vallies not match 1");

			return solvePeaksVallies(peaks, vallies);
		}
	};

	// Solution and Solution1 is not tested.

	class Solution2
	{
	public:

		int vallyOpCount{ 0 };

		void solveVallies(vector<int>::iterator first, vector<int>::iterator last)
		{
			//static int opCount = 0;  
			// if I use a static variable inside this function, 
			// it's good for the first use, 
			// I can't do multiple tests with the same one Solution3 object

			if (first == last) return; //return 0;
			if (distance(first, last) == 1)
			{
				vallyOpCount += *first; return;
				//return opCount;
			}

			auto minIt = min_element(first, last);
			vallyOpCount += *minIt;
			int heightOfMin = *minIt;
			for_each(first, last, [&](int& i) { i -= heightOfMin; });

			solveVallies(first, minIt);
			solveVallies(minIt + 1, last);

			//return opCount;
		}

		int solvePeaks(vector<int>& p, vector<int>& v)
		{
			int opCount = 0;
			int len = p.size();

			if (p.size() == 1) return p[0];

			opCount += p[0] - v[0];
			opCount += p[len - 1] - v[len - 2];
			for (int i = 1; i <= len - 2; i++)
			{
				int higherValley = max(v[i - 1], v[i]);

				opCount += (p[i] - higherValley);
			}
			return opCount;
		}

		int solvePeaksVallies(vector<int>& p, vector<int>& v)
		{
			int count1 = solvePeaks(p, v);

			vallyOpCount = 0;
			solveVallies(v.begin(), v.end());
			
			return count1 + vallyOpCount;
		}


		int minNumberOperations(vector<int>& target)
		{
			int len = target.size();
			
			if (len == 1) return target[0];
			if (len == 2) return max({ target[0], target[1] });
			
			bool ascOrDesc = target[1] >= target[0] ? true : false;
			
			vector<int> peaks;
			vector<int> vallies;
			
			int high = ascOrDesc ? target[1] : target[0];
			if (!ascOrDesc)
				peaks.push_back(high);

			int low = ascOrDesc ? 0 : target[1];
			
			for (int i = 2; i < len; i++)
			{
				if (target[i] >= target[i - 1] && ascOrDesc)
				{
					high = target[i];
					continue;
				}
				if (target[i] < target[i - 1] && !ascOrDesc)
				{
					low = target[i];
					continue;
				}
				if (target[i] >= target[i - 1] && !ascOrDesc)
				{
					vallies.push_back(low);
					high = target[i];
					ascOrDesc = true;
					continue;
				}
				if (target[i] < target[i - 1] && ascOrDesc)
				{
					peaks.push_back(high);
					low = target[i];
					ascOrDesc = false;
					continue;
				}
			}
			if (ascOrDesc)
				peaks.push_back(high);

			if (peaks.size() != vallies.size() + 1)
				throw runtime_error("peaks and vallies not match 1");

			return solvePeaksVallies(peaks, vallies);
		}
	};


	class Solution3
	{
	public:
		int minNumberOperations(vector<int>& target)
		{
			stack<int> a;
			int opCount = 0;
			a.push(0);
			for (int& height : target)
			{
				if (height > a.top())
				{
					opCount += (height - a.top());
					a.push(height);
				}
				else
				{
					while (a.top() >= height)
						a.pop();
					a.push(height);
				}
			}
			return opCount;
		}
	};

	class BruteForceSolution
	{
	public:
		int minNumberOperations(vector<int>& target)
		{
			int opCount = 0;
			int len = target.size();

			while (true)
			{
				bool hasSeenNonZeroSegment = false;
				for (int i = 0; i < len; i++)
				{
					if (target[i] > 0)
					{
						hasSeenNonZeroSegment = true;
						target[i]--;
						if (i == len - 1)
							opCount++;
					}
					else // target[i] == 0
					{
						if (hasSeenNonZeroSegment)
						{
							opCount++;
							break;
						}
						else
						{
							if (i == len - 1)
							{
								return opCount;
							}
							else
								continue;
						}
					}
				}
			}
		}
	};

	/*
	* 6,8,10,11,8,5,4,2,3,4,6,4,3,5,6,9,7,10,12,13,11,10,5,4,3,5
	*
	* peaks:      11     6     9     13      5
	* vallies:        2    3      7      3
	*
	* answer: 29
	*/

	void Test_1526_Minimum_Increments_Subarrays_to_Target_Array()
	{
		vector<int> target;
		string targetInStr;

		Solution3 s;
		//BruteForceSolution bfs;

		while (true)
		{
			target.clear();
			targetInStr.clear();
			cout << "give input array: ";
			getline(cin, targetInStr);
			if (targetInStr.size() == 0) break;
			istringstream ss(targetInStr);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(target));
			cout << s.minNumberOperations(target) << "\n\n";
		}
	}
}