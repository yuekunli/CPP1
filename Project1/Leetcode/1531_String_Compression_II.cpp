#include "LibHeaders.h"

namespace _1531_String_Compression_II {

	using namespace std;

	/*
	* This must not use a backtracking solution.
	* I can't make keep-or-delete decision on individual characters.
	* I must view a character together with its neighbors.
	* This uses a dynamic programming approach. The index should not move one character at a time.
	* If I don't do grouping, then the index should move in a "jumpy" way.
	*/

	class Solution
	{
		struct Group
		{
			char letter{ 'a' };
			int length{ 0 };
			Group(char let, int len) :letter(let), length(len) {}
		};

		int solveGroups(vector<Group>& groups, int k)
		{
			int len = groups.size();
			vector<vector<int>> r(len, vector<int>(k + 1, 100));
			{
				// initialize the record book for the right-most group
				int gl = groups[len - 1].length;
				for (int _k = 0; _k <= k; _k++)
				{
					int reducedGL = gl - _k;
					if (reducedGL == 100)
						r[len - 1][_k] = 4;
					else if (10 <= reducedGL)
						r[len - 1][_k] = 3;
					else if (reducedGL > 1)
						r[len - 1][_k] = 2;
					else if (reducedGL == 1)
						r[len - 1][_k] = 1;
					else
						r[len - 1][_k] = 0;
				}
			}

			for (int i = len - 2; i >= 0; i--)
			{
				for (int cr = 0; cr <= k; cr++)  // "cr" means credit. 'k' can be treated as credit which I can use
				{
					int gl = groups[i].length;

					int	result1 = gl == 100 ? 4 : (gl >= 10 ? 3 : (gl > 1 ? 2 : 1));  // don't do anything to this group
					if (i + 1 < len)
						result1 += r[i+1][cr];


					int result3 = 100;   // reduce from 3 digits to 2 digits
					if (gl >= 100 && cr >= gl-99)
					{
						result3 = 3;
						if (i + 1 < len)
							result3 += r[i+1][cr-gl+99];
					}

					int result5 = 100;   // reduce to single digit
					if (gl >= 10 && cr >= gl - 9)
					{
						result5 = 2;
						if (i + 1 < len)
							result5 += r[i+1][cr-gl+9];
					}

					int result6 = 100;   // reduce to just 1
					if (cr >= gl - 1)
					{
						result6 = 1;
						if (i + 1 < len)
							result6 += r[i + 1][cr - gl + 1];
					}

					int result2 = 100;   // completely delete this group
					if (gl <= cr)
					{
						result2 = 0;
						if (i + 1 < len)
							result2 += r[i + 1][cr - gl];
					}

					/*
					* There may be such case:
					* ........ g5, g6, g7, g8, ....
					* group#5 and group#7 have the same letter. If I insist on cutting
					* the entire string in between group#5 and group#6, i.e. I only consider
					* what I can do to group#5 and view everything from group#6 and onward as
					* an entirety, I won't explore the possibility of deleting group#6 and
					* merge group#5 and group#7. Therefore I need to consider using the
					* available credit to delete groups after current group and merge as many
					* groups with current group as possible. Of course, the optimal solution
					* may not appear when I merge as many as I can, so test the result every
					* time I do get to merge with a group.
					*/

					char gletter = groups[i].letter;
					int combinedLength = gl;
					int _cr = cr;
					int j = i + 1;
					int result7 = 100;
					while (j < len)
					{
						if (groups[j].letter != gletter)
						{
							if (_cr >= groups[j].length)
								_cr -= groups[j].length;
							else
								break;
						}

						else // (groups[j].letter == gletter)
						{
							combinedLength += groups[j].length;

							int _result7 = combinedLength == 100 ? 4 : (combinedLength >= 10 ? 3 : (combinedLength > 1 ? 2 : 1));
							if (j + 1 < len)
								_result7 += r[j + 1][_cr];

							if (_result7 < result7)
								result7 = _result7;
						}
						
						j++;
					}

					r[i][cr] = min({ result1, result2, result3, result5, result6, result7 });
				}
			}

			return r[0][k];
		}

