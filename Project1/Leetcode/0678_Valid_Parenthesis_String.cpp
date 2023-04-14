#include<iostream>
#include<string>
#include<iomanip>
#include<vector>

namespace _0678_Valid_Parenthesis_String {

	using namespace std;

	class Solution
	{
		/*
		* This is accepted but not very fast
		*/

		size_t n;
		bool backtrackSolve(string& s, vector<vector<int>>& r, size_t index, int accumulatedOpenParen)
		{
			if (index == n && accumulatedOpenParen == 0)
				return true;
			else if (index == n)
				return false;

			if (r[index][accumulatedOpenParen] != -1)
				return r[index][accumulatedOpenParen];

			if (s[index] == '(')
				return r[index][accumulatedOpenParen] = backtrackSolve(s, r, index + 1, accumulatedOpenParen + 1);

			if (s[index] == ')')
			{
				if (accumulatedOpenParen == 0)
					return r[index][accumulatedOpenParen] = false;
				else
					return r[index][accumulatedOpenParen] = backtrackSolve(s, r, index + 1, accumulatedOpenParen - 1);
			}
			if (s[index] == '*')
			{
				if (accumulatedOpenParen > 0)
				{
					if (backtrackSolve(s, r, index + 1, accumulatedOpenParen))
						return r[index][accumulatedOpenParen] = true;
					else if (backtrackSolve(s, r, index + 1, accumulatedOpenParen + 1))
						return r[index][accumulatedOpenParen] = true;
					else if (accumulatedOpenParen >= 1 && backtrackSolve(s, r, index + 1, accumulatedOpenParen - 1))
						return r[index][accumulatedOpenParen] = true;
				}
				else // accumulatedOpenParen == 0
				{
					if (backtrackSolve(s, r, index + 1, accumulatedOpenParen))
						return r[index][accumulatedOpenParen] = true;
					else if (backtrackSolve(s, r, index + 1, accumulatedOpenParen + 1))
						return r[index][accumulatedOpenParen] = true;
				}
			}

			return r[index][accumulatedOpenParen] = false;
		}
	public:
		bool checkValidString(string s)
		{
			n = s.size();

			vector<vector<int>>records(n, vector<int>(n, -1));

			return backtrackSolve(s, records, 0, 0);
		}
	};


	class Solution2
	{
		/*
		* This solution doesn't work for this case:
		* ( * * * * ( ( ( *
		* If I just count the opens and stars, I can say there are 4 opens and 5 stars, I just need to
		* turn 4 of the 5 stars into closing parentheses then the string is valid. But obviously the
		* relative position of stars and open parentheses matter, the first few stars can help offset
		* the last few open parentheses.
		*/
	public:
		bool checkValidString(string s)
		{
			int openForLaterConsideration = 0;
			int visibleOpen = 0;
			int closeForLaterConsideration = 0;
			int star = 0;
			for (size_t i = 0; i < s.size(); i++)
			{
				if (s[i] == '(')
				{
					visibleOpen++;
				}
				else if (s[i] == '*')
				{
					star++;
					openForLaterConsideration += visibleOpen;
					visibleOpen = 0;
				}
				else
				{
					if (visibleOpen > 0)
					{
						visibleOpen--;
					}
					else
					{
						closeForLaterConsideration++;
						if (closeForLaterConsideration > openForLaterConsideration + star)
							return false;
					}
				}
			}
			if (visibleOpen > 0)
				return false;

			if (abs(openForLaterConsideration - closeForLaterConsideration) <= star)
				return true;
			
			return false;
		}
	};

	class Solution3
	{
		/*
		* accepted and fast
		*/
	public:
		bool checkValidString(string s)
		{
			int maxOpen = 0; // maximum possible open parentheses that are un-paired
			int minOpen = 0; // minimum possible open parentheses that are un-paired

			for (size_t i = 0; i < s.size(); i++)
			{
				if (s[i] == '(')
				{
					maxOpen++;
					minOpen++;
				}
				else if (s[i] == '*')
				{
					maxOpen++;
					if (minOpen > 0)
						minOpen--;
				}
				else
				{
					maxOpen--;
					if (maxOpen < 0)
						return false;
					if (minOpen > 0)
						minOpen--;
				}
			}
			if (minOpen > 0)
				return false;

			return true;
		}


		/*
		* Say maxOpen is 15 and minOpen is 7, if I need 11 open parentheses, can it really be 7?
		* Because if I treat a '*' as an open, the maxOpen can increment by 1, and if I treat it as
		* a close, the minOpen can decrement by 1, which means the overall difference is 2.
		* So if maxOpen is 15 and minOpen is 7, does it mean 7, 8, 9, 10, 11, 12, 13, 14, 15 are all
		* possible values? --- I don't need to care about any of these!
		* As long as I'm not at the end of the string, as long as maxOpen doesn't drop below 0, that
		* means there is a way to maintain the potential of it being valid, I can just proceed.
		* If I am at the end of the string, as long as minOpen is 0, that means there is a way to
		* make minOpen to be 0, which further means there is a way to match up all parentheses and stars.
		* Then that is it, that is all I need to know.
		*/
	};


	void Test_0678_Valid_Parenthesis_String()
	{
		Solution3 solu;

		string s;

		while (true)
		{
			s.clear();
			cout << "input: ";
			getline(cin, s);
			if (s.size() == 0) break;
			cout << boolalpha << solu.checkValidString(s) << "\n\n";
		}
	}
}