#include<iostream>
#include<string>
#include<unordered_map>


namespace _1044_Longest_Duplicate_Substring {

	using namespace std;

	class Solution1
	{
		/*
		* For example the string is "a b c d e f g h i j k l m n p q r s t"
		* For example the longest duplicate substring is "defg" which repeats itself at "klmn"
		* If I run the KMP string matching preprocessing on this string "d e f g h i j k l m n p q r s t"
		* I'll find a matching situation like this:
		* d e f g h i j k l m n p q r s t
		*               d e f g
		* which means when I stand on 'n', it matches a prefix of length 4.
		* After running the pre-processing, I'll find a longest match.
		* But how do I know I should cut off "abc" and then run the pre-processing?
		* I don't know, so I have to cut off the letters one by one and run the pre-processing every time I cut off a letter.
		*/

	public:

		string longestDupSubstring(string s)
		{
			size_t len = s.size();

			int* matchPfxLen = new int[len];
			for (int i = 0; i < len; i++)
			{
				matchPfxLen[i] = 0;
			}

			int longestMatch = 0;
			int startForLongestMatch = 0;
			
			for (int start = 0; start <= len - 2; start++)
			{
				int i = start + 1;
				int k = start;
				int matchingLen = 0;
			
				// if the longest match is 4-element long so far, the length of [start, end] has to be at least
				// 6 in order to have 2 5-element substrings and if they match, they can surpass the longest match so far.
				// 0 1 2 3 4 5 6 7 8 9
				//       |
				//      start        total length = 10, when start is 3, length of [start,end] is 7 =  10 - 3
				if (len - start < longestMatch + 2)
					break;

				while (i < len)
				{
					if (s[i] == s[k])
					{
						matchingLen++;
						matchPfxLen[i] = matchingLen;

						if (matchingLen > longestMatch)
						{
							longestMatch = matchingLen;
							startForLongestMatch = start;
						}

						i++; k++;
					}
					else
					{
						/*
						*   0  1  2  3  4  5  6  7  8  9
						*         |           2  3  X
						*                        2
						* 
						* start = 2, when i = 7, matchingLen = 2
						* when i = 8, s[8] doesn't match s[4]
						* if the matchingLen for when i = 3 is 1
						* next k to try is 3
						* 
						*/
						if (k != start)
						{
							matchingLen = matchPfxLen[k - 1];
							k = matchingLen + start;
						}
						else
							i++;
					}
				}

			}

			delete[] matchPfxLen;

			if (longestMatch > 0)
				return s.substr(startForLongestMatch, longestMatch);
			else
				return "";
		}
	};


	class Solution2 //accepted
	{
		/*
		* if I already found that there are two 5-element substrings that match,
		* there must also be 4-element substrings that match, also be 3-element substrings that match, so on so forth.
		* if the total length is 10, the maximum length of substrings that can match is 9.
		* So I know there are 5-element substrings that match, I should try to look for 8-element substrings, if that fails
		* I try between 5 and 8. This is a binary search approach.
		*/

		/*
		* What is Rabin-Karp algorithm?
		* Turn each substring into a number, each digit multiply 26^x, then modulus a big prime such as 1'000'000'007
		* 
		*/

	public:

		bool compareTwoStrings(string&s, size_t s1, size_t s2, int length)
		{
			return equal(s.begin()+s1, s.begin()+s1+length, s.begin()+s2);
		}

		string findDupSubstringForCertainLength(string& s, int subStringLength)
		{
			size_t fullLength = s.size();
			unordered_map<long long, size_t> seen;
			int modBase = 1'000'000'007;
			//int highestPositionWeight = (static_cast<int>(pow(26, subStringLength - 1))) % modBase;

			long long highestPositionWeight = 1;
			for (int i = 1; i < subStringLength; i++)
			{
				highestPositionWeight *= 26;
				highestPositionWeight %= modBase;
			}

			// total length is 10, index 0 to 9, substring length 4, start can be at 6, 
			long long hash = 0;
			for (size_t start = 0; start <= fullLength - subStringLength; start++)
			{
				if (start == 0)
				{
					for (int i = 0; i < subStringLength; i++)
					{
						hash *= 26;
						hash %= modBase;
						hash += s[i] - 'a';
						hash %= modBase;
					}
				}
				else
				{
					// Note the example on "Introduction to Algorithms  -- Cormen, Leiserson, ..." page 992
					// this part "old hash - (high digit * high position weight)" should especially be applied with the modulu operation
					int c = s[start - 1] - 'a';
					hash -= c * highestPositionWeight;  // this cat cause "hash" to become negative
					while (hash < 0)
						hash += modBase;
					hash *= 26;
					hash %= modBase;
					hash += s[start + subStringLength - 1] - 'a';
					hash %= modBase;
				}
				if (seen.find(hash) != seen.end())
				{
					size_t startOfAnother = seen[hash];
					if (compareTwoStrings(s, startOfAnother, start, subStringLength)) 
						// if I use "26" as the coefficient and *don't* do modulus operation, I don't need to do a char-by-char compare
						// it's just like for example, if two decimal numbers are equal, they have to have every digit equal.
						// but if I do do modulus operation, the char-by-char compare is necessary
					{
						return s.substr(start, subStringLength);
					}
				}
				else
				{
					// there is a flaw!
					// what if two different substrings both produce the same hash
					// and I just found a third substring that produces the same hash again!
					// I need to compare the third substring with each of the previous two
					// so the map should really be unordered_map<long, vector<int>>
					// it needs to store all the substrings producing the same hash.
					// The original Rabin-Karp algorithm is to compare each substring of a string
					// against a pattern, so it doesn't have this problem.
					seen[hash] = start;
				}
			}
			return "";
		}

		string longestDupSubstring(string s)
		{
			size_t len = s.size();

			int attemptLengthLowerLimit = 1;
			int attemptLengthUpperLimit = (int)len-1;

			string answer;  // return *any* such longest duplicated substring

			while (attemptLengthLowerLimit <= attemptLengthUpperLimit)
			{
				int attemptLength = (attemptLengthLowerLimit + attemptLengthUpperLimit) / 2;   //  a + (b-a) / 2  =  a + b/2 - a/2  =  b/2 + a/2  =  (a + b)/2 
				string tempAnswer = findDupSubstringForCertainLength(s, attemptLength);
				if (tempAnswer != "")
				{
					attemptLengthLowerLimit = attemptLength + 1;
					answer = tempAnswer;
				}
				else
					attemptLengthUpperLimit = attemptLength - 1;
			}
			return answer;
		}
	};


	void Test_1044_Longest_Duplicate_Substring()
	{
		Solution2 s;

		while (true)
		{
			string st;
			cout << "string: ";
			getline(cin, st);
			if (st.size() == 0) break;
			cout << s.longestDupSubstring(st) << '\n';
		}
	}
}