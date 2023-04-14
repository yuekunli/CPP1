#include "LibHeaders.h"

namespace _1345_Jump_Game_IV {

using namespace std;

class Solution
{
	// This solution uses a dynamic programming approach in the top-down fashion with recursive calls.
	// 
	// But there is a fatal flaw in this approach:
	// The sub-problems and origianl problems (in dynamic programming terminology) aren't separated.
	// For example I'm on index 5, let's say the value at index 12 is the same as the one at index 5.
	// So now I say I have 3 sub-problems: 
	//     (1). what is the minimum steps when I stand on index 4
	//     (2). what is the minimum steps when I stand on index 6 
	//     (3). what is the minimum steps when I stand on index 12.
	// But when I stand on index 4, the minimum steps needed when I step on index 5 becomes a sub-problem
	// of that. Therefore a problem and a sub-problem depend on each other.  
	// 
	// for an example:  100 -23 -23 404 100 23 23 23 3 404
	// by the exact implementation in this solution, I must not left jump by 1 step at the beginning, because I start from index 0
	// and "right jump by 1" gets executed. and I put index 0 in the "visited" set. Then I'm standing on index 1 but I still won't
	// try left jumping by 1 because index 0 is in the visited set. When I get to "100" from "404", I again won't try left jump
	// because 404 is put in the visited set. Eventually I put a non-optimal value in the record book for "100" at index 4
	// This implementation gives an answer of 4 instead of 3. This answer consists a series of jumps as the following:
	// 0 -> 1 -> 2 -> 3 -> 9
	// I can't not track visited indexes because if I don't I'll be in dead loop.

public:

	int countJumps(int *rb, unordered_set<int>&visited, vector<int>& arr, unordered_map<int, vector<int>>& valueToIdx, int currentIdx)
	{
		if (rb[currentIdx] != -1)
			return rb[currentIdx];

		int left1 = 50'000; // don't use numeric_limits<int>::max() because I assign "left1" to 1 + result of a recrusive call
                            // what if that recrusive call returns max int, then what does 1 + max int become?
		if (currentIdx - 1 >= 0 && visited.find(currentIdx - 1) == visited.end())
		{
			visited.insert(currentIdx - 1);
			left1 = 1 + countJumps(rb, visited, arr, valueToIdx, currentIdx - 1);
		}
		int right1 = 50'000;
		if (currentIdx + 1 < arr.size() && visited.find(currentIdx + 1) == visited.end())
		{
			visited.insert(currentIdx + 1);
			right1 = 1 + countJumps(rb, visited, arr, valueToIdx, currentIdx + 1);
		}

		int minJumps = min({ left1, right1 });

		if (valueToIdx.find(arr[currentIdx]) != valueToIdx.end())
		{
			int minForThisChoice = 50'000;
			for (auto& i : valueToIdx[arr[currentIdx]])
			{
				if (i != currentIdx && visited.find(i) == visited.end())
				{
					visited.insert(i);
					minForThisChoice = 1 + countJumps(rb, visited, arr, valueToIdx, i);
					if (minForThisChoice < minJumps)
						minJumps = minForThisChoice;
				}
			}
		}
		rb[currentIdx] = minJumps;
		return minJumps;
	}

	int minJumps(vector<int>& arr)
	{
		int len = arr.size();
		unordered_map<int, vector<int>> m;
		int idx = 0;
		for (auto& i : arr)
		{
			m[i].emplace_back(idx);
			idx++;
		}

		int* rb = new int[len]; // record book
		for (int i = 0; i < len; i++)
			rb[i] = -1;
		rb[len - 1] = 0;

		unordered_set<int> visited;
		visited.insert(0);
		int answer = countJumps(rb, visited, arr, m, 0);
		
		delete[] rb;

		return answer;
	}
};

class Solution2
{
	// this question should be treated as a undirected graph single source shortest path problem.
	// with all path having equal weight, breadth-first-search ought to work.
public:
	int minJumps(vector<int>& arr)
	{
		int len = arr.size();
		unordered_map<int, vector<int>> m;
		int index = 0;
		for (auto& value : arr)
		{
			m[value].emplace_back(index);
			index++;
		}

		unordered_set<int> visited;

		vector<int> q[2];
		vector<int>* currentQ = &q[0];
		vector<int>* nextQ = &q[1];

		currentQ->emplace_back(0);

		int jumps = 0;

		while (true)
		{
			int found = false;
			for (auto& idx : (*currentQ))
			{
				if (idx == len - 1)
				{
					found = true;
					break;
				}
				if (idx - 1 >= 0 && visited.find(idx - 1) == visited.end())
				{
					nextQ->emplace_back(idx - 1);
					visited.insert(idx - 1);
				}
				if (idx + 1 < len && visited.find(idx + 1) == visited.end())
				{
					nextQ->emplace_back(idx + 1);
					visited.insert(idx + 1);
				}
				if (m.find(arr[idx]) != m.end())
				{
					for (auto& sameValueIdx : m[arr[idx]])
					{
						if (visited.find(sameValueIdx) == visited.end())
						{
							nextQ->emplace_back(sameValueIdx);
							visited.insert(sameValueIdx);
						}
					}
				}
			}

			if (found) break;

			jumps++;
			currentQ->clear();
			swap(currentQ, nextQ);
		}

		return jumps;
	}
};

void Test_1345_Jump_Game_IV()
{
	Solution2 s;
	vector<int>arr;
	string s1;
	while (true)
	{
		s1.clear();
		cout << "array: ";
		getline(cin, s1);
		if (s1.size() == 0) break;
		stringstream ss(s1);
		arr.clear();
		copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(arr));
		cout << s.minJumps(arr) << '\n';
	}
}

}