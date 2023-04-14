#include "LibHeaders.h"

namespace _1340_Jump_Game_V {

using namespace std;

class Solution
{
public:

	int countJumps_Recur(int* rb, vector<int>& arr, int d, int idx)
	{
		if (rb[idx] != -1)
			return rb[idx];

		int maxIndicesFromHereOn = 0;
		int maxIndicesForThisChoice = 0;
		for (int i = 1; i <= d; i++)
		{
			if (idx - i >= 0 && arr[idx - i] < arr[idx])
			{
				maxIndicesForThisChoice = 1 + countJumps_Recur(rb, arr, d, idx - i);
				if (maxIndicesFromHereOn < maxIndicesForThisChoice)
					maxIndicesFromHereOn = maxIndicesForThisChoice;
			}
			else if (idx - i >= 0 && arr[idx - i] >= arr[idx])
				break;
		}
		for (int i = 1; i <= d; i++)
		{
			if (idx + i < arr.size() && arr[idx + i] < arr[idx])
			{
				maxIndicesForThisChoice = 1 + countJumps_Recur(rb, arr, d, idx + i);
				if (maxIndicesFromHereOn < maxIndicesForThisChoice)
					maxIndicesFromHereOn = maxIndicesForThisChoice;
			}
			else if (idx + i < arr.size() && arr[idx + i] >= arr[idx])
				break;
		}
		rb[idx] = maxIndicesFromHereOn;
		return maxIndicesFromHereOn;
	}

	pair<int,int> maxJumps(vector<int>& arr, int d)
	{
		int len = arr.size();
		int* rb = new int[len];  // rb: record book
		for (int i = 0; i < len; i++)
			rb[i] = -1;
		
		int maxIndices = 0;
		int startForMax = -1;
		for (int i = 0; i < len; i++)
		{
			int maxIndicesForThisStart = 1 + countJumps_Recur(rb, arr, d, i);
			if (maxIndices < maxIndicesForThisStart)
			{
				maxIndices = maxIndicesForThisStart;
				startForMax = i;
			}
		}
		delete[] rb;
		return { maxIndices, startForMax };
	}
};


class Solution2
{
	/*
	* This solution tries to do dynamic programming in the bottom-up fashion.
	* Sort the heights first, and try starting points from lowest to highest.
	* When I try to start at a certain index, it must have had all its surrounding
	* indexed with lower heights solved.
	*/
public:
	pair<int, int> maxJumps(vector<int>& arr, int d)
	{
		int len = arr.size();
		vector<int> indices(len);
		iota(indices.begin(), indices.end(), 0);
		sort(indices.begin(), indices.end(),
			[&](int& a, int& b)
			{
				return arr[a] < arr[b];
			});

		vector<int> maxIndicesCanBeVisited(len);
		int startIndexWhenMaxAchieved = indices[0];
		int maxOverall = 1;
		for (int i = 0; i < len; i++)
		{
			int id = indices[i];
			int maxBySuchFirstJump = 1; // if I don't jump at all, I can at least say I have visited this index
			for (int j = 1; j <= d; j++)
			{
				if (id - j >= 0 && arr[id - j] < arr[id])
				{
					maxBySuchFirstJump = 1 + maxIndicesCanBeVisited[id - j];
					if (maxBySuchFirstJump > maxIndicesCanBeVisited[id])
					{
						maxIndicesCanBeVisited[id] = maxBySuchFirstJump;
					}
				}
				else
					break;
			}
			for (int j = 1; j <= d; j++)
			{
				if (id + j < len && arr[id + j] < arr[id])
				{
					maxBySuchFirstJump = 1 + maxIndicesCanBeVisited[id + j];
					if (maxBySuchFirstJump > maxIndicesCanBeVisited[id])
					{
						maxIndicesCanBeVisited[id] = maxBySuchFirstJump;
					}
				}
				else
					break;
			}
			if (maxIndicesCanBeVisited[id] > maxOverall)
			{
				maxOverall = maxIndicesCanBeVisited[id];
				startIndexWhenMaxAchieved = id;
			}
		}
		return {maxOverall, startIndexWhenMaxAchieved};
	}
};

void Test_1340_Jump_Game_V()
{
	Solution s;
	int d;
	string s1;
	vector<int> arr;
	while (true)
	{
		s1.clear();
		cout << "array: ";
		getline(cin, s1);
		if (s1.size() == 0) break;
		stringstream ss(s1);
		arr.clear();
		copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(arr));
		cout << "d: ";
		cin >> d;
		cin.ignore();
		auto [maxIdx, start] = s.maxJumps(arr, d);
		cout << "max indices: " << maxIdx << "  starting index: " << start << '\n';
	}
}

}