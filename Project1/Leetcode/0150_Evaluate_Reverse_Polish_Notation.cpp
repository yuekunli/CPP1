#include<iostream>
#include<string>
#include<sstream>
#include<vector>

namespace _0150_Evaluate_Reverse_Polish_Notation {

	using namespace std;

	class Solution
	{
	public:
		int evalRPN(vector<string>& tokens)
		{
			size_t n = tokens.size();
			vector<int>stack(n);
			int stackEnd = -1; // when I use stackEnd as index in stack, I'll be converting int to size_t which is OK, there is no data loss.
			for (size_t i = 0; i < n; i++)
			{
				if (tokens[i] == "+")
				{
					int a1 = stack[stackEnd];
					stackEnd--;
					int a2 = stack[stackEnd];
					stack[stackEnd] = a1 + a2;
				}
				else if (tokens[i] == "-")
				{
					int a1 = stack[stackEnd];
					stackEnd--;
					int a2 = stack[stackEnd];
					stack[stackEnd] = a2 - a1;
				}
				else if (tokens[i] == "*")
				{
					int a1 = stack[stackEnd];
					stackEnd--;
					int a2 = stack[stackEnd];
					stack[stackEnd] = a1 * a2;
				}
				else if (tokens[i] == "/")
				{
					int a1 = stack[stackEnd];
					stackEnd--;
					int a2 = stack[stackEnd];
					stack[stackEnd] = a2 / a1;
				}
				else
				{
					stackEnd++;
					stack[stackEnd] = stoi(tokens[i]);
				}
			}
			return stack[stackEnd];
		}
	};

	void Test_0150_Evaluate_Reverse_Polish_Notation()
	{
		Solution solu;
		vector<string>tokens;
		string s;
		while (true)
		{
			s.clear();
			cout << "tokens: ";
			getline(cin, s);
			if (s.size() == 0) break;
			stringstream ss(s);
			tokens.clear();
			copy(istream_iterator<string>(ss), istream_iterator<string>(), back_inserter(tokens));
			cout << solu.evalRPN(tokens) << "\n\n";
		}
	}
}