#include<iostream>
#include<sstream>
#include<vector>
#include<unordered_map>
#include<unordered_set>

namespace _0128_Longest_Consecutive_Sequence {

	using namespace std;

	/*
	* Both solutions are accepted, Solution2 is better in terms of time and space
	*/

	class Solution
	{
	public:
		int longestConsecutive(vector<int>& nums)
		{
			unordered_map<int, int>startToEnd;
			unordered_map<int, int>endToStart;
			unordered_set<int> seen;
			int maxLength = 1;

			if (nums.size() == 0)
				return 0;

			for (int& n : nums)
			{
				if (!seen.contains(n))
				{
					if (startToEnd.contains(n + 1) && endToStart.contains(n - 1))
					{
						int start = endToStart[n - 1];
						int end = startToEnd[n + 1];
						startToEnd.erase(n + 1);
						endToStart.erase(end);
						startToEnd.erase(start);
						endToStart.erase(n - 1);
						startToEnd[start] = end;
						endToStart[end] = start;
						if (maxLength < (end - start + 1))
							maxLength = end - start + 1;
					}
					else if (startToEnd.contains(n + 1))
					{
						int end = startToEnd[n + 1];
						startToEnd.erase(n + 1);
						
						startToEnd[n] = end;
						endToStart[end] = n;
						if (maxLength < (end - n + 1))
							maxLength = end - n + 1;
					}
					else if (endToStart.contains(n - 1))
					{
						int start = endToStart[n - 1];
						endToStart.erase(n - 1);
						endToStart[n] = start;
						startToEnd[start] = n;
						if (maxLength < n - start + 1)
							maxLength = n - start + 1;
					}
					else
					{
						startToEnd[n] = n;
						endToStart[n] = n;
					}
					seen.insert(n);
				}
			}
			return maxLength;
		}
	};

	class Solution2
	{
	public:
		int longestConsecutive(vector<int>& nums)
		{
			unordered_set<int>s = unordered_set<int>(nums.begin(), nums.end());
			int maxLength = 0;
			for (int& i : nums)
			{
				int attemptedLength = 0;
				int n = i;
				while (s.contains(n))
				{
					attemptedLength++;
					s.erase(n);
					n++;
				}
				n = i - 1;
				while (s.contains(n))
				{
					attemptedLength++;
					s.erase(n);
					n--;
				}
				maxLength = max(maxLength, attemptedLength);
			}
			return maxLength;
		}
	};


	void Test_0128_Longest_Consecutive_Sequence()
	{
		Solution2 solu;
		while (true)
		{
			cout << "array  >  ";		
			string s1;
			vector<int> nums;
			getline(cin, s1);
			if (s1.size() == 0)
				return;
			stringstream ss(s1);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << solu.longestConsecutive(nums) << '\n';
		}
	}
}