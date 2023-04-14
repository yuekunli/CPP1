#include "LibHeaders.h"

namespace _1872_Stone_Game_VIII {

	using namespace std;

	class Solution
	{
		/*
		* For the cases where there are only 1 or 2 stones left. Don't set the value in the record book,
		* just return the value. Only check the record book when the stones left are greater than 2.
		* For example, this case "1 stone left, it's Bob's turn" can appear many times but under different
		* circumstances. In that case, Bob really doesn't have any choice, the game has already ended
		* at that point. The call frame with that condition should just return what Alice got in her last play.
		* For example: stones: a, b, c, d, e,
		* Alice takes 2, Bob takes 2, alternate so on so forth. Eventually I arrive this condition "1 stone left
		* it's Bob's turn". Alice can also try take all stones in her very 1st play, now I immediately arrive
		* at the "1 stone left, it's Bob's turn" case, apperantly these two cases are different.
		*/
		int n;
		int solve(vector<vector<int>>& r, vector<int>& st, int index, int turn) // turn 0: Alice    turn 1: Bob
		{
			if (index == n - 1)
			{
				if (turn == 0)
				{
					// one stone left, it's Alice's turn, the game ends now.
					// Alice and Bob have had equal number of turns

					return 0;
				}
				else
				{
					// one stone left, it's Bob's turn, but Bob can't make a play any more
					// Alice's last play needs to be tallied. The total Alice got in her
					// last play is the value of the only stone left.

					return st[index];
				}
			}
			if (index == n-2)
			{
				if (turn == 0)
				{
					// two stones left, Alice's turn, she must take both

					return st[n - 2] + st[n - 1];
				}
				else
				{
					// two stones left, Bob's turn, he must take both

					return -st[n - 1];
				}
			}

			if (r[index][turn] != -1)
				return r[index][turn];
				
			if (turn == 0)
			{
				int maxScoreDiff = -1'000'000'007;
				int sum = 0;
				for (int i = index; i < n; i++)
				{
					sum += st[i];
					int oldValue = st[i];
					st[i] = sum;
					int result = solve(r, st, i, 1);
					if (result > maxScoreDiff)
						maxScoreDiff = result;
					st[i] = oldValue;
				}
				r[index][0] = maxScoreDiff;
				return r[index][0];

			}
			else
			{
				int minScoreDiff = 1'000'000'007;
				int sum = 0;
				for (int i = index+1; i < n; i++)
				{
					sum += st[i];
					int result = -sum;
					int oldValue = st[i];
					st[i] = sum;
					int result2 = solve(r, st, i, 0);
					if (result + result2 < minScoreDiff)
						minScoreDiff = result + result2;
					st[i] = oldValue;
				}
				r[index][1] = minScoreDiff;
				return r[index][1];
			}
		}
	public:
		int stoneGameVIII(vector<int>& stones)
		{
			n = stones.size();
			vector<vector<int>> recordbook(n, vector<int>(2, -1));
			return solve(recordbook, stones, 0, 0);
		}
	};

	void Test_1872_Stone_Game_VIII()
	{
		Solution s;
		string s1;
		vector<int>stones;
		while (true)
		{
			s1.clear();
			cout << "stones: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stones.clear();
			stringstream ss(s1);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(stones));
			cout << s.stoneGameVIII(stones) << "\n\n";
		}
	}
}