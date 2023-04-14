#include "LibHeaders.h"

namespace _1301_Number_of_Paths_with_Max_Score {

/*
* The modulus operation should be on the number of paths rather than on the max sum.
* The dimension of the board is at most 100 cells long and 100 cells wide.
* Each cell has single digit number if it doesn't have obstacle.
* The maximum possible sum is about 900. But on the other hand, the number of paths
* can multiple after each step I take, so the modulus operation is on the number of paths.
*/

class Solution
{
	/*
	*     E B C D
	*     A F G H
	*     I J K L
	*     M N P X
	* This solution is the bottom-up dynamic programming.
	* In order to get to 'E', there are 3 ways: B->E, A->E, and F->E.
	* In order to know the max when I reach 'E', I need to know the max when I reach 'A',
	* 'B', and 'F'. This solution solves the subproblems first instead of waiting for bigger
	* problem to call a recrusive function.
	* It solves the max to reach each cell. From (closer to 'X') to (farther from 'X').
	* Specifically, it visits cells on the bottom-left-to-top-right diagnoal line, and from lower
	* cells to higher cells.
	* In this case, it visits cells in such order:
	* P L
	* N K H
	* M J G D
	* I F C
	* A B
	* E
	* 
	* Does this cause me to solve things that aren't necessarily needed to be solved?
	* No, because in order to get the number of paths to achieve max sum, even if I use
	* top-down dynamic programming with recursion, I still need to visit all the cells.
	*/
public:

	struct Info
	{
		int max {};
		int paths {};
	};

	void findLocalMax(std::vector<std::string>& b, std::vector<std::vector<Info>>& t, int r, int c)
	{
		size_t s = b.size();

		Info& i = t[r][c];
		char ch = b[r][c];

		if (ch == 'X')
		{
			i.max = 0;
			i.paths = 0;
			return;
		}

		std::vector<Info> neighbors;
		neighbors.reserve(3);

		if (r + 1 < s && b[r + 1][c] != 'X' && t[r+1][c].paths != 0)
			neighbors.push_back(t[r + 1][c]);

		if (r + 1 < s && c + 1 < s && b[r + 1][c + 1] != 'X' && t[r+1][c+1].paths != 0)
			neighbors.push_back(t[r + 1][c + 1]);

		if (c + 1 < s && b[r][c + 1] != 'X' && t[r][c+1].paths != 0)
			neighbors.push_back(t[r][c + 1]);

		if (neighbors.size() == 0)
		{
			i.paths = 0;
			i.max = 0;
			return;
		}

		std::sort(neighbors.begin(), neighbors.end(), [](Info& a, Info& b)
		{
			return a.max > b.max;
		});

		int max = neighbors[0].max;
		int paths = neighbors[0].paths;

		if (neighbors.size() >= 2 && neighbors[0].max == neighbors[1].max)
		{
			paths += neighbors[1].paths;
			paths %= 1'000'000'007;
		}

		if (neighbors.size() == 3 && neighbors[0].max == neighbors[2].max)
		{
			paths += neighbors[2].paths;
			paths %= 1'000'000'007;
		}

		if (ch != 'E')
			i.max = (ch - '0') + max;
		else
			i.max = max;

		i.paths = paths;

		return;
	}

	std::vector<int> pathsWithMaxScore(std::vector<std::string>& board)
	{
		size_t s = board.size();

		std::vector<std::vector<Info>> t(s, std::vector<Info>(s));

		t[s - 1][s - 1].max = 0;
		t[s - 1][s - 1].paths = 1;

		// starting cell of each diagonal line, from bottom-left to top-right
		int scri = s - 1; // start cell row index
		int scci = s - 2; // start cell column index

		while (scri >= 0)
		{
			int r = scri, c = scci;
			while (r >= 0 && r < s && c >= 0 && c < s)
			{
				findLocalMax(board, t, r, c);

				// update row index and c index, move toward up-right
				r--;
				c++;
			}

			// update starting cell
			if (scci > 0)
				scci--;
			else
				scri--;
		}

		return { t[0][0].max, t[0][0].paths % 1'000'000'007 };
	}
};


static void Test1()
{
	std::vector<std::string> board { "E23", "2X2", "12S" };
	Solution solu;
	std::vector<int> result = solu.pathsWithMaxScore(board);
	std::cout << std::left << std::setw(8) << result[0] << std::left << std::setw(8) << result[1] << '\n';
}


static void Test2()
{
	std::vector<std::string> board { "E12", "1X1", "21S" };
	Solution solu;
	std::vector<int> result = solu.pathsWithMaxScore(board);
	std::cout << std::left << std::setw(8) << result[0] << std::left << std::setw(8) << result[1] << '\n';
}


static void Test3()
{
	std::vector<std::string> board { "E11", "XXX", "11S" };
	Solution solu;
	std::vector<int> result = solu.pathsWithMaxScore(board);
	std::cout << std::left << std::setw(8) << result[0] << std::left << std::setw(8) << result[1] << '\n';
}



void Test_1301_Number_of_Paths_with_Max_Score()
{
	Test1();
	Test2();
	Test3();
}

}