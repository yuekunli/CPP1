#include "LibHeaders.h"

namespace _1307_Verbal_Arithmetic_Puzzle {

	// single letter word can be a zero even though it's a leading zero.

	using namespace std;

	class Solution1
	{
		// Solution from discussion
	public:
		unordered_map<char, int> m;
		vector<char> chs;
		unordered_set<char> leading;
		int used[10] = {};

		bool valid(vector<string>& words, string result)
		{
			int sum = 0;
			for (int i = 0; i < result.size(); i++)
			{
				for (auto& w : words)
				{
					if (i >= w.size())
						continue;
					char c = w[w.size() - i - 1];
					if (m.count(c) == 0)
						return true;
					sum += m[c];
				}
				char c = result[result.size() - i - 1];
				if (m.count(c) == 0)
					return true;
				sum -= m[c];
				if (sum % 10)
					return false;
				sum /= 10;
			}
			return true;
		}

		bool dfs(vector<string>& words, string result, int index)
		{
			if (index == chs.size())
				return true;
			for (int i = 0; i < 10; i++)
			{
				if (used[i] || (i==0 && leading.count(chs[index])))
					continue;
				used[i] = 1;
				m[chs[index]] = i;
				if (valid(words, result) && dfs(words, result, index + 1))
					return true;
				m.erase(chs[index]);
				used[i] = 0;
			}
			return false;
		}

		void addChar(char charToAdd)
		{
			for (char c : chs)
			{
				if (c == charToAdd)
					return;
			}
			chs.push_back(charToAdd);
		}

		bool isSolvable(vector<string>& words, string result)
		{
			for (auto& w : words)
			{
				if (w.size() > result.size())
					return false;  // I'm doing addition, the number of digits in "result" can only be equal to or greater than number of digits in each adden

				if (w.size() > 1)
					leading.insert(w[0]); // remember all the letters that lead a word, they must not be 0
			}
			
			if (result.size() > 1)
				leading.insert(result[0]);

			for (int i = 0; i < result.size(); i++)  // when i = 0, i'm considering the right-most digits
			{
				for (auto& w : words)
				{
					if (i < w.size())  // if a word's size is 5, when i=0, I'm considering w[4] which is the least significant digit
						addChar(w[w.size() - i - 1]);
				}
				addChar(result[result.size() - i - 1]);
			}
			return dfs(words, result, 0);
		}

	};


	class Solution2
	{
		// my solution
		/*
		* If I'm trying to solve SEND + MORE = MONEY
		* I first convert this equation to a 2-dimension array like this:
		* {
		*     {D, E, Y},
		*     {N, R, E},
		*     {E, O, N},
		*     {S, M, O},
		*           {M}
		* }
		* 
		* back track assign a value (0 to 9) to each letter in the order of them appearing in this 2-dimension array
		* To be specific, I assign a value to 'D', then to 'E', then to 'Y', now one row is complete, I quickly verify this row
		*/

		/*
		* This solution can still be made faster. I'm basically solving an array of this: D E Y N R E E O N S M O M
		* It's longer than needed. The recursion tree eventually becomes bigger. Even though the extra nodes
		* in the tree don't have multiple children (because when I arrive at the 2nd 'E', I move on quickly),
		* they do appear in many many places. In another word, when I arrive at 'R', I need to loop through
		* all possible values for 'R', every possibility then lead to a letter E which is a duplicate.
		* Even though that 'E' doesn't expand, it appears many times.
		*/

	public:
		bool isSolvable(vector<string>& words, string result)
		{
			vector<int> m(26, -1);  // each letter's value

			vector<bool> lead(26, false); // whether a letter is the leading letter in a word or the result

			vector<bool> used(10, false); // whether a value (0 to 9) is already used

			vector<vector<int>> columnsToVerify; // I call it "columns to verify", is because when doing arithmetic addition, we line up addens vertically
			// each column in the vertical line up eventually becomes a row in this 2-dimension vector
			// if the equation is RA + TB = PC
			// this vector will have {{0, 1, 2}, {18, 20,16}}   R is 18th letter, T is 20th letter, P is 16th letter
			//                         A  B  C    R   T  P

			vector<int> carry(result.size(), 0);

			for (string& w : words)
			{
				if (w.size() > result.size())
					return false;

				int p = w[0] - 'A';
				if(w.size() > 0)  // this word is not a single letter word
					lead[p] = true;
			}
			int p = result[0] - 'A';
			if (result.size() > 0)
				lead[p] = true;

			for (int i = 0; i < result.size(); i++)
			{
				columnsToVerify.push_back({});
				// if i=3, I'm considering the 4th digit counting from the right and the right-most being the 1st digit
				// the right-most digit has index size()-1, if i is 3, it means jumping 3 hops to the left.
				vector<int>& v = *(columnsToVerify.rbegin());
				for (string& w : words)
				{
					if (w.size() > i) // if 'i' is 1, which means I can jump 1 hop to the left, there have to be at least 2 elements
					{
						// if i=3, a word is "RPQW",   I'm considering 'R' which is w[0]        w.size()-1-i = 4-1-3 = 0
						v.push_back(w[(w.size() - 1) - i] - 'A');
					}
				}
				v.push_back(result[(result.size() - 1) - i] - 'A');
			}
			return backtrackSolve(m, used, lead, columnsToVerify, carry, 0, 0);
		}

