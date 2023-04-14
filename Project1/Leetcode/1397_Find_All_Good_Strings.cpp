#include "LibHeaders.h"

namespace _1397_Find_All_Good_Strings {

	using namespace std;

	class Solution
	{
		/*
		* This solution is built on top of brute force. It increases s1 by 1 at a time
		* all the way till it reaches s2. It also optimizes a little. If "evil" string
		* is already present in the middle of the string under consideration, then increase
		* all the characters behind "evil" to 'z'.
		* . . . . e v i l . . .
		* For example there are 3 letters after "evil", once "evil" appears, it doesn't
		* matter what those 3 letters are. So set them to 'z', so that when I increase 1,
		* "evil" will be changed.
		*/

	public:

		// 1 ... i, i+1 ... n
		// 1 to i are the same between s1 and s2
		// i+1 is the first different bit

		// is there a match of "evil" within 1 to i?
		// is it possible to match *part* of "evil" at the end of 1 to i?
		//     1.........i
		//              evil

		// will there be match of evil in i+1 to n?

		bool isEvilInCommonPart(string& s1, string& s2, string& evil)
		{
			string::iterator mis = mismatch(s1.begin(), s1.end(), s2.begin()).first;

			if (mis - s1.begin() < evil.size())
				return false;

			//  0 1 2 3 4 5 6 7 8 9 . . . . .
			//      e v i l ^
			//              |
			//              mismatch starts here
			// mis - s1.begin() is 6, n should be less than or equal to 2

			string_view commonView(s1.data(), mis-s1.begin());
			string::size_type t = commonView.find(evil);
			if (t != string::npos)
			{
				return true;
			}
			return false;
		}

		int findGoodStrings(int n, string s1, string s2, string evil)
		{
			if (isEvilInCommonPart(s1, s2, evil))
				return 0;

			string::iterator mis = mismatch(s1.begin(), s1.end(), s2.begin()).first;
			int diffIndex = mis - s1.begin(); // since which index s1 and s2 start to differ, as I increase s1, this diffIndex moves toward the end gradually
			int i = n - 1;
			int total = 0;

			while (true)
			{
				string::size_type matchStart = s1.find(evil);  // check if evil appears
				if (matchStart == string::npos)
				{
					total++;
					total %= (int)(pow(10, 9) + 7);
				}
				else  // there is a match of evil, the first iteration certainly doesn't come inside this "else"
				{
					string::size_type matchEnd = matchStart + evil.size() - 1;

					if (matchEnd < diffIndex)
					{
						break;
					}

					if (matchEnd < s1.size() - 2)
					{
						fill(s1.begin() + matchEnd + 1, s1.end(), 'z');
					}
				}

				if (diffIndex >= n)
					break;

				// increase s1 by 1
				while (s1[i] == 'z')
				{
					s1[i] = 'a';
					i--;
				}
				s1[i]++;

				if (i == diffIndex && s1[i] == s2[i])
				{
					// ..... c a a a
					// ..... b z z z
					// diffIndex is at the 4th (counting from right)
					// increase by 1, "bzzz" becomes "caaa", diffIndex needs to be adjusted by more than 1 digit
					while (diffIndex < n && s1[diffIndex] == s2[diffIndex])
						diffIndex++;
				}
				i = n - 1;
			}
			return total;
		}
	};


	class Solution2
	{
		/*
		* The main idea of this solution is correct, it uses dynamic programming. And this solution
		* does yield correct answer. The flaws of this solution are (1) it uses too many dimensions
		* to describe the "state" of the progression. "flag" shouldn't be a dimension. (2). the way
		* it determines the flag is a little convoluted.
		*/

	public:

		int findGoodStrings(int n, string s1, string s2, string evil)
		{
			int eLen = evil.size();

			int* suffixPrefix = new int[eLen];
			for (int i = 0; i < eLen; i++)
			{
				suffixPrefix[i] = -1;
			}

			preprocessEvil(evil, suffixPrefix);

			int*** t = new int** [n];
			for (int i = 0; i < n; i++)
			{
				t[i] = new int* [eLen+1];
				for (int j = 0; j < eLen+1; j++)
				{
					t[i][j] = new int[4] {-1,-1,-1,-1};
				}
			}

			auto suffixPrefixPlus1 = new int[eLen+1][26]; 
			for (int i = 0; i < eLen+1; i++)
			{
				for (int j = 0; j < 26; j++)
				{
					suffixPrefixPlus1[i][j] = -1;
				}
			}

			int result = dfs(n, s1, s2, evil, t, suffixPrefixPlus1, suffixPrefix, 0/*index*/, 0/*lenOfEvilPrefix*/, 0/*flag*/);

			// delete memory

			delete[] suffixPrefix;
			delete[] suffixPrefixPlus1;
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < eLen; j++)
					delete[] t[i][j];
				delete[] t[i];
			}
			delete[] t;

