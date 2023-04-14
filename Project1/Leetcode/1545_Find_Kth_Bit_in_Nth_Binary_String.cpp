#include "LibHeaders.h"

namespace _1545_Find_Kth_Bit_in_Nth_Binary_String {

	using namespace std;

	class Solution
	{
	public:


		/*
		*  x = 1                       length of S1
		*  2x  + 1                     length of S2
		*  4x  + 2 + 1                 length of S3
		*  8x  + 4 + 2 + 1             length of S4
		*  16x + 8 + 4 + 2 + 1
		*  32x + 16+ 8 + 4 + 2 + 1
		* 
		*  
		*  length of Sn  =  [2^(n-1)]  + {1 + 2 + 4 + ... + [2^(n-2)]}
		* 
		*  geometric series sum: a1(1-r^n)/(1-r)
		*  in this particular case, for a certain 'n'
		*  The geometric series has n-1 items
		*  1 * (1 - 2^(n-1))
		* -------------------  =  2^(n-1) - 1
		*     1 - 2
		*  
		* length of Sn = 2 * 2^(n-1) - 1 = 2^n - 1
		* 
		* This holds true for S1 too.
		* 
		* 
		*/

		/*
		*  for example of S4
		*         1  2  3  4  5  6  7  _  !7  !6  !5  !4  !3  !2  !1
		* index:  1  2  3  4  5  6  7  8   9  10  11  12  13  14  15
		*                                     |
		*                                    15-10+1 = 6
		* 
		*/

		char getReverse(char c)
		{
			return c == '0' ? '1' : '0';
		}

		char findKthBit(int n, int k)
		{
			if (n == 1) return '0';

			int len = static_cast<int>(pow(2, n)) - 1;
			int half = (len - 1) / 2;
			if (k <= half)
				return findKthBit(n - 1, k);
			else if (k >= half + 2)
				return getReverse(findKthBit(n - 1, len - k + 1));
			else
				return '1';
		}
	};


	void Test_1545_Find_Kth_Bit_in_Nth_Binary_String()
	{
		int n, k;

		Solution s;

		while (true)
		{
			cout << "n: ";
			cin >> n;
			if (n == 0) break;
			cout << "k: ";
			cin >> k;
			cout << s.findKthBit(n, k) << "\n";
		}
	}
}