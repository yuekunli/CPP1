#include "LibHeaders.h"

namespace _0319_Bulb_Switcher {

	using namespace std;

	class Solution
	{
	public:

		/*
		* When will a certain bulb be flipped?
		* If the bulb is the 12th one (1-index), it will be flipped the 1st round, 2nd round, 3rd round, 4th round, 6th round, 12th round
		* So on the rounds that are 12's factors, it will be flipped.
		* How many factors does it have? -- 6. How many times it's flipped? -- 6 times.
		* If it's flipped even times, it's the same as no flip.
		* When will a bulb be flipped odd number times? -- When it's some number's square, so that it has odd number of factors
		* If 5^2 <= n < 6^2, there are 5 numbers in [1,n] that are some other number's square.
		* Find n's position:
		* 1 4 9 16 25 36 49 .... 10^8
		* 1 2 3 4  5  6  7       10^4
		* 
		* constraints: 0 <= n <= 10^9
		*/

		int bulbSwitch2(int n)
		{
			int high = static_cast<int>(sqrt(1'000'000'000));

			if (n >= static_cast<int>(pow(high, 2)))
				return high;

			int low = 0;

			while (low < high)
			{
				int mid = (high + low) / 2;
				if (n >= pow(mid, 2) && n < pow(mid + 1, 2))
					return mid;
				if (n >= pow(mid + 1, 2))
					low = mid;
				else
					high = mid;
			}
		}

		int bulbSwitch(int n)
		{
			return static_cast<int>(sqrt(n));
		}
	};


	void Test_0319_Bulb_Switcher()
	{
		int n;
		Solution s;
		while (true)
		{
			cout << "\n\nrun test? give an \'n\': ";
			cin >> n;
			if (n == -1) break;
			cout << s.bulbSwitch(n) << '\n';
		}
	}
}