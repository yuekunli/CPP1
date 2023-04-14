#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<numeric>

namespace _0911_Online_Election {

	using namespace std;

	class TopVotedCandidate
	{
	public:

		struct TimeLeader
		{
			int epoch;
			int leader;
			TimeLeader(int epoch_, int leader_) :epoch(epoch_), leader(leader_) {}
		};

		vector<TimeLeader> epochLeaderChronicle;

		unordered_map<int, int> m; // "candidate id" : "votes received"
		                           // another approach is to scan the "persons" array once, and find the max id, and use an array to keep tally

		TopVotedCandidate(vector<int>& persons, vector<int>& times)
		{
			int currentLeader = -1;

			// using "inner_product" is kind of an over-kill. All I want is just to process each (vote, time) pair.
			(void)inner_product(persons.begin(), persons.end(), times.begin(), 0,
				[](int const& a, int const& b)
				{
					return 0;
				},
				[&](int const& id, int const& t)
				{
					if (m.find(id) == m.end())
					{
						m[id] = 1;
					}
					else
					{
						m[id]++;
					}
					if (currentLeader == -1 || (currentLeader != id && m[currentLeader] <= m[id]))
					{
						// leader change
						epochLeaderChronicle.emplace_back(t, id);
						currentLeader = id;
					}
					return 0;
				});
		}

		int q(int t)
		{
			auto compLambda = [&](TimeLeader const& a, int const& b)
			{
				return a.epoch < b;
			};

			auto lb_it = lower_bound(epochLeaderChronicle.begin(), epochLeaderChronicle.end(), t, compLambda);

			if (lb_it != epochLeaderChronicle.end())
			{
				if (lb_it->epoch == t)
					return lb_it->leader;
				else
				{
					if (lb_it == epochLeaderChronicle.begin()) // no vote have been cast at time t
						return -1;
					else
						return prev(lb_it)->leader;
				}
			}
			else
			{
				return (epochLeaderChronicle.rbegin())->leader;
			}
		}
	};

	void Test_0911_Online_Election()
	{
		vector<int>persons{0,1,1,0,0,1,0};
		vector<int>times{0,5,10,15,20,25,30};

		vector<int>query_times{3,12,25,15,24,8};
		vector<int>query_results;
		
		TopVotedCandidate* obj = new TopVotedCandidate(persons, times);

		transform(query_times.begin(), query_times.end(), back_inserter(query_results),
			[&](int&t) {
				int result = obj->q(t);
				return (result);
			});

		for (int& i : query_results)
			cout << i << ' ';
		cout << "\n";

		delete obj;
	}
}