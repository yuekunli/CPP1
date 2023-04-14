#include<unordered_set>
#include<vector>
#include<iostream>
#include<string>


namespace _0301_Remove_Invalid_Parentheses {

	using namespace std;


	// 0 1 2 3 4 5 6 7
	// ( ) ( ) ) ( ) )
	//         |     |
	//         these two are identified extra closes
	// but if I delete 2 closes within [0,4] and none within [5,7], it also works

	// 0 1 2 3 4 5 6 7 8 9
	// ( ) ) ) ) ) ) ( ) )
	// I need to delete 6 closing parentheses in total.
	// but I must not delete 2 within [7,9]


	// for a string from i to j, if s[i] and s[j] are '(' and ')', it's either one big expression or a few parallel expressions
	// either:   ( ..............)   --- for this case,  possibilities(i,j) = possibilities(i+1, j-1)
	//           i               j
	// or    :   (..) (.......)(.)   --- for this case,  possibilities(i,j) = possibilities(k+1, p-1)
	//              k          p
	// k is the nearest closing to i, and p is the nearest opening to j
	// actually k should be immediately after i, and p should be immediately preceeding j, if there is no letter between 'i and k' and 'p and j'

	// there are more cases acutally
	// it can be () (............)
	//           ik              j
	// s[i] doesn't match s[j] and the pairing open paren that does match s[j] is somewhere in the middle insted of immediately
	// preceeding s[j].
	// likewise it can be (............)()
	//                    i             pj
	// 
	// but how do I know k and p are right next to i and j? --  I don't. That is a wrong assumption.
	// The parentheses that actually match s[i] and s[j] may be anywhere.
	// So after all, looks like I have to find a "first cut".
	//


	// what if s[i] and s[j] are '(' and '('?
	// -- it's possible that a segment doesn't produce any valid string. for example if [i,j] is "( ( ( ( ( (", 
	// then no valid string comes out of this segment
	// -- so if s[i] and s[j] don't match, I just take the better of [i+1, j] and [i, j-1]
	// 
	// have to take letters into consideration...
	// -- if s[i] and s[j] are both letters
	// -- if one of them is a letter


	// for a 10-element segment, do I look for a "first cut" or I just follow the a few scenarios described above?
	// If I look for "first cut":
	// segment length: O(N),  starting point: O(N),   first cut within segment: O(N)  ===>  overall O(N^3)
	// maybe I don't need to look for first cut


	// The solution in Solution2 actually gives *all* possible ways to remove invalid parentheses.
	// some of them don't meet the requirement of "remove *minimum* number of invalid parentheses"

	// for this example: )))()(((
	// the total number of open and close parentheses are equal.
	// but the best I can find is just "()"

	// one improvement: locate the first open paren from the left and the first close paren from the right
	// only deal with the part within those two first parentheses

	// what about this case:  )))())))(((()(((
	// drop the first 3 on the left and on the right, I get: ())))(((()



	class Solution1
	{
		/*
		* This solution tries dynamic programming, it uses a 2-dimension table.
		* cell (i, j) in the table has a series of strings.
		* These strings are all the valid strings derived from the [i,j] substring of the input
		* -------------------------------------------------------------------------------------
		* This is kind of the implementation of the above discussed idea, but this doesn't work.
		* In fact the above discussed idea is in a wrong direction.
		*/

	public:

