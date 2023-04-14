#include "LibHeaders.h"

namespace _1840_Maximum_Building_Height {

	using namespace std;

	class Solution
	{
		/*
		* This is a brute force solution. It uses memory in O(n).
		* n can be as big as 10^9, that is O(4000MB).
		* I need a vector of integers with length of n+1. Each integer is 4 bytes.
		* 
		* The idea of this solution is that a building's height is essentially
		* determined by 3 things: (1) its own restriction, (2), restrictions coming
		* from its left, (3). restrictions coming from its right.
		* 
		* Start processing each building from the left, try to increase by 1,
		* if a building's own restriction limits that then try to set it to its own
		* restriction. If setting to its own restriction makes it significantly lower
		* than the one on its left, then reconcile the building on its left until
		* the "drag-down" force from the right doesn't really drag a building down.
		*/

		struct Building
		{
			int height{ 0 };
			int restrict{ -1 };
		};

	public:

		int maxBuilding(int n, vector<vector<int>>& restrictions)
		{
			vector<Building> b(n + 1);
			sort(restrictions.begin(), restrictions.end(), [](vector<int>const& a, vector<int>const& b) {return a[0] < b[0]; });
			for (vector<int>& r : restrictions)
			{
				int position = r[0];
				int restrict = r[1];
				b[position].restrict = restrict;
			}
			b[1].restrict = 0;
			b[1].height = 0;
			for (int i = 2; i <= n; i++)
			{
				if (b[i].restrict == -1 || b[i].restrict >= b[i - 1].height + 1)
					b[i].height = b[i - 1].height + 1;
				else
				{
					b[i].height = b[i].restrict;
					int j = i;
					while (true)
					{
						if (b[j].height < b[j - 1].height - 1)
						{
							b[j - 1].height = b[j].height + 1;
							j--;
						}
						else
							break;

					}
				}
			}
			int maxHeight = 0;
			for (Building& a : b)
			{
				if (maxHeight < a.height)
					maxHeight = a.height;
			}
			return maxHeight;
		}
	};

	void Test_1840_Maximum_Building_Height()
	{
		int n;
		vector<vector<int>>restrictions;
		Solution solu;
		string s;
		vector<int> input;

		while (true)
		{
			cout << "n: (0 to exit) > ";
			cin >> n;
			cin.ignore();
			if (n == 0) break;
			s.clear();
			cout << "restrictions: ";
			getline(cin, s);
			stringstream ss(s);
			input.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(input));
			restrictions.clear();
			for (int i = 0; i <= ((int)input.size() - 2); i += 2)
				restrictions.emplace_back(initializer_list<int>{input[i], input[i + 1]});
			cout << solu.maxBuilding(n, restrictions) << '\n';
		}
	}
}