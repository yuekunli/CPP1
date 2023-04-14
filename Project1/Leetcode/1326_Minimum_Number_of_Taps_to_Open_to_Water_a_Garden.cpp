#include "LibHeaders.h"

namespace _1326_Minimum_Number_of_Taps_to_Open_to_Water_a_Garden {

using namespace std;

class Solution
{
public:

	int minTaps(int n, vector<int>& ranges)
	{
		//multimap<int, int> m; // key is the start point of a segment, value is the end point. A segment is the range a tap can cover
		                      // Do I really need a multiset? Probably not, if multiple segments start at the same point, why don't I just
		                      // the one that reaches farthest.
		map<int, int>m;

		int i = 0;
		for (auto& radius : ranges)
		{
			int left = i - radius > 0 ? i - radius : 0;
			int right = i + radius;
			//m.insert({ left, right });
			if (m.find(left) == m.end())
				m[left] = right;
			else
				if (m[left] < right)
					m[left] = right;
			i++;
		}

		int currentCover = 0;  // the farthest point current open taps can cover.
		                       // the next tap must cover an interval starting on or before this point
		int taps = 0;
		int farthest = 0;

		for (auto& interval : m)
		{
			if (interval.first <= currentCover)
			{
				if (farthest < interval.second)
					farthest = interval.second;
			}
			else
			{
				if (farthest <= currentCover)
					// no previous interval's end can go beyond currentCover
					// yet this interval's start is already out of currentCover
					// there is a gap.
					break;
				else
				{
					currentCover = farthest;
					// Do not reset farthest
					// the next set of intervals I check may all be very short
					// their starts are within currentCover
					// their ends may be also within currentCover
					// so keep farthest equal to currentCover
					// when comparing ends with farthest, I essentially compare them with currentCover
					taps++;
				}
				if (interval.first <= currentCover && farthest < interval.second) // I'm actually examining the first segment of the next "group".
					                                                              // the group who is out of the old current cover and who
					                                                              // is potentially under the newly updated current cover.
					farthest = interval.second;
				else
					/*
					*             old cover
					*                |
					*     -----------|
					*         --------------|
					*            -----------------|
					*                             ^    |------|
					*                             |      examining this segment
					*                     just reset cover to this point
					* 
					* I'm examining that short segment, it's out of old current cover, so I reset current cover to
					* the farthest point of the segments that start under the old current cover.
					* Now the current cover has been updated, but the short segment is still out of the updated
					* current cover. So there is a gap. 
					*/
					break;
			}
			if (currentCover >= n)
				break;
		}
		if (currentCover >= n)
			return taps;
		else
			return -1;
	}
};


void Test1()
{
	vector<int> ranges { 3,4,1,1,0,0 };
	Solution s;
	cout << s.minTaps(5, ranges) << '\n';
}

void Test2()
{
	vector<int> ranges { 0,0,0,0 };
	Solution s;
	cout << s.minTaps(3, ranges) << '\n';
}

void Test_1326_Minimum_Number_of_Taps_to_Open_to_Water_a_Garden()
{
	Test1();
	Test2();
}

}