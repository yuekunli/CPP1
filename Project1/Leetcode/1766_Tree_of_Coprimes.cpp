#include "LibHeaders.h"

namespace _1766_Tree_of_Coprimes {

	using namespace std;



	/*
	*
	* node value in parentheses
	* 
	*              0(5)
	*         +-----+-----+
	*         |
	*       1(6)
	*    +---+----+
	*    |
	*  2(7)
	* 
	* 
	* For example, if I descend down from node#0, I create an array "nearest_coprime_ancestor_idx" (ncai) of size 51 (need #1 to #50)
	* I fill in some cells of ncai and pass it down to node#1. When I stand on node#0, for example, I fill ncai[7] with 0.
	* This means, for the next node on my path down, if its value is '7', it should use index 0 as its nearest coprime ancestor's index.
	* Likewide, I would set ncai[6] to '0' too. When I stand on node#1 whose value really is 6, so node#1's nearest coprime ancestor's index is '0'
	* Before descending down further, I need to make a copy of ncai and overwrite some cells and then pass this new copy of ncai downward.
	* What cells do I need to overwrite? for example, ncai[7] is now set to '1', because if the next node has value '7', its *nearest*
	* coprime ancestor index is '1' now. '6' and '7' are coprimes too.
	* 
	*/


	class Solution2
	{
	public:

		int gcd(int a, int b)
		{
			if (b == 0) return a;

			return gcd(b, a % b);
		}

		void buildCoprimeList(vector<vector<int>>& cop)
		{
			for (int i = 1; i <= 50; i++)
			{
				for (int j = 1; j <= 50; j++)
				{
					if (gcd(i, j) == 1)
						cop[i].push_back(j);
				}
			}
		}

		void buildAdjList(vector<vector<int>>& adj, vector<vector<int>>& edges)
		{
			for (vector<int>& e : edges)
			{
				int n1 = e[0];
				int n2 = e[1];
				adj[n1].push_back(n2);
				adj[n2].push_back(n1);
			}
		}

		/*
		* If it's guaranteed to be a tree with a root, each node only has 1 parent, when I run dfs,
		* I just need to make sure I don't re-visit its parent node, so instead of keeping track of
		* all visited node, I just pass in the parent node.
		*/

		void dfs(vector<vector<int>>& adj, vector<int>&nodeIdValue, vector<vector<int>>&cop, int nodeId, 
			unordered_set<int>&visited, vector<vector<int>>&closestCoprimeNodeId, vector<int>&answer)
		{
			int nodeValue = nodeIdValue[nodeId];
			answer[nodeId] = closestCoprimeNodeId[nodeValue].size() > 0  ?  *(closestCoprimeNodeId[nodeValue].rbegin()) : -1;

			for (int& i : cop[nodeValue])
				closestCoprimeNodeId[i].push_back(nodeId);

			visited.insert(nodeId);

			for (int& connected : adj[nodeId])
			{
				if (visited.find(connected) == visited.end())
				{
					dfs(adj, nodeIdValue, cop, connected, visited, closestCoprimeNodeId, answer);
				}
			}

			for (int& i : cop[nodeValue])
				closestCoprimeNodeId[i].pop_back();
		}

		vector<int> getCoprimes(vector<int>& nums, vector<vector<int>>& edges)
		{
			int n = nums.size();
			vector<vector<int>> adj(n);
			vector<vector<int>> cop(51);

			buildAdjList(adj, edges);
			buildCoprimeList(cop);

			vector<int> answer(n);
			unordered_set<int>visited;
			vector<vector<int>> closestCoprimeNodeId(51);
			dfs(adj, nums, cop, 0, visited, closestCoprimeNodeId, answer);

			return answer;
		}
	};




	class Solution3
	{
		/*
		*              6(13)
		*         +------+------+
		*         |
		*        710(4)
		*          |
		*          +------+
		*                 |
		*                336(25)
		* 
		* I keep a list of the last time I see a certain value:
		* 1: <12, 281>, <15, 440>, <21, 375>, <23, 712>, <31, 78>   (this means I saw value '1' at level 12 node#281, then at level 15 node#440, I last saw it at level 31 node#78)
		* 2:
		* 3:
		* 4:
		* ...
		* 
		* When I arrive at node#6, I push_back '6' to the list linked to '13', because I last saw value '13' at node#6.
		* Now I arrive at node#710. This node's value is 4, I still pre-calculate all the coprimes of all 50 numbers.
		* 4's coprimes are: 1, 3, 5, 7, 9, 11, 13, 15, 17, .....
		* Now for example, I last saw value '1' at level 3, and saw value '3' and level 10, and saw value '5' at level 7,....
		* So I find the deepest level among all the coprimes of '4', the node at that level is the answer.
		*/
	};




	class Solution
	{
	public:
		void fillAdjList(vector<vector<int>>& edges, vector<vector<int>>& adjList)
		{

		}

		void fillParentArray(vector<vector<int>>& adjList, int* parent)
		{

		}

		void fillCoprimesMatrix(bool cpm[51][51])
		{

		}

		int gcd(int a, int b)
		{
			if (b == 0) return a;

			return (b, a % b);
		}

		vector<int> getCoprimes(vector<int>& nums, vector<vector<int>>& edges)
		{
			vector<vector<int>> adjList; // adjacency list
			int len = nums.size();
			int* parent = new int[len];  // set node#0's parent '-1'
			for (int i = 0; i < len; i++)
				parent[i] = -1;

			bool coprimesMatrix[51][51]{};

			fillCoprimesMatrix(coprimesMatrix);
			fillAdjList(edges, adjList);
			fillParentArray(adjList, parent);

			vector<int> answer{ len, -2 };

			for (int nodeIdx = 0; nodeIdx < len; nodeIdx++)
			{
				if (answer[nodeIdx] == -2)
				{
					int parentIdx = parent[nodeIdx];
					while (parentIdx != -1 && !coprimesMatrix[nums[nodeIdx]][nums[parentIdx]])
					{
						parentIdx = parent[parentIdx];
					}
					answer[nodeIdx] = parentIdx;
				}
			}
			return answer;
		}
	};





	static void Test1()
	{
		vector<int> nums{ 2, 3, 3, 2 };
		vector<vector<int>> edges{ {0, 1}, {1, 2}, {1, 3} };

		Solution2 s;

		vector<int>const& answer = s.getCoprimes(nums, edges);
		for (int const& i : answer)
			cout << i << ' ';
		cout << '\n';
	}

	static void Test2()
	{
		vector<int> nums{ 5, 6, 10, 2, 3, 6, 15 };
		vector<vector<int>>edges{ {0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {2, 6} };
		Solution2 s;
		vector<int>const& answer = s.getCoprimes(nums, edges);
		for (int const& i : answer)
			cout << i << ' ';
		cout << '\n';
	}

	void Test_1766_Tree_of_Coprimes()
	{
		Test1();
		Test2();
	}
}