#include "LibHeaders.h"

namespace _1420_Build_Array_Where_You_Can_Find_Maximum_Exactly_K_Comparisons {

	using namespace std;

	class Solution
	{
		/*
		* if k = 5, the maximum needs to be the 5th element, array starts with 1st(not 0th)
		* and the first 5 elements need to strictly increase
		* if there are multiple maximums the earliest of them needs to be the 5th element		
		*/
	   /* 
	    * "if k = 5, the maximum needs to be the 5th element"
		* This statement is very wrong!The maximum doesn't need to be the 5th element.
		* There can be ups and downs prior to maximum, there must be 5 ups.
		*/

	public:

		int pow_recur(int base, int order)
		{
			if (order == 0) return 1;
			if (order == 1) return base;
			int half = order / 2;
			int a = pow_recur(base, half) % 1'000'000'007;
			if (order % 2 == 0)
			{
				
				return (a * a) % 1'000'000'007;
			}
			else
			{
				return (a * a * base) % 1'000'000'007;
			}
		}


		// select a few out of total, ordering doesn't matter
		int combination(int total, int select)
		{
			int a = 1;
			for (int i = 0; i < select; i++)
			{
				a *= (total - i);
			}
			for (int i = 2; i <= select; i++)
			{
				a /= i;
			}
			return a % 1'000'000'007;
		}

		// select a few out of total, order matters, and elements in the pool are replenished after being selected
		// for example, total 6, select 4
		// I have 4 positions to fill, there are 6 options for each position, because elements are replenished
		// 6^4
		int arrange(int total, int select)
		{
			return pow_recur(total, select);
		}

		int numOfArrays(int n, int m, int k)
		{
			int beforeMax = k-1; // number of elements before max, elements before max must be all different and strictly increase
			int afterMax = n-k; // number of elements after max

			int total = 0;
			for (int possibleMax = beforeMax+1; possibleMax <= m; possibleMax++)
			{
				int lessThanMax = possibleMax - 1; // how many numbers in the range of [1,m] are less than max
				
				int possibilitiesBeforeMax = combination(lessThanMax, beforeMax);

				int possibilitiesAfterMax = arrange(lessThanMax + 1, afterMax);

				total += (possibilitiesBeforeMax * possibilitiesAfterMax) % 1'000'000'007;
			}
			return total;
		}
	};

	class Solution2
	{
	public:

		/*
		* a  b  c  d  e  f  g  h  i  j  p  q  r  s
		*                      |
		*                     max
		* max is at cell#7 (0-index), max's value is determined to be 'm', cost is 'k', total length is 'n'
		* The possibilities for such a configuration is rb[n][m][k]. 'm' means the max's value is *determined* to be 'm'
		* 
		* The portion before 'h' is an array with length of 7, max is 'm'-1 (or 'm'-2, 'm'-3, ...), cost is 'k'-1.
		* The portion after 'h' (assume length is l2) max^l2.
		* 
		* in order to get rb[n][m][k], I have to gather different first portion's length and different first portion's max
		*               rb[1][m-1][k-1] + rb[2][m-1][k-1] + ... rb[n-1][m-1][k-1]
		*             + rb[1][m-2][k-1] + rb[2][m-2][k-1] + ... rb[n-1][m-2][k-1]
		*             + ...
		*             + ...                                     rb[n-1][1][k-1]
		* 
		* But I need to multiple every item in this big sum with the corresponding 2nd portion possibilities.
		* A lot of items in this big sum will be zero, so multiplying it with something also yields zero.
		* 
		* possibilities of the 2nd portion depends on its length (of course ultimately depends on the 1st portion's length)
		* If I look at this big sum as a "matrix", items in each column should multiply the same number.
		* So if I really want to calculate the big sum, I should add up each column and multiply the possibilities for 2nd portion,
		* then move on to the next column.
		* 
		* 
		* If I include the max in the 2nd portion, then the 2nd portion just becomes an array with a certain length
		* whose cost is 1 and max is the value of 'h'.
		* 
		* But if I just want to solve this problem "length = 7, max = 4, cost = 1", solve the number of possibilites.
		* I'll again use 4^6 (first number must be 4, the next 6 numbers can be 1 to 4, 4 options, select 6 times)
		* 
		*/

	};

