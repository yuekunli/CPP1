#include "LibHeaders.h"

namespace _1311_Get_Watched_Videos_by_Your_Friends {

	using namespace std;

	class Solution
	{
		struct Video
		{
			string name;
			int freq;
			Video(string& n, int f) :name{ n }, freq{ f }{}
		};

	public:

		// level k of videos are all watched videos by people with the **shortest** path exactly equal to k with you.

		vector<string> watchedVideosByFriends(vector<vector<string>>& watchedVideos, vector<vector<int>>& friends, int id, int level)
		{
			deque<int> q[2];
			int qSwitch = 0;
			q[qSwitch].emplace_back(id);

			unordered_set<int> s;
			s.insert(id);

			for (int i = 0; i < level; i++)
			{
				deque<int>& current = q[qSwitch];
				qSwitch = (qSwitch + 1) % 2;
				deque<int>& next = q[qSwitch];
				while (!current.empty())
				{
					int u = current.front(); // user id's on the current level
					current.pop_front();
					for (int& f : friends[u])  // friends of the users(who are on current level)
					{
						if (s.find(f) == s.end())
						{
							s.insert(f);
							next.emplace_back(f);
						}
					}
				}
			}

			deque<int>& kLevelFriends = q[qSwitch];

			vector<Video> v;
			unordered_map<string, Video*> m;

			for (int& u : kLevelFriends)
			{
				for (string& vid : watchedVideos[u])
				{
					if (m.find(vid) != m.end())
						m[vid]->freq++;
					else
					{
						v.emplace_back(vid, 1);
						m[vid] = &(*(v.end()-1));
					}
				}
			}

			sort(v.begin(), v.end(),
				[](Video& a, Video& b)
				{
					if (a.freq < b.freq)
						return true;
					if (a.freq > b.freq)
						return false;
					return lexicographical_compare(a.name.begin(), a.name.end(), b.name.begin(), b.name.end());
				});

			vector<string> answer;
			transform(v.begin(), v.end(), back_inserter(answer),
				[](Video& a)->string
				{
					return a.name;
				});

			return answer;
		}
	};

	static void print(vector<string> const& v)
	{
		for (auto& s : v)
		{
			cout << s << "   ";
		}
		cout << '\n';
	}

	static void Test1()
	{
		vector<vector<string>> watchedVideos { {"A", "B"}, {"C"}, {"B", "C"}, {"D"} };
		vector<vector<int>>friends { {1,2},{0,3},{0,3},{1,2} };
		Solution s;
		vector<string>const& answer = s.watchedVideosByFriends(watchedVideos, friends, 0, 1);
		print(answer);
	}

	static void Test2()
	{
		vector<vector<string>> watchedVideos { {"A", "B"}, {"C"}, {"B", "C"}, {"D"} };
		vector<vector<int>>friends { {1,2},{0,3},{0,3},{1,2} };
		Solution s;
		vector<string>const& answer = s.watchedVideosByFriends(watchedVideos, friends, 0, 2);
		print(answer);
	}

	void Test_1311_Get_Watched_Videos_by_Your_Friends()
	{
		Test1();
		Test2();
	}
}