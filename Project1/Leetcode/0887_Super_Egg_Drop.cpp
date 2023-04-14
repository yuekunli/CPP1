#include<iostream>
#include<vector>
#include<numeric>

namespace _0887_Super_Egg_Drop {

	using namespace std;

	class Solution
	{
	public:

		/*
		*         |  1 floor  | 2 floors   3 floors ....   n floors
		* --------+-----------+------------------------------------
		* 1 egg   |           |
		* --------+-----------+------------------------------------
		* 2 eggs  |           |
		* --------+-----------+------------------------------------
		* ....    |           |
		* --------+-----------+------------------------------------
		* k eggs  |           |
		*/
		int superEggDrop(int k, int n)
		{
			vector<vector<int>> t(k+1, vector<int>(n+1, 1));
		
			iota(t[1].begin(), t[1].end(), 0);

			for (int i = 2; i <= k; i++) // i eggs
			{
				// t[i][1] should be 1, but don't need to do that here, because every cell in the table is initialized to 1
				for (int j = 2; j <= n; j++) // j floors
				{
					int minimum = j;
					for (int firstTry = 1; firstTry <= j; firstTry++)
					{
						int movesNeededIfFirstTryBreak;
						int movesNeededIfFirstTrySurvive;

						if (firstTry > 1)
							movesNeededIfFirstTryBreak = t[i - 1][firstTry - 1] + 1;
						else
							movesNeededIfFirstTryBreak = 1;  // first try at floor#1, and it breaks, I don't need any more trys. Overall attempt is just 1.

						if (firstTry < j)
							movesNeededIfFirstTrySurvive = t[i][j - firstTry] + 1;
						else
							movesNeededIfFirstTrySurvive = 1;

						int m = max(movesNeededIfFirstTryBreak, movesNeededIfFirstTrySurvive);
						if (m < minimum)
							minimum = m;
					}
					t[i][j] = minimum;
				}
			}
			return t[k][n];
		}
	};

	class Solution2
	{
	public:

		int superEggDrop(int k, int n)
		{
			vector<vector<int>> t(k + 1, vector<int>(n + 1, 0));

			iota(t[1].begin(), t[1].end(), 0);

			for (int i = 0; i <= k; i++)
				t[i][1] = 1;

			for (int i = 2; i <= k; i++) // i eggs
			{
				for (int j = 2; j <= n; j++) // j floors
				{
					int minimum = j;
					for (int firstTry = 1; firstTry <= j; firstTry++)
					{
						int movesNeededIfFirstTryBreak;
						int movesNeededIfFirstTrySurvive;

						movesNeededIfFirstTryBreak = t[i - 1][firstTry - 1] + 1;
						movesNeededIfFirstTrySurvive = t[i][j - firstTry] + 1;

						int m = max(movesNeededIfFirstTryBreak, movesNeededIfFirstTrySurvive);
						if (m < minimum)
							minimum = m;
					}
					t[i][j] = minimum;
				}
			}
			return t[k][n];
		}
	};


	void Test_0887_Super_Egg_Drop()
	{
		//Solution s;
		Solution2 s2;
		int k, n;
		while (true)
		{
			cout << "Floors: ";
			cin >> n;
			if (n == 0) break;
			cout << "Eggs: ";
			cin >> k;
			//std::cout << s.superEggDrop(k, n) << "   |   " << s2.superEggDrop(k, n) << '\n';
			std::cout << s2.superEggDrop(k, n) << '\n';
		}
	}
}