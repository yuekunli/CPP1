#include <vector>
#include <iostream>
#pragma warning(disable: 4267 4244)

namespace _0037_Sudoku_Solver {

using namespace std;

class Solution
{
public:

	vector<pair<int, int>> emptyCells;

	vector<int> getPossibleValuesBasedOnRow(pair<int, int>& rc, vector<vector<char>>&b)  // row and column
	{
		vector<int>used(10, 0);
		int r = rc.first;
		for (auto& c : b[r])
		{
			if (c != '.')
			{
				int n = c - '0';
				used[n] = 1;
			}
		}

		vector<int>possibles;
		for (int n = 1; n <= 9; n++)
		{
			if (used[n] == 0)
				possibles.emplace_back(n);
		}
		return possibles;
	}

	/*
	* 'n' is the number I'm potentially putting at cell "rc".
	* I first collect the existing values on the same column
	* then check if n is already appearing in some cell on this column
	*/
	bool verifyCol(pair<int, int>&rc, int n, vector<vector<char>>& b)
	{
		int col = rc.second;
		for (auto& r : b)
		{
			if (r[col] != '.')
			{
				int number = r[col] - '0';
				if (number == n)
					return false;
			}
		}
		return true;
	}

	bool verifyRegion(pair<int, int>&rc, int n, vector<vector<char>>& b)
	{
		int rowStart = rc.first / 3 * 3;
		int rowEnd = rowStart + 2;
		int colStart = rc.second / 3 * 3;
		int colEnd = colStart + 2;
		for (int i = rowStart; i <= rowEnd; i++)
		{
			for (int j = colStart; j <= colEnd; j++)
			{
				if (b[i][j] != '.')
				{
					int number = b[i][j] - '0';
					if (number == n)
						return false;
				}
			}
		}
		return true;
	}

	void solveSudoku(vector<vector<char>>& board)
	{
		int row = -1;
		
		for (auto& r : board)
		{
			row++;
			int col = -1;
			for (auto& c : r)
			{
				col++;
				if (c == '.')
				{
					emptyCells.emplace_back(row, col);
				}
			}
		}
		backtrackRecursiveSolve(0, board);
	}

	bool backtrackRecursiveSolve(int i, vector<vector<char>>&b)
	{
		vector<int>const& possibles = getPossibleValuesBasedOnRow(emptyCells[i],b);
		// potential improvement:
		// I may not need to run "getPossibleValuesBasedOnRow" for every empty cell
		// if the next empty cell is on the same row as this one, possible values for
		// the next empty cell are just one fewer than what this one has.
		for (auto& n : possibles)
		{
			if (!verifyCol(emptyCells[i], n, b))
				continue;
			if (!verifyRegion(emptyCells[i], n, b))
				continue;
			b[emptyCells[i].first][emptyCells[i].second] = '0' + n;
			if (i == emptyCells.size() - 1)
				return true;
			else
			{
				if (backtrackRecursiveSolve(i + 1, b))
					return true;
				else
					b[emptyCells[i].first][emptyCells[i].second] = '.';
			}
		}
		return false;
	}
};


static void print(vector<vector<char>>& b)
{
	for (auto& r : b)
	{
		for (auto& c : r)
		{
			cout << c << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
}

static void Test1()
{
	vector<vector<char>> board = {
		{'5', '3', '.', '.', '7', '.', '.', '.', '.'},
			{'6', '.', '.', '1', '9', '5', '.', '.', '.'},
			{'.', '9', '8', '.', '.', '.', '.', '6', '.'},
			{'8', '.', '.', '.', '6', '.', '.', '.', '3'},
			{'4', '.', '.', '8', '.', '3', '.', '.', '1'},
			{'7', '.', '.', '.', '2', '.', '.', '.', '6'},
			{'.', '6', '.', '.', '.', '.', '2', '8', '.'},
			{'.', '.', '.', '4', '1', '9', '.', '.', '5'},
			{'.', '.', '.', '.', '8', '.', '.', '7', '9'} };

	Solution s;
	s.solveSudoku(board);
	print(board);

}

void Test_0037_Sudoku_Solver()
{
	Test1();
}

}