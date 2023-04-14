#include<iostream>


namespace _0214_Shortest_Palindrome {

	using namespace std;

	class Solution
	{
	public:


		// input: abacabax
		// answer should be: x + abacabax
		// it shouldn't be:  xabac + abacabax

		// reverse input:  xabacaba
		// a prefix of the original input (first 7 letters): abacaba
		// this prefix can match a suffix of the reverse of the input
		// reverse input:    xabacaba
		// 7-letter prefix:   abacaba

		// input   1 2 3 4 5 6 7 8 9
		// goal: find the longest substring starting from the left that is already a palindrome
		// if [1--6] is already a palindrom
		// 1 == 6, 2 == 5, 3 == 4
		// I can fold [1--6] along the gap between 3 and 4, all 3 pairs should match
		// I can also take a copy of [1-6], reverse it to make it [6--1], overlap this [6--1] on top of [1--6], all 6 pairs should match
		// If I reverse the input, I get: 9 8 7 6 5 4 3 2 1
		// take the 6-letter prefix of the original input: 1 2 3 4 5 6
		// align them in a right-just way:
		// 9 8 7 6 5 4 3 2 1
		//       1 2 3 4 5 6
		// the goal becomes finding the longest prefix of the original input that can match a suffix of the reverse of the input

		string shortestPalindrome(string s)
		{
			int i = 1, k = 0;
			size_t len = s.size();
			int* prefixSuffix = new int[len];  // this will record the length of the prefix that matches a suffix
			for (size_t j = 0; j < len; j++)
				prefixSuffix[j] = 0;


			// this is the pre-processing:

			while (i < len)
			{
				if (s[i] == s[k])
				{
					prefixSuffix[i] = k + 1; // don't declare k as size_t, prefixSuffix is an array of int.
					i++;
					k++;
				}
				else
				{
					if (k == 0)
						i++;
					else
						k = prefixSuffix[k - 1];
				}
			}


			string rev = s;
			reverse(rev.begin(), rev.end());

			i = 0; k = 0;

			while (i < len)
			{
				if (rev[i] == s[k])
				{
					i++;
					k++;
					// I don't need to worry about breaking out when they are fully matched,
					// because the string under search and the pattern are the same length.
				}
				else
				{
					if (k == 0)
						i++;
					else
						k = prefixSuffix[k - 1];
				}
			}

			delete[] prefixSuffix;

			// when I'm out of this loop, if k=5, that means the last of "rev" just matched s[4],
			// so a prefix of 5 letters can match the suffix of 5 letters in "rev"
			// so the first 5 of the original input is already a palindrome.

			// a corner case is that the entire input is already a palindrome, in that case, k = len
			if (k == len)
				return s;
			else
			{
				string addition = rev.substr(0, len - k);
				return addition + s;
			}
		}
	};


	void Test_0214_Shortest_Palindrome()
	{
		int test;
		string s;
		Solution solu;
		while (true)
		{
			cout << "run test? ";
			cin >> test;
			if (test == 0) break;
			s.clear();
			cout << "string: ";
			cin >> s;
			cout << solu.shortestPalindrome(s) << '\n';
		}
	}
}