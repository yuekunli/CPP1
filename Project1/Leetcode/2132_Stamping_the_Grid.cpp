#include "LibHeaders.h"

namespace _2132_Stamping_the_Grid {

using namespace std;

class Solution
{
public:

	struct pair_hash
	{
		std::size_t operator() (const std::pair<int, int>& v) const
		{
			return v.first * 100'000 + v.second;
		}
	};

	bool possibleToStamp(vector<vector<int>>& g, int h, int w)
	{
		unordered_set <pair<int, int>, pair_hash> covered;

		int gridHeight = g.size();
		int gridWidth = g[0].size();

		int rowIdx = 0;

		for (auto const& row : g)
		{
			int colIdx = 0;
			for (auto const& cell : row)
			{
				// if I put a stamp aligning its top left corner with this cell, will the stamp be out of bounds?
				bool stayInBounds = true;
				if (rowIdx + h - 1 >= gridHeight || colIdx + w - 1 >= gridWidth)
					stayInBounds = false;

				if (stayInBounds)
				{
					// if I put a stamp aligning its top left corner with this cell, will there be conflicts?
					bool conflict = false;

					for (int i = 0; i < h; i++)
					{
						for (int j = 0; j < w; j++)
						{
							if (g[rowIdx + i][colIdx + j] == 1)
							{
								conflict = true;
								goto CHECK_CONFLICT;
							}
						}
					}

CHECK_CONFLICT:
					if (!conflict)
					{
						// now put a stamp aligning its top left corner with this cell
						for (int i = 0; i < h; i++)
							for (int j = 0; j < w; j++)
								covered.emplace(rowIdx + i, colIdx + j); // this should be the bottleneck,
																		 // if the stamp is 4*3, for each move in the grid, 
																		 // I need to add 12 coordinates' pairs to the unordered_set,
																		 // a lot of them are duplicate
					}
				}
				// either I just put a new stamp or not, but regardless, if this cell was not occupied,
				// it has to be covered at this point, because I go from top to bottom and from left to
				// right. If I leave this cell uncovered, I have no chance to get back on this row and
				// while I'm still on this row, I will move farther to the right.
				if (cell == 0 && covered.find({ rowIdx, colIdx }) == covered.end())
					return false;

				colIdx++;
			}
			rowIdx++;
		}
		return true;
	}
};


class Solution2
{
public:
	bool possibleToStamp(vector<vector<int>>& g, int h, int w)
	{
		int R = g.size();    // number of rows
		int C = g[0].size(); // number of columns
		for (int r = 0; r < R; r++)
		{
			bool firstOnThisRow = true;
			int lastOccupied = 0;
			for (int c = 0; c < C; c++)
			{
				if (g[r][c] == 1)
				{
					if (firstOnThisRow)
					{
						if (0 < c && c < w)  // if stamp width is 3, first occupied can be at cell#3 (0-indexed)
							                 // note that the first occupied can be at #0
						{
							return false;
						}
						else
						{
							firstOnThisRow = false;
							lastOccupied = c;
						}
					}
					else
					{
						if (c - lastOccupied > 1 && c - lastOccupied <= w)  // if cell#5 is occupied, cell#9 can be occupied, 9-5 = 4, w is 3
							                                                // it's fine if two occupied are adjacent.
							                                                // what I really need to test is if there is a gap between two occupied and the size of the gap
						{
							return false;
						}
						else
							lastOccupied = c;
					}
				}
			}
			// compare the last occupied with right border
			if (!firstOnThisRow)  // at least I saw one or a few occupied
			{
				if (lastOccupied < C-1 && C - lastOccupied <= w)   //   4  5  6  7  8  
										                           //      |           |
										                           //    last          C   'C' is the size, so if last index is 8, 'C' is 9
					                                               // note that it's fine if the last occupied is in the last cell
				{
					return false;
				}
			}
		}

		for (int c = 0; c < C; c++)
		{
			bool firstOnThisColumn = true;
			int lastOccupied = 0;
			for (int r = 0; r < R; r++)
			{
				if (g[r][c] == 1)
				{
					if (firstOnThisColumn)
					{
						if (0 < r && r < h)
						{
							return false;
						}
						else
						{
							firstOnThisColumn = false;
							lastOccupied = r;
						}
					}
					else
					{
						if (r - lastOccupied > 1 && r - lastOccupied <= h)
						{
							return false;
						}
						else
							lastOccupied = r;
					}
				}
			}
			if (!firstOnThisColumn)
			{
				if (lastOccupied < R - 1 && R - lastOccupied <= h)
				{
					return false;
				}
			}
		}
		return true;
	}
};


static void Test1()
{
	vector<vector<int>>grid = { {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0} };
	int stampHeight = 4, stampWidth = 3;
	Solution2 s;
	cout << boolalpha << s.possibleToStamp(grid, stampHeight, stampWidth) << '\n';
}

static void Test2()
{
	vector<vector<int>>grid = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
	int stampHeight = 2, stampWidth = 2;
	Solution2 s;
	cout << boolalpha << s.possibleToStamp(grid, stampHeight, stampWidth) << '\n';
}

void Test_2132_Stamping_the_Grid()
{
	Test1();
	Test2();
}

}