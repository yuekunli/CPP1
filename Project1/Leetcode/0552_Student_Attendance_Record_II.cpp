#include "LibHeaders.h"

namespace _0552_Student_Attendance_Record_II {

	using namespace std;

	class Solution
	{
		/*
		* for a record string of length n, what are the different "classes" of composition?
		* (1). No 'A'
		* (2). 1 'A'
		* (3). Start with an 'L'
		* (4). start with 2 L's
		* 
		* But these are not mutually exlcusive.
		* 
		* (1). Start with an 'L' and there is no 'A'
		* (2). Start with an 'L' and there is 1 'A'
		* (3). Start with 2 L's and there is no 'A'.
		* (4). Start with 2 L's and there is 1 'A'.
		* (5). Doesn't start with 'L', there is no 'A'
		* (6). Doesn't start with 'L', there is 1 'A'
		* 
		* Try n==3, see what are the counts for each class
		* 
		* (1). start 1 'L', no 'A':    LPP, LPL
		* (2). start 1 'L', 1 'a':     LAP, LPA, LAL
		* (3). start 2 L's, no 'A':    LLP
		* (4). start 2 L's 1 'A':      LLA
		* (5). not start 'L', no 'A':  PPP, PPL, PLP, PLL
		* (6). not start 'L', 1 'A':   PAP, PAL, PPA, PLA, APP, APL, ALP, ALL
		* 
		* n==4
		* (1) LPPP, LPPL, LPLP, LPLL
		* (2) LAPP, LAPL, LALP, LALL, LPAP, LPAL, LPPA, LPLA
		* (3) LLPP, LLPL
		* (4) LLAP, LLPA
		* (5) PPPP, PPPL, PPLP, PPLL, PLPP, PLPL, PLLP
		* (6) APPP, APPL, APLP, APLL, ALPP, ALPL, ALLP, PAPP, PAPL, PALP, PALL, PPAP, PPAL, PLAP, PLAL, PPPA, PPLA, PLPA, PLLA
		*/
	public:
		int checkRecord(int n)
		{
			if (n == 1) return 3;
			if (n == 2) return 8;

			int m = 1'000'000'007;

			unsigned long long start1LNoA = 2;
			unsigned long long start1L1A = 3;
			unsigned long long start2LNoA = 1;
			unsigned long long start2L1A = 1;
			unsigned long long notStartLNoA = 4;
			unsigned long long notStartL1A = 8;

			unsigned long long start1LNoA_;
			unsigned long long start1L1A_;
			unsigned long long start2LNoA_;
			unsigned long long start2L1A_;
			unsigned long long notStartLNoA_;
			unsigned long long notStartL1A_;

			for (int i = 4; i <= n; i++)
			{
				start1LNoA_ = start1LNoA;
				start1L1A_ = start1L1A;
				start2LNoA_ = start2LNoA;
				start2L1A_ = start2L1A;
				notStartLNoA_ = notStartLNoA;
				notStartL1A_ = notStartL1A;

				start1LNoA = notStartLNoA_;
				start1L1A = notStartL1A_;
				start2LNoA = start1LNoA_;
				start2L1A = start1L1A_;
				notStartLNoA = ((start1LNoA_ + start2LNoA_) %m + notStartLNoA_) %m;
				               // prepend 'P' for all 3 cases
				notStartL1A = (((((start1LNoA_ + start1L1A_) % m + start2LNoA_) %m + start2L1A_) %m + notStartLNoA_) %m + notStartL1A_) %m;
				               // either prepend 'P' or prepend 'A'
			}

			return (int)((((((start1LNoA + start1L1A) % m + start2LNoA) % m + start2L1A) % m + notStartLNoA) % m + notStartL1A) % m);
		}
	};

	void Test_0552_Student_Attendance_Record_II()
	{
		Solution s;
		int n;
		while (true)
		{
			cout << "n: ";
			cin >> n;
			if (n == 0) break;
			cout << s.checkRecord(n) << "\n\n";
		}
	}
}