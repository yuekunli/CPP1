#include<vector>
#include<iostream>
#include<math.h>

namespace _2761_Prime_Pairs_With_Target_Sum {
	using namespace std;

	class Solution
	{
		// Prime must be odd numbers. If 'n' is odd, if 'x' is a prime, 'x' must be odd.
		// if x + y = n, y must be even, so 'y' must not be prime.
		// Therefore, if 'n' is odd, the only chance to find a qualified pair is 2 + y = n
	public:
		vector<vector<int>> findPrimePairs(int n)
		{
			vector<vector<int>> ret;
			vector<int> foundPrimes;
			int half = n / 2;
						
			foundPrimes.push_back(2);
			
			for (int i = 3; i <= n; i+=2)
			{
				bool isPrime = true;
				int root = (int)floor(sqrt(i));

				for (int& a : foundPrimes)
				{
					if (a > root)
						break;
					int r = i / a;
					if (r * a == i)
					{
						isPrime = false;
						break;
					}
				}
				if (isPrime)
					foundPrimes.push_back(i);
			}

			if (foundPrimes.size() == 1)
				return ret;


			int i = 0, j = foundPrimes.size() - 1;

			if (foundPrimes[j] < half)
				return ret;

			if (n % 2 == 1) // if 'n' is odd, only chance to find qualified pairs is 2 plus another prime
			{
				while (j >= 0 && 2 + foundPrimes[j] > n)
					j--;

				if (j >= 0 && 2 + foundPrimes[j] == n)
					return { {2, foundPrimes[j]} };
			}

			while (i < foundPrimes.size() && j >= 0 && foundPrimes[i] <= foundPrimes[j])
			{
				if (foundPrimes[i] + foundPrimes[j] == n)
				{
					ret.push_back({ foundPrimes[i], foundPrimes[j] });
					i++;
				}
				else if (foundPrimes[i] + foundPrimes[j] > n)
				{
					j--;
				}
				else
					i++;
			}

			return ret;
		}
	};

	void Test_2761_Prime_Pairs_With_Target_Sum()
	{
		Solution solu;

		int test_case = 0;
		while (true)
		{
			cout << "choose n, (0 to exit) > ";
			cin >> test_case;

			if (test_case == 0)
				break;

			vector<vector<int>> result = solu.findPrimePairs(test_case);

			for (auto& pair : result)
			{
				cout << pair[0] << "  " << pair[1] << endl;
			}
			cout << "\n\n";
		}
	}
}