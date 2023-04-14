#include<vector>

namespace _0419_Battleships_On_Board {

	using namespace std;

	class Solution
	{
		/*
		* I go from top down, left to right, as long as I see an 'X',
		* I couldn't ve run into the same ship on this cell's top adjacent cell or the left adjancent cell.
		* this is because I go top down and left to right.
		* So check top and left neighbor whenever I can.
		*/
	public:
		int countBattleships(vector<vector<char>>& board)
		{
			size_t ROW = board.size();
			size_t COL = board[0].size();

			int total = 0;
			for (int i = 0; i < ROW; i++)
			{
				for (int j = 0; j < COL; j++)
				{
					if (i == 0)
					{
						if (j == 0)
						{
							if (board[i][j] == 'X')
								total++;
						}
						else
						{
							if (board[i][j] == 'X' && board[i][j - 1] == '.')
								total++;
						}
					}
					else
					{
						if (j == 0)
						{
							if (board[i][j] == 'X' && board[i - 1][j] == '.')
								total++;
						}
						else
						{
							if (board[i][j] == 'X' && board[i - 1][j] == '.' && board[i][j - 1] == '.')
								total++;
						}
					}
				}
			}
			return total;
		}
	};

	void Test_0419_Battleships_On_Board()
	{

	}
}