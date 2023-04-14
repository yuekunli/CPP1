#include<vector>
#include<iostream>

namespace _0130_Surrounded_Regions {

	using namespace std;

	class Solution
	{
		size_t ROW;
		size_t COL;
		void dfs(vector<vector<char>>& board, size_t r, size_t c)
		{
			board[r][c] = '1';
			if (r > 0 && board[r-1][c] == 'O')
				dfs(board, r - 1, c);
			if (r + 1 < ROW && board[r + 1][c] == 'O')
				dfs(board, r + 1, c);
			if (c > 0 && board[r][c - 1] == 'O')
				dfs(board, r, c - 1);
			if (c + 1 < COL && board[r][c + 1] == 'O')
				dfs(board, r, c + 1);
		}

		void remark(vector<vector<char>>& board)
		{
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					if (board[i][j] == 'O')
						board[i][j] = 'X';
					else if (board[i][j] == '1')
						board[i][j] = 'O';
				}
			}
		}

	public:

		void solve(vector<vector<char>>& board)
		{
			ROW = board.size();
			COL = board[0].size();

			for (size_t i = 0; i < COL; i++)
			{
				if (board[0][i] == 'O')
					dfs(board, 0, i);
			}
			for (size_t i = 0; i < COL; i++)
			{
				if (board[ROW - 1][i] == 'O')
					dfs(board, ROW - 1, i);
			}
			for (size_t i = 0; i < ROW; i++)
				if (board[i][0] == 'O')
					dfs(board, i, 0);

			for (size_t i = 0; i < ROW; i++)
				if (board[i][COL - 1] == 'O')
					dfs(board, i, COL - 1);

			remark(board);
		}
	};

	void Test_0130_Surrounded_Regions()
	{
		vector<vector<char>>board = {{'X', 'X', 'X', 'X'}, {'X', 'O', 'O', 'X'}, {'X', 'X', 'O', 'X'}, {'X', 'O', 'X', 'X'}};
		Solution s;
		s.solve(board);

		for (vector<char>& row : board)
		{
			for (char& c : row)
				cout << c << " ";
			cout << "\n";
		}
		cout << "\n\n";
	}
}