#include "LibHeaders.h"

namespace _1312_Minimum_Insertion_Steps_to_Make_a_String_Palindrome {

using namespace std;

static bool is_palindrome(const std::string_view& s)
{
	return std::equal(s.begin(), s.begin() + s.size() / 2, s.rbegin());
}


class Solution
{
public:

	// I never need to add a character so that it becomes the middle of the new palindrom.
	// for example if this is the palindrome after insertion:
	// ... a ...   if 'a' is inserted, it's still a palindrome even without it.
	// ... a a ... if one of the a's is inserted, it's still a palindrome even without it.
	//              if both a's are inserted, I certainly don't need them.
	// if input is "aab", using the existing gap between the two a's yields a better solution than
	// any by using an existing character as the middle.

	// a b c d x | y z z y x 
	//           |    |

	// if I use the gap between two z's, I need 4 insertions (inserting dcba to the right)
	// If I use the middle gap, I need to insert yzzy to the left and do abcd to the right.

	// so the first step is to find the longest palindrome.

	// but it won't work with this case: "leetcode", whose longest palindrome is "ee"
	// If I use "ee" as the base, I need to add "tcode" to the left and add 'l' to the right

	// find the longest subsequence that is a palindrome
	// subsequence: maintaining order but can be discontinuous

	int minInsertions(string s)
	{
		int l = s.size();
		
		// I can add a check to see if the string is already palindrome
		// but I want to test the algorithm

		vector<vector<int>> t(l, vector<int>(l));

		for (int i = 0; i < l; i++)
			t[i][i] = 1;

		for (int segLength = 2; segLength <= l; segLength++)
		{
			for (int i = 0; i <= l - segLength; i++)
			{
				int endIndex = i + segLength - 1;
				if (s[i] == s[endIndex])
					t[i][endIndex] = t[i + 1][endIndex - 1] + 2;
				else
				{
					t[i][endIndex] = max(t[i + 1][endIndex], t[i][endIndex - 1]);
				}
			}
		}
		return l - t[0][l - 1];  // all the characters that don't make up the longest palindromic subsequence need to be paired
	}
};


void Test_1312_Minimum_Insertion_Steps_to_Make_a_String_Palindrome()
{
	Solution s;
	int answer;

	answer = s.minInsertions("zzazz");
	std::cout << answer << '\n';

	answer = s.minInsertions("mbadm");
	std::cout << answer << '\n';

	answer = s.minInsertions("leetcode");
	std::cout << answer << '\n';

	answer = s.minInsertions("abcdxyzzyx");
	std::cout << answer << '\n';
}

}