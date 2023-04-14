#include "LibHeaders.h"

namespace _1547_Minimum_Cost_to_Cut_a_Stick {

	using namespace std;

	class Solution
	{
	public:
		int minCostRecurse(int n, vector<int>& cuts, vector<vector<int>>& r, int a, int b)
		{
			if (r[a][b] != -1)
				return r[a][b];

			int len = cuts.size();
			if (a == b)
			{
				if (a == 0)
				{
					if (len == 1)
					{
						r[a][b] = n;
					}
					else
					{
						r[a][b] = cuts[1];
					}
				}
				else if (a == len - 1)
				{
					r[a][b] = n - cuts[len - 2];
				}
				else
				{
					r[a][b] = cuts[a + 1] - cuts[a - 1];
				}
				return r[a][b];
			}

			int stickStart = a == 0 ? 0 : cuts[a - 1];
			int stickEnd = b == len - 1 ? n : cuts[b + 1];
			int stickLength = stickEnd - stickStart;

			int minCost = numeric_limits<int>::max();
			for (int i = a; i <= b; i++)
			{
				int costOfThisChoice = stickLength;

				if (i > a)
					costOfThisChoice += minCostRecurse(n, cuts, r, a, i - 1);

				if (i < b)
					costOfThisChoice += minCostRecurse(n, cuts, r, i + 1, b);

				if (minCost > costOfThisChoice)
					minCost = costOfThisChoice;
			}

			r[a][b] = minCost;
			return r[a][b];
		}

		int minCost(int n, vector<int>& cuts)
		{
			int len = cuts.size();
			vector<vector<int>> r(len, vector<int>(len, -1));
			sort(cuts.begin(), cuts.end());
			return minCostRecurse(n, cuts, r, 0, len - 1);
		}
	};

	void Test_1547_Minimum_Cost_to_Cut_a_Stick()
	{
		int n;
		vector<int> cuts;
		string cutsInStr;

		Solution s;

		while (true)
		{
			cout << "n: (0 to exit)  > ";
			cin >> n;
			if (n == 0) break;
			
			cutsInStr.clear();
			cin.ignore();
			cout << "cuts: ";
			getline(cin, cutsInStr);
			istringstream ss(cutsInStr);
			cuts.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(cuts));
			cout << s.minCost(n, cuts) << '\n';
		}
	}
}