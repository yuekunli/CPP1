#include "LibHeaders.h"

namespace _1349_Maximum_Students_Taking_Exam {

using namespace std;

class Solution
{
public:
	virtual int maxStudents(vector<vector<char>>& seats)
	{
		return 0;
	}
};

class Solution1 : public Solution
{
public:

	// back tracking algorithm is suitable for things that we can quickly declare invalid.
	// back tracking is good for 1 depth-first tree instead of a depth-first forest.

	// assign each seat 0 or 1, then check conflicts?

	// how to check optimal?

	// there is no memoization in this solution, so in another word, this solution is brute force.

	struct pair_hash
	{
		std::size_t operator() (const std::pair<int, int>& v) const
		{
			return v.first * 10 + v.second;
		}
	};

	int rowCount;
	int colCount;
	unordered_set<pair<int, int>, pair_hash> mustLeftVacant;
	unordered_set<pair<int, int>, pair_hash> occupied;

	int vacantSeatsOfBestSolutionSoFar;

	int maxStudents_Recur(vector<vector<char>>& seats, vector<pair<int, int>>& chairs, int index, int seatedStudents)
	{
		if (index == chairs.size() - 1)
		{
			if (mustLeftVacant.find(chairs[index]) != mustLeftVacant.end())
			{
				vacantSeatsOfBestSolutionSoFar = mustLeftVacant.size();
				return seatedStudents;
			}
			//else if (occupied.find(chairs[index]) != occupied.end())
			//{
				// this case actually won't happen, i.e. coming in a recursive call but the chair is already occupied
			//}
			else
			{
				vacantSeatsOfBestSolutionSoFar = mustLeftVacant.size();
				return seatedStudents + 1;
			}
		}

		if (mustLeftVacant.find(chairs[index]) != mustLeftVacant.end() ) //|| occupied.find(chairs[index]) != occupied.end())
		{
			return maxStudents_Recur(seats, chairs, index + 1, seatedStudents);
		}

		// check how many more I must leave vacant if I were to occupy this chair:

		vector<pair<int, int>> vacantCausedByThisChair;

		auto [r, c] = chairs[index];

		if (r - 1 >= 0 && c - 1 >= 0 && seats[r - 1][c - 1] == '.' && mustLeftVacant.find({r-1,c-1}) == mustLeftVacant.end())
		{
			//mustLeftVacant.emplace(r - 1, c - 1);
			vacantCausedByThisChair.emplace_back(r - 1, c - 1);
		}
		if (c - 1 >= 0 && seats[r][c - 1] == '.' && mustLeftVacant.find({r,c-1}) == mustLeftVacant.end())
		{
			//mustLeftVacant.emplace(r, c - 1);
			vacantCausedByThisChair.emplace_back(r, c - 1);
		}
		if (r + 1 <= rowCount - 1 && c - 1 >= 0 && seats[r + 1][c - 1] == '.' && mustLeftVacant.find({r+1,c-1}) == mustLeftVacant.end())
		{
			//mustLeftVacant.emplace(r+1, c - 1);
			vacantCausedByThisChair.emplace_back(r+1, c - 1);
		}
		if (r - 1 >= 0 && c + 1 <= colCount-1 && seats[r - 1][c + 1] == '.' && mustLeftVacant.find({r-1,c+1}) == mustLeftVacant.end())
		{
			//mustLeftVacant.emplace(r - 1, c + 1);
			vacantCausedByThisChair.emplace_back(r - 1, c + 1);
		}
		if (c + 1 <= colCount-1 && seats[r][c + 1] == '.' && mustLeftVacant.find({r,c+1}) == mustLeftVacant.end())
		{
			//mustLeftVacant.emplace(r, c + 1);
			vacantCausedByThisChair.emplace_back(r, c + 1);
		}
		if (r + 1 <= rowCount - 1 && c + 1 <= colCount-1 && seats[r + 1][c + 1] == '.' && mustLeftVacant.find({r+1,c+1}) == mustLeftVacant.end())
		{
			//mustLeftVacant.emplace(r + 1, c + 1);
			vacantCausedByThisChair.emplace_back(r + 1, c + 1);
		}

		int result1;

		if (mustLeftVacant.size() + vacantCausedByThisChair.size() > vacantSeatsOfBestSolutionSoFar)
		{
			// this strategy (i.e. occupying this chair) is no good
			result1 = -1;
		}
		else
		{
			occupied.insert(chairs[index]);

			for (auto& i : vacantCausedByThisChair)
			{
				mustLeftVacant.insert({ i.first, i.second });
			}

			result1 = maxStudents_Recur(seats, chairs, index + 1, seatedStudents + 1);

			occupied.erase(chairs[index]);

			for (auto& i : vacantCausedByThisChair)
			{
				mustLeftVacant.erase({ i.first, i.second });
			}
		}


		// if I leave this chair vacant:

		int result2;

		if (mustLeftVacant.size() + 1 > vacantSeatsOfBestSolutionSoFar)
		{
			// this strategy is no good
			result2 = -1;
		}
		else
		{
			mustLeftVacant.insert(chairs[index]);
			result2 = maxStudents_Recur(seats, chairs, index + 1, seatedStudents);
			mustLeftVacant.erase(chairs[index]);
		}

		return max({ result1, result2 });
	}


