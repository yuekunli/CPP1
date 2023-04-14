#include "LibHeaders.h"

namespace Combination_Permutation {

	using namespace std;

	class Solution1
	{
	public:
		void takeTwoOutOfFive()
		{
			int used[5] = { 0,0,0,0,0 };
			for (int i = 0; i <= 4; i++)
			{
				used[i] = 1;
				for (int j = 0; j <= 4; j++)
				{
					if (used[j] == 1)
						continue;
					std::cout << i << "  " << j << '\n';

				}
				used[i] = 0;
			}
		}

		// select two out of a range, no aprior knowledge about range's length
		void takeTwo(std::vector<int>::iterator start, std::vector<int>::iterator end)
		{
			int l = std::distance(start, end);
			std::vector<int> used(l);
			for (int i = 0; i <= l - 1; i++)
			{
				used[i] = 1;
				for (int j = 0; j <= l - 1; j++)
				{
					if (used[j] == 1)
						continue;
					std::cout << *(start + i) << "  " << *(start + j) << '\n';
				}
				used[i] = 0;
			}
		}

		void print1(std::vector<int>& v)
		{
			for (int& i : v)
				std::cout << i << "  ";
			std::cout << '\n';
		}

		/*
		* This is very brute force.
		* For example, if I want to take 3 out a group of objects, the 3 taken has no ordering.
		* This recursion can go at most 3-level deep. Each level picks one object. Each level
		* iterates though *all* objects in the group. Using an array to remember which object
		* has already been picked.
		*/
		void takeN_Recur(std::vector<int>::iterator start, std::vector<int>::iterator end, int n)
		{
			static int l = std::distance(start, end);
			static std::vector<int> taken;
			static std::vector<int> used(l, 0);

			for (int i = 0; i <= l - 1; i++)
			{
				if (used[i] == 1)
					continue;

				used[i] = 1;
			
				taken.emplace_back(*(start+i));

				if (taken.size() == n)
				{
					print1(taken);
				}
				else
				{
					takeN_Recur(start, end, n);   // is this tail recursive?
				}
				taken.erase(taken.end()-1);
				used[i] = 0;
			}
		}

		void takeN(std::vector<int>::iterator start, std::vector<int>::iterator end, int n)
		{
			takeN_Recur(start, end, n);
		}

	};

	class Solution2
	{
		/*
		* The general idea is to move the '1' on the right end to the right
		* for example if I select 3 out of 5, the progression would be:
		* 1 1 1 0 0
		* 1 1 0 1 0
		* 1 1 0 0 1
		* 1 0 1 1 0
		* 1 0 1 0 1
		* 1 0 0 1 1
		* ....
		* There may be a few 1's that are already "jammed" to the right end,
		* they can't be moved any more, find the first '1' that can be moved,
		* move it to its right by 1 bit, then move those -- that are already jammed --
		* right next to the '1' that is being moved by 1 bit.
		* 
		* 1 0 1 0 0 0 1 0 0 1 1
		*             |     | |
		*             |     these two can't be moved to their "right" any more
		*            this one can be moved to its right by 1 bit
		* ..........  0 1 1 1 0
		*               | | |
		*               | these two are being "reset"
		*             this '1' is moved to the right by 1 bit
		*/
		/*
		* Select 4 out of 10
		*    0   1   2   3   4   5   6   7   8   9
		*    |           |                   |   |   <- these 4 bits are set right now
		*
		*  when i == 6, 1 << 6 is:
		*    0   1   2   3   4   5   6   7   8   9
		*    0   0   0   1   0   0   0   0   0   0
		*
		*  I now find the selected bit that can be moved to its right
		*
		*	 0   1   2   3   4   5   6   7   8   9
		*    1   0   0   1   0   0   0   0   1   1     these 4 are set right now
		*
		*    0   0   0   1   0   0   0   0   0   0      1 << 6
		*
		*    0   0   0   1   1   0   0   0   0   0   0b11 << 5
		*
		*    1   0   0   0   1   0   0   0   1   1   after clearing #3 and set#4, but I still need to move the bits at #8 and #9 to #5 and #6
		*
		*    1   1   1   1   1   1   1   1   1   1   pow(2, n) - 1 = full
		*
		*    0   0   0   0   0   1   1   1   1   1   pow(2, (i-1)) - 1     (note that i right now is 6)
		*
		*    1   1   1   1   1   0   0   0   0   0   XOR the last two sequences, use this to clear #5 to #9
		*
		*    1   0   0   0   1   0   0   0   0   0   after clearing #5 to #9
		*
		*                                    1   1    I need two bits
		*                        1   1   0   0   0    shift to left by 3 hops.
		*                                             i == 6 right now, clear the bit at 6-hop position (#3), set the bit at 5-hop position (#4)
		*                                             set the bit at 4-hop position, because I'm setting two at once, I just need 3 hops
		*                                             3 = i - 1 - 1 - (2 - 1)
		*
		*/

		bool nextSelection(int& selection, int n, int k)
		{
			bool openFound = false;    // have I found a '0' from right side
			int bitsToTheRight = 0;    // there are bits that are already jammed to the right-end, how many are they?
			for (int i = 0; i < n; i++)
			{
				int bit = selection & (1 << i);
				if (bit && !openFound)
				{
					bitsToTheRight++;
					continue;
				}
				if (!bit && !openFound)
				{
					openFound = true;
					continue;
				}
				if (bit && openFound)
				{
					selection ^= (0b11 << (i - 1));  // clear that bit and set the one on its right
					int full = pow(2, n) - 1;        // every bit is set
					int seg = pow(2, i - 1) - 1;     // the segment after the bit that just got moved, set every bit in that segment
					selection &= (full ^ seg);
					int needed = pow(2, bitsToTheRight) - 1;  // prepare the bits that need to be "reset"
					needed = needed << (i - 1 - 1 - (bitsToTheRight - 1));  // move the bits that need to be "reset" into position
					selection |= needed;
					return true;
				}
			}
			return false;
		}

		void showSelection(int selection, int n)
		{
			vector<int> a;
			for (int i = 0; i < n; i++)
			{
				if (selection & (1 << i))
					a.emplace_back(n-i);
			}
			reverse(a.begin(), a.end());
			for (int& i : a)
				cout << i << "  ";
			cout << '\n';
		}

	public:

		void select(int n, int k)
		{
			int needed = pow(2, k) - 1;
			int selection = needed << (n - k);
			do {
				showSelection(selection, n);
			} while (nextSelection(selection, n, k));
		}
	};

	static void Test_Solution1()
	{
		Solution1 s;
		s.takeTwoOutOfFive();

		std::vector<int> v{ 3,4,5,6 };
		s.takeTwo(v.begin(), v.end());

		s.takeN(v.begin(), v.end(), 3);
	}

	static void Test_Solution2()
	{
		Solution2 s;
		int n, k;
		while (true)
		{
			cout << "select k out of n\n";
			cout << "n: (0 to exit)  >  ";
			cin >> n;
			if (n == 0) break;
			cout << "k: ";
			cin >> k;
			s.select(n, k);
		}
		
	}

	void Test_Combination_Permutation()
	{
		Test_Solution2();
	}

}