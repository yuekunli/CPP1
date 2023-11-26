#include<vector>
#include<iostream>
#include<string>

namespace _0132_Palindrome_Partitioning_II {

	using namespace std;

	class Solution
	{
		size_t n;

		int solve(string& s, vector<vector<bool>>& isPalin, vector<int>& r, size_t index)
		{
			if (index == n)  // actually not needed, if the tail is a palindrome, it's taken care of later in the "for" loop
				return 0;

			if (r[index] != -1)
				return r[index];

			int minimumCuts = numeric_limits<int>::max();
			for (size_t i = index; i < n; i++)
			{
				if (i == index || (i - index == 1 && s[i] == s[index]) || (s[i] == s[index] && isPalin[index + 1][i - 1]))
				{
					isPalin[index][i] = true;
					if (i == n - 1)
					{
						r[index] = 0;
						return 0;
					}
					int cutsForTheRest = solve(s, isPalin, r, i + 1);
					minimumCuts = min(minimumCuts, (1 + cutsForTheRest));
				}
			}
			r[index] = minimumCuts;
			return r[index];
		}

	public:
		int minCut(string s)
		{
			n = s.size();
			vector<vector<bool>> isPalin(n, vector<bool>(n, false));

			for (size_t i = 0; i < n; i++)
			{
				isPalin[i][i] = true;
			}

			vector<int>minimumCutsRecord(n, -1);  // value in cell#7 means when I stand on character#7 in original string
			                                  // that the minimum cuts needed to divide the substring from that character (including it) to the end
			return solve(s, isPalin, minimumCutsRecord, 0);
		}
	};


	// Can I use a breadth first search algorithm?
	/*
	*                        no-cut
	*      cut @1     cut@2     ..............   cut@n-1
	*     /   |  \
	* cut@2  @3   @4
	* 
	* probably not.
	* 
	* I can check all the substrings whether they are palindromes or not by spending n^2 time.
	* For example, check all substrings with length=1, check all substrings with length=2, with length=3, ...
	* But after knowing all these substrings, I still don't know where to cut when given a substring.
	* For example, for the substring [5, 41], maybe the best way to cut is [5, 13], [14, 22], [23, 35], [36, 41].
	* wherever I make the first cut, the two halves may not be palindromes
	*/


	void Test_0132_Palindrome_Partitioning_II()
	{
		Solution s;
		string s1;
		while (true)
		{
			s1.clear();
			cout << "string: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			cout << s.minCut(s1) << "\n\n";
		}
	}
}