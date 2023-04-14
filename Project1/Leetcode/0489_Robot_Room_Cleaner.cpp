#include<unordered_set>

namespace _0489_Robot_Room_Cleaner {

	using namespace std;

	class Robot
	{
	public:
		bool move();
		void turnLeft();
		void turnRight();
		void clean();
	};
	


	class Solution
	{
		struct Cell
		{
			int r;
			int c;
			Cell(int _r, int _c) : r(_r), c(_c) {}
			Cell() :r(0), c(0) {}
		};
		struct CellHash
		{
			size_t operator()(Cell const& a) const
			{
				return a.r * 1000 + a.c;
			}
		};
		struct CellComp
		{
			bool operator()(Cell const& a, Cell const& b) const
			{
				return a.c == b.c && a.r == b.r;
			}
		};

		unordered_set<Cell, CellHash, CellComp> seen;

		void dfs(Robot& rob, Cell a, int direction) // 1: up;  2: left;  3: down;  4: right
		{
			rob.clean();
			if (direction == 1)
			{
				bool moveSuccessful = false;
				Cell next{ a.r - 1, a.c }; // if robot is facing the border and tries to move, it will stay, that's fine, but how would the hash set work with (r-1) being negative
				if (/*a.r - 1 >= 0 &&*/ seen.count(next) == 0) // I can check r - 1 >= 0 here, but I can't check if the row or column index is greater than the room size.
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 1);
						moveSuccessful = true;
					}
				}
				if (moveSuccessful)
				{
					// I went up just now, when I come back, I'm facing down.
					rob.turnRight();
					moveSuccessful = false; // reset this flag to get ready for the next move
				}
				else
					rob.turnLeft();

				next = Cell{a.r, a.c-1};
				if (/*a.c - 1 >= 0 &&*/ seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 2);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnRight();
					moveSuccessful = false;
				}
				else
					rob.turnLeft();

				next = Cell{ a.r + 1, a.c };
				if (seen.count(next) == 0) // I can't check r+1 < room size, because I don't know the room size.
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 3);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnRight();
					moveSuccessful = false;
				}
				else
					rob.turnLeft();

				next = Cell{ a.r, a.c+1 };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 4);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnLeft();
				}
				else
					rob.turnRight();

				rob.move(); // move back
			}
			else if (direction == 2)
			{
				bool moveSuccessful = false;
				Cell next{ a.r, a.c-1 };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 2);
						moveSuccessful = true;
					}
				}
				if (moveSuccessful)
				{
					rob.turnRight();
					moveSuccessful = false;
				}
				else
					rob.turnLeft();

				next = Cell{ a.r+1, a.c };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 3);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnRight();
					moveSuccessful = false;
				}
				else
					rob.turnLeft();


				next = Cell{ a.r, a.c+1 };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 4);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnRight();
					moveSuccessful = false;
				}
				else
					rob.turnLeft();


				next = Cell{ a.r-1, a.c };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 1);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnLeft();
				}
				else
					rob.turnRight();

				rob.move(); // move back
			}
			else if (direction == 3)
			{
				bool moveSuccessful = false;
				Cell next{ a.r+1, a.c };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 3);
						moveSuccessful = true;
					}
				}
				if (moveSuccessful)
				{
					rob.turnRight();
					moveSuccessful = false;
				}
				else
					rob.turnLeft();

				next = Cell{ a.r, a.c+1 };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 4);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnRight();
					moveSuccessful = false;
				}
				else
					rob.turnLeft();


				next = Cell{ a.r-1, a.c };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 1);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnRight();
					moveSuccessful = false;
				}
				else
					rob.turnLeft();


				next = Cell{ a.r, a.c-1 };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 2);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnLeft();
				}
				else
					rob.turnRight();

				rob.move(); // move back
			}
			else
			{
				bool moveSuccessful = false;
				Cell next{ a.r, a.c+1 };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 4);
						moveSuccessful = true;
					}
				}
				if (moveSuccessful)
				{
					rob.turnRight();
					moveSuccessful = false;
				}
				else
					rob.turnLeft();

				next = Cell{ a.r-1, a.c };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 1);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnRight();
					moveSuccessful = false;
				}
				else
					rob.turnLeft();


				next = Cell{ a.r, a.c-1 };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 2);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnRight();
					moveSuccessful = false;
				}
				else
					rob.turnLeft();


				next = Cell{ a.r+1, a.c };
				if (seen.count(next) == 0)
				{
					if (rob.move())
					{
						seen.insert(next);
						dfs(rob, next, 3);
						moveSuccessful = true;
					}
				}

				if (moveSuccessful)
				{
					rob.turnLeft();
				}
				else
					rob.turnRight();

				rob.move(); // move back
			}
		}

	public:

		void cleanRoom(Robot& robot)
		{
			seen.clear();
			seen.emplace(0, 0);
			dfs(robot, { 0,0 }, 1); //constraints: robot initially faces up.
		}
	};
}