#include<iostream>
#include<unordered_set>
#include<algorithm>
#include<queue>

namespace _0778_Swim_in_Rising_Water {

	using namespace std;

	class Solution
	{
		/*
		* This solution works but is slow. This is Dijkstra's algorithm.
		* I can never achieve Dijkstra's algorithm's theoretical performance.
		*/
	public:
		int swimInWater(vector<vector<int>>& grid)
		{
			size_t n = grid.size();
			vector<vector<int>>maxElevation(n, vector<int>(n, INT32_MAX));
			maxElevation[n - 1][n - 1] = grid[n - 1][n - 1];

			vector<pair<int, int>>heap(n * n, { 0,0 });

			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					heap[i * n + j].first = i;
					heap[i * n + j].second = j;
				}
			}

			auto comp = [&](pair<int, int>const& a, pair<int, int>const& b)
			{
				return maxElevation[a.first][a.second] > maxElevation[b.first][b.second];
			};

			make_heap(heap.begin(), heap.end(), comp);
			size_t heapLength = heap.size();

			while (true)
			{
				auto [r, c] = heap[0];
				
				if (r == 0 && c == 0)
					return maxElevation[0][0];

				pop_heap(heap.begin(), heap.begin() + heapLength, comp);
				heapLength--;

				if (r > 0)
					maxElevation[r - 1][c] = min(maxElevation[r-1][c], max(grid[r-1][c], maxElevation[r][c]));
				if (r < n-1)
					maxElevation[r + 1][c] = min(maxElevation[r + 1][c], max(grid[r + 1][c], maxElevation[r][c]));
				if (c > 0)
					maxElevation[r][c-1] = min(maxElevation[r][c-1], max(grid[r][c-1], maxElevation[r][c]));
				if (c < n-1)
					maxElevation[r][c + 1] = min(maxElevation[r][c + 1], max(grid[r][c + 1], maxElevation[r][c]));

				make_heap(heap.begin(), heap.begin()+heapLength, comp);
			}
			return 0;
		}
	};

	class Solution2
	{
		/*
		* Dijkstra's (or Prim's minimum spanning tree) algorithm requires that whenever I add a vertex into the tree,
		* I update all the connected vertices' "distance" and maintain the heap.
		* The reason that I don't need to update every connected vertices' distances is because I just need to find
		* the smallest among them.
		* 
		* +---+---+---+
		* | A | B | C |
		* +---+---+---+
		* | D | E | F |
		* +---+---+---+
		* | G | H | J |
		* +---+---+---+
		* 
		*  What is the "distance" here? The distance from cell 'E' to cell 'J' (the destination) is the maximum elevation
		*  of all paths that start at 'E' and end at 'J'. For example, let's say there are 5 paths from 'E' to 'J'. Each
		*  path has its own maximum elevation (i.e. the maximum value of all the cells on that path). The distance from 'E'
		*  to 'J' is the min of those maximum elevations.
		*  
		*  For example, I get the distance from 'F' to 'J'. 'F' is connected to 'C' and 'E'. Theoretically I need to update
		*  E's and C's distance. But if C's value is bigger than F's distance, then C's distance is dominated by its own value.
		*  If F's is connected with 10 vertices and 9 of them has a value bigger than F's distance, then updating those 9
		*  vertices' distance is just to assign them their own value. The only viable path toward the beginning point is through
		*  that 1 lonely vertex whose value is smaller than F's distance.
		* 
		*  What if there are 2 vertices (among 10 connected to F) that have smaller value than F's distance, I still find the
		*  smallest first, I go down the path where the smallest leads, but all of sudden it runs into a few big value cells.
		*  I need to "retreat". But that is fine because I can swim to any cell instantaneously. That second smallest cell
		*  connected to F is still in the priority queue, when I take the head of the priority queue, I will find it.
		* 
		* 
		*  I can't keep track of individual distance, because if I do that, I can't avoid the updating of connected vertices.
		*  So I have the heap:
		*   +---+---+---+---+---+- - - 
		*   | A | B | C |   |   |
		*   +---+---+---+---+---+- - -
		*  What is the "key" that eventually is used to compare cells of the heap?
		*  Is it the "distance" from that vertex to the destination?
		*  If I do use such "distance" (i.e. max elevation among all paths from that vertex to the destination), I'll have
		*  to keep all nodes in the heap and do exactly what Dijkstra's (or Prim's) algorithms require. i.e. whenever I add
		*  a vertex to the spanning tree, I need to update the "distance" of all connected vertices.
		*  But instead the "key" in this case is just the value of that cell in the original grid.
		*  What does this "key" mean? It means the *potential* of that cell contributes to the overall distance.
		*  i.e. if a path already has a max elevation of 20, and that cell's value is 15, it's not going to contribute to the
		*  distance of that path. It's *potential* to increase the overall distance is zero.
		*  Eventually the heap is sorted based on each cell's *potential*.
		*/
		struct PairHash
		{
			size_t operator()(pair<int, int>const& a) const
			{
				return a.first * 100 + a.second;
			}
		};

		struct PairComp
		{
			bool operator()(pair<int, int>const& a, pair<int, int>const& b) const
			{
				return a.first == b.first && a.second == b.second;
			}
		};
	public:
		int swimInWater(vector<vector<int>>& grid)
		{
			size_t n = grid.size();

			unordered_set<pair<int, int>, PairHash, PairComp> seen;
			auto pair_heap_comp = [&](pair<int, int>const& a, pair<int, int>const& b)
			{
				return grid[a.first][a.second] > grid[b.first][b.second];
			};

			priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(pair_heap_comp)> q(pair_heap_comp);

			q.push({ (int)n - 1, (int)n - 1 });
			seen.emplace((int)n - 1, (int)n - 1);

			int currentMax = grid[n - 1][n - 1];

			while (q.size() > 0)
			{
				auto [r, c] = q.top();
				q.pop();
				currentMax = max(currentMax, grid[r][c]);
				if (r == 0 && c == 0)
					return currentMax;

				if (r > 0 && seen.count({ r - 1, c }) == 0)
				{
					q.push({r-1, c});
					seen.emplace(r - 1, c);
				}
				if (r < n - 1 && seen.count({ r + 1, c }) == 0)
				{
					q.push({ r + 1, c });
					seen.emplace(r + 1, c);
				}
				if (c > 0 && seen.count({ r, c - 1 }) == 0)
				{
					q.push({ r, c - 1 });
					seen.emplace(r, c - 1);
				}
				if (c < n - 1 && seen.count({ r, c + 1 }) == 0)
				{
					q.push({ r, c + 1 });
					seen.emplace(r, c + 1);
				}
			}
			return currentMax;
		}
	};

	static void Test1()
	{
		vector <vector<int>> grid {{0, 1, 2, 3, 4}, {24, 23, 22, 21, 5}, {12, 13, 14, 15, 16}, {11, 17, 18, 19, 20}, {10, 9, 8, 7, 6}};

		Solution2 s;

		cout << s.swimInWater(grid) << "\n";

	}

	void Test_0778_Swim_in_Rising_Water()
	{
		Test1();
	}
}