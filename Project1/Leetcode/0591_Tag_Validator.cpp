#include "LibHeaders.h"

namespace _0591_Tag_Validator {

	using namespace std;

	class Solution
	{
	public:

		/*
		* This solution doesn't work, this trys to find '<' (the beginning of an open bracket) and
		* '>' (the end of a close bracket) and then extract two tags and turn the segment in between them
		* into a sub-problem. There can be excessive close brackets, only open brackets can be used to determine validity
		*/

		bool isValidCdata(string& code, int a, int b)
		{
			// minimum cdata: <![CDATA[]]>   length is 12

			if (b - a + 1 < 12) return false;

			if (code.substr(a, 9) == "<![CDATA[")
				if (code.substr(b - 2, 3) == "]]>")
					return true;

			return false;
		}

		pair<int, int> findBrackets(string& code, int a, int b)
		{
			int i;
			for (i = a; i <= b; i++)
				if (code[i] == '<') break;
			int j;
			for (j = b; j >= a; j--)
				if (code[j] == '>') break;

			if (i >= j)
				return { -1, -1 };

			return { i, j };
		}

		pair<string, int> findStartTag(string& code, int a, int b)
		{
			int i;
			for (i = a + 1; i <= b; i++)
			{
				if (code[i] == '>')
					break;
				if (code[i] < 'A' || code[i] > 'Z')
					return { string(), -1 };
			}

			// <ABC>text ...
			// |   |
			// 5   9


			if (i == b) return { string(), -1 };
			if (i == a + 1) return { string(), -1 };
			if (i - a - 1 > 9) return { string(), -1 };  // tag names have length in range [1,9]

			return { code.substr(a + 1, i - a - 1), i + 1 };
		}

		pair<string, int> findEndTag(string& code, int a, int b)
		{
			int j;
			for (j = b - 1; j >= a; j--)
			{
				if (code[j] == '<' && code[j + 1] == '/') break;

				if ((code[j] < 'A' || code[j] > 'Z') && code[j] != '/')
					return { string(), -1 };

			}
			//  ...text</ABC>
			//         |    |
			//         12   17

			if (j == a || j >= b - 2 || b - j - 2 > 9)
				return { string(), -1 };

			return { code.substr(j + 2, b - j - 2), j - 1 };
		}

		bool isValidWithinBrackets(string& code, int a, int b)
		{
			if (a == 0 && b == code.size() - 1)
			{
				// if a and b are the out most pair of brackets, I'm testing the entire string
				// if the entire string is a cdata, it's not valid
			}
			else
			{
				if (isValidCdata(code, a, b))
					return true;
			}

			auto [startTag, afterStartTag] = findStartTag(code,a,b);
			auto [endTag, beforeEndTag] = findEndTag(code, a, b);

			if (afterStartTag == -1 || beforeEndTag == -1) return false;

			if (startTag == endTag)
			{
				auto [i, j] = findBrackets(code, afterStartTag, beforeEndTag);
				return isValidWithinBrackets(code, i, j);
			}
			else
				return false;
		}

		bool isValid(string code)
		{
			int len = code.size(); // minimum len: <A></A>
			if (len < 7) return false;
			if (code[0] != '<' || code[len - 1] != '>') return false;

			return isValidWithinBrackets(code, 0, len - 1);
		}
	};


	class Solution2
	{
	public:

		int findOpenBracket(string& code, int a)
		{
			auto it = find(code.begin() + a, code.end(), '<');
			if (it != code.end())
				return distance(code.begin(), it);

			return -1;
		}

		pair<string, bool> findTag(string& code, int a, int& b)
		{
			size_t len = code.size();

			bool openOrClose;  // true: open; false: close

			if (code[a + 1] == '/')
				openOrClose = false;
			else if (code[a + 1] >= 'A' && code[a + 1] <= 'Z')
				openOrClose = true;
			else
				return {string(), false};

			bool valid = true;

			for (size_t i = a + 2; i < len; i++)
			{
				if (code[i] == '>')
				{
					b = i;
					break;
				}
				if (code[i] < 'A' || code[i] > 'Z')
				{
					valid = false;
					break;
				}
			}
			if (!valid)
				return { string(), false };

			return { openOrClose ? code.substr(a + 1, b - a - 1) : code.substr(a + 2,b - a - 2), openOrClose };
		}

		bool isValidCdata(string& code, int a, int& b)
		{
			int len = code.size();
			string startPattern = "<![CDATA[";
			string endPattern = "]]>";

			auto it1 = search(code.begin() + a, code.end(), boyer_moore_searcher(startPattern.begin(), startPattern.end()));
			if (it1 != code.begin() + a) return false;
			
			if (it1 + startPattern.size() == code.end()) return false;
			
			auto it2 = search(it1+ startPattern.size(), code.end(), boyer_moore_searcher(endPattern.begin(), endPattern.end()));

			if (it2 == code.end()) return false;
			
			b = distance(code.begin(), it2 + endPattern.size() - 1);

			return true;
		}

		bool isValid(string code)
		{
			size_t len = code.size(); // minimum len: <A></A>
			if (len < 7) return false;
			if (code[0] != '<' || code[len - 1] != '>') return false;  // this makes sure initializing openBracketIndex to 0 is appropriate

			stack<string> tags;

			int openBracketIndex = 0;  // I'm not just initializing openBracketIndex to 0, it really is 0, because if code[0] is not '<', it would've returned false
			int closeBracketIndex;

			while (true)
			{
				if (openBracketIndex > 0 && openBracketIndex + 1 < len && code[openBracketIndex + 1] == '!')
				{
					if (isValidCdata(code, openBracketIndex, closeBracketIndex))
					{
						if (closeBracketIndex == len - 1)
							return false;  // if I just found a valid cdata block but string ends, that is an invalid string
					}
					else
						return false;
				}
				else
				{
					auto [tag, openOrClose] = findTag(code, openBracketIndex, closeBracketIndex); // very first iteration comes here, closeBracketIndex will be set by this function
					if (tag.size() > 0 && tag.size() <= 9)
					{
						if (openOrClose)
						{
							tags.push(tag);
						}
						else
						{
							if (tags.size() == 0)
								return false;
							string previousOpen = tags.top();
							if (previousOpen != tag)
								return false;
							tags.pop();
						}
					}
					else
					{
						return false;
					}
				}

				if (closeBracketIndex == len - 1)
					return tags.size() == 0 ? true : false;

				// the entire input needs to be enclosed by a pair of matching tags.
				// <A></A><B>abcde</B> is invalid
				if (tags.size() == 0 && closeBracketIndex != len - 1)
					return false;

				openBracketIndex = findOpenBracket(code, closeBracketIndex + 1);

				if (openBracketIndex == -1 || openBracketIndex == len - 1) return false;
			}
		}
	};



	void Test_0591_Tag_Validator()
	{
		Solution2 s;

		string input;

		while (true)
		{
			input.clear();

			cout << "give input: ";
			getline(cin, input);

			if (input.size() == 0) break;

			cout << s.isValid(input) << '\n';
		}
	}
}
