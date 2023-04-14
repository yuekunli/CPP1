#include "LibHeaders.h"

namespace _2128_Remove_All_Ones_Row_Column_Flip {

	using namespace std;

	class Solution
	{
	public:
		bool removeOnes(vector < vector<int>>& grid)
		{
			int ROW = grid.size();
			int COL = grid[0].size();

			vector<int>colWithOne;
			for (int i = 0; i < COL; i++)
			{
				if (grid[0][i] == 1)
					colWithOne.emplace_back(i);
			}
			for (int r = 1; r < ROW; r++)
			{				
				bool flipped = false;

				if (colWithOne.size() > 0 && colWithOne[0] == 0 && grid[r][0] == 0)
					flipped = true;
				else if (colWithOne.size() > 0 && colWithOne[0] > 0 && grid[r][0] == 1)
					flipped = true;
				else if (colWithOne.size() == 0 && grid[r][0] == 1)
					flipped = true;
				
				int indexInColWithOne = 0;
				//if (colWithOne.size() > 0 && colWithOne[0] == 0)
				//	indexInColWithOne = 1;

				for (int i = 0; i < COL; i++)
				{
					if (indexInColWithOne < colWithOne.size())
					{
						if (i == colWithOne[indexInColWithOne])
						{
							// grid[r][i] should be '1', it can be '1' originally or it was 0 but I decided to flip
							if ((grid[r][i] == 1 && !flipped) || (grid[r][i] == 0 && flipped))
							{
								indexInColWithOne++;
							}
							else
								return false;
						}
						else
						{
							// grid[r][i] is not supposed to be '1'
							if ((grid[r][i] == 1 && !flipped) || (grid[r][i] == 0 && flipped))
								return false;
						}
					}
					else
					{
						// already gone past the last '1'
						// grid[r][i] is not supposed to be '1'
						if ((grid[r][i] == 1 && !flipped) || (grid[r][i] == 0 && flipped))
							return false;
					}
				}
			}
			return true;
		}
	};


	class Solution2
	{
	public:
		bool removeOnes(vector < vector<int>>& grid)
		{
			int ROW = grid.size();
			int COL = grid[0].size();

			if (ROW == 0) return true;

			vector<int>& row1 = grid[0];
			
			for (int r = 1; r < ROW; r++)
			{
				vector<int>& row2 = grid[r];
				if (row1[0] == row2[0])
				{
					bool e = equal(row1.begin(), row1.end(), row2.begin(), row2.end(),
						[](int const& a, int const&b)
						{
							return a == b;
						});
					if (!e)
						return false;
				}
				else
				{
					bool e = equal(row1.begin(), row1.end(), row2.begin(), row2.end(),
						[](int const& a, int const& b)
						{
							return a != b;
						});
					if (!e)
						return false;
				}
			}
			return true;
		}
	};
}