		/*
		* row and col are the position of a letter in columnsToVerify
		* this letter is the one I need to solve in this call to this function
		* 
		* Note that if vector<vector<int>>&v is not declared as passing by reference,
		* instead if it's passed by copy, the run time is significantly slower!
		*/
		bool backtrackSolve(vector<int>&m, vector<bool>&used, vector<bool>&lead, vector<vector<int>>&v, vector<int>&carry, int row, int col)
		{
			int letter = v[row][col];

			// is the letter already assigned with a value?
			if (m[letter] != -1)
			{
				// if it's already assigned, and if it's from an adden, just move on
				if (col < v[row].size() - 1)
					return backtrackSolve(m, used, lead, v, carry, row, col + 1);

				// if this letter is from result, I can do a quick verify
				int carryToNext;
				if (checkRow(m, v[row], carry[row], carryToNext))
				{
					// this row checks out
					
					if (row == v.size() - 1)
					{
						// this is the last row
						return true;
					}
					else
					{
						// not the last row, set carry and move on
						carry[row + 1] = carryToNext;
						bool resultOfTheRest = backtrackSolve(m, used, lead, v, carry, row + 1, 0);
						if (!resultOfTheRest)
						{
							carry[row + 1] = 0;
							return false;
						}
						return true;
					}
				}
				else
				{
					// this row doesn't add up
					return false;
				}
			}
			else
			{
				// have not assigned a value to this letter yet.
				for (int i = 0; i <= 9; i++)
				{
					if (i == 0 && lead[letter])
						continue;

					if (!used[i])
					{
						m[letter] = i;
						used[i] = true;
						if (col != v[row].size() - 1)
						{
							// this is a letter from a word (not the result), move on
							bool furtherResult = backtrackSolve(m, used, lead, v, carry, row, col + 1);
							if (!furtherResult)
							{
								m[letter] = -1;
								used[i] = false;
								// continue the loop try next value for this letter
							}
							else
								return true;
						}
						else
						{
							// this is the letter from the result, check this row,
							int carryToNext;
							if (checkRow(m, v[row], carry[row], carryToNext))
							{
								if (row == v.size() - 1)
								{
									// this is the last row
									return true;
								}

								carry[row + 1] = carryToNext;
								int furtherResult = backtrackSolve(m, used, lead, v, carry, row + 1, 0);
								if (!furtherResult)
								{
									carry[row + 1] = 0;
									m[letter] = -1;
									used[i] = false;
									// continue the loop try next value for this letter
								}
								else
									return true;
							}
							else
							{
								// this row doesn't add up, just continue,
								m[letter] = -1;
								used[i] = false;
							}
						}
					}
				}
				return false;
			}
		}

		bool checkRow(vector<int>&m, vector<int>& r, int& carryFromBefore, int &carryToNext)
		{
			int sum = 0;
			if (r.size() > 1) // this row doesn't just have the result letter
			{
				for (int i = 0; i <= (int)(r.size()) - 2; i++)  // .size() function return size_t which is unsigned int
                                                                // if r.size() == 1, r.size()-2 is cast to an unsigned int instead of a negative signed int
				{
					sum += m[r[i]];
				}
			}
			sum += carryFromBefore;
			int tens = sum / 10;
			sum %= 10;
			if (sum == m[r[r.size() - 1]])
			{
				carryToNext = tens;
				return true;
			}
			return false;
		}
	};


	class Solution3
	{
		// attempt to improve on Solution2
		/*
		* example: SEND + MORE = MONEY
		* convert this equation to a 2-dimension array like this:
		* {
		*     {D, E, Y},
		*        {N, R},
		*           {O},
		*        {S, M},
		*            {}
		* }
		* Still break the equation down to rows, but don't include duplicate letters
		* Note that there can be empty rows.
		*/
	public:
		bool isSolvable(vector<string>& words, string result)
		{
			vector<int> m(26, -2);  // each letter's value, use '-1' to denote that it has been added to "to be solved"

			vector<bool> lead(26, false); // whether a letter is the leading letter in a word or the result

			vector<bool> used(10, false); // whether a value (0 to 9) is already used

			vector<vector<int>> toBeSolved;  // break the equation down to rows without duplicate letters
			vector<vector<int>> toBeVerified; // break the equation down to rows *with* duplicate letters, used for checkRow

			vector<int> carry(result.size(), 0);

			for (string& w : words)
			{
				if (w.size() > result.size())
					return false;

				int p = w[0] - 'A';
				if (w.size() > 0)  // this word is not a single letter word
					lead[p] = true;
			}
			int p = result[0] - 'A';
			if (result.size() > 0)
				lead[p] = true;

			for (int i = 0; i < result.size(); i++)
			{
				toBeSolved.push_back({});
				toBeVerified.push_back({});
				// if i=3, I'm considering the 4th digit counting from the right and the right-most being the 1st digit
				vector<int>& v = *(toBeSolved.rbegin());
				vector<int>& v2 = *(toBeVerified.rbegin());
				for (string& w : words)
				{
					if (w.size() > i)
					{
						// if i=3, a word is "RPQW",   I'm considering 'R' which is w[0]        w.size()-i-1 = 4-3-1 = 0
						int letter = w[w.size() - i - 1] - 'A';
						if (m[letter] != -1)
						{
							v.push_back(letter);
							m[letter] = -1;
						}
						v2.push_back(letter);
					}
				}
				int letter = result[result.size() - i - 1] - 'A';
				if (m[letter] != -1)
				{
					v.push_back(letter);
					m[letter] = -1;
				}
				v2.push_back(letter);
			}
			return backtrackSolve(m, used, lead, toBeSolved, toBeVerified, carry, 0, 0);
		}


