#include "LibHeaders.h"

namespace _1048_Longest_String_Chain {

	using namespace std;

	class Solution
	{
		/*
		* Depth-first-search. Each node is a word, the edges are directional.
		* An edge goes from node1 to node2 means the word in node1 can become the word
		* in node2 by substracting a letter from word in node1.
		*/
		int solve(unordered_set<string>& s, unordered_map<string, int>& m, string& w)
		{
			if (m.count(w) > 0)
				return m[w];
			
			int n = w.size();
			
			if (n == 1)
			{
				m[w] = 1;
				return 1;
			}
			int maxLength = 1;
			for (int i = 0; i < n; i++)
			{
				string a;
				if (i < n)
					a = w.substr(0, i) + w.substr(i + 1, n - i - 1);
				else
					a = w.substr(0, i);
				if (s.count(a) > 0)
				{
					int ret = solve(s, m, a);
					maxLength = max(maxLength, ret+1);
				}
			}
			m[w] = maxLength;
			return m[w];
		}

	public:
		int longestStrChain(vector<string>& words)
		{
			unordered_set<string>s(words.begin(), words.end()); // used to check whether a given string is in the orignal input
			unordered_map<string, int> m; // records the length of the longest chain ending at each string.

			int maxLength = 1;
			for (auto& w : words)
			{
				if (m.count(w) == 0)
				{
					int len = solve(s, m, w);
					maxLength = max(maxLength, len);
				}
				else
				{
					maxLength = max(maxLength, m[w]);
				}
			}

			return maxLength;
		}
	};

	void Test_1048_Longest_String_Chain()
	{
		vector<string>words = { "a", "b", "ba", "bca", "bda", "bdca" };
		vector<string>words2 = { "xbc", "pcxbcf", "xb", "cxbc", "pcxbc" };
		Solution solu;

		cout << solu.longestStrChain(words2) << "\n\n";
	}
}