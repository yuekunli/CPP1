#include<iostream>
#include<vector>

namespace _0174_Dungeon_Game {

	using namespace std;

	class Solution
	{
	public:
		int calculateMinimumHP(vector<vector<int>>& dungeon)
		{
			size_t ROW = dungeon.size();
			size_t COL = dungeon[0].size();

			vector<vector<int>>records(ROW, vector<int>(COL, -1));
			records[ROW - 1][COL - 1] = dungeon[ROW - 1][COL - 1] >= 0 ? 1 : (1 - dungeon[ROW - 1][COL - 1]);
			int r_start, c_start;
			if (COL == 1)
			{
				// Use the bottome-right corner cell as a lighthouse, I start from its left, go north-ease, arraive at its above.
				// what if there is only 1 colume, there is no cell to the left of the lighthouse.
				// I don't need to do special handling when there is only 1 row, the ROW-1 and COL-2 are still valid for having only 1 row
				r_start = ROW - 2;
				c_start = 0;
			}
			else
			{
				r_start = (int)(ROW - 1);
				c_start = (int)(COL - 2);  
			}
			while (r_start >= 0 && c_start >= 0)
			{
				int r = r_start;
				int c = c_start;
				while (r >= 0 && c < COL)
				{
					int result1 = numeric_limits<int>::max();
					int result2 = numeric_limits<int>::max();
					if (r < ROW - 1)
					{
						/*
						*     -3   X    '-3' is the cell's value in "dungeon"
						*      5   X    '5' is the cell's requirement in "records"
						* Standing in the cell with value '-3', the cell down below requires at least '5' to enter.
						* If I want to take this path, I need at least 8 to enter the cell with '-3'.
						* 
						*      7   X
						*      5   X
						* Technically if I enter the cell, whose value is '7', with HP of '-2', I can get a boost and
						* successfully enter the cell down below, but when the HP get down to '-2', I'll have died.
						*/
						result1 = records[r + 1][c] - dungeon[r][c] <= 0 ? 1 : records[r + 1][c] - dungeon[r][c];
					}
					if (c < COL - 1)
					{
						result2 = records[r][c + 1] - dungeon[r][c] <= 0 ? 1 : records[r][c + 1] - dungeon[r][c];
					}
					records[r][c] = min(result1, result2);

					r--;
					c++;
				}
				if (c_start > 0)
					c_start--;
				else
					r_start--;
			}

			return records[0][0];
		}
	};

	void Test_0174_Dungeon_Game()
	{
		vector<vector<int>> d1 = {{-2, -3, 3}, {-5, -10, 1}, {10, 30, -5}};
		vector<vector<int>> d2 = { {0} };
		Solution s;
		cout << s.calculateMinimumHP(d1) << "\n\n";
		cout << s.calculateMinimumHP(d2) << "\n\n";
	}
}