#include "LibHeaders.h"

namespace _1866_Number_Ways_Rearrange_Sticks_K_Visible {

	using namespace std;

	class Solution
	{
		int recursiveSolve(vector<vector<int>>& r, int n, int k)
		{
			if (r[n][k] != -1)
				return r[n][k];

			if (n == 0)
			{
				r[n][k] = 0;
				return 0;
			}
			if (k == 0)
			{
				r[n][k] = 0;
				return 0;
			}
			if (k == 1)
			{
				unsigned long long prod = 1;
				for (int i = 1; i <= n - 1; i++)
				{
					prod = (prod * (i % 1'000'000'007)) % 1'000'000'007;
				}
				r[n][k] = (int)prod;
				return r[n][k];
			}

			/*
			* 8 sticks, 5 visible:
			* (1). set the longest aside, arrange the other 7 so that 4 are visible, put the longest at the end, I get 5 visible
			* (2). set aside any one stick except the longest, arrange the other 7 so that 5 are visible, put the reserved one
			* at the end, that one won't be visible because the longest must be one of the first 7 so that last one must be
			* blocked by the longest.
			* 
			* The return value of "recursiveSolve" is guaranteed to be less than 1'000'000'007, but multiple it by (n-1) can
			* take it over the upper limit of signed integer (which is 32 bits long), so I need to convert both the return
			* value of "recursiveSolve" and "n-1" to unsigned long long (which is 64 bits long) and do the multiplication,
			* then do the modules. It will become less than 1'000'000'007 after the modulus, so I can convert the final result
			* back to "int".
			*/
			int result1 = recursiveSolve(r, n - 1, k - 1);
			int result2 = recursiveSolve(r, n - 1, k);
			result2 = (int)((unsigned long long)result2 * (unsigned long long)(n - 1) % 1'000'000'007);
			r[n][k] = (int)(((unsigned long long)result1 + (unsigned long long)result2) % 1'000'000'007);
			return r[n][k];
		}

	public:

		int rearrangeSticks(int n, int k)
		{
			vector<vector<int>>recordbook(n + 1, vector<int>(k + 1, -1));
			return recursiveSolve(recordbook, n, k);
		}
	};


	void Test_1866_Number_Ways_Rearrange_Sticks_K_Visible()
	{
		Solution s;
		int n, k;
		while (true)
		{
			cout << "n:  ";
			cin >> n;
			if (n == 0) break;
			cout << "k:  ";
			cin >> k;
			cout << s.rearrangeSticks(n, k) << '\n';
		}
	}
}