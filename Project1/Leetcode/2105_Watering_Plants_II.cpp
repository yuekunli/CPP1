#include "LibHeaders.h"

namespace _2105_Watering_Plants_II {

	using namespace std;

	class Solution
	{
	public:

		int minimumRefill(vector<int>& plants, int capacityA, int capacityB)
		{
			int len = plants.size();
			int half = (len-1) / 2;      // lower half
			// len = 8 :  0 1 2 3*   4 5 6 7
			// len = 9 :  0 1 2 3  4*  5 6 7 8
			int aWater = capacityA, bWater = capacityB;
			int refill = 0;
			for (int i = 0; i <= half; i++)
			{
				int j = len - 1 - i;
				if (i != j)
				{
					if (aWater < plants[i])
					{
						refill++;
						aWater = capacityA;
					}
					if (bWater < plants[j])
					{
						refill++;
						bWater = capacityB;
					}
					aWater -= plants[i];
					bWater -= plants[j];
				}
				else
				{
					if (aWater >= bWater)
					{
						if (aWater < plants[i])
							refill++;
					}
					else
					{
						if (bWater < plants[j])
							refill++;
					}
				}
			}
			return refill;
		}
	};

	void Test_2105_Watering_Plants_II()
	{
		int capacityA, capacityB;
		vector<int> plants;
		string input;

		Solution s;

		while (true)
		{
			cout << "input: ";
			getline(cin, input);
			if (input.size() == 0) break;
			istringstream ss(input);
			plants.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(plants));
			cout << "capacity A: ";
			cin >> capacityA;
			cout << "capacity B: ";
			cin >> capacityB;
			cin.ignore();
			cout << s.minimumRefill(plants, capacityA, capacityB) << "\n\n";
		}
	}
}