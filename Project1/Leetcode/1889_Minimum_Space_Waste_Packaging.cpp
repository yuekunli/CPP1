#include "LibHeaders.h"

namespace _1889_Minimum_Space_Waste_Packaging {

	using namespace std;

	class Solution
	{
		/*
		* sort packages and calculate the rolling sum of the package array.
		* for example if this is the sorted package array:
		*   a   b   c   d   e   f   g   h   i   j   k
		*                 |
		*                if a box's size is located in this array here, I know
		*                the packages that can be contained by this box.
		*                I can know the total volumn of these packages by checking
		*                the rolling sum array. I can know the total boxes space
		*                by simply multiplying number of boxes with box size.
		*                Then I can know the wasted space with these first group
		*                of packages
		*/
	public:
		int minWastedSpace(vector<int>& packages, vector<vector<int>>& boxes)
		{
			sort(packages.begin(), packages.end());
			for (vector<int>& b : boxes)
			{
				sort(b.begin(), b.end());
			}

			int n = packages.size();
			vector<int>rollSum(n);
			partial_sum(packages.begin(), packages.end(), rollSum.begin());
			vector<int>waste(boxes.size());
			for (int i = 0; i < boxes.size(); i++)
			{
				vector<int>& sizes = boxes[i];
				if (*sizes.rbegin() < *packages.rbegin())
				{
					waste[i] = -1;
				}
				else
				{
					int wasteForThisSupplier = 0;
					auto searchStart = packages.begin();
					for (int& b : sizes)
					{
						auto it = lower_bound(searchStart, packages.end(), b);
						
						if (it == packages.begin() && *it > b)
							continue;

						if (it == packages.end())
							it = it - 1;
						else if (*it > b)
							it = it - 1;

						int d1 = distance(packages.begin(), searchStart);
						int d2 = distance(packages.begin(), it);

						wasteForThisSupplier += (d2-d1 + 1) * b - (rollSum[d2]-rollSum[d1] + packages[d1]);
						searchStart = it + 1;
						if (searchStart == packages.end())
							break;
					}
					waste[i] = wasteForThisSupplier;
				}
			}
			return *(min_element(waste.begin(), waste.end(),
				[](int& a, int& b)
				{
					if (a > 0 && b < 0)
						return true;
					if (a < 0 && b > 0)
						return false;

					return a < b;
				}));
		}
	};

	void Test_1889_Minimum_Space_Waste_Packaging()
	{
		Solution solu;

		string s;
		vector<int>packages;
		vector<vector<int>>boxes;

		while (true)
		{
			s.clear();
			cout << "packages: ";
			getline(cin, s);
			if (s.size() == 0) break;
			stringstream ss(s);
			packages.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(packages));
			boxes.clear();
			while (true)
			{
				s.clear();
				cout << "boxes: ";
				getline(cin, s);
				if (s.size() == 0) break;
				vector<int>tmp;
				stringstream ss2(s);
				copy(istream_iterator<int>(ss2), istream_iterator<int>(), back_inserter(tmp));
				boxes.emplace_back(tmp);
			}
			cout << solu.minWastedSpace(packages, boxes) << "\n\n";
		}
	}
}