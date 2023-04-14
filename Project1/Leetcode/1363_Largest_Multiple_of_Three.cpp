#include "LibHeaders.h"

namespace _1363_Largest_Multiple_of_Three {

	using namespace std;

	// keep as many digits as possible, put them in descending order

	// need clarification on what the answer is if the input is a series of zeros
	// or after removal, I only have a few zeros left.
	// input: [0,0,0,0]
	// answer: "" or "0"?

	class Solution
	{
		/*
		* if I need to reduce the sum by 1, I'm *guaranteed* to find
		* either (1) an extra 1 or 4 or 7
		* or     (2) any extra two of 2, 5, 8
		* 
		* if I need to reduce the sum by 2, I'm guaranteed to find
		* either (1) an extra 2 or 5 or 8
		* or     (2) any extra two of 1 or 4 or 7
		* 
		* I'll never need to remove 3 or more numbers.
		*/

	public:
		string largestMultipleOfThree(vector<int>& d)
		{
			int digitCount[10] {0,0,0,0,0,0,0,0,0,0};
		
			int sum = 0;

			bool exist147 = false;
			bool exist258 = false;

			for (auto& n : d)
			{
				digitCount[n]++;
				sum += n;
				if (n == 1 || n == 4 || n == 7)
					exist147 = true;
				if (n == 2 || n == 5 || n == 8)
					exist258 = true;
			}

			bool needReduceBy1 = sum % 3 == 1 ? true : false;
			bool needReduceBy2 = sum % 3 == 2 ? true : false;

			while (sum != 0 && sum % 3 != 0)
			{
				if (exist147 && (needReduceBy1 || (needReduceBy2 && !exist258)))
				{
					for (int i = 1; i <= 7; i += 3)
					{
						if (digitCount[i] > 0)
						{
							digitCount[i]--;
							sum -= i;
							break;
						}
					}
				}

				if (exist258 && (needReduceBy2 || (needReduceBy1 && !exist147)))
				{
					for (int i = 2; i <= 8; i += 3)
					{
						if (digitCount[i] > 0)
						{
							digitCount[i]--;
							sum -= i;
							break;
						}
					}
				}
			}

			if (sum == 0)
				return "0";

			stringstream s1;
			for (int i = 9; i >= 0; i--)
			{
				for (int j = digitCount[i]; j > 0; j--)
				{
					s1.clear();
					s1 << i;
				}
			}
			return s1.str();
		}
	};

	class Solution2
	{
		/*
		* Same principle as Solution1, just a different way to draw out the logic
		*/
	public:
		string largestMultipleOfThree(vector<int>& d)
		{
			int digitCount[10]{ 0,0,0,0,0,0,0,0,0,0 };
			int sum = 0;
			int countOf147 = 0;
			int countOf258 = 0;
			for (auto& n : d)
			{
				digitCount[n]++;
				sum += n;
				if (n == 1 || n == 4 || n == 7)
					countOf147++;
				if (n == 2 || n == 5 || n == 8)
					countOf258++;
			}

			bool needReduceBy1 = sum % 3 == 1 ? true : false;
			bool needReduceBy2 = sum % 3 == 2 ? true : false;

			if (needReduceBy1)
			{
				if (countOf147 > 0) // remove 1 of '1', '4', '7'
				{
					for (int i = 1; i <= 7; i += 3)
					{
						if (digitCount[i] > 0)
						{
							digitCount[i]--;
							sum -= i;
							break;
						}
					}
				}
				else  // remove 2 of '2', '5', '8'
				{
					int i = 2; int removeTarget = 2;
					while (removeTarget > 0)
					{
						if (digitCount[i] > 0)
						{
							digitCount[i]--;
							sum -= i;
							removeTarget--;
						}
						else
							i += 3;
					}
				}
			}
			else if (needReduceBy2)
			{
				if (countOf258 > 0)
				{
					for (int i = 2; i <= 8; i += 3)
					{
						if (digitCount[i] > 0)
						{
							digitCount[i]--;
							sum -= i;
							break;
						}
					}
				}
				else
				{
					int i = 1; int removeTarget = 2;
					while (removeTarget > 0)
					{
						if (digitCount[i] > 0)
						{
							digitCount[i]--;
							sum -= i;
							removeTarget--;
						}
						else
							i += 3;
					}
				}
			}

			if (sum == 0)
				return "0";

			stringstream s1;
			for (int i = 9; i >= 0; i--)
			{
				for (int j = digitCount[i]; j > 0; j--)
				{
					s1.clear();
					s1 << i;
				}
			}
			return s1.str();
		}
	};


	void Test_1363_Largest_Multiple_of_Three()
	{
		vector<int> digits;
		Solution2 s;
		string s1;
		while (true)
		{
			s1.clear();
			cout << "array: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			digits.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(digits));
			cout << s.largestMultipleOfThree(digits) << '\n';
		}
	}
}