	int maxStudents(vector<vector<char>>& seats)
	{
		mustLeftVacant.clear();
		occupied.clear();
		rowCount = seats.size();
		colCount = seats[0].size();
		vacantSeatsOfBestSolutionSoFar = rowCount * colCount;

		vector<pair<int, int>>chairs;

		for (int i = 0; i < rowCount; i++)
		{
			for (int j = 0; j < colCount; j++)
			{
				if (seats[i][j] == '.')
					chairs.emplace_back(i, j);
			}
		}

		return maxStudents_Recur(seats, chairs, 0, 0);
	}
};

class Solution2 : public Solution
{
	/*
	* Each row is given a bitmask to represent the assignment of seats.
	* For example if there are 8 rows, the assignment of 6th row depends on the assignment
	* of 5th row. As long as row#5 is decided, the most students I can seat in row#6, #7 and #8
	* are decided. According to the constraints, there aren't too many rows and each row doesn't
	* have too many seats.
	*/
	int R;
	int C;
	int rowMaskMax;  // the value of the mask for a row if I set every bit to '1'

	bool isValidForRow(vector<vector<char>>&seats, int rowId, int thisRowSeatMask, int prevRowCompleteMask, int& seatedOnThisRow, int& thisRowCompleteMask)
	{
		vector<int> thisRow(C);
		vector<int> prevRow(C);
		int seatCount = count(seats[rowId].begin(), seats[rowId].end(), '.');
		int seatId = 0;
		for (int i = 0; i < C; i++)
		{
			if (seats[rowId][i] == '#')
				thisRow[i] = 0;
			else
			{
				thisRow[i] = (thisRowSeatMask & (1 << (seatCount-seatId-1))) > 0;
				seatId++;
			}

			prevRow[i] = (prevRowCompleteMask & (1 << (C-i-1))) > 0;
		}
		int seated = 0;
		for (int i = 0; i < C; i++)
		{
			if (i > 0 && thisRow[i] == 1 && thisRow[i - 1] == 1)
				return false;
			if (i < C - 1 && thisRow[i] == 1 && thisRow[i + 1] == 1)
				return false;
			if (i > 0 && thisRow[i] == 1 && prevRow[i - 1] == 1)
				return false;
			if (i < C - 1 && thisRow[i] == 1 && prevRow[i + 1] == 1)
				return false;
			if (thisRow[i] == 1)
				seated++;
		}
		seatedOnThisRow = seated;
		thisRowCompleteMask = 0;
		for (int i = 0; i < C; i++)
		{
			if (thisRow[i] == 1)
				thisRowCompleteMask |= (1 << (C - i - 1));
		}
		return true;
	}

	int maxStudentsOnRowAndAfter(vector<vector<char>>& seats, vector<vector<int>>& records, int rowId, int prevRowMask)
	{
		if (rowId == R) return 0;

		if (records[rowId][prevRowMask] != -1)
			return records[rowId][prevRowMask];

		int maxSeated = 0;
		int seatsInRow = count(seats[rowId].begin(), seats[rowId].end(), '.');
		int seatMaskMax = static_cast<int>(pow(2, seatsInRow)) - 1;

		// if a row has 8 spaces but only has 3 good chairs, then I just need to generate masks for seats
		// instead of trying 0 to 2^8-1, I can just try 0 to 2^3-1
		for (int thisRowSeatMask = 0; thisRowSeatMask <= seatMaskMax; thisRowSeatMask++)
		{
			int seatedOnThisRow;
			int thisRowCompleteMask;
			if (isValidForRow(seats, rowId, thisRowSeatMask, prevRowMask, seatedOnThisRow, thisRowCompleteMask))
			{
				int maxThisScheme = maxStudentsOnRowAndAfter(seats, records, rowId + 1, thisRowCompleteMask);
				if (maxThisScheme + seatedOnThisRow > maxSeated)
					maxSeated = maxThisScheme + seatedOnThisRow;
			}
		}
		records[rowId][prevRowMask] = maxSeated;
		return maxSeated;
	}

public:
	int maxStudents(vector<vector<char>>& seats)
	{
		R = seats.size();
		C = seats[0].size();
		rowMaskMax = static_cast<int>(pow(2, C)) - 1;

		vector<vector<int>> records(R, vector<int>(rowMaskMax + 1, -1));
		return maxStudentsOnRowAndAfter(seats, records, 0/*rowId*/, 0/*previous row mask*/);
	}
};

static void Test1(Solution& s)
{
	vector<vector<char>> seats { {'#','.','#','#','.','#'},{'.','#','#','#','#','.'},{'#','.','#','#','.','#'} };
	cout << s.maxStudents(seats) << '\n';
}

static void Test2(Solution& s)
{
	vector<vector<char>> test2 {
		{'.', '#'},
		{'#', '#'},
		{'#', '.'},
		{'#', '#'},
		{'.', '#'} };
	cout << s.maxStudents(test2) << '\n';
}

static void Test3(Solution& s)
{
	vector<vector<char>> test3 {
	{'#', '.', '.', '.', '#'},
	{'.', '#', '.', '#', '.'},
	{'.', '.', '#', '.', '.'},
	{'.', '#', '.', '#', '.'},
	{'#', '.', '.', '.', '#'} };
	cout << s.maxStudents(test3) << '\n';
}

void Test_1349_Maximum_Students_Taking_Exam()
{
	Solution2 s;
	Test1(s);
	Test2(s);
	Test3(s);
}

}