	class Solution3
	{
		/*
		*   example: n = 4, m = 2, k = 2
		*   answer: 7
		*
		*   1 2 1 1   max at cell#1  (0-index)
		*   1 2 1 2
		*   1 2 2 1
		*   1 2 2 2
		*
		*   1 1 2 1   max at cell#2
		*   1 1 2 2
		*
		*   1 1 1 2   max at cell#3
		*
		*  I want the 4th digit to raise the cost by 1:
		*  possibilities = rb[3][1][1] = 1
		*
		*  I want the 4th digit to not raise the cost:
		*  possibilities = rb[3][2][2] * 2 + rb[3][1][2] * 1
		*                       3      * 2 +     0       * 1  = 6
		*
		*  total = 1 + 6 = 7
		*
		*/
		/*
		* example: n = 3, m = 3, k = 2
		* 1 2 1   max = 2, max at cell#1
		* 1 2 2
		* 
		* 1 3 1   max = 3, max at cell#1
		* 1 3 2
		* 1 3 3
		* 
		* 2 3 1
		* 2 3 2
		* 2 3 3
		* 
		* 1 1 2   max = 2, max at cell#2
		* 
		* 1 1 3   max = 3, max at cell#2
		* 2 1 3
		* 2 2 3
		*/

		/*
		* example: n = 8, m = 5, k = 6
		* An array of 8, the maximum can be 5 (not necessarily though), cost is 6
		* 
		* a b c d e f g h
		* 
		* Now think about how the result of [a,g] can help with this case.
		* [a,g] is an array of length 7. I want the cost of [a,g] to be 5 and the
		* max of [a,g] to be 1 ('1' is determined to be the maximum). So when I
		* put '5' at h's position, I get a valid array.
		* So this process can be repeated a few time:
		* I want the cost of [a,g] to be 5 and the max of [a,g] to be '1', I'll put '5' at h's position
		* I want the cost of [a,g] to be 5 and the max of [a,g] to be '2', I'll put '5' at h's position
		* I want the cost of [a,g] to be 5 and the max of [a,g] to be '3', I'll put '5' at h's position
		* I want the cost of [a,g] to be 5 and the max of [a,g] to be '4', I'll put '5' at h's position
		* To calculate these 4 sub-problems, I need these values:
		* rb[7][1][5], rb[7][2][5], rb[7][3][5], rb[7][4][5]
		* I must put '5' at h's position in order to get a maximum of '5'
		* Remeber rb[x][5][y] means the maximum must be '5' instead of anything less than '5'
		* 
		* I can also build [a,h] in a different way: (basically 'h' doesn't raise the cost)

		* I want the cost of [a,g] to be 6 (reach the full cost) and the max of [a,g] to be '5', I'll put anyone of '1', '2', '3', '4', '5' at h's position
		* To calculate this sub-problems, I need to do this:   rb[7][5][6]*5
		* 
		* Note that I don't need to consider this situation:
		* I want the cost of [a,g] to be 6 and the max of [a,g] to be '4', I'll put '1', '2', '3', or '4' at h's position
		* rb[7][4][6]*4  because this doesn't give me a maximum of '5'.
		* Remember rb[7][5][6] means the maximum must be '5' instead of anything less than '5'
		*/

