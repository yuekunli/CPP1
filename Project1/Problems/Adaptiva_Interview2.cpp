#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>

namespace Adaptiva_Interview2 {
	using namespace std;

	namespace Activation_Order {
		

		/*
		* A series of machines are lined up sequentially.
		* Activation_order array gives the order of machines that will be activated.
		* Machines are OFF initially.
		* I need to carry out a task.
		* The task needs a consecutive group of machines that are all ON.
		* The minSize gives the minimum size of the group.
		* Machines are labels 1 throught n.
		* The activation order array is a permutation of 1 to n integers.
		*/


		class Solution
		{

			/*
			*   machines:  1   2    3    4   5    6    7    8    9
			*   status:    ON  OFF  ON   ON  ON   OFF  OFF  OFF  ON
			*   
			*   one or a few consecutive ON machines form a segment. Use a ordered map,
			*   the key is the starting index of a segment, the value is the length of
			*   a segment.
			*/

		public:
			int findActivationStep(int total, int minSize, vector<int>& activationOrder)
			{
				map<int, int> m;  // key:start index    value:segment length

				int step = 1;
				for (int& i : activationOrder)
				{
					if (m.size() == 0)
					{
						m.insert({ i, 1 });
					}
					else
					{
						auto it = m.lower_bound(i);  // even if I use lower_bound, it will find the one greater than i, there is no entry whose key is equal to i
						if (it == m.begin())
						{
							if (i + 1 == it->first)
							{
								if (it->second + 1 >= minSize)
								{
									return step;
								}
								else
								{
									int newLen = it->second + 1;
									m.erase(it);
									m.insert({ i, newLen });
								}
							}
							else
							{
								m.insert({ i, 1 });
							}
						}
						else if (it == m.end())
						{
							auto itprev = prev(it);
							int prevEnd = itprev->first + itprev->second - 1;
							if (prevEnd + 1 == i)
							{
								if (itprev->second + 1 >= minSize)
									return step;
								else
								{
									itprev->second++;
								}
							}
							else
							{
								m.insert({ i, 1 });
							}
						}
						else
						{
							auto itprev = prev(it);
							int prevEnd = itprev->first + itprev->second - 1;
							if (prevEnd + 1 == i && i + 1 == it->first)
							{
								if (itprev->second + 1 + it->second >= minSize)
									return step;
								else
								{
									int newLen = itprev->second + 1 + it->second;
									m.erase(it);
									itprev->second = newLen;
								}
							}
							else if (prevEnd + 1 == i)
							{
								if (itprev->second + 1 >= minSize)
								{
									return step;
								}
								else
								{
									itprev->second++;
								}
							}
							else if (i + 1 == it->first)
							{
								if (it->second + 1 >= minSize)
								{
									return step;
								}
								else
								{
									int newLen = it->second + 1;
									m.erase(it);
									m.insert({i, newLen});
								}
							}
							else
							{
								m.insert({ i, 1 });
							}
						}
					}
					step++;
				}
				return -1;
			}
		};


	}

	namespace Triangles {


		/*
		* A geometric shape that is containes.
		* No lines sticking out of boundary.
		* Not necessarily convex or concave.
		* A point exist at every intersection of lines.
		* Count the number of triangles in the geometric shape.
		*/

		class Solution
		{

			/*
			* the visited array has to differentiate two different states.
			* 
			* 
			* 
			* 
			*                       m
			*                     / | \
			*                    /  |  \
			*                   /  |    \
			*                  / _/\_y   \
			*               x /_/____\____\z
			* 
			* 
			* When I use 'x' as the 1st point of a triangle, I find 'y' as a neighbor of 'x',
			* I then use 'y' as the 2nd point of a triangle, I then find 'z' as a neighbor of 'y'.
			* I can verify that x-y-z does form a triangle.
			* Because 'z' is also connected to 'x', I will use 'z' as the 2nd point of a triangle,
			* then I find 'y' as a neighbor of 'z', but I must not verify x-z-y because it's duplicate.
			* 
			* When the 1st point of a triangle is decided, the 2nd and 3rd points of a triangle can
			* switch position and get counted more than once. The solution is to record whether a point
			* has been used as 2nd point of a triangle, if it has been, must not use it as the 3rd point
			* of a triangle. This is all under the premises that the 1st point is decided.
			* 
			* Once I change to a different point as the 1st point, all the records regarding who has been
			* seleted as the 2nd point should be erased.
			* 
			* Given the premises that I select 'x' as the 1st, if I select 'z' as the 2nd,
			* I must not use 'z' as the 3rd. But 'z' can still be the 1st point of a triangle so that I can
			* count z-y-m.
			* 
			* Therefore whether a point has been selected to be the 2nd point of a triangle has nothing to do
			* with this point's eligibility to serve as the 1st point of a triangle.
			* 
			* 
			*/

			void createAdjList(vector<vector<vector<int>>>& m, vector<vector<int>>& l)
			{
				size_t len = m.size();
				for (int i = 0; i < len; i++)
				{
					for (int j = 0; j < len; j++)
					{
						if (m[i][j].size() > 0)
						{
							l[i].push_back(j);
						}
					}
				}
			}

			int total;

			void solve(vector<vector<vector<int>>>& m, vector<vector<int>>& l, int currentNode, int prevNode, short step, vector<short>& visited)
			{
				if (step == 1)
				{
					// the 1st point of a triangle is selected, I'm looking for the 2nd point right now.
					vector<int>& connected = l[currentNode];
					for (int& i : connected)
					{
						if (visited[i] == 0)
						{
							visited[i] = 2;
							solve(m, l, i, currentNode, 2, visited);
						}
					}
					for (int& i : connected)
					{
						if (visited[i] == 2)
							visited[i] = 0;
					}
				}
				else // step == 2
				{
					vector<int>& connected = l[currentNode];
					for (int& i : connected)
					{
						if (visited[i] == 0)
						{
							if (m[prevNode][i].size() > 0)
							{
								vector<int>& line = m[currentNode][prevNode];
								if (find(line.begin(), line.end(), i) == line.end())
									total++;
							}
						}
					}
				}
			}

		public:
			int countTriangles(vector<vector<vector<int>>>& adjM)
			{
				total = 0;
				size_t len = adjM.size();
				vector<vector<int>>adjL(len);

				createAdjList(adjM, adjL);

				vector<short> visited(len, 0);

				for (int i = 0; i < len; i++)
				{
					visited[i] = 1;
					solve(adjM, adjL, i /*current node*/, -1 /*prev node*/, 1 /*step*/, visited);
				}

				return total;
			}
		};

		void Test_1()
		{
			/*
			*           * 0
			*          *| *
			*         * |  *
			*        *  *1  *
			*       * *   *  *
			*     2************3
			*/

			vector<vector<vector<int>>>m
			{
				{{}, {0,1}, {0,2}, {0,3}},
				{{1,0}, {}, {1,2}, {1,3}},
				{{2,0}, {2,1}, {}, {2,3}},
				{{3,0}, {3,1}, {3,2}, {}}
			};

			Solution s;
			cout << s.countTriangles(m) << "\n";
		}

		void Test_Triangles()
		{
			Test_1();
		}
	}
}