#include "LibHeaders.h"

namespace _1786_Number_Restricted_Paths_From_First_to_Last_Node {

	using namespace std;

	class Solution
	{
	public:

		struct Edge
		{
			int node;
			int cost;
		};
		struct Vertex
		{
			int parent{ -1 };
			int dist{numeric_limits<int>::max()};
		};
		int countRestrictedPaths(int n, vector<vector<int>>& edges)
		{
			vector<vector<Edge>> adj(n+1);
			for (vector<int>& e : edges)
			{
				adj[e[0]].emplace_back(e[1], e[2]);
				adj[e[1]].emplace_back(e[0], e[2]);
			}
			vector<Vertex> vertices(n + 1);   // The key is to make the vertices stationary, and the heap stores a number of pointers.

			vertices[n].dist = 0;

			vector<vector<Vertex>::iterator> vHeap(n + 1);
			
			for (int i = 0; i <= n; i++)
			{
				vHeap[i] = vertices.begin() + i;
			}
			
			make_heap(vHeap.begin(), vHeap.end(),
				[](auto const& a, auto const& b)
				{return a->dist > b->dist; });   
				// if Comp function implements "less than" relation, the heap is going to be max-heap, I want a min-heap instead

			for (int i = 0; i < n; i++)
			{
				pop_heap(vHeap.begin(), vHeap.end() - i);
				auto v = vHeap.end() - i - 1;
				int id = distance(vertices.begin(), *v);
				for (Edge& e : adj[id])
				{
					if (vertices[e.node].dist > (*v)->dist + e.cost)
					{
						vertices[e.node].dist = (*v)->dist + e.cost;
						vertices[e.node].parent = id;
					}
				}
				make_heap(vHeap.begin(), vHeap.end()-i-1,
					[](auto const& a, auto const& b)
					{return a->dist > b->dist; });
			}

			vector<int> r(n, -1);
			int result = countRestrictedPathAfterShortestPathFound(r, vertices, adj, 1);
			return result;
		}


		int countRestrictedPathAfterShortestPathFound(vector<int>& r, vector<Vertex>& vert, vector<vector<Edge>>&adj, int currentVertex)
		{
			if (r[currentVertex] != -1)
				return r[currentVertex];

			int paths = 0;
			for (Edge& e : adj[currentVertex])
			{
				if (e.node == vert.size() - 1)
					paths++;
				else if (vert[e.node].dist < vert[currentVertex].dist)
					paths += countRestrictedPathAfterShortestPathFound(r, vert, adj, e.node);
			}
			r[currentVertex] = paths;
			return paths;
		}
	};


	class Solution2
	{
		/*
		* Mostly the same as Solution 1, the difference is in the implementation of Dijkstra's
		* shortest path algorithm. Instead of storing iterators in the heap, just storing index
		*/
	public:

		struct Edge
		{
			int node;
			int cost;
		};
		struct Vertex
		{
			int parent{ -1 };
			int dist{ numeric_limits<int>::max() };
		};
		int countRestrictedPaths(int n, vector<vector<int>>& edges)
		{
			vector<vector<Edge>> adj(n + 1);
			for (vector<int>& e : edges)
			{
				adj[e[0]].emplace_back(e[1], e[2]);
				adj[e[1]].emplace_back(e[0], e[2]);
			}
			vector<Vertex> vertices(n + 1);   // The key is to make the vertices stationary, and the heap stores a number of pointers.

			vertices[n].dist = 0;

			vector<int> vHeap(n + 1);
			iota(vHeap.begin(), vHeap.end(), 0);

			make_heap(vHeap.begin(), vHeap.end(),
				[&](int & a, int & b)
				{
					return vertices[a].dist > vertices[b].dist;
					// if Comp function implements "less than" relation, the heap is going to be max-heap, I want a min-heap instead
				});
			

			for (int i = 0; i < n; i++)
			{
				// heap:  0 1 2 3 4 5 6 7 8
				//                    |      ^
				//                end()-3    |
				//                           this is end()
				// when i is 3, that means 3 elements at the end of the container are *not* part of the heap
				// So #6, #7 and #8 are not in the heap. [0, 6) is the valid range for the heap.
				pop_heap(vHeap.begin(), vHeap.end() - i);
				auto it = vHeap.end() - i - 1;
				int id = *it;
				for (Edge& e : adj[id])
				{
					if (vertices[e.node].dist > vertices[id].dist + e.cost)
					{
						vertices[e.node].dist = vertices[id].dist + e.cost;
						vertices[e.node].parent = id;
					}
				}
				make_heap(vHeap.begin(), vHeap.end() - i - 1,   // when i is n-1, this is equivalent to make_heap(begin(), begin(), ...); it's a no-op
					[&](auto const& a, auto const& b)
					{
						return vertices[a].dist > vertices[b].dist;
					});
			}

			vector<int> r(n+1, -1);
			r[n] = 1;
			int result = countRestrictedPathAfterShortestPathFound(r, vertices, adj, 1);
			return result;
		}


		int countRestrictedPathAfterShortestPathFound(vector<int>& r, vector<Vertex>& vert, vector<vector<Edge>>& adj, int currentVertex)
		{
			if (r[currentVertex] != -1)
				return r[currentVertex];

			int paths = 0;
			for (Edge& e : adj[currentVertex])
			{
				if (vert[e.node].dist < vert[currentVertex].dist)
					paths += (countRestrictedPathAfterShortestPathFound(r, vert, adj, e.node) % 1'000'000'007);
			}
			r[currentVertex] = paths;
			return paths;
		}
	};



	void Test1()
	{
		vector<vector<int>>edges{ {1, 2, 3}, {1, 3, 3}, {2, 3, 1}, {1, 4, 2}, {5, 2, 2}, {3, 5, 1}, {5, 4, 10} };
		int n = 5;
		Solution2 s;
		cout << s.countRestrictedPaths(n, edges) << "\n\n";
	}

	void Test2()
	{
		vector<vector<int>>edges {{1, 3, 1}, {4, 1, 2}, {7, 3, 4}, {2, 5, 3}, {5, 6, 1}, {6, 7, 2}, {7, 5, 3}, {2, 6, 4}};
		int n = 7;
		Solution s;
		cout << s.countRestrictedPaths(n, edges) << "\n\n";
	}


	void Test_1786_Number_Restricted_Paths_From_First_to_Last_Node()
	{
		function<void(void)> tests[] = {Test1, Test2};
		for (auto const& f : tests)
		{
			f();
		}
	}
}