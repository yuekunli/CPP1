#include "LibHeaders.h"

namespace _1900_Rounds_Where_Players_Compete {

	using namespace std;

	class Solution
	{
		/*
		* If the contests in a round is scheduled like this:
		* game 1:  1   vs.   12
		* game 2:  2*  vs.   11
		* game 3:  3   vs.   10
		* game 4:  4   vs.   9
		* game 5:  5   vs.   8*
		* game 6:  6   vs.   7
		* 
		* '2' is the firstPlayer, '8' is the secondPlayer.
		* 
		* What is the position of the first and second players in the next round?
		* There is 1 game before firstPlayer's game, so firstPlayer can be the 1st or 2nd in the next round.
		* There are 4 games prior to secondPlayer's game, one of those 4 involves the firstPlayer who sure wins,
		* so secondPlayer can be 2nd, 3rd, 4th, 5th in the next round.
		* 
		* One thing to note is that before the '2' and '8' meet each other, it doesn't matter which I call
		* firstPlayer or secondPlayer, I can say '2' is the "secondPlayer", '8' is the "firstPlayer",
		* it doesn't matter because they always win before they meet. But if firstPlayer's id is less than
		* secondPlayer's id, it will remain that way till they meet, also because they always win.
		* 
		* Essentially, what I need to consider is the number of games won by smaller ID players before
		* firstPlayer's game and before secondPlayer's game.
		* 
		* I have to consider a few difference scenarios:
		* (1). both player's IDs are in the first half:
		* game 1:  1   vs.   12
		* game 2:  2*  vs.   11
		* game 3:  3   vs.   10
		* game 4:  4   vs.   9
		* game 5:  5*  vs.   8
		* game 6:  6   vs.   7
		* 
		* (2). 
		* game 1:  1   vs.   12
		* game 2:  2*  vs.   11
		* game 3:  3   vs.   10
		* game 4:  4   vs.   9
		* game 5:  5   vs.   8*
		* game 6:  6   vs.   7
		*
		* (3). this case is special as the secondPlayer's game is played first
		* game 1:  1   vs.   12
		* game 2:  2   vs.   11* <== this is secondPlayer's game
		* game 3:  3   vs.   10
		* game 4:  4   vs.   9
		* game 5:  5   vs.   8*  <== this is firstPlayer's game
		* game 6:  6   vs.   7
		* 
		* If I guarantee firstPlayer's ID is less than secondPlayer's, there won't be the scenario
		* that firstPlayer is in the 2nd half while secondPlayer is in the 1st half
		*/

		pair<int, int> solve(vector<vector<vector<pair<int,int>>>>& r, int total, int p1Pos, int p2Pos)
		{
			if (r[total][p1Pos][p2Pos].first != -1)
				return r[total][p1Pos][p2Pos];

			if (total+1 == p1Pos + p2Pos)
			{
				r[total][p1Pos][p2Pos] = { 1,1 };
				return r[total][p1Pos][p2Pos];
			}

			int gamesBeforeP1 = p1Pos - 1;
			if (p1Pos <= (total + 1) / 2)
			{
				gamesBeforeP1 = p1Pos - 1;
			}
			else
			{
				int p1Rival = total + 1 - p1Pos;
				gamesBeforeP1 = p1Rival - 1;
			}

			int gamesBeforeP2;
			if (p2Pos <= (total + 1) / 2)
			{
				gamesBeforeP2 = p2Pos - 1;
			}
			else
			{
				int p2Rival = total + 1 - p2Pos;
				gamesBeforeP2 = p2Rival - 1;
			}

			int nextRoundTotal = (total + 1) / 2;

			int earliest = 100;
			int latest = 0;

			if (p1Pos <= (total + 1) / 2)
			{
				int gamesInBetween = gamesBeforeP2 - gamesBeforeP1 - 1;
				for (int i = 0; i <= gamesBeforeP1; i++)
				{
					for (int j = 0; j <= gamesInBetween; j++)
					{
						auto [early, late] = solve(r, nextRoundTotal, i + 1, i + 1 + j + 1);
						earliest = min(earliest, early + 1);
						latest = max(latest, late + 1);
					}
				}
			}
			else
			{
				/*
				* game 1:  1   vs.   12
				* game 2 : 2   vs.   11 * <= = this is secondPlayer's game
				* game 3 : 3   vs.   10
				* game 4 : 4   vs.   9
				* game 5 : 5   vs.   8 * <= = this is firstPlayer's game
				* game 6 : 6   vs.   7
				* */
				// there are 3 segments: games played before secondPlayer's game,
				// games played in between secondPlayer's and firstPlayer's,
				// and games played after firstPlayer's
				// i, j represent the number of games won by smaller ID player in corresponding segments
				int gamesInBetween = gamesBeforeP1 - gamesBeforeP2 - 1;
				int gamesAfterP1 = (total + 1) / 2 - gamesBeforeP1 - 1;
				for (int i = 0; i <= gamesBeforeP2; i++)
				{
					for (int j = 0; j <= gamesInBetween; j++)
					{
						auto [early, late] = solve(r, nextRoundTotal, i + j + gamesAfterP1 + 1, i + gamesInBetween+1+gamesAfterP1);
						earliest = min(earliest, early + 1);
						latest = max(latest, late + 1);
					}
				}
			}

			return { earliest, latest };
		}

	public:

		/*
		* I actually don't need the entire 3-D array. For examle if the tournament starts with 28 players,
		* but when the total is for example '14', p1's position and p2's position won't exceed 14.
		* I should initialize the 2nd and 3rd dimension of the 3-D array based on the value of the 1st dimension
		*/
		vector<int> earliestAndLatest(int n, int firstPlayer, int secondPlayer)
		{
			vector<vector<vector<pair<int, int>>>>records(n + 1, vector<vector<pair<int,int>>>(n + 1, vector<pair<int,int>>(n + 1, {-1,-1})));
			auto [earliest, latest] = solve(records, n, firstPlayer, secondPlayer);
			return { earliest, latest };
		}
	};

	void Test_1900_Rounds_Where_Players_Compete()
	{
		Solution s;
		string s1;
		vector<int>parameters(3);
		while (true)
		{
			s1.clear();
			cout << "n:   firstPlayer:   secondPlayer: ";
			getline(cin, s1);
			if (s1.size() == 0)break;
			stringstream ss(s1);
			copy_n(istream_iterator<int>(ss), 3, parameters.begin());
			vector<int>const& answer = s.earliestAndLatest(parameters[0], parameters[1], parameters[2]);
			cout << answer[0] << "  " << answer[1] << "\n\n";
		}
	}
}