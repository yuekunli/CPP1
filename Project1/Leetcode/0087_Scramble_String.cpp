#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>

namespace _0087_Scramble_String {

using namespace std;


/*
* if s2 is a permutation of s1, can I just say there must be a way to scramble s1 so that the result is s2?
* 
* No.
* 
* Example:  s1: 1 2 3 4 5
*           s2: 1 3 5 2 4
* 
*/



/* need some caching machanism to remember what has been done

 s1: a b c d e f g h i j k
 s2: m n o p q r s t u v w

 first cut between 'a' and 'b'

	a | b c d e f g h i j k

	 1st possibility: no swap:

		 s1: a | b c d e f g h i j k  
		 s2: m   n o p q r s t u v w

		 subsequently check b--k vs. n--w
		 I'll eventually check b--j vs. n--v

	 2nd possibility: swap:

		 s1: b c d e f g h i j k | a
		 s2: m n o p q r s t u v   w

			 second cut between 'j' and 'k'

				 s1: b c d e f g h i j | k    |   a

				 swap:

					 s1: k | b c d e f g h i j    |    a
					 s2: m   n o p q r s t u v         w
					 I'll need to check b--j  vs. n--v again, it'll be good if the result has been cached.
*/

class Solution
{

	/*
	* This solution uses an entire substring as the "pattern" .
	* And it uses a map to record :  pattern -->  what this pattern can be scrambled into
	* This is not a good strategy
	*/

public:

	unordered_map<string, unordered_set<string>> m;  // for a certain string, what are all the strings it can be scrambled into
	int useCache = 0;

	// alternative: for a certain range in s1 (identified by a pair of indices)
	// what are the ranges of s2 it(the range in s1) can be scrambled into

	// There is a problem with both approaches. They only tell us what *can* be done, they don't tell us what *cannot* be done
	// if it's already checked that a range in s1 *cannot* be scrambled into a range in s2, I want to cache that too.


	bool check_Recur(string& s1, string& s2, size_t s1Start, size_t s1End, size_t s2Start, size_t s2End)
	{
		if (s1Start == s1End)
		{
			return s1[s1Start] == s2[s2Start] ? true : false;
		}

		// check if these two segments are already equal
		auto result = mismatch(s1.begin()+s1Start, s1.begin()+s1End+1, s2.begin()+s2Start);
		if (result.first == s1.begin() + s1End + 1)
			return true;

		size_t totalLen = s1End - s1Start + 1;

		string sv1(s1.begin() + s1Start, s1.begin()+s1End+1);
		string sv2(s2.begin() + s2Start, s2.begin()+s2End+1);
		if (m.find(sv1) != m.end() && m[sv1].find(sv2) != m[sv1].end())
		{
			useCache++;
			return true;
		}
		
		for (size_t firstPartLen = 1; firstPartLen <= totalLen-1; firstPartLen++)
		{
			// s1: 5,  6,  7,  8,  9,  10 
			// s2: 17, 18, 19, 20, 21, 22

			// no swap
			if (check_Recur(s1, s2, s1Start, s1Start + firstPartLen - 1, s2Start, s2Start + firstPartLen - 1) &&
				check_Recur(s1, s2, s1Start + firstPartLen, s1End, s2Start + firstPartLen, s2End))
			{
				m[sv1].insert(sv2);
				return true;
			}

			// swap
			if (check_Recur(s1, s2, s1Start, s1Start + firstPartLen - 1, s2End - firstPartLen + 1, s2End) &&
				check_Recur(s1, s2, s1Start + firstPartLen, s1End, s2Start, s2End - firstPartLen))
			{
				m[sv1].insert(sv2);
				return true;
			}
		}
		return false;
	}



	bool isScramble(string s1, string s2)
	{
		m.clear();
		useCache = 0;

		return check_Recur(s1, s2, 0, s1.size() - 1, 0, s2.size()-1);
	}
};



class Solution2
{
public:

	bool check_Recur(signed char***t, string& s1, string& s2, size_t s1Start, size_t s1End, size_t s2Start, size_t s2End)
	{
		size_t totalLen = s1End - s1Start + 1;

		if (t[s1Start][s2Start][totalLen] != -1)
			return t[s1Start][s2Start][totalLen] == 1;

		if (s1Start == s1End)
		{
			t[s1Start][s2Start][totalLen] = s1[s1Start] == s2[s2Start] ? 1 : 0;
			return t[s1Start][s2Start][totalLen] == 1;
		}

		// check if these two segments are already equal
		auto result = mismatch(s1.begin() + s1Start, s1.begin() + s1End + 1, s2.begin() + s2Start);
		if (result.first == s1.begin() + s1End + 1)
		{
			t[s1Start][s2Start][totalLen] = 1;
			return true;
		}

		// should I check if these two segments at least have the same letters?
		// i.e. maybe use is_permutation? It could be a waste of time, but also could save some time.

		for (size_t firstPartLen = 1; firstPartLen <= totalLen - 1; firstPartLen++)
		{
			// s1: 5,  6,  7,  8,  9,  10      (these are indexes)
			// s2: 17, 18, 19, 20, 21, 22

			// no swap
			if (check_Recur(t, s1, s2, s1Start, s1Start + firstPartLen - 1, s2Start, s2Start + firstPartLen - 1) &&
				check_Recur(t, s1, s2, s1Start + firstPartLen, s1End, s2Start + firstPartLen, s2End))
			{
				t[s1Start][s2Start][totalLen] = 1;
				return true;
			}

			// swap
			if (check_Recur(t, s1, s2, s1Start, s1Start + firstPartLen - 1, s2End - firstPartLen + 1, s2End) &&
				check_Recur(t, s1, s2, s1Start + firstPartLen, s1End, s2Start, s2End - firstPartLen))
			{
				t[s1Start][s2Start][totalLen] = 1;
				return true;
			}
		}
		t[s1Start][s2Start][totalLen] = 0;
		return false;
	}

	bool isScramble(string s1, string s2)
	{
		size_t len = s1.size();

		signed char*** t = new signed char** [len];

		for (size_t i = 0; i < len; i++)
		{
			t[i] = new signed char* [len];
			for (size_t j = 0; j < len; j++)
			{
				t[i][j] = new signed char[len+1];
				for (size_t k = 0; k <= len; k++)
				{
					t[i][j][k] = -1;
				}
			}
		}

		bool result = check_Recur(t, s1, s2, 0, len - 1, 0, len - 1);

		for (size_t i = 0; i < len; i++)
		{
			for (size_t j = 0; j < len; j++)
			{
				delete[] t[i][j];
			}
			delete[] t[i];
		}
		delete[] t;

		return result;
	}
};


void Test_0087_Scramble_String()
{
	Solution2 s;

	while (true)
	{
		string s1;
		string s2;

		cout << "input s1: ";
		getline(cin, s1);
		if (s1.size() == 0) break;
		cout << "input s2: ";
		cin >> s2;
		cin.ignore(); 	
		cout << s1 << "  -->  " << s2 << "    " << boolalpha << s.isScramble(s1, s2) << '\n';
			//<<"use cache "<<s.useCache<<" times     cache size "<<s.m.size()<<'\n';
	}
}

}