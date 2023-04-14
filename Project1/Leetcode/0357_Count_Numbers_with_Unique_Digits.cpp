#include<iostream>
#include<algorithm>
#include<vector>
#include<string>

namespace _0357_Count_Numbers_with_Unique_Digits {

	using namespace std;

	/*
	* I think the requirement demands *all* digits must be unique
	* For example, 151 is invalid even though it doesn't have *consecutive* repeating digits
	*/

	/*
	* If n = 4, 10^n = 10000,   0<=x<10000,  basically x is at most 4 digits
	*/

	/*
	* constraint:  0 <= n <= 8
	*/

	class Solution
	{
		/*
		* if n = 4, x can be 4 digits, it can also be 3, 2, or 1 digit.
		* A 4-digit all unique number, using combinatorial method:
		* choice for 1st digit (left-most): 9
		* choice for 2nd digit            : 9
		* choice for 3rd digit            : 8
		* choice for 4th digit            : 7
		* 
		* Answer = valid 4-digit number + valid 3-digit number + valid 2-digit number + valid 1-digit number
		*          9 * 9 * 8 * 7          9 * 9 * 8              9 * 9                  10
		* 
		* The result of valid 3-digit number can help when calculating valid 4-digit number
		*/
	public:
		int countNumbersWithUniqueDigits(int n)
		{
			if (n == 0) return 1;
			if (n == 1) return 10;
			
			int intermediateResult = 9 * 9; // valid 2-digit number in range [10, 99] excluding 11, 22, 33, 44, 55, 66, 77, 88, 99
			
			int total = intermediateResult;

			for (int i = 3; i <= n; i++)
			{
				intermediateResult = intermediateResult * (11 - i);
				total += intermediateResult;
			}
			return total + 10; // add valid 1-digit number at the end

			/*
			* if n == 2, for loop skipped, just return 9*9 + 10
			* if n == 5,  total is initialized to 9 * 9
			*     i = 3
			*         intermediate result = 9 * 9 * 8
			*         total = 9 * 9 + 9 * 9 * 8
			*     i = 4
			*         intermediate result = 9 * 9 * 8 * 7
			*         total = 9*9 + 9*9*8 + 9*9*8*7
			*     i = 5
			*         intermediate result = 9 * 9 * 8 * 7 * 6
			*         total = 9*9 + 9*9*8 + 9*9*8*7 + 9*9*8*7*6
			* eventuall return total + valid 1-digit number
			*/
		}
	};

	class BruteForceSolution
	{
	public:
		int countNumbersWithUniqueDigits(int n)
		{
			int maximum = static_cast<int>(pow(10, n));
			vector<bool> r(10, false);
			int tally = 0;
			for (int i = 0; i < maximum; i++)
			{
				fill(r.begin(), r.end(), false);
				bool foundRepeat = false;
				string s = to_string(i);
				for (char& c : s)
				{
					int p = c - '0';
					if (r[p])
					{
						foundRepeat = true;
						break;
					}
					r[p] = true;
				}
				if (!foundRepeat)
					tally++;
			}
			return tally;
		}
	};

	void Test_0357_Count_Numbers_with_Unique_Digits()
	{
		int n;
		Solution s;
		BruteForceSolution bfs;
		while (true)
		{
			cout << "(-1 exit) n: ";
			cin >> n;
			if (n == -1) break;
			cout << "fast solution: " << s.countNumbersWithUniqueDigits(n) 
				<< ",  brute force solution: " << bfs.countNumbersWithUniqueDigits(n) << '\n';
		}
	}
}