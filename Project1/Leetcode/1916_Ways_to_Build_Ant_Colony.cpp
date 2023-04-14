#include "LibHeaders.h"

namespace _1916_Ways_to_Build_Ant_Colony {

	using namespace std;

	class Solution
	{
		/*
		* Node 'a' has two children 'b' and 'c'. There are two subtress rooted at 'b' and 'c'.
		* The subtree rooted at 'b' has 5 nodes in total. The one rooted at 'c' has 3 nodes in total.
		* First assume both these two subtrees are just linked lists. There is only 1 way to build
		* either subtree. Now I want to build both subtrees, I can go back and forth between these two.
		* How many ways to build them? i.e. How many ways to go back and forth?
		* This is equivalent to "there are 5 red balls and 3 blue balls, how many ways to arrangement them?"
		* Not consider the case when I swap the positions of two balls with the same color.
		* In another word, the relative order among the 5 red balls are fixed. So is that among 3 blue balls.
		* The 5 red balls create 6 spots for blue balls to fill:
		*   spot1  ball1  spot2  ball2  spot3  ball3  spot4  ball4  spot5  ball5  spot6
		* If I put the first red ball at spot3, the 2nd red ball can go to spot3, spot4, or spot5.
		* Two red balls can be adjacent so spot3 is still available.
		* First task is to figure out how many ways to arrange things with certain spots and certain objects.
		* This task itself needs a dynamic programming algorithm. Once I put down the first red ball,
		* the rest of the red balls and the rest available spots become a sub-problem.
		* 
		* So far I've been assuming the subtrees at node 'b' and 'c' are linked lists.
		* Now assume there are 8 ways to build the subtree rooted at node 'b' and there are 4 ways to
		* build the one rooted at node 'c'.
		* Select 1 certain way of building subtree at 'b', and select 1 certain way of building subtree
		* at 'c'. Now I can go back and forth between these two subtrees. Assume there are 26 different
		* ways to go back and forth.
		* 
		* plan 1 of building subtree at 'b'
		*                                              plan 1 of building subtree at 'c'
		* plan 2 of building subtree at 'b'
		*                                              plan 2 of building subtree at 'c'
		* ......
		*                                              ......
		* plan 8 of building subtree at 'b'
		*                                              plan 4 of building subtree at 'c'
		* 
		* mix and match the plans of 'b' and 'c', I got 8 * 4 = 32 different combinations.
		* For each combination, there are 26 different ways to go back and forth.
		* Put everything together, there are 8 * 4 * 26 different ways to build both subtrees.
		* 
		* I need to know the size of the subtrees rooted at each node.
		* The flaw of this solution is that process of getting the size of each subtree may not be very efficient.
		* 
		* For example if a tree degrades to a linked list:
		*  a --> b --> c --> d --> e --> f
		* 'a' is the root and 'f' is the leaf.
		* To find the size of every subtree
		* Algorithm 1: examine 'f', find all its ancestors, increment their subtrees' size by 1.
		* examine 'e', find all its ancestors, increment their subtrees' size by 1. Repeat this process
		* until I've examined all nodes. The cost is O(n^2).
		* Algorithm 2: start from 'a', recursively calculate its children's subtrees' size.
		* The recursize calls' stack size is O(n).
		*/

		int waysToArrange(vector<vector<int>>&r, int spots, int objects)
		{
			if (r[spots][objects] != -1)
				return r[spots][objects];

			if (objects == 1)
			{
				r[spots][1] = spots;
				return r[spots][1];
			}

			int total = 0;
			for (int i = 1; i <= spots; i++)
			{
				total += waysToArrange(r, spots - i + 1, objects - 1); // 5 spots, i == 2, take the 2nd, I still have 2nd, 3rd, 4th, 5th spots available
				total %= 1'000'000'007;
			}
			r[spots][objects] = total;
			return total;
		}

		void buildAdj(vector<vector<int>>& adj, vector<int>& prevRoom)
		{
			int n = prevRoom.size();
			for (int i = 0; i < n; i++)
			{
				int parent = prevRoom[i];
				if (parent != -1)
				{
					adj[parent].emplace_back(i);
				}
			}
		}

		void countSubtreeSize(vector<int>& prevRoom, vector<int>& subtreeSize)
		{
			int n = prevRoom.size();
			for (int i = 0; i < n; i++)
			{
				int parent = prevRoom[i];
				while (parent != -1)
				{
					subtreeSize[parent]++;
					parent = prevRoom[parent];
				}
			}
		}

		int solve(vector<int>& waysToBuild, vector<vector<int>>& arrange, vector<vector<int>>adj, vector<int>& subtreeSize, int currentNode)
		{
			if (waysToBuild[currentNode] != -1)
				return waysToBuild[currentNode];

			if (adj[currentNode].size() == 0)
			{
				waysToBuild[currentNode] = 1;
				return 1;
			}

			for (int i = 0; i < adj[currentNode].size(); i++)
			{
				int child = adj[currentNode][i];
				if (waysToBuild[child] == -1)
					waysToBuild[child] = solve(waysToBuild, arrange, adj, subtreeSize, child);
			}

			unsigned long long arrangement = 1;
			int child1 = adj[currentNode][0];
			int length1 = subtreeSize[child1];
			for (int i = 1; i < adj[currentNode].size(); i++)
			{
				int child2 = adj[currentNode][i];	
				int length2 = subtreeSize[child2];
				if (length1 < length2)
					swap(length1, length2);
				int a = waysToArrange(arrange, length1 + 1, length2);
				arrangement = arrangement * (unsigned long long)a % 1'000'000'007;
				length1 = length1 + length2;
			}

			unsigned long long build = arrangement;
			for (int i = 0; i < adj[currentNode].size(); i++)
			{
				int child = adj[currentNode][i];
				build = build * (unsigned long long)waysToBuild[child] % 1'000'000'007;
			}
			waysToBuild[currentNode] = static_cast<int>(build);
			return waysToBuild[currentNode];
		}

	public:

		int waysToBuildRooms(vector<int>& prevRoom)
		{
			int n = prevRoom.size();

			vector<int> subtreeSize(n, 1);
			vector<vector<int>>adj(n);
			buildAdj(adj, prevRoom);
			countSubtreeSize(prevRoom, subtreeSize);

			vector<vector<int>>arrangementRecords(n, vector<int>(n, -1));
			vector<int>waysToBuildSubtreeRecords(n, -1);
			return solve(waysToBuildSubtreeRecords, arrangementRecords, adj, subtreeSize, 0);
		}
	};

	/*
	* test case:
	*           0
	*      +----+----+
	*      1         4
	*   +--+--+   +--+--+
	*   2     3   5     6
	* 
	*  number of ways to build subtree rooted at '1': 2
	*  number of ways to build subtree rooted at '4': 2
	*  number of ways to arrange 3 red balls with 3 blue balls (4 spots with 3 objects)  : 20
	*  Overall ways to building the colony: 80
	*  test input: -1 0 1 1 0 4 4
	*/

	void Test_1916_Ways_to_Build_Ant_Colony()
	{
		Solution s;
		string s1;
		vector<int>prevRoom;
		while (true)
		{
			s1.clear();
			cout << "prevRoom: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			prevRoom.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(prevRoom));
			cout << s.waysToBuildRooms(prevRoom) << "\n";
		}
	}
}