		public:

		int getLengthOfOptimalCompression(string s, int k)
		{
			vector<Group> groups;

			int len = s.size();
			char current = s[0];
			int currentLen = 1;
			for (int i = 1; i < len; i++)
			{
				if (s[i] == current)
					currentLen++;
				else
				{
					groups.emplace_back(current, currentLen);
					current = s[i];
					currentLen = 1;
				}
			}
			groups.emplace_back(current, currentLen);

			return solveGroups(groups, k);
		}
	};


	class BruteForceSolution
	{
		bool isKOnes(int code, int n, int k)
		{
			int count = 0;
			for (int i = 0; i < n; i++)
			{
				if (code & (1 << i))
					count++;
			}
			return count == k;
		}
		int grayCode(int i)
		{
			return i ^ (i >> 1);
		}

		int compressString(string& s)
		{
			int len = s.size();
			char current = s[0];
			int currentLen = 1;
			int compressedLength = 0;
			for (int i = 1; i <= len; i++)
			{
				if (i < len && s[i] == current)
					currentLen++;
				else
				{
					compressedLength++;  // add the letter
					
					if (1 < currentLen && currentLen < 10)
						compressedLength++;
					else if (10 <= currentLen && currentLen < 100)
						compressedLength += 2;
					else if (currentLen >= 100)
						compressedLength += 3;

					if (i == len) 
						break;

					current = s[i];
					currentLen = 1;
				}
			}
			return compressedLength;
		}
	public:
		int getLengthOfOptimalCompression(string s, int k)
		{
			int len = s.size();
			int codeMax = 1 << len;
			int minLength = 100;
			for (int i = 0; i < codeMax; i++)
			{
				int code = grayCode(i);
				if (isKOnes(code, len, k))
				{
					string s2;
					s2.reserve(len - k);
					for (int j = len-1; j >= 0; j--)
					{
						if (!(code & (1 << j)))
						{
							s2.push_back(s[j]);
						}
					}
					int compressedLength = compressString(s2);
					if (compressedLength < minLength)
						minLength = compressedLength;
				}
			}
			return minLength;
		}
	};

	class AutoTest
	{
		random_device rd;
		mt19937 gen;
		uniform_int_distribution<> uid;

		Solution fast;
		BruteForceSolution brute;

		void generateString(string& s, int & k)
		{
			int len = 0;
			while (len < 8)
				len = gen() % 14;
			s.resize(len);
			generate(s.begin(), s.end(), bind(uid, ref(gen)));
			k = 0;
			while (k < 4 || k > len-2)
				k = gen() % len;
		}

		bool runOneTest()
		{
			string s;
			int k;
			generateString(s, k);
			int a1 = fast.getLengthOfOptimalCompression(s, k);
			int a2 = brute.getLengthOfOptimalCompression(s, k);
			if (a1 != a2)
			{
				cout << "ERROR !!!\n";
				cout << s << '\n';
				cout << "fast answer: " << a1 << "     brute force answer: " << a2 << '\n';
				return false;
			}
			return true;
		}

	public:

		AutoTest() : gen(rd()), uid(97, 100) {}

		void run(int n)
		{
			for (int i = 0; i < n; i++)
				if (!runOneTest())
					break;
		}
	};

	void Test_1531_String_Compression_II()
	{
		string s1;
		int k;
		int option;
		Solution s;
		AutoTest ats;

		while (true)
		{
			cout << "1: manual test;     2: auto test;    0: exit  > ";
			cin >> option;
			if (option == 0) break;
			if (option == 1)
			{
				s1.clear();
				cin.ignore();
				cout << "input string: ";
				getline(cin, s1);
				if (s1.size() == 0) break;
				cout << "k: ";
				cin >> k;
				cin.ignore();
				cout << s.getLengthOfOptimalCompression(s1, k) << "\n\n";
			}
			else if (option == 2)
			{
				int n;
				cout << "number of test cases: ";
				cin >> n;
				ats.run(n);
			}
		}
	}
}