#include<iostream>
#include<string>
#include<bitset>
#include<vector>
#include <thread>
#include <mutex>

namespace _1012_Numbers_With_Repeated_Digits {

	using namespace std;

	/*
	* Doesn't have to be adjacent repeated digits.
	* Numbers like 1215 are counted.
	*/

	class Solution  //accepted
	{
		/*
		* example: n = 759
		* If I fix the 1st digit (left-most) to be 1, the number is certainly
		* less than n, the 2nd and 3rd digits can free run. Let's say there 
		* are x combinations of 2nd and 3rd digits so that 2nd and 3rd are 
		* unique and they are unique comparing to 1st digit, given the fact
		* that 1st digit is fixed to be '1'. Now I fix the 1st digit to be '2',
		* still 2nd and 3rd digits can free run, although the 1st digit changed,
		* it's still one fixed number, the possible combinations of 2nd and
		* 3rd digits for all 3 digits to be unique should be the same as when
		* the 1st digit is fixed to be '1'.
		* 
		* So a sub-problem is: I have x digits fixed. I have y digits to decide.
		* Those y digits can free run. Those x digits are know to be unique.
		* How many combinations can those y digits have?
		* 
		* In another word, decide y digits, I have (10-x) choices, how many combinations?
		*/
	public:
		/*
		* left-most digit is the 0th digit
		*/
		int a(vector<vector<int>>&r, bitset<10>& digitsMask, int currentIndex, bool alreadyLessThanN, string& n)
		{
			size_t len = n.size();
			if (currentIndex == len)  // I'm already 1 digit past the last digit (right most), a candidate solution is complete now.
				return 1;

			int used = 0;  // number of options already used
			for (int i = 0; i < 10; i++)
				if (digitsMask[i])
					used++;

			size_t toDecide = len - currentIndex;  // number of digits yet to be decided
												// example: length = 8, currentIndex = 6,  digits to be decided: 6th, 7th

			if (alreadyLessThanN && r[toDecide][10 - used] != -1)
				return r[toDecide][10 - used];

			if (alreadyLessThanN)
			{
				// I have a few digits to decide, they can free run, there are non-zero digits in front of them

				int result = 1;
				for (int i = 0; i < toDecide; i++)
				{
					result *= 10 - used - i;
				}
				r[toDecide][10 - used] = result;
				return r[toDecide][10 - used];
			}

			// previous digits are all up to the upper limit
			int limit = n[currentIndex] - '0';
			int result = 0;
			for (int i = 0; i <= limit; i++)
			{
				if (currentIndex == 0 && i == 0)
					continue;

				if (!digitsMask[i])
				{
					digitsMask[i]=true;
					result += a(r, digitsMask, currentIndex + 1, (i<limit), n);
					digitsMask[i] = false;
				}
			}
			return result;
		}

		int b(size_t k)
		{
			if (k == 1) return 9;

			int intermediateResult = 9 * 9; // valid 2-digit number in range [10, 99] excluding 11, 22, 33, 44, 55, 66, 77, 88, 99

			int total = intermediateResult;

			for (int i = 3; i <= k; i++)
			{
				intermediateResult = intermediateResult * (11 - i);
				total += intermediateResult;
			}
			return total + 9; // 9 unique 1-digit number, 0 is not included
		}

		int numDupDigitsAtMostN(int n)
		{
			if (n <= 9) return 0;

			string s = to_string(n);
			size_t len = s.size();
			
			/*
			* example n = 539, let me first find out the unique digit numbers in range [1, 100)
			* basically if n is x-digit number, first found out unique digit numbers with less than x digits
			*/
			int withoutRepeatWithLessDigits = b(len-1);

			/*
			* The reason I split the range into [1,100) and [100,539] is because if I just run backtracking on [1,539],
			* I can certainly choose first digit to be 0, and for example, I choose second digit to be 5. Now the third
			* digit can still use '0'. This each-number-used-once rule can't be applied universally, I have to deal with
			* '0' specially. If I only deal with [100,539], all I need to deal with is to remember to not pick '0' on the
			* very first digit.
			*/

			/*
			* now let's find out unique digit numbers in range [100, 539]
			*/
			vector<vector<int>> r(len, vector<int>(10,-1));
			bitset<10> mask;
			int withoutRepeatWithEqualDigits = a(r, mask, 0/*current index*/, false/*already less than n*/, s/*n in string form*/);
			return n - withoutRepeatWithLessDigits - withoutRepeatWithEqualDigits;
		}
	};


	class MultiThreadBruteForceSolution
	{
		mutex m;
		int total{ 0 };
	public:

		int run(int n)
		{
			total = 0;
			if (n < 1000)
				numDupDigitsAtMostN(1, n);
			else
			{
				vector<thread> workers(3);
				int segment = n / 4;
				int prevEnd = 0;
				for (int i = 0; i < 3; i++)
				{
					int start = prevEnd + 1;
					int end = i == 3 ? n : prevEnd + segment;
					prevEnd = end;
					workers[i] = thread(&MultiThreadBruteForceSolution::numDupDigitsAtMostN, this, start, end);
				}

				numDupDigitsAtMostN(prevEnd+1, n); // use main thread to process the last segment

				for (int i = 0; i < 3; i++)
				{
					workers[i].join();
				}
			}
			return total;
		}

		void numDupDigitsAtMostN(int start, int end)
		{
			bitset<10> mask;
			int tally = 0;
			for (int i = start; i <= end; i++)
			{
				mask.reset();
				bool foundRepeat = false;
				string s = to_string(i);
				for (char& c : s)
				{
					int p = c - '0';
					if (mask[p])
					{
						foundRepeat = true;
						break;
					}
					mask[p] = true;
				}
				if (foundRepeat)
					tally++;
			}
			unique_lock<mutex> ulk(m);
			total += tally;
			return;
		}
	};


	void Test_1012_Numbers_With_Repeated_Digits()
	{
		int n;
		Solution s;
		MultiThreadBruteForceSolution mtbfs;
		while (true)
		{
			cout << "(-1 exit) n: ";
			cin >> n;
			if (n == -1) break;
			cout << "fast solution: " << s.numDupDigitsAtMostN(n)
				<< ",  brute force solution: " << mtbfs.run(n) << '\n';
		}
	}
}