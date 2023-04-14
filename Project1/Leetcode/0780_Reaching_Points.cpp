#include<iostream>
#include<sstream>
#include<vector>

namespace _0780_Reaching_Points {

	using namespace std;


	/*
	Constraints
	sx, sy, tx, ty are all positive
	
	based this constraint, I can get this:
	tx >= sx and ty >= sy
	
	*/

	

	class Solution
	{
		/*
		for each step, I have exactly two choices, (1) increase x or (2) increase y
		sounds like a back track algorithm, but can I make use of caching?
		*/

	public:

		bool reachP_recur(int sx, int sy, int tx, int ty)
		{
			if (sx == tx && sy == ty)
				return true;
			if (sx > tx || sy > ty)
				return false;
			if (sx == tx && sy < ty)
				return reachP_recur(sx, sx + sy, tx, ty);
			if (sx < tx && sy == ty)
				return reachP_recur(sx + sy, sy, tx, ty);

			if (reachP_recur(sx+sy, sy, tx, ty))
				return true;
			else
				return reachP_recur(sx, sx+sy, tx, ty);
		}

		bool reachingPoints(int sx, int sy, int tx, int ty)
		{
			return reachP_recur(sx, sy, tx, ty);
		}
	};


	class Solution2
	{
	public:

		/*
		decrease from (tx, ty), if at some point, I have (a, b) and a < b,
		the previous step that leads to this state must be a step that increases
		the 2nd coordinate
		*/

		bool reachingPoints(int sx, int sy, int tx, int ty)
		{
			if (sx == tx && sy == ty)
				return true;
			if (sx == tx && sy > ty)
				return false;
			if (sx > tx && sy == ty)
				return false;

			if (tx < ty)
				return reachingPoints(sx, sy, tx, ty - tx);
			if (tx > ty)
				return reachingPoints(sx, sy, tx - ty, ty);

			/*
			if tx == ty, decreasing again will give me either (0, ty) or (tx, 0)
			constraints : 1<= sx,sy,tx,ty<=10^9
			sx, sy won't be 0.
			so if at some point, tx==ty, unless they are equal to sx and sy at that point
			otherwise they won't reach (sx, sy)
			and the condition of sx == tx && sy==ty is already checked at the beginning

			In fact, because sx sy tx ty are all positive, as long as I apply the operation once,
			two coordinates won't be equal. The only case they can equal is that sx == sy ==  tx == ty
			i.e. no operation needed.
			*/
			return false;
		}
	};

	class Solution3 // accepted.
	{
		/*
		* same principle as Solution2, but use iteration instead of recrusion
		* 
		* Consider this example:
		* (1, 12)  --?--> (999'999'997, 12)
		* The result is true, and this solution can produce the correct result.
		* But this solution is slow in this example.
		* It has to repeated subtract 12 from a big number.
		* 
		* (10, 7)  --?-->  (17, 7)
		* The result is obviously true, but if I use the straightforward modulus method
		* I would get 17 % 7 = 3. And I would assume the point before reaching (17, 7) is (3, 7)
		* 
		* If ty > tx, I cut tx out of ty repeatedly by doing modulus. If ty ends up too small (even smaller than sy),
		* I'll have to add some tx back to ty until ty recovers back to above sy. But if that brings ty to its original value
		* then that is just a dead loop.
		* 
		* for example  (1, 1)  --?--> (2,2)  result is false.
		* Let's say I'm cutting tx out of ty
		* 2%2 = 0,  0 is too small, so add some tx back to ty, then ty just becomes 2 again.
		*/
	public:
		bool reachingPoints(int sx, int sy, int tx, int ty)
		{
			while (true)
			{
				if (sx == tx && sy == ty)
					return true;
				if (tx < sx || ty < sy)
					// the goal of the logic below is to cut tx or ty
					// but maintain them above or equal to sx, sy,
					// but if tx or ty is smaller than sx or sy to start with,
					// then answer is obviously false. Example (3, 7) --?--> (3,4)
					return false;

				if (tx <= ty)
				{
					int oldTy = ty;
					ty = ty % tx;
					while (ty < sy)
						ty += tx;
					if (ty == oldTy)
						return false;
				}
				else
				{
					int oldTx = tx;
					tx = tx % ty;
					while (tx < sx)
						tx += ty;
					if (tx == oldTx)
						return false;
				}
			}
		}
	};

	void Test_0780_Reaching_Points()
	{
		int test;
		vector<int> input;
		Solution3 s;
		while (true)
		{
			cout << "run test?  ";
			cin >> test;
			if (test == 0) break;
			input.clear();
			cout << "4 coordinates:";
			copy_n(istream_iterator<int>(cin), 4, back_inserter(input));
			cout << boolalpha << s.reachingPoints(input[0], input[1], input[2], input[3]) << '\n';
		}
	}
}