#include "LibHeaders.h"

namespace _1377_Frog_Position_After_T_Seconds {

	using namespace std;

	/*
	* "Given an undirected tree"
	* This statement should imply two things:
	* (1). no cycle
	* (2). fully connected
	* And these two things should jointly imply another thing:
	* there is only one path from start to target.
	*/

	class Solution
	{
	public:

		// find all the paths by which I can reach target with t hops, or with fewer than t hops and stay put

		vector<vector<int>> adjMap;

		void buildAdjMap(vector<vector<int>>& edges)
		{
			for (auto& edge : edges)
			{
				auto& v = adjMap[edge[0]];
				if (find(v.begin(), v.end(), edge[1]) == v.end())
				{
					v.emplace_back(edge[1]);
				}
				auto& v2 = adjMap[edge[1]];
				if (find(v2.begin(), v2.end(), edge[0]) == v2.end())
				{
					v2.emplace_back(edge[0]);
				}
			}
		}

		// depth-first-search every path from 1 to target
		// actually I don't need to consider multiple paths leading to target situation.
		// and I don't need to add on "probability" once I find a path.
		// I can change the "dfs" function to return boolean, once it finds path, set "probablity"
		// and return true so the caller can stop trying other paths.

		double probability = 0;

		void dfs(int current, int target, int hopsSoFar, int tSeconds, vector<int>& visited, double prob)
		{
			if (current == target && hopsSoFar == tSeconds)
			{
				probability += prob;
				return;
			}
			else if (current == target && hopsSoFar < tSeconds && adjMap[current].size() == 0)
			{
				probability += prob;
				return;
			}
			else if (current == target && hopsSoFar < tSeconds && adjMap[current].size() > 0)
			{
				// I need to check if adjacent vertices are all visited
				bool allAdjVisited = find(adjMap[current].begin(), adjMap[current].end(), 0) != adjMap[current].end() ? false : true;

				if (allAdjVisited)
				{
					probability += prob;
					return;
				}
				else
				{
					// I have to jump past the target and I can't come back, this path is no good
					return;
				}
			}
			else if (current != target && hopsSoFar < tSeconds)
			{
				// check if there are adjacent vertices that are not visited yet
				vector<int> unvisited;
				for (auto& next : adjMap[current])
				{
					if (visited[next] == 0)
					{
						unvisited.emplace_back(next);
					}
				}
				if (unvisited.size() == 0)  // acutally don't need this check
					return;
				else
				{
					double possibleNext = (double) unvisited.size();
					for (auto& next : unvisited)
					{
						visited[next] = 1;
						dfs(next, target, hopsSoFar + 1, tSeconds, visited, prob * (1.0 / possibleNext));
						visited[next] = 0;
					}
					return;
				}
			}
			else if (current != target && hopsSoFar == tSeconds)
			{
				return;
			}
			else
			{
				throw runtime_error("unaccounted scenario");
			}
		}

		double frogPosition(int n, vector<vector<int>>& edges, int t, int target)
		{
			adjMap.resize(n+1);
			buildAdjMap(edges);
			vector<int>visited(n+1,0);
			visited[1] = 1;
			dfs(1, target, 0, t, visited, 1.0);
			return probability;
		}
	};

	class Solution2
	{
		/*
		* breadth-first-search should also work.
		* What is the information I put in the bfs queue?
		* It can't just be the nodes' IDs. It needs to be nodes' IDs and the probability
		* of reaching each of those nodes.
		*                  1
		*         +--------+-------+
		*         |        |       |
		*         2        5       6
		* When I examine node#1, I put these 3 compound structures in the queue:
		* (2, 1/3), (5, 1/3), (6, 1/3)
		* When I examine node#2's children, I multiple 1/3 by their possibility of visiting.
		* If node#2 has 4 children (12, 14, 17, 19), then each gets (id, 1/3 * 1/4) 
		*/
	};

	static void Test1()
	{
		vector<vector<int>> edges = {{1, 2}, {1, 3}, {1, 7}, {2, 4}, {2, 6}, {3, 5}};

		Solution s;
		cout << s.frogPosition(7, edges, 2, 4) << '\n';
	}

	static void Test2()
	{
		vector<vector<int>> edges = {{1, 2}, {1, 3}, {1, 7}, {2, 4}, {2, 6}, {3, 5}};
		Solution s;
		cout << s.frogPosition(7, edges, 1, 7) << '\n';
	}

	void Test_1377_Frog_Position_After_T_Seconds()
	{
		Test1();
		Test2();
	}
}