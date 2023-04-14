#include "LibHeaders.h"

namespace _1392_Longest_Happy_Prefix {

using namespace std;

class Solution
{
public:

	// essentially looking for the auxiliary array in Knuth's string matching algorithm

	string longestPrefix(string s)
	{
		size_t l = s.size();
		vector<int> v(l, 0);

		int i = 0, k = 1;
		while (k < s.size())
		{
			if (s[i] == s[k])
			{
				v[k] = i + 1;
				k++;
				i++;
			}
			else
			{
				if (i == 0)
				{
					k++;
				}
				else
				{
					int prefixLength = v[i - 1];
					i = prefixLength;
				}
			}
		}

		return v[l-1] == 0 ? "" : s.substr(0, v[l-1]);
	}
};


void Test_1392_Longest_Happy_Prefix()
{
	Solution s;

	cout << left << setw(10) << "level" << s.longestPrefix("level") << '\n'
		<< left << setw(10) << "ababab" << s.longestPrefix("ababab") << '\n';
}

}