#include "LibHeaders.h"

//#define DEBUG
namespace _1937_Maximum_Number_Points_with_Cost {

	using namespace std;

	class Solution
	{
		int ROW;
		int COL;

		long long solve(vector<vector<int>>& points, vector<vector<long long>>& r, int currentRow, int colOfPrevRow)
		{
			if (currentRow == ROW)
				return 0;
			if (currentRow > 0 && r[currentRow][colOfPrevRow] != -1)
				return r[currentRow][colOfPrevRow];

			long long maxPoints = 0;
			for (int i = 0; i < COL; i++)
			{
				long long p = 0;
				if (currentRow > 0)
					p = -abs(i - colOfPrevRow);
				p += points[currentRow][i];
				p += solve(points, r, currentRow + 1, i);
				maxPoints = max(maxPoints, p);
			}
			if (currentRow > 0)
				r[currentRow][colOfPrevRow] = maxPoints;
			return maxPoints;
		}


	public:
		long long maxPoints(vector<vector<int>>& points)
		{
			ROW = points.size();
			COL = points[0].size();
			vector<vector<long long>>records(ROW, vector<long long>(COL, -1));

			return solve(points, records, 0, -1);
		}
	};

	class Solution2
	{
		/*
		* bottom up dynamic programming
		* records[i][j] means when points[i-1][j] is picked, the maximum points row 'j' to last row can provide
		*/
	public:
		long long maxPoints(vector<vector<int>>& points)
		{
			int ROW = points.size();
			int COL = points[0].size();
			vector<vector<long long>>records(ROW, vector<long long>(COL, -1));

			for (int i = ROW - 1; i >= 0; i--)
			{
				for (int j = 0; j < COL; j++)
				{
					long long m1 = 0;
					for (int k = 0; k < COL; k++)
					{
						long long p;
						if (i == ROW - 1)
							p = -abs(k - j) + points[i][k];
						else if (i == 0)
							p = points[i][k] + records[i + 1][k];
						else
							p = -abs(k - j) + points[i][k] + records[i + 1][k];

						if (m1 < p)
						{
							m1 = p;
						}
					}
					records[i][j] = m1;
				}
			}

#ifdef DEBUG

			for (int i = 0; i < ROW; i++)
			{
				copy(records[i].begin(), records[i].end(), ostream_iterator<long long>(std::cout, " "));
				cout << "\n";
			}
			cout << "\n\n";
#endif

			return *max_element(records[0].begin(), records[0].end());
		}
	};



	class Solution3
	{

	public:
		long long maxPoints(vector<vector<int>>& points)
		{
			int ROW = points.size();
			int COL = points[0].size();

			if (ROW == 1)
			{
				return *max_element(points[0].begin(), points[0].end());
			}

			vector<vector<long long>>records(ROW, vector<long long>(COL, -1));

			for (int i = ROW - 1; i > 0; i--)
			{
				int m1Index = 0;
				for (int j = 0; j < COL; j++)
				{
					long long m1 = 0;
					if (j > 0 && j <= m1Index)
					{
						records[i][j] = records[i][0] + 1;
						continue;
					}
					for (int k = 0; k < COL; k++)
					{
						long long p;
						if (i == ROW - 1)
							p = -abs(k - j) + points[i][k];
						else if (i == 0)
							p = points[i][k] + records[i + 1][k];
						else
							p = -abs(k - j) + points[i][k] + records[i + 1][k];

						if (m1 < p)
						{
							m1 = p;
							if (j == 0)
								m1Index = k;
						}
					}
					records[i][j] = m1;
				}
			}
			long long answer = 0;
			for (int e = 0; e < COL; e++)
			{
				answer = max(answer, (points[0][e] + records[1][e])); // if the input grid has only 1 row, "records" will have only 1 row, records[1][.] is outside boundary.
			}

#ifdef DEBUG

			for (int i = 0; i < ROW; i++)
			{
				copy(records[i].begin(), records[i].end(), ostream_iterator<long long>(std::cout, " "));
				cout << "\n";
			}
			cout << "\n\n";
#endif
			return answer;
		}
	};





