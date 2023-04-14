#include "LibHeaders.h"

namespace _2092_Find_All_People_With_Secret {

	using namespace std;

	class Solution
	{

		struct A
		{
			int id;
			A* p;
			int r; // rank
			A() :id(0), p(nullptr), r(0) {}
		};

		void makeSet(vector<A*>&nodes, int id)
		{
			if (nodes[id] == nullptr)
			{
				nodes[id] = new A{};
				nodes[id]->id = id;
				nodes[id]->p = nodes[id];
			}
		}

		A* findSet(A* a)
		{
			if (a != a->p)
				a->p = findSet(a->p);
			return a->p;
		}

		void link(A* x, A* y)
		{
			if (x->r > y->r)
				y->p = x;
			else
			{
				x->p = y;
				if (x->r == y->r)
					y->r++;
			}
		}

		void unionSet(A* x, A* y)
		{
			link(findSet(x), findSet(y));
		}

		void secretSpread(vector<vector<int>>& meetings, vector<bool>& records, vector<A*>& nodes, int startIndex, int endIndex)
		{
			unordered_map<int, vector<int>>m;
			vector<int>trueSets;

			for (int i = startIndex; i <= endIndex; i++)
			{
				makeSet(nodes, meetings[i][0]);
				makeSet(nodes, meetings[i][1]);
				unionSet(nodes[meetings[i][0]], nodes[meetings[i][1]]);
			}

			for (int i = 0; i < nodes.size(); i++)
			{
				if (nodes[i] != nullptr)
				{
					A* const& p = findSet(nodes[i]);

					m[p->id].push_back(i);
					if (records[i])
						trueSets.push_back(p->id);
				}
			}

			for (int i = 0; i < trueSets.size(); i++)
			{
				vector<int> &v = m[trueSets[i]];
				for (int& k : v)
					records[k] = true;
			}
		}