		vector<string> removeInvalidParentheses(string s)
		{
			size_t len = s.size();
			vector<string>** t = new vector<string>*[len];
			for (size_t i = 0; i < len; i++)
			{
				t[i] = new vector<string>[len];
			}

			for (size_t i = 0; i < len; i++)
			{
				if (s[i] != '(' && s[i] != ')')
					t[i][i].push_back( s.substr(i, 1) );
			}
			
			// should I initialize all 2-element segments? as well as all 3-element segments?
			// there can be consecutive letters, so initialization will never completely avoid some nasty processing

			// len = 10, indices: 0 -- 9
			// segmentLen = 3:  last segment: 7,8,9
			
			for (int segmentLen = 2; segmentLen <= len; segmentLen++)
			{
				for (int i = 0; i <= len - segmentLen; i++)  // i: start of the segment
				{
					int j = i + segmentLen - 1;

					vector<string>& v = t[i][j];

					if ((s[i] == '(' && s[j] == ')') || 
						(s[i] != '(' && s[i] != ')' && s[j] != '(' && s[j] != ')'))  // s[i] and s[j] can match or both letters
					{
						vector<string>const& r1 = t[i + 1][j - 1];
						if (r1.size() > 0)
						{
							for (string x : r1)
							{
								x.insert(0, 1, s[i]);
								x += s[j];
								v.push_back(x);
							}
						}
						else
						{
							// if the segment length right now is just 2, the middle part I get after stripping s[i] and s[j]
							// is just empty, I won't get any answer by checking t[i+1][j-1]
							// even if the segment length is greater than 2, the middle part can still return no valid string
							// but at least s[i] and [j] match each other and should be added to answer

							string x = "";
							x += s[i];
							x += s[j];
							v.push_back(x);  // need the empty string so that both char's will be type cast to strings
						}

						if (segmentLen >= 4 && s[i] == '(' && s[i + 1] == ')' && s[j - 1] == '(' && s[j] == ')')
						{
							if (i + 2 < len && j - 2 >= 0)
							{
								vector<string>const& r2 = t[i + 2][j - 2];
								if (r2.size() > 0)
								{
									for (string x : r2)
									{
										x.insert(0, 1, ')');
										x.insert(0, 1, '(');
										x += "()";
										v.push_back(x);
									}
								}
								else
								{
									// if the segment length is exactly 4, t[i+2][j-2] won't give me anything
									// or even if the segment length is greater than 4, t[i+2][j-2] can still return empty 
									v.push_back("()()");
								}
							}
						}
					}
					else // s[i] and s[j] don't match
					{
						vector<string>const& r1 = t[i][j - 1];
						vector<string>const& r2 = t[i+1][j];
						vector<string>const& r3 = t[i + 1][j - 1];

						unordered_set<string> set1;
						for (auto const& x : r1)
							set1.insert(x);
						for (auto const& x : r2)
							set1.insert(x);
						for (auto const& x : r3)
							set1.insert(x);
						for (auto& x : set1)
							v.push_back(x);
					}
				}
			}

			vector<string>answer = t[0][len-1];

			for (int i = 0; i < len; i++)
			{
				delete[] t[i];
			}
			delete[] t;

			return answer;
		}
	};


	class Solution2
	{
		/*
		* This solution still uses dynamic programming and uses a 2-dimension table
		* For a substring of s, for example, from s[i] to s[j] inclusively.
		* Make a cut right after s[i]. s[i] can be '(', ')', or letter.
		* The situation for s[i] being ')' or letter is easier.
		* When s[i] is '('. I need to delete a closing parenthesis from s[i+1] to s[j]
		*/

	public:

		vector<string> removeInvalidParentheses(string s)
		{
			size_t len = s.size();
			vector<string>** t = new vector<string>*[len];
			for (int i = 0; i < len; i++)
			{
				t[i] = new vector<string>[len];
				for (int j = 0; j < len; j++)
					t[i][j].push_back("");
			}

			for (int i = 0; i < len; i++)
			{
				if (s[i] != '(' && s[i] != ')')
				{
					t[i][i].clear();
					t[i][i].push_back(s.substr(i, 1));
				}
				//else
				//	t[i][i].push_back("");
			}

			// len = 10, indices: 0 -- 9
			// segmentLen = 3:  last segment: 7,8,9

			for (int segmentLen = 2; segmentLen <= len; segmentLen++)
			{
				for (int i = 0; i <= len - segmentLen; i++)  // i: start of the segment
				{
					int segEnd = i + segmentLen - 1;
					if (s[i] == ')')
					{
						t[i][segEnd] = t[i + 1][segEnd];
					}
					else if (s[i] >= 'a' && s[i] <= 'z')
					{
						vector<string>const& theRest = t[i + 1][segEnd];
						vector<string>& whole = t[i][segEnd];
						for (auto const& x : theRest)
							whole.push_back(s[i]+x);
					}
					else  // s[i] == '('
					{
						vector<string>& whole = t[i][segEnd];
						for (int j = i + 1; j <= segEnd; j++)
						{
							if (s[j] == ')')
							{
								vector<string> thisScheme;
								vector<string>firstPart;
								vector<string>const& middle = t[i + 1][j - 1];
								for (auto const& x : middle)
									firstPart.push_back('(' + x + ')');

								if (j < segEnd)
								{
									vector<string>const& secondPart = t[j + 1][segEnd];
									for (auto const& x1 : firstPart)
									{
										for (auto const& x2 : secondPart)
										{
											thisScheme.push_back(x1 + x2);
										}
									}
								}
								else // there is no second part any more, j has reached segment end
								{
									thisScheme = firstPart;
								}

								for (auto& x : thisScheme)
									whole.push_back(x);
							}
						}
					}
				}
			}

			vector<string>&allPoss = t[0][len - 1];  //all possibilities of removing some parentheses (not just minimum invalid parentheses)
			size_t maxLen = 0;
			for (auto& x : allPoss)
			{
				if (x.size() > maxLen)
					maxLen = x.size();
			}
			unordered_set<string> answerSet; // have to use this hash set to prune duplicates
			vector<string>answer;
			for (auto& x : allPoss)
			{
				if (x.size() == maxLen)
					answerSet.insert(x);
			}
			for (auto& x : answerSet)
				answer.push_back(x);

			for (int i = 0; i < len; i++)
				delete[] t[i];

			delete[] t;

			return answer;
		}
	};


