#include<iostream>
#include<vector>
#include<bitset>
#include<math.h>

namespace _0089_Gray_Code {

	using namespace std;

	class Solution
	{
	public:

		/*
		* Decimal     Gray Code
		* 0           0 0 0 0
		* 1           0 0 0 1
		* 2           0 0 1 1
		* 3           0 0 1 0
		* 4           0 1 1 0
		* 5           0 1 1 1
		* 6           0 1 0 1
		* 7           0 1 0 0
		* 8           1 1 0 0
		* 9           1 1 0 1
		* 10          1 1 1 1
		* 11          1 1 1 0
		* 12          1 0 1 0
		* 13          1 0 1 1
		* 14          1 0 0 1
		* 15          1 0 0 0
		*                 | |
		*                 | +-- value changes every 2 codes
		*                 +--- value changes every 4 codes
		*
		* the second bit (from right) laid out:
		* 0   0   1   1   1   1   0   0   0   0   1   1   1   1   0   0
		* \___/   \___________________________________________________/
		*   |                              |
		*   |                              these follow the rule of "change every 4 codes"
		*  these are called "first group", their values are always 0, just need to figure out home many are in this group
		*/ 

		/*
		* 0...0 being the 1st code
		* 
		* right-most bit being the 1st bit
		*/
		bool xBitInKthCode(int x, int k)
		{
			int normalInterval = (int)pow(2, x);
			int firstGroup = normalInterval / 2;
			
			if (k <= firstGroup) return false;

			k -= firstGroup;
			int remain = k % normalInterval;

			k = remain == 0 ? (k / normalInterval - 1) : k / normalInterval;
			return (k % 2 == 0) ? true : false;
		}

		vector<int> grayCode(int n)
		{
			bitset<16> b;
			int total = (int)pow(2, n);
			vector<int> answer(total);
			for (int i = 1; i <= total; i++)
			{
				for (int j = 0; j < n; j++)
				{
					b[j] = xBitInKthCode(j + 1, i);
					// unlike array, bitset[0] returns a reference to the right most bit
				}
				answer[i - 1] = b.to_ulong();
				b.reset();
			}
			return answer;
		}
	};

	class Solution2
	{
		// from cp-algorithm.com/algebra/gray-code.html

	public:
		int nthGrayCode(int n)
		{
			return (n ^ (n >> 1));
		}

		vector<int> grayCode(int N)
		{
			int total = 1 << N;
			vector<int> answer(total);
			for (int n = 0; n < total; n++)
				answer[n] = nthGrayCode(n);
			return answer;
		}
	};

	void Test_0089_Gray_Code()
	{
		int n;
		Solution s;

		while (true)
		{
			cout << "n: (-1 exit)   ";
			cin >> n;
			if (n == -1)break;
			vector<int> const& answer = s.grayCode(n);
			for (int const & i : answer)
				cout << i << " ";
			cout << '\n';
		}
	}
}