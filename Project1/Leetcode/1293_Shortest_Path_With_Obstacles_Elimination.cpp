#include "LibHeaders.h"

namespace _1293_Shortest_Path_With_Obstacles_Elimination {

	using namespace std;

	class Solution
	{
		struct ProgramState
		{
			int r;
			int c;
			int k;
			int steps;
			int estimatedTotalSteps;
			ProgramState()
			{
				r = c = k = steps = estimatedTotalSteps = 0;
			}
			ProgramState(int _r, int _c, int _k, int _steps, int _estimate)
			{
				r = _r; c = _c; k = _k; steps = _steps; estimatedTotalSteps = _estimate;
			}
		};

		struct ProgramStateComp
		{
			bool operator()(ProgramState& a, ProgramState& b)
			{
				return a.estimatedTotalSteps > b.estimatedTotalSteps;
			}
		};

		struct ProgramStateHash
		{
			size_t operator()(ProgramState const& a) const   // the input argument must be reference to const object
			{
				return a.r * 1000'000 + a.c * 10'000 + a.k;
			}
		};
		struct ProgramStateEqual
		{
			bool operator()(ProgramState const& a, ProgramState const& b) const
			{
				if (a.r == b.r && a.c == b.c && a.k == b.k)
					return true;
				return false;
			}
		};

	public:
		int shortestPath(vector<vector<int>>& grid, int k)
		{
			int ROW = grid.size();
			int COL = grid[0].size();

			vector<ProgramState> q;  // instead of using a vector, maybe I should use a priority queue
			unordered_set <ProgramState, ProgramStateHash, ProgramStateEqual> seen;
			pair<int, int> adjacentCells[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
			ProgramStateComp psComp{};

			q.emplace_back(0, 0, k, 0, ROW - 1 + COL - 1);
			while (q.size() > 0)
			{
				ProgramState ps = q[0]; // must make a copy of the heap head, must not use a reference to that element
				pop_heap(q.begin(), q.end(), std::ref(psComp));
				q.erase(q.end()-1);
				int manDist = ps.estimatedTotalSteps - ps.steps;
				if (manDist <= ps.k)
					return ps.estimatedTotalSteps;

				int newR, newC;

				for (int i = 0; i < 4; i++)
				{
					pair<int, int>& delta = adjacentCells[i];

					newR = ps.r + delta.first;
					newC = ps.c + delta.second;
					if (newR >= 0 && newR < ROW && newC >= 0 && newC < COL)
					{
						int newManDist = ROW - 1 - newR + COL - 1 - newC;
						ProgramState newPs;
						bool createdNewState = false;
						if (grid[newR][newC] == 0)
						{
							newPs = ProgramState{ newR, newC, ps.k, ps.steps + 1, ps.steps + 1 + newManDist };
							createdNewState = true;
						}
						else if (ps.k > 0)
						{
							newPs = ProgramState{ newR, newC, ps.k - 1, ps.steps + 1, ps.steps + 1 + newManDist };
							createdNewState = true;
						}
						if (createdNewState && seen.count(newPs) == 0)
						{
							q.push_back(newPs);
							push_heap(q.begin(), q.end(), std::ref(psComp));
							seen.insert(newPs);
						}
					}
				}
			}
			return -1;
		}
	};

	void Test_1293_Shortest_Path_With_Obstacles_Elimination()
	{
		vector<vector<int>>grid1 = {{0, 0, 0}, {1, 1, 0}, {0, 0, 0}, {0, 1, 1}, {0, 0, 0}};
		vector<vector<int>>grid2 = {{0, 1, 1}, {1, 1, 1}, {1, 0, 0}};

		Solution s;
		cout << "run test 1: ";
		cout << s.shortestPath(grid1, 1) << "\n";
		cout << "run test 2: ";
		cout << s.shortestPath(grid2, 1) << "\n";
	}
}