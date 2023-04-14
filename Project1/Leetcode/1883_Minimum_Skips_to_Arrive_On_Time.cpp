#include "LibHeaders.h"

namespace _1883_Minimum_Skips_to_Arrive_On_Time {
	using namespace std;
	class Solution
	{
		/*
		* This is a brute force breadth-first-search solution.
		* But this won't work with the given constraints.
		* 1 <= n <= 1000, I need 1000 bits to represent which stops to rest/skip
		*/

		int n; // number of roads
		int numberOfRests;
		int base;
		bool isOnTime(vector<int>& dist, int skipMask, int speed, int hoursBefore)
		{
			int totalTime = 0;

			int accumulatedRoadLength = 0;
			for (int i = 0; i < n-1; i++)
			{
				accumulatedRoadLength += dist[i];
				bool keepGoingAfterThisRoad = skipMask & (base >> i);  // 0: rest, 1: skip
				if (keepGoingAfterThisRoad)
					continue;
				else
				{
					totalTime += std::ceil((float)accumulatedRoadLength/(float)speed);
					accumulatedRoadLength = 0;
				}
			}
			accumulatedRoadLength += dist[n - 1];
			int lastDashTime = accumulatedRoadLength / speed;
			int lastDashResidual = accumulatedRoadLength - speed * lastDashTime;
			if (totalTime + lastDashTime > hoursBefore)
				return false;
			if (totalTime + lastDashTime == hoursBefore && lastDashResidual > 0)
				return false;
			return true;
		}

	public:

		int minSkips(vector<int>& dist, int speed, int hoursBefore)
		{
			n = dist.size();
			// for example, if there are 8 roads, the maximum rests I can take is 7
			numberOfRests = n - 1;
			base = 1 << (numberOfRests - 1);

			vector<int> q[2];
			int qSwitch = 0;
			vector<int>* current = &q[0];
			vector<int>* next = &q[1];
			unordered_set<int> seen;
			for (int i = 0; i < numberOfRests; i++)
			{
				current->emplace_back(base>>i);
				seen.emplace(base >> i);
			}
			int skips = 1;
			while (current->size() > 0)
			{
				for (int& mask : *current)
				{
					if (isOnTime(dist, mask, speed, hoursBefore))
						return skips;
					else
					{
						for (int i = 0; i < numberOfRests; i++)
						{
							if (!(mask & (base >> i)))
							{
								int newMask = mask | (base >> i);
								if (seen.count(newMask) == 0)
								{
									next->emplace_back(newMask);
									seen.emplace(newMask);
								}
							}
						}
					}
				}
				qSwitch = (qSwitch + 1) % 2;
				current->clear();
				current = next;
				next = &q[qSwitch];
				skips++;
			}
			return -1;
		}
	};

	class Solution2
	{
		/*
		* The "accumulatedLength" in this solution is different than the "accumulatedRoadLength" in Solution 1.
		* Progression state is defined by 3 dimensions:
		* (1). which road am I considering
		* (2). how many hours left to make it on time
		* (3). the skip streak right before this road
		* 
		* basically speaking, a cell in the record book tells me: when I stand at a stop, with certain time left,
		* given the fact that I have skipped a certain number of stops in a row right before this stop, what is
		* the minimum skip I have to do in order to make it to the meeting on time.
		*/

		int n;
		int stops;
		int mininumSkips = 2000;  // constraints: there are at most 1000 roads
		int speed_;
		int hoursBefore_;
		/*
		*    road#0         road#1        road#2        road#3        road#4        road5
		*           stop#0         stop#1        stop#2        stop#3        stop#4
		*/


		int solve(vector<vector<vector<int>>>& r, vector<int>& dist, int skipsStreak, int index, int timeLeft,
			int accumulatedTotalLength, int accumulatedLengthSinceLastStart)
		{
			//if (skipsSoFar > mininumSkips)
			//	return 2000;

			if (accumulatedTotalLength > speed_ * hoursBefore_)
				return 2000;

			if (index == stops)
			{
				// I have traveled all the roads
				if (accumulatedTotalLength + dist[index] <= speed_ * hoursBefore_)
					return 0;
				else
					return 2000;
			}
			if (timeLeft < 0)
				return 2000;

			if (r[index][timeLeft][skipsStreak] != -1)
				return r[index][timeLeft][skipsStreak];


			// if I want to rest:
			int lastDash = accumulatedLengthSinceLastStart + dist[index];
			int lastDashTime = ceil((float)lastDash / (float)speed_);
			int lastDashEquivalentLength = lastDashTime * speed_;
			int extra = lastDashEquivalentLength - lastDash;

			int result1 = solve(r, dist, 0, index + 1, timeLeft-lastDashTime, accumulatedTotalLength + dist[index] + extra, 0);
			

			// if I want to skip (keep going):

			int result2 = solve(r, dist, skipsStreak+1, index + 1, timeLeft, accumulatedTotalLength + dist[index], accumulatedLengthSinceLastStart + dist[index]);

			r[index][timeLeft][skipsStreak] = min({ result1, result2+1 });
			return r[index][timeLeft][skipsStreak];
		}

	public:
		int minSkips(vector<int>& dist, int speed, int hoursBefore)
		{
			n = dist.size();
			stops = n - 1;
			speed_ = speed;
			hoursBefore_ = hoursBefore;
			vector<vector<vector<int>>> recordbook(n, vector<vector<int>>(hoursBefore+1, vector<int>(n, -1)));
			int result = solve(recordbook, dist, 0, 0, hoursBefore, 0, 0);
			return result >= 2000 ? -1 : result;
		}
	};

	void Test_1883_Minimum_Skips_to_Arrive_On_Time()
	{
		Solution2 s;
		string s1;
		vector<int>dist;
		int speed, hoursBefore;
		vector<int>tmp;
		while (true)
		{
			s1.clear();
			cout << "distances, speed, hours before: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			tmp.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(tmp));
			int sz = tmp.size();
			dist.assign(sz - 2, 0);
			copy(tmp.begin(), tmp.end() - 2, dist.begin());
			speed = tmp[sz - 2];
			hoursBefore = tmp[sz - 1];
			cout << s.minSkips(dist, speed, hoursBefore) << "\n\n";
		}
	}
}