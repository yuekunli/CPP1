#include<iostream>
#include<string>
#include<sstream>
#include<map>
#include<unordered_map>
#include<vector>
#include<algorithm>

namespace _0939_Minimum_Area_Rectangle {

	using namespace std;

	class Solution
	{

		/*
		*         * (1,5)
		*         * (1,4)                 * (7,4)
		*                    * (3,3)
		*         * (1,2)                 * (7,2)
		*                    * (3,1)
		* 
		* m:
		*   key(x coordinate of a vertical line)    value (y coordinates of points on that line)
		*   1                                       2, 4, 5
		*   3                                       1, 3
		*   7                                       2, 4
		* 
		* m2:
		*   key (y coordinates of two points on a same vertial line)   value (x coordinates of the vertial lines)
		*   [2,4]                                                      1, 7
		*   [2,5] (point (1,2) and (1,5) form this pair)               1
		*   [4,5]                                                      1
		*   [1,3]                                                      3
		*/
		struct PairComp
		{
			bool operator()(pair<int, int>const& a, pair<int, int>const& b) const
			{
				return a.first == b.first && a.second == b.second;
			}

		};
		struct PairHash
		{
			size_t operator()(pair<int, int>const& a) const
			{
				return ((size_t)a.first) * 10'000 + (size_t)a.second;
			}
		};
	public:
		int minAreaRect(vector<vector<int>>& points)
		{
			map<int, vector<int>>m;
			for (auto& p : points)
			{
				int x = p[0];
				m[x].emplace_back(p[1]);
			}

			unordered_map<pair<int, int>, vector<int>, PairHash, PairComp> m2; // a pair of points on the same vertical line
			for (auto& a : m)
			{
				vector<int>& yCoordinates = a.second;
				sort(yCoordinates.begin(), yCoordinates.end());
				size_t sz = yCoordinates.size();
				for (size_t i = 0; i < sz; i++)
				{
					for (size_t j = i + 1; j < sz; j++)
					{
						pair<int, int> p{ yCoordinates[i], yCoordinates[j] };
						m2[p].emplace_back(a.first);
					}
				}
			}

			int minArea = INT32_MAX;
			bool foundRectangle = false;
			for (auto& a : m2)
			{
				vector<int>& xCoordinates = a.second;
				if (xCoordinates.size() <= 1)
					continue;
				foundRectangle = true;
				sort(xCoordinates.begin(), xCoordinates.end());

				size_t sz = xCoordinates.size();
				int height = a.first.second - a.first.first;
				int minWidth = INT32_MAX;
				for (size_t i = 1; i < sz; i++)
				{
					if (xCoordinates[i] - xCoordinates[i - 1] < minWidth)
						minWidth = xCoordinates[i] - xCoordinates[i - 1];
				}
				if (height * minWidth < minArea)
					minArea = height * minWidth;
			}

			return foundRectangle ? minArea : 0;

		}
	};

	void Test_0939_Minimum_Area_Rectangle()
	{
		Solution solu;
		string s;
		vector<int>tmp;
		vector<vector<int>>points;
		while (true)
		{
			s.clear();
			cout << "points: ";
			getline(cin, s);
			if (s.size() == 0)break;
			stringstream ss(s);
			tmp.clear();
			points.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(tmp));
			size_t sz = tmp.size();
			for (size_t i = 0; i < sz; i += 2)
			{
				points.emplace_back(initializer_list({ tmp[i], tmp[i + 1] }));
			}
			cout << solu.minAreaRect(points) << "\n\n";
		}
	}
}