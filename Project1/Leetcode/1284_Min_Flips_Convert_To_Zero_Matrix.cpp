#include "LibHeaders.h"

namespace _1284_Min_Flips_Convert_To_Zero_Matrix {

	using namespace std;

	class Solution
	{
		/*
		* The size of the matrix is at most 3 by 3, there are at most 9 cells.
		* Use a bitmap to represent the value in each cell. I only need a bit map of 9 bits long.
		* The total possible states of the matrix is only 2^9 = 512.
		* Do breadth-first-search. The key to check visited states.
		*/
		
		vector<pair<int, int>> findAllAffectedCells(int r, int c, int m, int n) // m rows and n columns
		{
			vector<pair<int, int>>answer;
			
			answer.emplace_back(r, c);
			if (r > 0)
				answer.emplace_back(r - 1, c);
			if (r < m - 1)
				answer.emplace_back(r + 1, c);
			if (c > 0)
				answer.emplace_back(r, c - 1);
			if (c < n - 1)
				answer.emplace_back(r, c + 1);
			return answer;
		}

		void flipOneCell(int& bitmap, pair<int, int>cell, int m, int n)
		{
			int totalBits = m * n;
			int bitPos = cell.first * n + cell.second;
			int bitPosBackward = totalBits - bitPos - 1;
			int mask = 1 << bitPosBackward;
			bitmap = bitmap ^ mask;
		}

		void flipCellAndConnected(int& bitmap, int r, int c, int m, int n)
		{
			vector<pair<int, int>>const& affectedCells = findAllAffectedCells(r, c, m, n);
			for (pair<int, int>const& p : affectedCells)
			{
				flipOneCell(bitmap, p, m, n);
			}
		}

		void setInitialState(vector<vector<int>>& mat, int& state)
		{
			int m = mat.size();
			int n = mat[0].size();

			int k = 0;
			for (int i = m - 1; i >= 0; i--)
			{
				for (int j = n - 1; j >= 0; j--)
				{
					if (mat[i][j])
						state ^= (1 << k);
					k++;
				}
			}
		}

	public:
		int minFlips(vector<vector<int>>& mat)
		{
			int m = mat.size();
			int n = mat[0].size();

			unordered_set<int> seen;

			int state = 0;
			setInitialState(mat, state);
			if (state == 0) return 0;
			
			vector<int> q[2];
			int qSwitch = 0;
			q[qSwitch].emplace_back(state);
			seen.insert(state);
			int steps = 0;
			while (q[qSwitch].size()>0)
			{
				int theOtherQ = (qSwitch + 1) % 2;
				vector<int>& examine = q[qSwitch];
				vector<int>& addTo = q[theOtherQ];
				int sz = examine.size();
				for (int k = 0; k < sz; k++)
				{
					for (int i = 0; i < m; i++)
					{
						for (int j = 0; j < n; j++)
						{
							int s = examine[k];
							flipCellAndConnected(s, i, j, m, n);
							if (s == 0)
								return steps + 1;
							if (seen.count(s) == 0)
							{
								seen.insert(s);
								addTo.push_back(s);
							}
						}
					}
				}
				examine.clear();
				qSwitch = theOtherQ;
				steps++;
			}
			return -1;
		}
	};


	void Test_1284_Min_Flips_Convert_To_Zero_Matrix()
	{
		int m; // number of rows;
		vector<vector<int>>mat;
		string s;
		vector<int>allNumbers;
		Solution solu;
		while (true)
		{
			cout << "rows: ";
			cin >> m;
			if (m == 0)break;
			cin.ignore();
			cout << "all numbers in matrix: ";
			getline(cin, s);
			allNumbers.clear();
			stringstream ss(s);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(allNumbers));
			int n = allNumbers.size() / m;  // number of columns
			mat.clear();
			mat.assign(m, vector<int>());
			int k = 0;
			for (int i = 0; i < m; i++)
			{
				mat[i].assign(n, 0);
				for (int j = 0; j < n; j++)
				{
					mat[i][j] = allNumbers[k];
					k++;
				}
			}
			cout << solu.minFlips(mat) << "\n\n";
		}
	}
}