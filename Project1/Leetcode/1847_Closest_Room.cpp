#include "LibHeaders.h"

namespace _1847_Closest_Room {

	using namespace std;

	class Solution
	{
		/*
		* May not be the best solution, need re-visit!
		*/

		/*
		* The idea is that cut the array of rooms into a few segments.
		* The dividing points between segments are queries' size requirements.
		* For example:
		* this is the rooms' sizes in sorted order:
		* 1  4  7  12  14  17  23  25  27  30  31  35
		*            |                |
		*            13               26
		* I have two queries, whose size requirements are '13' and '26' respectively.
		* Prepare two ordered sets, put the room Ids whose size within 13 and 26 in the 1st ordered set.
		* put the room IDs whose size greater than 26 in the 2nd ordered set.
		* For the query that require size 13, I need to search both ordered sets. For the query
		* that requires size 26, I need to search the 2nd ordered set.
		*/
	public:

		vector<int> closestRoom(vector<vector<int>>& rooms, vector<vector<int>>& queries)
		{
			int qlen = queries.size();
			vector<int>querySizes(qlen);
			transform(queries.begin(), queries.end(), querySizes.begin(), [](vector<int>& q) {return q[1]; });
			sort(querySizes.begin(), querySizes.end());
			vector<int> distinctQuerySize;
			distinctQuerySize.emplace_back(querySizes[0]);

			int j = 0;
			for (int i = 1; i < qlen; i++)
			{
				if (querySizes[i] > distinctQuerySize[j])
				{
					distinctQuerySize.emplace_back(querySizes[i]);
					j++;
				}
			}

			int numberOfBags = j + 1;
			vector<set<int>>roomIdsInRange(numberOfBags);

			for (vector<int>& r : rooms)
			{
				int id = r[0];
				int sz = r[1];

				auto it = upper_bound(distinctQuerySize.begin(), distinctQuerySize.end(), sz);
				if (it != distinctQuerySize.begin())
				{
					it = it - 1;

					// distinct size:  3   7   10   15   20   22
					//                       |
					//                      this room's size is 9
					// bags' indexes:    0   1    2    3    4     5
					// if an iterator in "distinctSize" array points to "15", its distance to begin() is '3'.
					// The bag containing room Ids between 15 and 20 is the #3 bag
					int index = distance(distinctQuerySize.begin(), it);
					roomIdsInRange[index].insert(id);
				}
				else
					continue;
			}

			vector<int>answer(qlen);
			for (int i = 0; i < qlen; i++)
			{
				int id = queries[i][0];
				int sz = queries[i][1];
				auto it = lower_bound(distinctQuerySize.begin(), distinctQuerySize.end(), sz);
				int bagIdx = distance(distinctQuerySize.begin(), it);
				int closestRoom = -10'000'000;
				while (bagIdx < numberOfBags)
				{
					if (roomIdsInRange[bagIdx].size() == 0)
					{
						bagIdx++;
						continue;
					}
					auto it2 = roomIdsInRange[bagIdx].lower_bound(id);
					if (it2 != roomIdsInRange[bagIdx].end())
					{
						if (*it2 == id)
						{
							closestRoom = id;
							break;
						}
						if (abs(*it2 - id) < abs(closestRoom - id))
						{
							closestRoom = *it2;
						}
					}
					if (it2 != roomIdsInRange[bagIdx].begin())
					{
						it2 = prev(it2);
						if (abs(*it2 - id) < abs(closestRoom - id))
						{
							closestRoom = *it2;
						}
					}
					bagIdx++;
				}
				answer[i] = closestRoom > 0 ? closestRoom : -1;
			}
			return answer;
		}
	};


	/*
	* Test cases:
	* rooms: 2 2 1 2 3 2
	* queries: 3 1 3 3 5 2
	* answer: 3 -1 3
	* 
	* rooms: 1 4 2 3 3 5 4 1 5 2
	* queries: 2 3 2 4 2 5
	* answer: 2 1 3
	*/
	void Test_1847_Closest_Room()
	{
		Solution s;

		string s1;
		vector<vector<int>>rooms;
		vector<vector<int>>queries;
		vector<int>temp;
		while (true)
		{
			s1.clear();
			cout << "rooms: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			temp.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(temp));
			rooms.clear();
			for (int i = 0; i <= temp.size() - 2; i += 2)
			{
				rooms.emplace_back(initializer_list<int>{temp[i], temp[i+1]});
			}
			s1.clear();
			cout << "queries: ";
			getline(cin, s1);
			stringstream ss2(s1);
			temp.clear();
			copy(istream_iterator<int>(ss2), istream_iterator<int>(), back_inserter(temp));
			queries.clear();
			for (int i = 0; i <= temp.size() - 2; i += 2)
			{
				queries.emplace_back(initializer_list<int>{temp[i], temp[i + 1]});
			}
			vector<int>const&answer = s.closestRoom(rooms, queries);
			copy(answer.begin(), answer.end(), ostream_iterator<int>(std::cout, " "));
			cout << "\n\n";
		}
	}
}