			return result;
		}

		// flag: 0: all previous digits are equal to s1 and s2
		//       1: first deviate from s2 is less than s2
		//       2: first deviate from s1 is greater than s1 and equal to s2
		//       3: first deviate from s1 is greater then s1 and less than s2

		// coming in a call with index = 24, this means this call needs to work on the 24th (0-indexed) character
		//   ....  21   22   23   24    25  .....
		//                         |
		//                         work on this character
		//  |                 |
		//              0 1 2 3
		// the first 4 of "evil" can match the tail of 0 to 23, the passed-in lenOfEvilPrefix will 4.
		// if lenOfEvilPrefix is 0, that means nothing matches the tail of 0 to index-1

		int dfs(int n, string& s1, string& s2, string& evil, int*** t, int suffixPrefixPlus1[][26], int* suffixPrefix, 
			int index, int lenOfEvilPrefix, int flag)
		{
			if (lenOfEvilPrefix == evil.size())
				return 0;

			if (index == n)
			{
				return 1;
			}

			if (t[index][lenOfEvilPrefix][flag] != -1)
				return t[index][lenOfEvilPrefix][flag];

			int sum = 0;
			int modBase = 1'000'000'007;

			// figure out the range of possible next character
			char lo;
			char hi;

			switch (flag)
			{
				case 0:
					lo = s1[index];
					hi = s2[index];
					break;
				case 1:
					lo = s1[index];
					hi = 'z';
					break;
				case 2:
					lo = 'a';
					hi = s2[index];
					break;
				case 3:
				default:
					lo = 'a';
					hi = 'z';
					break;
			}

			for (char c = lo; c <= hi; c++)
			{
				int newFlag = 3;
				switch (flag)
				{
					case 0:
						if (c == lo && c == hi)
							newFlag = 0;
						if (c == lo && c < hi)
							newFlag = 1;
						if (c > lo && c == hi)
							newFlag = 2;
						if (c > lo && c < hi)
							newFlag = 3;
						break;
					case 1:
						if (c == lo)
							newFlag = 1;
						if (c > lo)
							newFlag = 3;
						break;
					case 2:
						if (c < hi)
							newFlag = 3;
						if (c == hi)
							newFlag = 2;
				}

				int newEvailPrefix = getNewPrefix(evil, suffixPrefix, suffixPrefixPlus1, lenOfEvilPrefix, c-'a');

				int result = dfs(n, s1, s2, evil, t, suffixPrefixPlus1, suffixPrefix, index + 1, newEvailPrefix, newFlag);
			
				sum = (sum + result) % modBase;
			}
			t[index][lenOfEvilPrefix][flag] = sum;
			return sum;
		}



		int getNewPrefix(string&evil, int* suffixPrefix, int suffixPrefixPlus1[][26], int currentLen, int character)
		{
			if (suffixPrefixPlus1[currentLen][character] != -1)
			{
				return suffixPrefixPlus1[currentLen][character];
			}

			if (character == (evil[currentLen] - 'a'))
			{
				return suffixPrefixPlus1[currentLen][character] = currentLen + 1;
			}

			if (currentLen == 0) // currentLen is 0 and the new character doesn't match the start of evil
			{
				return suffixPrefixPlus1[currentLen][character] = 0;
			}


			int matchedPrefixIdx = suffixPrefix[currentLen-1];

			if (character == (evil[matchedPrefixIdx + 1]-'a'))
			{
				suffixPrefixPlus1[currentLen][character] = matchedPrefixIdx + 1 + 1;
			}
			else
			{
				while (matchedPrefixIdx != -1 && character != (evil[matchedPrefixIdx + 1]-'a'))
				{
					matchedPrefixIdx = suffixPrefix[matchedPrefixIdx];
				}
				if (matchedPrefixIdx == -1)
					suffixPrefixPlus1[currentLen][character] = character == (evil[0]-'a') ? 1 : 0;
				else
					suffixPrefixPlus1[currentLen][character] = matchedPrefixIdx + 1 + 1;
			}

			return suffixPrefixPlus1[currentLen][character];
		}


		// if suffixPrefix is initialized with 0 in every cell, then the meaning of a cell should be the *length* of a prefix
		// if it's initialized with -1 in every cell, it can mean index
		// because there is scenario where no prefix can match a prefix, '0' will cause confusion if it means to store index
		// 0 1 2 3 4 5 6 7
		// a b c _ _ _ _ _
		//               |
		//           a b c
		//           0 1 2
		// suffixPrefix[7]: when the index points to the 7th cell in the original string, 
		// now the frist 3 can match when aligned to the riht
		// What the value of suffixPrefix[7] should be?  '3' can mean the length, '2' can mean index
		// if I want the value to mean index, I must not initialize the array with all 0's.

		// I'm using suffixPrefix to store index in this practice, so they are initialized with -1

		void preprocessEvil(string& evil, int* suffixPrefix)
		{
			int i = 1; int k = 0;

			//suffixPrefix[0] = 0;
			// it's important to leave the 0th value to be -1
			// for example if I'm using this in KMP string match
			// search pattern in string s
			//       s: ...  8   9   10 ...
			// pattern:      |   |
			//               |   comparing this position
			//             this matches
			// if s[9] != pattern[9], I would then check the prefix for the tail of 0 to 8
			//
			//       s:   0   1   2
			// pattern:   |   |
			//            |   comparing this position
			//            this matches
			//
			// if s[1] != pattern[1], I would then check the prefix for the tail of 0 to 0, if that prefix's ending index is 0, 
			// (note that I'm storing the prefix's ending index in this assistant array)
			// I'll keep comparing s[1] with pattern[1] whereas I should be comparing s[1] with pattern[0]. I'll end up in dead loop
			// 
		

			while (i < evil.size())
			{
				if (evil[i] == evil[k])
				{
					suffixPrefix[i] = k;
					i++;
					k++;
				}
				else
				{
					if (k == 0)
					{
						i++;
						continue;
					}
					k = suffixPrefix[k - 1] + 1;	
				}
			}
		}
	};


	class Solution3
	{
		/*
		* This solution is built on top of Solution2, this one is cleaner and more streamlined.
		*/

		int getNewMatchingPfxLength(string& evil, vector<int>& matchingPfx, int matchedLength, int nextChar)
		{
			// 0 1 2 3 4 5 6 7 8 9
			//           |
			//           already matched  6, next to be compared is at index 6
			// if next char doesn't match the char at index 6, 
			// I want to know what is the matched length when I stand on index 5

			if (evil[matchedLength] == nextChar)
				return matchedLength + 1;
			else
			{
				while (matchedLength > 0 && evil[matchedLength] != nextChar)
				{
					matchedLength = matchingPfx[matchedLength - 1];
				}
			}
			return evil[matchedLength] == nextChar ? matchedLength + 1 : 0;
		}

		void preprocessEvil(string& evil, vector<int>& matchingPfx)
		{
			int i = 1, k = 0;
			while (i < evil.size())
			{
				if (evil[i] == evil[k])
				{
					matchingPfx[i] = k;
					i++;
					k++;
				}
				else
				{
					if (k == 0)
					{
						i++;
					}
					else
					{
						k = matchingPfx[k - 1];
					}
				}
			}
		}

		/*
		* Meaning of these flags:
		* DownToFloor: all the previous letters are equal to s1 and at some point, the first that deviates from s2 is smaller than s2
		* UpToCeiling: all the previous letters are equal to s2 and at some point, the first that deviates from s2 is bigger than s1
		* FloorAndCeiling: all the previous letters are equal to s1 and s2
		* FreeRun: this index and afterwards can free run
		* 
		* 
		* DownToFloor:
		* 
		* s2: abcmpts
		* s1: abcdefg
		* 
		*     abcd_
		*         |
		*         "abcd" are down to s1, 'd' and 'm' deviate, and 'd' is smaller than 'm', 
		*         current position under consideration can't go too low, can't go lower than 'e',
		*         but its upper limit is not bound by s2's letter on this position.
		* 
		* UpToCeiling:
		* 
		* s2:  efghtuv
		* s1:  efgabcm
		* 
		*      efgh_
		*          |
		*          "efgh" are up to s2, 'h' and 'a' deviate, and 'h' is greater than 's'.
		*          current position under consideration can't go too high, can't go higher than 't',
		*          but its lower limit is not bound by s1's letter on this position.
		* 
		* FloorAndCeiling:
		* 
		* s2:  dupwvng
		* s1:  dupwkbc
		* 
		*      dupw_
		*          |
		*          "dupw" is down to s1 as well as up to s2, current position under consideration can't
		*          go too high or too low, it must stay between s1's and s2's letters on that position,
		*          its range is [k, v]
		* 
		*/
		enum class Flag
		{
			DownToFloor = 0,
			UpToCeiling = 1,
			FloorAndCeiling = 2,
			FreeRun = 3,
		};

		Flag getNewFlag(Flag oldFlag, char c, char lowerLimit, char upperLimit)
		{
			if (oldFlag == Flag::FreeRun)
				return Flag::FreeRun;

			if (oldFlag == Flag::FloorAndCeiling)
			{
				if (lowerLimit == c && c == upperLimit)
					return Flag::FloorAndCeiling;
				if (lowerLimit == c && c < upperLimit)
					return Flag::DownToFloor;
				if (lowerLimit < c && c < upperLimit)
					return Flag::FreeRun;
				if (lowerLimit < c && c == upperLimit)
					return Flag::UpToCeiling;
			}

			if (oldFlag == Flag::DownToFloor)
			{
				if (lowerLimit == c)
					return Flag::DownToFloor;
				if (lowerLimit < c)
					return Flag::FreeRun;
			}

			if (oldFlag == Flag::UpToCeiling)
			{
				if (c < upperLimit)
					return Flag::FreeRun;
				if (c == upperLimit)
					return Flag::UpToCeiling;
			}
			throw runtime_error("unaccounted flag situation");
		}

		int backtrackSolve(int n, string& s1, string& s2, string& evil, vector<vector<int>>& records, vector<int>&matchingPfxRecords, int index, int matchedEvilLength, Flag flag)
		{
			if (matchedEvilLength == evil.size())
				return 0;

			if (index == n)
				return 1;

			if (flag == Flag::FreeRun && records[index][matchedEvilLength] != -1)
				return records[index][matchedEvilLength];

			char lowerLimit;
			char upperLimit;
			if (flag == Flag::FloorAndCeiling)
			{
				lowerLimit = s1[index];
				upperLimit = s2[index];
			}
			else
			{
				lowerLimit = flag == Flag::DownToFloor ? s1[index] : 'a';
				upperLimit = flag == Flag::UpToCeiling ? s2[index] : 'z';
			}

			int total = 0;
			for (char c = lowerLimit; c <= upperLimit; c++)
			{
				int result;
				int newMatchedEvilLength = getNewMatchingPfxLength(evil, matchingPfxRecords, matchedEvilLength, c);
				Flag newFlag = getNewFlag(flag, c, lowerLimit, upperLimit);
				result = backtrackSolve(n, s1, s2, evil, records, matchingPfxRecords, index + 1, newMatchedEvilLength, newFlag);
				total += result;
				total %= 1'000'000'007;
			}
			return total;
		}

	public:

		int findGoodStrings(int n, string s1, string s2, string evil)
		{
			int evilLength = evil.size();
			vector<vector<int>>records(n, vector<int>(evilLength, -1));
			vector<int> evilSelfMatchingPfxLengthRecord(evilLength);
			preprocessEvil(evil, evilSelfMatchingPfxLengthRecord);
			return backtrackSolve(n, s1, s2, evil, records, evilSelfMatchingPfxLengthRecord, 0, 0, Flag::FloorAndCeiling);
		}
	};

	void Test_1397_Find_All_Good_Strings()
	{
		Solution3 s;
		int n;
		string s1;
		string s2;
		string evil;

		while (true)
		{
			s1.clear();
			cout << "s1: ";
			getline(cin, s1);

			if (s1.size() == 0) break;
		
			s2.clear();
			cout << "s2: ";
			cin >> s2;

			evil.clear();
			cout << "evil: ";
			cin >> evil;

			cout << "n: ";
			cin >> n;

			cin.ignore();

			cout << s.findGoodStrings(n, s1, s2, evil) << '\n';
		}
	}
}