	class Solution3
	{
		/*
		* Scan the string once first, figure out the number of open and close parentheses I need to keep and delete.
		* Backtrack solve this problem. At each index, if it's a letter, I keep it. If it's a parenthesis, I have two
		* options, i.e. delete it or keep it. Make the decision based on how many I have kept and deleted to that point.
		* ------------------------------------------------------------------------------------------------------------
		* The biggest flaw of this implementation is that when I'm looking at index 'x', I'm not considering
		* whether I've taken an open parenthesis prior to arriving at 'x'.
		* So an input like this:()()((()
		* would produce these answers:
		*         ))((()
        *         ()()()
        *         )()(()
        *         ())(()
		* They are only matching the numbe of open and close parentheses, 3 open, 3 close, but obviously it failed to
		* consider their positions.
		*/

	public:

		int openParenToBeDeleted{ 0 };
		int closeParenToBeDeleted{ 0 };
		int openParenToBeKept{ 0 };
		int closeParenToBeKept{ 0 };
		size_t len;

		void figureOutMinimumRemoval(string& s)
		{
			int openParen = 0;
			int closeParen = 0;
			for (char& c : s)
			{
				if (c == '(')
					openParen++;
				if (c == ')')
				{
					if (openParen > closeParen)
						closeParen++;
					else
						closeParenToBeDeleted++;
				}
			}
			openParenToBeDeleted = openParen - closeParen;
			openParenToBeKept = closeParenToBeKept = closeParen;
		}

		void backtrackSolve(string& s, int index, int openParenAlreadyDeleted, int closeParenAlreadyDeleted, int openParenCounted, int closeParenCounted,
			unordered_set<string>& answersSet, string& assemble)
		{
			if (index == len)
			{
				if (openParenAlreadyDeleted == openParenToBeDeleted && closeParenAlreadyDeleted == closeParenToBeDeleted &&
					openParenCounted == openParenToBeKept && closeParenCounted == closeParenToBeKept)
				{
					answersSet.insert(assemble);
				}
				return;
			}

			if (s[index] != '(' && s[index] != ')')
			{
				assemble.push_back(s[index]);
				backtrackSolve(s, index + 1, openParenAlreadyDeleted, closeParenAlreadyDeleted, openParenCounted, closeParenCounted, answersSet, assemble);
				assemble.pop_back();
				return;
			}

			if (s[index] == '(' && openParenCounted < openParenToBeKept)
			{
				if (openParenAlreadyDeleted < openParenToBeDeleted)
				{
					// two options, (1) delete this open parenthesis or (2) keep it

					//(1)
					backtrackSolve(s, index + 1, openParenAlreadyDeleted + 1, closeParenAlreadyDeleted, openParenCounted, closeParenCounted, answersSet, assemble);
					//(2)
					assemble.push_back('(');
					backtrackSolve(s, index + 1, openParenAlreadyDeleted, closeParenAlreadyDeleted, openParenCounted + 1, closeParenCounted, answersSet, assemble);
					assemble.pop_back();
					return;
				}
				else
				{
					// I have to keep this open parenthesis
					assemble.push_back('(');
					backtrackSolve(s, index + 1, openParenAlreadyDeleted, closeParenAlreadyDeleted, openParenCounted + 1, closeParenCounted, answersSet, assemble);
					assemble.pop_back();
					return;
				} // this if-else can be simplified, but I'm keeping it this way to show the logic.

				return;
			}
			
			if (s[index] == ')' && closeParenCounted < closeParenToBeKept)
			{
				if (closeParenAlreadyDeleted < closeParenToBeDeleted)
				{
					if (openParenCounted == closeParenCounted)
					{
						// I must delete this close parenthesis
						backtrackSolve(s, index + 1, openParenAlreadyDeleted, closeParenAlreadyDeleted + 1, openParenCounted, closeParenCounted, answersSet, assemble);
					}
					else
					{
						// I can keep or delete
						// delete
						backtrackSolve(s, index + 1, openParenAlreadyDeleted, closeParenAlreadyDeleted + 1, openParenCounted, closeParenCounted, answersSet, assemble);

						//keep
						assemble.push_back(')');
						backtrackSolve(s, index + 1, openParenAlreadyDeleted, closeParenAlreadyDeleted, openParenCounted, closeParenCounted + 1, answersSet, assemble);
						assemble.pop_back();
					}
				}
				else
				{
					// I must keep
					assemble.push_back(')');
					backtrackSolve(s, index + 1, openParenAlreadyDeleted, closeParenAlreadyDeleted, openParenCounted, closeParenCounted + 1, answersSet, assemble);
					assemble.pop_back();
				}
				return;
			}
		}

