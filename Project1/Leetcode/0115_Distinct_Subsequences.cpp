#include <string>
#include <sstream>
#include <iostream>
#include <vector>

namespace _0115_Distinct_Subsequences {

	using namespace std;

	class Solution
	{
		size_t sLength;
		size_t tLength;

		/*
		*               sIndex
		*               |
		* s:  X    X    X    X    X    X
		* t:  Y    Y    Y    Y    Y
		*                    |
		*                    tIndex
		* 
		* Consider the substrings [sIndex, end) and [tIndex, end)
		* How many distinct subsequences in [sIndex, end) are equal to [tIndex, end)
		* The length of [sIndex, end) is sLength - sIndex
		*/
		int solve(string&s, string&t, size_t sIndex, size_t tIndex, vector<vector<int>>&r)
		{
			if (sIndex <= sLength && tIndex == tLength)
				return 1;
			else if (sIndex == sLength && tIndex < tLength)
				return 0;
			
			if (r[sIndex][tIndex] != -1)
				return r[sIndex][tIndex];

			int count = 0;
			for (size_t i = sIndex; i < sLength; i++)
			{
				if (s[i] == t[tIndex])
				{
					count += solve(s, t, i + 1, tIndex + 1, r);
				}
			}
			r[sIndex][tIndex] = count;
			return count;
		}
	public:
		int numDistinct(string s, string t)
		{
			sLength = s.size();
			tLength = t.size();
			vector<vector<int>> records(sLength, vector<int>(tLength, -1));
			return solve(s, t, 0, 0, records);
		}
	};


	class Solution2
	{
		size_t sLength;
		size_t tLength;
		int solve(string& s, string& t, size_t sIndex, size_t tIndex, vector<vector<int>>& r)
		{

			if (sIndex <= sLength && tIndex == tLength)
				return 1;
			else if (sIndex == sLength && tIndex < tLength)
				return 0;

			if (r[sIndex][tIndex] != -1)
				return r[sIndex][tIndex];

			size_t tRemainLength = tLength - tIndex;
			size_t sRemainLength;
			int count = 0;
			for (size_t i = sIndex; i < sLength; i++)
			{
				sRemainLength = sLength - i;        // improvement comparing to Solution 1
				if (sRemainLength < tRemainLength)  // if the remaining length of 's' is already shorter than that of 't', no need to keep looping
					break;

				if (s[i] == t[tIndex])
				{
					count += solve(s, t, i + 1, tIndex + 1, r);
				}
			}
			r[sIndex][tIndex] = count;
			return count;
		}
	public:
		int numDistinct(string s, string t)
		{
			sLength = s.size();
			tLength = t.size();
			vector<vector<int>> records(sLength, vector<int>(tLength, -1));
			return solve(s, t, 0, 0, records);
		}
	};



	void Test_0115_Distinct_Subsequences()
	{
		string s;
		string t;
		Solution2 solu;

		string a;
		while (true)
		{
			a.clear();
			cout << "s: ";
			getline(cin, a);
			if (a.size() == 0)
				break;
			stringstream ss(a);
			ss >> s;
			cout << "t: ";
			cin >> t;
			cin.ignore();
			cout << solu.numDistinct(s, t) << "\n\n";
		}
	}
}