#include<iostream>
#include<cstdint>
#include<bitset>
#include<algorithm>
#include<string>
#include<vector>

namespace _0600_Non_negative_Integers_without_Consecutive_Ones {

	using namespace std;


	/*  the number of complying integers in [0, 1000 0000]
	* = the number of complying integers in [0,  111 1111] + 1
	*                                       |                |
	*  +------------------------------------+            because 1000 0000 is a complying one
	*  |
	* the number of complying integers in   [0, 111 1111]
	* = the number of complying integers in [0, 100 0000] +
	*   the number of complying integers in    [100 0000   
	*                                           101 1111]
	*                           0
	*                             \
	*                         ...  this range has some valid numbers
	*                             /
	*                    100 0000  this is valid
	*                             \
	*                         ...  this range has some valid numbers, this range can be treated as if 0 -- 1 1111
	*                             /
	*                    101 1111  this is valid
	*                    110 0000  this is invalid
	*                             \
	*                         ...  every one in this range is invalid
	*                             /
	*                    111 1111
	* 
	*
	*   the number of complying integers in [0, 11 0101]
	* = the number of complying integers in [0, 10 0000] +
	*   the number of complying integers in [0,    1111]
	* 
	* this gives me all the complying integers in [0, 10 1111]
	* nothing is valid in [11 0000, 11 0101]
	*
	* find the first '1' from left, break it into two parts, 1st part: 100..000..0  (1 and all 0's)
	* 2nd part: the rest, if the next '1' is right next to the first 1, then the 2nd part is 111...11 (all 1's)
	*
	* 1010 1010  --- 1st part [0,1000 0000]  2nd part: [0, 10 1010]
	* 1101 1111  --- 1st part [0,1000 0000]  2nd part: [0, 11 1111]
	* '0' is a complying integer for the 2nd part, but when adding the 2nd part to the 1st part, '0' is not a valid answer
	* because that just gives me the max of 1st part which is already counted in 1st part
	*/

	class Solution  // This is accepted by leetcode but a little slower that Solution2
	{

	public:

		/*
		* Meaning of rb[x]:
		* A binary with x bits, in the form of 1000...000  (first bit '1', all others are '0')
		* How many numbers in the range of [0, 1000...000] are valid.
		* 
		* If the input string is something like this: 1000 0100, i.e. there are more than one 1's,
		* I must not get the answer by checking the rb array
		*/
		int findNonConsec_Recur(int rb[64], string&& s)
		{
			size_t len = s.size();

			if (len == 1)
			{
				rb[len] = 2;
				return 2;
			}

			// for example, input s is 10-digit long
			// first (left-most) digit is 1
			// I need to find out the number of complying integers in [0, 10 0000 0000]
			// which is = the number of complying in [0, 1 1111 1111] + 1
			int firstPartResult;
			if (rb[len] != -1)
				firstPartResult = rb[len];
			else
			{
				firstPartResult = findNonConsec_Recur(rb, string(len - 1, '1'));
				firstPartResult++;
				rb[len] = firstPartResult;
			}

			auto secondOne = find(s.begin() + 1, s.end(), '1');
			int secondPartResult = 0;

			if (secondOne != s.end())
			{
				if (secondOne == s.begin() + 1)
				{	
					if (len - 2 >= 1)
					{
						//string c(len-2, '1');
						secondPartResult = findNonConsec_Recur(rb, string(len - 2, '1')) - 1; // discount by 1 because '0' is counted in 1st part
					}
					else
					{
						// input is "11"
						secondPartResult = 0;
					}
				}
				else
				{
					//string d(secondOne, s.end());
					secondPartResult = findNonConsec_Recur(rb, string(secondOne, s.end())) - 1;
				}
			}
			return firstPartResult + secondPartResult;  
			
		}

		// constraint: n >= 1
		int findIntegers(int n)
		{
			bitset<64> a{ static_cast<uint64_t>(n) };
			string b = a.to_string();
			
			int rb[64];
			for_each(begin(rb), end(rb), [](int& i) {i = -1; });

			auto firstOne = find(b.begin(), b.end(), '1');
			return findNonConsec_Recur(rb, string(firstOne, b.end()));
		}
	};

	class Solution2
	{
		// back track + dynamic programming
	public:

		/*
		* rb[k] stores the number of valid numbers when I'm given k digits and I can free run
		* for example   1010 1010
		* if I have decideed first 3 digits to be 100x xxxx
		* I have 5 digits to decide, but these 5 digits can free run, i.e. their range is [0, 11111]
		* rb[5] provides the count of how many valid numbers in this range.
		*/

		size_t len;

		int backtrackSolve(vector<int>&rb, string const & s, int index, bool alreadyLessThan, char previous)
		{
			
			if (index == s.size())
			{
				return 1;
			}
			if (alreadyLessThan && previous == '0' && (rb[len - index] != -1))
				return rb[len - index];

			char thisBit = s[index];
			if (alreadyLessThan)
			{
				if (previous == '0')
				{
					rb[len - index] = backtrackSolve(rb, s, index + 1, true, '0') + backtrackSolve(rb, s, index + 1, true, '1');
					return rb[len - index];
				}
				else
					return backtrackSolve(rb, s, index + 1, true, '0');
			}
			else
			{
				if (thisBit == '0')
				{
					return backtrackSolve(rb, s, index + 1, false, '0');
				}
				else // this bit is '1'
				{
					if (previous == '0')
					{
						return backtrackSolve(rb, s, index + 1, true, '0') + backtrackSolve(rb, s, index + 1, false, '1');
					}
					else // previous is '1', set this bit to '0'
						return backtrackSolve(rb, s, index + 1, true, '0');
				}
			}
		}

		int findIntegers(int n)
		{
			bitset<64> a{ static_cast<uint64_t>(n) };
			string b = a.to_string();
			auto firstOne = find(b.begin(), b.end(), '1');
			string c = string(firstOne, b.end());
			len = c.size();
			/*
			int* rb = new int[len];
			memset(rb, -1, len);     // memset sets value on a per byte basis
			*/
			vector<int>rb(len, -1);
			return backtrackSolve(rb, c, 0, false, '0');
			//delete[] rb;
		}
	};

	void Test_0600_Non_negative_Integers_without_Consecutive_Ones()
	{
		int n;
		Solution s1;
		Solution2 s2;
		int format;
		bitset<64> nb;

		while (true)
		{
			cout << "give a decimal number, press 1.   or give a binary number, press 2.     or exit press 0  >  ";
			cin >> format;
			if (format == 0) break;
			if (format == 1)
			{
				cout << "n: ";
				cin >> n;
				cout << s1.findIntegers(n) << "    " << s2.findIntegers(n) << '\n';
			} 
			else if (format == 2)
			{
				nb.reset();
				cout << "binary n: ";
				cin >> nb;
				cout << s1.findIntegers(nb.to_ulong()) << "    " << s2.findIntegers(nb.to_ulong()) << '\n';
			}
		}
	}
}