#include "LibHeaders.h"

namespace _1335_Minimum_Difficulty_of_a_Job_Schedule {

	using namespace std;

	class Solution
	{
	public:

		// for example, a call to this function has idx = 5,  d = 3
		// I need to schedule the 5th job (0-indexed) and the ones after it of course,
		// I have decided to not do this 5th job in any day prior to the last 3 days.
		// In another word, I have 3 days left, the earliest I can schedule this 5th job
		// is the first of the last 3 days.

		int len{ 0 };

		int minDiffi_Recur(int**t, vector<int>& dif, int d, int idx)
		{
			if (len - idx < d)  // total number of jobs yet to be scheduled: len - idx
				return -1;

			if (d <= 0 && idx == len) // no more days, no more jobs either
				return 0;

			if (t[idx][d] != -1)
				return t[idx][d];

			int maxJobDifficulty = dif[idx];  // maximum difficulty of jobs I want to schedule for this day
			int minScheduleDifficulty = numeric_limits<int>::max();
			int difficultyForTheScheme = 0;
			int difficultyForTheRest = 0;

			// If I have 12 jobs in total and 4 days in total, I can't put more than 9 jobs in the first day
			for (int i = idx; i < len && len - i >= d-1; i++)
			{
				if (dif[i] > maxJobDifficulty)
				{
					maxJobDifficulty = dif[i];
				}
				difficultyForTheRest = minDiffi_Recur(t, dif, d - 1, i + 1);
				if (difficultyForTheRest >= 0) // this inequality must be >= instead of strict >, because if the rest is the case, "0 jobs, 0 days" it will return 0
				{
					difficultyForTheScheme = maxJobDifficulty + difficultyForTheRest;
					if (difficultyForTheScheme < minScheduleDifficulty)
						minScheduleDifficulty = difficultyForTheScheme;
				}
			}
			t[idx][d] = minScheduleDifficulty;
			return minScheduleDifficulty;
		}

		int minDifficulty(vector<int>& jobDifficulty, int d)
		{
			len = jobDifficulty.size();
			int** t = new int* [len];
			for (int i = 0; i < len; i++)
			{
				t[i] = new int[d+1];
				for (int j = 0; j <= d; j++)
				{
					t[i][j] = -1;
				}
			}

			int answer = minDiffi_Recur(t, jobDifficulty, d, 0);

			for (int i = 0; i < len; i++)
			{
				delete[] t[i];
			}
			delete[] t;

			return answer;
		}
	};

	void Test_1335_Minimum_Difficulty_of_a_Job_Schedule()
	{
		Solution s;
		vector<int> v;
		string s1;
		int days = 0;

		while (true)
		{
			s1.clear();
			cout << "job difficulties: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			v.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(v));
			cout << "days: ";
			cin >> days;
			cin.ignore();
			cout << s.minDifficulty(v, days) << '\n';
		}
	}
}