	class Solution4
	{
		/*
		* The values on the same row in "records" array are not always in hill shape.
		* i.e. not always with 1 peak and decrement 1 at a time along each side.
		* There could be multiple equal high peaks
		*/
	public:
		long long maxPoints(vector<vector<int>>& points)
		{
			int ROW = points.size();
			int COL = points[0].size();

			if (ROW == 1)
			{
				return *max_element(points[0].begin(), points[0].end());
			}

			int maxIndex = 0;
			int newMaxIndex = 0;
			long long maxValue = 0;
			long long newMaxValue = 0;

			for (int i = ROW - 1; i >= 0; i--)
			{
				if (i == ROW - 1)
				{
					for (int j = 0; j < COL; j++)
					{
						if (maxValue < points[i][j])
						{
							maxIndex = j;
							maxValue = points[i][j];
						}
					}
				}
				else
				{
					for (int j = 0; j < COL; j++)
					{
						long long p = points[i][j] + (maxValue - abs(j - maxIndex));
						if (p > newMaxValue)
						{
							newMaxValue = p;
							newMaxIndex = j;
						}
					}
					maxIndex = newMaxIndex;
					maxValue = newMaxValue;
					newMaxIndex = newMaxValue = 0;
				}
			}
			return maxValue;
		}
	};



	class Solution5
	{

	public:
		long long maxPoints(vector<vector<int>>& points)
		{
			int ROW = points.size();
			int COL = points[0].size();

			if (ROW == 1)
			{
				return *max_element(points[0].begin(), points[0].end());
			}

			long long maxValue = 0;

			vector<long long>a(COL, 0);

			for (int i = ROW - 1; i >= 0; i--)
			{
				for (int j = 0; j < COL; j++)
				{
					long long p = points[i][j] + a[j];
					if (p > maxValue)
					{
						maxValue = p;
					}
				}

				if (i == 0)
					return maxValue;

				vector<int>maxIndexes;
				for (int j = 0; j < COL; j++)
				{
					long long p = points[i][j] + a[j];
					if (p == maxValue)
					{
						maxIndexes.emplace_back(j);
					}
				}
				int k = 0;
				for (int j = 0; j < COL; j++)
				{
					if (k == 0 && j <= maxIndexes[k])
					{
						a[j] = maxValue - (maxIndexes[0] - j);
						if (j == maxIndexes[k])
							k++;
					}
					else if (k < maxIndexes.size())
					{
						int minDistanceToMax = min((j - maxIndexes[k - 1]), (maxIndexes[k] - j));
						a[j] = maxValue - minDistanceToMax;
						if (j == maxIndexes[k])
							k++;
					}
					else
					{
						a[j] = maxValue - (j - maxIndexes[k - 1]);
					}
				}
			}
			return 0;
		}
	};




	class Solution6
	{
		/*
		*        points:                     records:
		* 
		*         #  #  #  #  #              #  #  #  #  #
		*         #  #  #  #  # <--need this #  #  #  #  #  <-- in order to figure out this row in "records"
		*         #  #  #  #  #              #  #  #  #  #  <-- I need this row and that same row in "points"
		*         #  #  #  #  #              #  #  #  #  #
		* 
		*   record[2][3]: if on row#1 I pick 'd', what can row#2 and row# provide me?
		* 
		*     row#1 :    a  b  c  d  e
		*     row#2 :    f  g  h  *i j
		*     row#3 :    h  k  m  n  p
		* 
		*   row#1 is picked, I need to try picking each on row#2, therefore I need the points[2][0], points[2][1], ... points[2][]
		*   then I need records[3][x], because that means when row#2 is picked what the rest can provide.
		* 
		*   So I need points  row#2   #  #  #  #  #
		*             records row#3   #  #  #  #  #    ==> add up these two rows vertically
		* 
		* For a certain value in records row#2, for instance records[2][3], its eventual contributor can only come from either left or right.
		* Again records[2][3] means when row#1 is picked (at column#3) what can row#2 and row#3 provide. In order to figure that out, the very
		* first step is to pick one on row#2. The one picked that can provide maximum can come from column#3's left or its right, of course
		* it can also be right on the same column.
		* 
		*/