		vector<string> removeInvalidParentheses(string s)
		{
			len = s.size();
			openParenToBeDeleted = closeParenToBeDeleted = 0;
			figureOutMinimumRemoval(s);
			string assemble;
			string::size_type validLen = s.size() - openParenToBeDeleted - closeParenToBeDeleted;
			assemble.reserve(validLen);
			unordered_set<string> answersSet;  // the backtrack solution has a problem is that different depth-first-search paths may lead to the same answer.
			// Even though along the different paths I delete different open/close parentheses, the final strings may appear to be the same
			// So I must use a set to keep duplicate answers out.

			backtrackSolve(s, 0/*index*/, 0/*open paren already delted*/, 0/*close paren already deleted*/,
				0/*open paren counted*/, 0/*close paren counted*/, answersSet, assemble);

			vector<string>answer(answersSet.begin(), answersSet.end());
			return answer;
		}
	};


	class Solution4
	{
	public:

		size_t len;
		string::size_type validLen;
		int parenthesesToKeep{ 0 }; // total open parentheses to keep (it's also the total close parentheses to keep)

		void figureOutMinimumRemoval(string& s)
		{
			int openParen = 0;
			int closeParen = 0;
			validLen = 0;
			for (char& c : s)
			{
				if (c == '(')
					openParen++;
				else if (c == ')')
				{
					if (openParen > closeParen)
						closeParen++;
				}
				else
					validLen++; // letter
			}
			validLen += (closeParen * 2);
			parenthesesToKeep = openParen;
		}

		void backtrackSolve(string&s, size_t index, int unmatchedOpenParen, int totalKept, unordered_set<string>&answersSet, string& assemble)
		{
			if (totalKept == validLen && unmatchedOpenParen == 0)
			{
				answersSet.insert(assemble);
				return;
			}
			if (index == len || totalKept > validLen)
				return;
			if (len - index + totalKept < validLen) // if all the characters left won't even make up to valid length, give up.
				return;

			if (s[index] != '(' && s[index] != ')')
			{
				assemble.push_back(s[index]);
				backtrackSolve(s, index + 1, unmatchedOpenParen, totalKept+1, answersSet, assemble);
				assemble.pop_back();
				return;
			}
			else if (s[index] == '(')
			{
				assemble.push_back('(');
				backtrackSolve(s, index + 1, unmatchedOpenParen + 1, totalKept+1, answersSet, assemble);
				assemble.pop_back();
				
				backtrackSolve(s, index + 1, unmatchedOpenParen, totalKept, answersSet, assemble);
				return;
			}
			else // current character is ')'
			{
				if (unmatchedOpenParen > 0)
				{
					assemble.push_back(')');
					backtrackSolve(s, index + 1, unmatchedOpenParen-1, totalKept + 1, answersSet, assemble);
					assemble.pop_back();
				}
				backtrackSolve(s, index + 1, unmatchedOpenParen, totalKept, answersSet, assemble);	
				return;
			}
		}

		vector<string> removeInvalidParentheses(string s)
		{
			len = s.size();

			figureOutMinimumRemoval(s);

			string assemble; // one possible answer
			assemble.reserve(validLen);

			unordered_set<string> answersSet;  // the backtrack solution has a problem is that different depth-first-search paths may lead to the same answer.
			// Even though along the different paths I delete different open/close parentheses, the final strings may appear to be the same
			// So I must use a set to keep duplicate answers out.

			backtrackSolve(s, 0/*index*/, 0/*unmatched open parentheses*/, 0/*total characters kept so far*/, answersSet, assemble);

			vector<string>answer(answersSet.begin(), answersSet.end());
			return answer;
		}
	};



	// leetcode examples:

	//  ()())()

	//  (a)())()

	//  )(
	// 
	// other test cases:
	
	// ()())())
	
	// ())))))())


	void Test_0301_Remove_Invalid_Parentheses()
	{
		Solution4 so;
		while (true)
		{
			string s;
			cout << "string: ";
			getline(cin, s);
			if (s.size() == 0) break;
			vector<string>const& answer = so.removeInvalidParentheses(s);
			for (auto& s1 : answer)
				cout << s1 << '\n';

			cout << "==================\n";
		}
	}
}