		bool backtrackSolve(vector<int>& m, vector<bool>& used, vector<bool>& lead, vector<vector<int>>&v, vector<vector<int>>&v2, vector<int>& carry, int row, int col)
		{
			if (v[row].size() > 0)
			{
				int letter = v[row][col];
				for (int i = 0; i <= 9; i++)
				{
					if (i == 0 && lead[letter])
						continue;

					if (!used[i])
					{
						m[letter] = i;
						used[i] = true;
						if (col != v[row].size() - 1)
						{
							// this is not the last letter on this row
							bool furtherResult = backtrackSolve(m, used, lead, v, v2, carry, row, col + 1);
							if (!furtherResult)
							{
								m[letter] = -1;
								used[i] = false;
								// continue the loop try next value for this letter
							}
							else
								return true;
						}
						else
						{
							// this is the last letter on this row, check this row,
							int carryToNext;
							if (checkRow(m, v2[row], carry[row], carryToNext))
							{
								if (row == v.size() - 1)
								{
									// this is the last row
									return true;
								}

								carry[row + 1] = carryToNext;
								int furtherResult = backtrackSolve(m, used, lead, v, v2, carry, row + 1, 0);
								if (!furtherResult)
								{
									carry[row + 1] = 0;
									m[letter] = -1;
									used[i] = false;
									// continue the loop try next value for this letter
								}
								else
									return true;
							}
							else
							{
								// this row doesn't add up, just continue,
								m[letter] = -1;
								used[i] = false;
							}
						}
					}
				}
				// I'm out of the for loop, no value can work for this letter
				return false;
			}
			else
			{
				// this row in toBeSolved is empty, which means all letters on this row have been decided
				int carryToNext;
				if (checkRow(m, v2[row], carry[row], carryToNext))
				{
					if (row == v.size() - 1)
						return true;

					carry[row + 1] = carryToNext;
					int furtherResult = backtrackSolve(m, used, lead, v, v2, carry, row + 1, 0);
					if (!furtherResult)
					{
						carry[row + 1] = 0;
						return false;
					}
					else
						return true;
				}
				else
					return false;
			}
		}

		bool checkRow(vector<int>& m, vector<int>& r, int& carryFromBefore, int& carryToNext)
		{
			int sum = 0;
			if (r.size() > 1)
			{
				// this row doesn't just have the result letter

				for (int i = 0; i <= (int)(r.size()) - 2; i++)  // if r.size() == 1, r.size()-2 is probably cast to an unsigned integer, so add the r.size check ahead, or cast r.size()
				{
					sum += m[r[i]];
				}
			}
			sum += carryFromBefore;
			int tens = sum / 10;
			sum %= 10;
			if (sum == m[r[r.size() - 1]])
			{
				carryToNext = tens;
				return true;
			}
			return false;
		}
	};



	static void Test1()
	{
		vector<string>words { "SEND", "MORE" };
		string result = "MONEY";

		Solution1 s1;
		Solution2 s2;
		Solution3 s3;

		auto t1 = std::chrono::high_resolution_clock::now();
		bool s1Result = s1.isSolvable(words, result);
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli>s1Time = t2 - t1;

		t1 = std::chrono::high_resolution_clock::now();
		bool s2Result = s2.isSolvable(words, result);
		t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli>s2Time = t2 - t1;

		t1 = std::chrono::high_resolution_clock::now();
		bool s3Result = s3.isSolvable(words, result);
		t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli>s3Time = t2 - t1;

		cout << "Solution 1: " << boolalpha << s1Result << ", s1 time: " << s1Time.count()
			<< ",  Solution 2: " << boolalpha << s2Result << ", s2 time: " << s2Time.count()
			<< ",  Solution 3: " << boolalpha << s3Result << ", s3 time: " << s3Time.count() << '\n';
	}

	void Test_1307_Verbal_Arithmetic_Puzzle()
	{
		Test1();
	}

}