	public:
		long long maxPoints(vector<vector<int>>& points)
		{
			int ROW = points.size();
			int COL = points[0].size();

			if (ROW == 1)
			{
				return *max_element(points[0].begin(), points[0].end());
			}

			vector<long long>a(COL, 0);
			vector<long long>b(COL, 0);

			for (int i = ROW - 1; i >= 0; i--)
			{
				for (int j = 0; j < COL; j++)
				{
					b[j] = points[i][j] + a[j];
				}

				if (i == 0)
					return *max_element(b.begin(), b.end());

				long long previousMax = b[0];
				int previousMaxIndex = 0;
				a[0] = b[0];
				for (int j = 1; j < COL; j++)
				{
					long long p1 = b[j];
					long long p2 = previousMax - (j - previousMaxIndex);
					if (p1 >= p2)
					{
						/*
						* if the previous big value's contribution at this point is smaller than
						* the current value's contribution, then use current value to contribute to the subsequent cells
						*/
						previousMax = p1;
						previousMaxIndex = j;
						a[j] = p1;
					}
					else
					{
						a[j] = p2;
					}
					
				}

				previousMax = b[COL - 1];
				previousMaxIndex = COL - 1;
				a[COL - 1] = max(a[COL - 1], b[COL - 1]);

				for (int j = COL - 2; j >= 0; j--)
				{
					long long p1 = b[j];
					long long p2 = previousMax - (previousMaxIndex-j);
					if (p1 >= p2)
					{
						previousMax = p1;
						previousMaxIndex = j;
						a[j] = max(a[j],p1);
					}
					else
					{
						a[j] = max(a[j],p2);
					}
				}

#ifdef DEBUG
				copy(a.begin(), a.end(), ostream_iterator<long long>(std::cout, " "));
				cout << "\n";
#endif
			}
			return 0;
		}
	};





	void Test_1937_Maximum_Number_Points_with_Cost()
	{
		Solution2 s2;
		Solution6 s6;

		vector<vector<int>>points = { {1,2,3},{1,5,1},{3,1,1} };

		vector<vector<int>>points2 = { {4, 9, 13, 18, 7},{6, 3, 10, 11, 9},{2, 10, 4, 15, 11},{5, 12, 14, 8, 11},{5, 6, 2, 9, 7} };
		vector<vector<int>>points3 = { {1,5},{2,3},{4,2} };
		vector<vector<int>>points4 = {{5, 5, 4, 5}, { 5, 3, 1, 1 }, { 2, 1, 0, 4 }};
		vector<vector<int>>points5 = {{0, 3, 0, 4, 2}, {5, 4, 2, 4, 1}, {5, 0, 0, 5, 1}, {2, 0, 1, 0, 3}};

		
		cout << s2.maxPoints(points) << "\n\n";
		cout << s6.maxPoints(points) << "\n\n";

		
		cout << s2.maxPoints(points2) << "\n\n";
		cout << s6.maxPoints(points2) << "\n\n";

		cout << s2.maxPoints(points3) << "\n\n";
		cout << s6.maxPoints(points3) << "\n\n";
		
		cout << s2.maxPoints(points4) << "\n\n";
		cout << s6.maxPoints(points4) << "\n\n";
		
		cout << s2.maxPoints(points5) << "\n\n";
		cout << s6.maxPoints(points5) << "\n\n";

	

	}
}