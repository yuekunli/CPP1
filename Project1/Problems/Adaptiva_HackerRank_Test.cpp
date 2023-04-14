#include<vector>
#include<string>
#include<iostream>

namespace Adaptiva_HackerRank_Test {
	using namespace std;

	/*
	* 2-D grid, each cell either empty '.' or blocked '#'.
	* Each step can go up, down, left, or right.
	* Each step takes 1 second.
	* Requirement is to reach to bottom-right cell within maxTime (can be equal to maxTime)
	* Start at top-left cell.
	*/
	string reachTheEnd(vector<string>grid, int maxTime)
	{	
		size_t rowCount = grid.size();
		size_t colCount = grid[0].size();
		vector<vector<bool>>seen = vector<vector<bool>>(rowCount, vector<bool>(colCount, false));
		
		vector<pair<int, int>>q[2];
		int currentQ = 0;
		q[currentQ].emplace_back(0, 0);
		int step = 0;
		while (q[currentQ].size() != 0)
		{
			int nextQ = (currentQ + 1) % 2;
			for (pair<int, int>& cell : q[currentQ])
			{
				if (cell.first == rowCount - 1 && cell.second == colCount - 1)
				{
					if (step <= maxTime)
						return "Yes";
				}
				else
				{
					int r = cell.first;
					int c = cell.second - 1;
					if (r >= 0 && r < rowCount && c >= 0 && c < colCount && grid[r][c] != '#' && seen[r][c] == false)
					{
						q[nextQ].emplace_back(r, c);
						seen[r][c] = true;
					}
					
					r = cell.first - 1;
					c = cell.second;
					if (r >= 0 && r < rowCount && c >= 0 && c < colCount && grid[r][c] != '#' && seen[r][c] == false)
					{
						q[nextQ].emplace_back(r, c);
						seen[r][c] = true;
					}

					r = cell.first;
					c = cell.second + 1;
					if (r >= 0 && r < rowCount && c >= 0 && c < colCount && grid[r][c] != '#' && seen[r][c] == false)
					{
						q[nextQ].emplace_back(r, c);
						seen[r][c] = true;
					}

					r = cell.first + 1;
					c = cell.second;
					if (r >= 0 && r < rowCount && c >= 0 && c < colCount && grid[r][c] != '#' && seen[r][c] == false)
					{
						q[nextQ].emplace_back(r, c);
						seen[r][c] = true;
					}
				}
			}
			step++;
			if (step > maxTime)
				return "No";
			q[currentQ].clear();
			currentQ = nextQ;
		}
		return "No";
	}


	/*
	* Given an array, each cell has a value
	* Start at cell#0, goal is to reach the end.
	* Each step can jump at least 1 step or at most maxStep.
	* Every time landing on a cell, add its value to sum.
	* Goal is to reach the end and achieve max sum.
	*/
	int maxStep_;
	size_t len;
	long solve(vector<int>& path, vector<long>& r, vector<bool>& visited, int index)
	{
		if (index >= len)
			return 0;
		if (visited[index])
			return r[index];

		int s = path[index];

		long maxTotal = solve(path, r, visited, index + 1);
		for (int i = 1; i <= maxStep_; i++)
		{
			if (index + i < len)
			{
				long t;
				if (visited[index + i])  // do this check here is important to speed up performance, to save one recursive call.
					t = r[index + i];
				else
					t = solve(path, r, visited, index + i);
				maxTotal = max(maxTotal, t);
			}
		}
		r[index] = s + maxTotal;
		visited[index] = true;
		return s + maxTotal;
	}

	long journey(vector<int> path, int maxStep)
	{
		maxStep_ = maxStep;
		len = path.size();
		if (len == 1)
			return path[0];
		vector<long>r(len, 0);
		vector<bool>visited(len, false);
		r[len - 1] = path[len - 1];
		visited[len - 1] = true;
		r[len - 2] = path[len - 1] + path[len - 2];
		visited[len - 2] = true;
		return solve(path, r, visited, 0);
	}

	void Test_journey()
	{
		vector<int>path{ 10, 2, -10, 5, 20 };
		cout << journey(path, 2) << '\n';
	}
}