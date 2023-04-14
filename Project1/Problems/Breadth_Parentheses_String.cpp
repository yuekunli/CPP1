#include "LibHeaders.h"

namespace Breadth_Parentheses_String {

	using namespace std;

	/*
	* 1. Test if a string containing parentheses and other characters is balanced with its parentheses
	* 
    * 2. Return the "depth" of a balanced parentheses string, meaning the maximum number of nested parentheses
	* 
	* 3. Return the "breadth" of a balanced parentheses string, meaning return the max number of 
	* parentheses together along the same depth. This part is confusing and took about 20 minutes 
	* to explain in their roundabout way, but basically with the following input, "( () () () ) ()", 
	* your function should return 3 due to the number of closed parentheses at a depth level of 1
	*/

	class Solution
	{
		struct Stat
		{
			int max;
			int count;
		};
	public:

		int depthParentheses(string s)
		{
			int openParen = 0;
			int maxDepth = 0;
			for (int i = 0; i < s.size(); i++)
			{
				if (s[i] == '(')
				{
					openParen++;
					if (openParen > maxDepth)
						maxDepth = openParen;
				}
				else
					openParen--;
			}
			return maxDepth;
		}

		int breadthParentheses(string s)
		{
			unordered_map<int, Stat>m;

			int openParen = 0;
			int maxBreadth = 0;

			for (int i = 0; i < s.size(); i++)
			{
				if (s[i] == '(')
				{
					openParen++;
					int depth = openParen - 1;
					if (m.count(depth) == 0)
					{
						m[depth] = { 0,0 };
					}
				}
				else
				{
					int depth = openParen - 1;
					m[depth].count++;
					if (m[depth].count > m[depth].max)
					{
						m[depth].max = m[depth].count;
						if (m[depth].max > maxBreadth)
							maxBreadth = m[depth].max;
					}
					
					if (m.count(depth + 1) > 0)
						m[depth + 1].count = 0;
					
					openParen--;
				}
			}

			/*
			for (auto& a : m)
			{
				if (maxBreadth < a.second.max)
					maxBreadth = a.second.max;
			}
			*/
			return maxBreadth;
		}
	};

	void Test_Breadth_Parentheses_String()
	{
		Solution solu;
		string s;
		while (true)
		{
			s.clear();
			cout << "input: ";
			getline(cin, s);
			if (s.size() == 0) break;
			//cout << solu.breadthParentheses(s) << "\n\n";
			cout << solu.depthParentheses(s) << "\n\n";
		}
	}
}