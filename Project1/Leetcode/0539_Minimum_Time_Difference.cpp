#include<vector>
#include<sstream>
#include<algorithm>


/*
* Similar problem:
* 164 Maximum Gap
* Solution hint: bucket sort
*/


namespace _0539_Minimum_Time_Difference {

	using namespace std;

	class Solution
	{
	public:
		int findMinDifference(vector<string>& timePoints)
		{
			size_t n = timePoints.size();
			vector<pair<int, int>>convertedTPs(n);
			transform(timePoints.begin(), timePoints.end(), convertedTPs.begin(),
				[](string const&a)
				{
					stringstream ss(a);
					int ha, ma;
					ss >> ha;
					ss.ignore();
					ss >> ma;
					return pair{ ha, ma };
				});
			sort(convertedTPs.begin(), convertedTPs.end(),
				[](pair<int, int>const&a, pair<int, int>const&b)
				{
					if (a.first < b.first)
						return true;
					if (a.first == b.first && a.second < b.second)
						return true;
					return false;
				});

			int minDiff = 24 * 60;
			for (int i = 1; i < n; i++)
			{
				int d = 0;
				int& hour2 = convertedTPs[i].first;
				int& minute2 = convertedTPs[i].second;
				int& hour1 = convertedTPs[i-1].first;
				int& minute1 = convertedTPs[i-1].second;
				if (minute2 >= minute1)
				{
					d += minute2 - minute1;
					d += (hour2 - hour1) * 60;
				}
				else
				{
					d += minute2 + 60 - minute1;
					d += (hour2 - hour1 - 1) * 60;
				}
				minDiff = min(minDiff, d);
			}

			{
				int d = 0;
				int& hour2 = convertedTPs[n-1].first;
				int& minute2 = convertedTPs[n-1].second;
				int& hour1 = convertedTPs[0].first;
				int& minute1 = convertedTPs[0].second;
				if (minute2 >= minute1)
				{
					d += minute2 - minute1;
					d += (hour2 - hour1) * 60;
				}
				else
				{
					d += minute2 + 60 - minute1;
					d += (hour2 - hour1 - 1) * 60;
				}
				d = 24 * 60 - d;
				minDiff = min(minDiff, d);
			}

			return minDiff;
		}
	};
}