		/*
		* one key thing about this solution is that the different meaning of 'm'
		* in the question and in the recordbook table.
		* The question gives 'n', 'm', 'k' three number. 'n' and 'k' are deterministic.
		* The array length must be 'n', the cost must be 'k'. But 'm' is just a limit.
		* The maximum can be 1, 2, 3, ..., or 'm'.
		* Howevery, in the recordbook table, a cell [8][5][6], means the possibilities
		* for an array of length 8, cost 6, and max being 5. Note that max here must be 5
		* instead of anything less than 5.
		* This is essentially why at the end I need to assemble values accross different m's
		* while fixing 'n' and 'k'.
		*/
		/*
		* Let's show how the iteration works. By the time I get to the 3rd "for", 'n' and 'k'
		* are already decided. for example, i=9, j=4
		* m = 6
		* total = 0
		* t = 1:
		*     total += rb[8][0][3]                   (after this, total = rb[8][0][3])
		*     rb[9][1][4] = total + rb[8][1][4]*1
		*                 = rb[8][0][3] + rb[8][1][4]*1
		*     
		* 
		* t = 2:
		*     total += rb[8][1][3]                   (after this, total = rb[8][0][3] + rb[8][1][3])
		*     rb[9][2][4] = total + rb[8][2][4]*2
		*                 = rb[8][0][3] + rb[8][1][3] + rb[8][2][4] * 2
		* 
		* t = 3:
		*     total += rb[8][2][3]  (after this, total = rb[8][0][3] + rb[8][1][3] + rb[8][2][3])
		*     rb[9][3][4] = total + rb[8][3][4] * 3
		*                 = rb[8][0][3] + rb[8][1][3] + rb[8][2][3] + rb[8][3][4] * 3
		*/
	public:

		int numOfArrays(int n, int m, int k)
		{
			vector<vector<vector<int>>> rb;
			rb.assign(n + 1, vector<vector<int>>(m + 1, vector<int>(k + 1, 0)));
			rb[0][0][0] = 1;
			for (int i = 1; i <= n; i++)
			{
				for (int j = 1; j <= k; j++)
				{
					int total = 0;
					for (int t = 1; t <= m; t++)
					{
						total += rb[i - 1][t - 1][j - 1];
						total %= 1'000'000'007;
						rb[i][t][j] = (total + (rb[i - 1][t][j] * t) % 1'000'000'007) % 1'000'000'007;
					}
				}
			}

			int result = 0;

			for (int t = 1; t <= m; t++)
			{
				result += rb[n][t][k];
				result %= 1'000'000'007;
			}

			// an improvement is to use 'm' as the 3rd dimension of the recordbook 3-D array
			// so that at the end when I need to assemble the result, I can call "accumulate"
			// and the 3-level iteration can also look less awkward.

			return result;
		}
	};

	class Solution4
	{
		/*
		* If n = 10, m = 7, what is the total number of ways to build an array
		* regardless where the max is? The total is just 7^10. For each position,
		* there are 7 choices.
		* Backtrack tests candidates. Move forward 1 position at a time, track
		* current cost. Once the cost or total length violates condition, retreat.
		*/
	public:
	};


	class BruteForceSolution
	{
	public:
		bool isValid(vector<int>& v, int n, int k)
		{
			int cost = 0;
			int maxValue = 0;
			for (int i = 0; i < n; i++)
			{
				if (maxValue < v[i])
				{
					maxValue = v[i];
					cost++;
				}
			}
			return cost == k;
		}
		bool increase(vector<int>& v, int n, int m)
		{
			int i = n - 1;
			while (i >= 0 && v[i] == m)
				i--;
			if (i < 0)
				return false;
			else
			{
				v[i]++;
				fill(v.begin() + i + 1, v.end(), 1);
				return true;
			}
		}
		int numOfArrays(int n, int m, int k)
		{
			vector<int>v(n, 1);
			int validCount = 0;
			do {
				if (isValid(v, n, k))
					validCount++;
			} while (increase(v, n, m));
			return validCount;
		}
	};

	void Test_1420_Build_Array_Where_You_Can_Find_Maximum_Exactly_K_Comparisons()
	{
		int n, m, k;
		Solution3 s;
		BruteForceSolution bts;

		while (true)
		{
			cout << "n: (0 to exit)  > ";
			cin >> n;
			if (n == 0) break;
			cout << "m: ";
			cin >> m;
			cout << "k: ";
			cin >> k;
			cout << s.numOfArrays(n, m, k) << "      " << bts.numOfArrays(n, m, k) << '\n';
		}
	}
}