	public:
		vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson)
		{
			vector<bool> records(n, false);
			records[0] = records[firstPerson] = true;

			sort(meetings.begin(), meetings.end(),
				[](auto const&a, auto const& b)
				{
					return a[2] < b[2];
				});

			vector<A*>nodes(n, nullptr);

			int time = meetings[0][2];
			int startIndex = 0;
			
			for (int i = 1; i < meetings.size(); i++)
			{
				if (meetings[i][2] != time)
				{
					secretSpread(meetings, records, nodes, startIndex, i - 1);		
					time = meetings[i][2];
					startIndex = i;
					for (auto& a : nodes)
						if (a != nullptr)
						{
							delete a;
							a = nullptr;
						}
				}
			}
			secretSpread(meetings, records, nodes, startIndex, meetings.size() - 1);
			for (auto& a : nodes)
				if (a != nullptr)
				{
					delete a;
					a = nullptr;
				}


			int m = count(records.begin(), records.end(), true);
			vector<int>answer(m);
			int i = 0, j = 0;
			while (i < n)
			{
				if (records[i])
				{
					answer[j] = i;
					j++;
				}
				i++;
			}
			return answer;
		}
	};



	class Solution2
	{

		/*
		* This is accepted, but not very good, only faster than 6%.
		* This takes advantage of that fact that at a certain time point, there aren't too many meetings happen simultaneously.
		*/

		void secretSpread(vector<vector<int>>& meetings, vector<bool>& records, int startIndex, int endIndex)
		{
			unordered_map<int, int>idToGroup;
			unordered_map<int, vector<int>>groupToMembers;
			int groupID = 0;

			for (int i = startIndex; i <= endIndex; i++)
			{
				int id1 = meetings[i][0];
				int id2 = meetings[i][1];
				int id1Group, id2Group;
				if (idToGroup.count(id1) == 0)
				{
					id1Group = groupID;
					groupID++;
					idToGroup[id1] = id1Group;
					groupToMembers[id1Group].push_back(id1);
				}
				else
				{
					id1Group = idToGroup[id1];
				}

				if (idToGroup.count(id2) == 0)
				{
					id2Group = id1Group;
					idToGroup[id2] = id2Group;
					groupToMembers[id2Group].push_back(id2);
				}
				else
				{
					id2Group = idToGroup[id2];
				}

				if (id1Group != id2Group)
				{
					vector<int>* v1, *v2;
					int keepGroup, deleteGroup;
					if (groupToMembers[id1Group].size() < groupToMembers[id2Group].size())
					{
						v1 = &groupToMembers[id1Group];
						v2 = &groupToMembers[id2Group];
						keepGroup = id2Group;
						deleteGroup = id1Group;
					}
					else
					{
						v1 = &groupToMembers[id2Group];
						v2 = &groupToMembers[id1Group];
						keepGroup = id1Group;
						deleteGroup = id2Group;
					}

					for (int& i : *v1)
					{
						idToGroup[i] = keepGroup;
						v2->push_back(i);
					}

					groupToMembers.erase(deleteGroup);

				}
			}

			for (auto& v : groupToMembers)
			{
				if (any_of(v.second.begin(), v.second.end(), [&](int& a) {return records[a]; }))
				{
					for (int& a : v.second)
						records[a] = true;
				}
			}
		}

	public:
		vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson)
		{
			vector<bool> records(n, false);
			records[0] = records[firstPerson] = true;

			sort(meetings.begin(), meetings.end(),
				[](auto const& a, auto const& b)
				{
					return a[2] < b[2];
				});


			int time = meetings[0][2];
			int startIndex = 0;

			for (int i = 1; i < meetings.size(); i++)
			{
				if (meetings[i][2] != time)
				{
					secretSpread(meetings, records, startIndex, i - 1);
					time = meetings[i][2];
					startIndex = i;
				}
			}
			secretSpread(meetings, records, startIndex, meetings.size() - 1);

			int m = count(records.begin(), records.end(), true);
			vector<int>answer(m);
			int i = 0, j = 0;
			while (i < n)
			{
				if (records[i])
				{
					answer[j] = i;
					j++;
				}
				i++;
			}
			return answer;
		}
	};


	static void Test1()
	{
		Solution2 solu;
		int n = 6;
		vector<vector<int>>meetings{ {1,2,5}, {2,3,8}, {1,5,10} };
		int first = 1;
		vector<int>const& answer = solu.findAllPeople(n, meetings, first);
		copy(answer.begin(), answer.end(), ostream_iterator<int>(std::cout, " "));
		cout << "\n\n";
	}

	static void Test2()
	{
		Solution2 solu;
		int n = 4;
		vector<vector<int>>meetings{ {3,1,3}, {1,2,2}, {0,3,3} };
		int first = 3;
		vector<int>const& answer = solu.findAllPeople(n, meetings, first);
		copy(answer.begin(), answer.end(), ostream_iterator<int>(std::cout, " "));
		cout << "\n\n";
	}

	static void Test3()
	{
		Solution2 solu;
		int n = 5;
		vector<vector<int>>meetings{ {3,4,2}, {1,2,1}, {2,3,1} };
		int first = 1;
		vector<int>const& answer = solu.findAllPeople(n, meetings, first);
		copy(answer.begin(), answer.end(), ostream_iterator<int>(std::cout, " "));
		cout << "\n\n";
	}

	static void Test4()
	{
		Solution2 solu;
		int n = 6;
		vector<vector<int>>meetings{ {0,2,1}, {1,3,1}, {4,5,1} };
		int first = 1;
		vector<int>const& answer = solu.findAllPeople(n, meetings, first);
		copy(answer.begin(), answer.end(), ostream_iterator<int>(std::cout, " "));
		cout << "\n\n";
	}

	void Test_2092_Find_All_People_With_Secret()
	{
		Test1();
		Test2();
		Test3();
		Test4();
	}
}