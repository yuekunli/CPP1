#include "LibHeaders.h"

namespace _0402_Remove_K_Digits {

	using namespace std;

	class Solution
	{
		/*
		* Find the smallest digit, use that as the first digit.
		* Then find the smallest in the rest, use that as the next digit.
		* If at some point, there is not enough left, just copy the rest.
		*/

		/*
		* This is a slow solution.
		* The worst case time cost is O(N*K)
		* This worst case happens when the input "num" consists of repeating same character, for instance, a lot of '1'
		* Basically I move 1 digit at a time, for every 'k' range, the smallest is the first one.
		*/
	public:
		string removeKdigits(string num, int k)
		{
			size_t n = num.size();
			if (num == "0" || n <= k)
				return "0";

			string answer(n - k, ' ');
			int start = 0;
			int answerEndIdx = 0;
			while (k > 0)
			{
				int mIndex = start;
				for (int i = 0; i <= k; i++)
				{
					int j = start + i;
					if (num[j] < num[mIndex])
						mIndex = j;
				}
				answer[answerEndIdx] = num[mIndex];
				answerEndIdx++;
				k = k - (mIndex - start);
				start = mIndex + 1;
				if (n - start <= k)
				{
					break;
				}
			}
			if (k == 0) // already deleted enough, just copy the rest
			{
				int i = start;
				while (i < n)
				{
					answer[answerEndIdx] = num[i];
					i++;
					answerEndIdx++;
				}
			}
			int i = 0;
			while (answer[i] == '0') // I may have put together an answer that starts with some zeros
				i++;

			if (i == answerEndIdx)
				return "0";
			
			return answer.substr(i, answerEndIdx - i);
		}
	};



	class Solution2
	{
	public:
		string removeKdigits(string num, int k)
		{
			size_t n = num.size();
			if (num == "0" || n <= k)
				return "0";

			stack<int>keptIndexes;
			keptIndexes.push(0);
			int i = 1;

			while (i < n)
			{	
				while (keptIndexes.size() > 0 && k > 0)
				{
					int j = keptIndexes.top();
					if (num[j] > num[i])
					{
						keptIndexes.pop();
						k--;
					}
					else
						break;
				}
				if (k == 0)
				{
					string s(keptIndexes.size(), ' ');
					int p = (int)(keptIndexes.size() - 1);
					while (p >= 0)
					{
						int t = keptIndexes.top();  // std::stack has no iterator
						s[p] = num[t];
						keptIndexes.pop();
						p--;
					}
					s.append(num.substr(i));
					p = 0;
					while (p < s.size() && s[p] == '0')
						p++;
					if (p == s.size())
						return "0";
					else if (p > 0)
						return s.substr(p);

					return s;
				}
				keptIndexes.push(i);
				i++;
			}
			while (k > 0)
			{
				// if "num" ends with a long increasing streak, I would have pushed all the tails onto the stack without deleting them
				// so I haven't deleted enough elements.
				keptIndexes.pop();
				k--;
			}
			string s(keptIndexes.size(), ' ');
			int p = keptIndexes.size() - 1;
			while (p >= 0)
			{
				int t = keptIndexes.top();
				s[p] = num[t];
				keptIndexes.pop();
				p--;
			}
			p = 0;
			while (p < s.size() && s[p] == '0')
				p++;
			if (p == s.size())
				return "0";
			else if (p > 0)
				return s.substr(p);

			return s;
		}
	};



	class Solution3
	{
		/*
		* Same logic as Solution2, just clean up and tidy up.
		* I don't really need to use std::stack to have the effect of using a stack
		* I can just use any sequential container to simulate a stack.
		*/
	public:
		string removeKdigits(string num, int k)
		{
			size_t n = num.size();
			if (num == "0" || n <= k)
				return "0";

			string s(n, ' ');
			int answerIndex = 0;
			s[0] = num[0];
			int i = 1;

			while (i < n)
			{
				while (answerIndex >= 0 && k > 0)
				{
					if (s[answerIndex] > num[i])
					{
						answerIndex--;
						k--;
					}
					else
						break;
				}
				if (k == 0)
				{
					copy(num.begin() + i, num.end(), s.begin() + (answerIndex+1)); // must put answerIndex+1 in parentheses,
																					// because answerIndex can be '-1'
																					// in that case, "s.begin() - 1 " throws exception

					int answerEndIndex = answerIndex + (n - i) + 1; // answerEndIndex is 1 past the last valid character in 's'
					int p = 0;
					while (p < answerEndIndex && s[p] == '0')
						p++;

					return p == answerEndIndex ? "0" : s.substr(p, answerEndIndex - p);
				}
				answerIndex++;
				s[answerIndex] = num[i];
				i++;
			}
			
			// if by this point, k is still greater than 0, that means I haven't deleted enough.
			// and the numbers in "s" should be in ascending order, I just need to delete the
			// last a few digits. And I do this by just moving "answerIndex" to the left.
			answerIndex -= k;
			int answerEndIndex = answerIndex + 1;
			int p = 0;
			while (p < answerEndIndex && s[p] == '0')
				p++;

			return p == answerEndIndex ? "0" : s.substr(p, answerEndIndex - p);
		}
	};


	class Solution4
	{
		/*
		* even more clean up.
		*/
	public:
		string removeKdigits(string num, int k)
		{
			size_t n = num.size();
			if (num == "0" || n <= k)
				return "0";

			string s(n, ' ');
			int answerIndex = 0;
			s[0] = num[0];
			int i = 1;

			while (i < n)
			{
				while (answerIndex >= 0 && k > 0)
				{
					if (s[answerIndex] > num[i])
					{
						answerIndex--;
						k--;
					}
					else
						break;
				}
				if (k == 0)
				{
					copy(num.begin() + i, num.end(), s.begin() + (answerIndex + 1)); // must put answerIndex+1 in parentheses,
																					// because answerIndex can be '-1'
																					// in that case, "s.begin() - 1 " throws exception
					break;
				}
				answerIndex++;
				s[answerIndex] = num[i];
				i++;
			}

			/*
			* I get out of the loop by either k == 0 or n == i.
			* if k == 0, "answerIndex -= k" is no-op, but the next statement is useful.
			* if n == i, "answerIndex -= k" is useful, but the (n-i) part in the next statement has no effect.
			*/
			answerIndex -= k; // if k==0, this is no-op
			int answerEndIndex = answerIndex + (n - i) + 1;  // if I got out of the loop because i==n, then the (n-i) term doesn't matter
			int p = 0;
			while (p < answerEndIndex && s[p] == '0')
				p++;

			return p == answerEndIndex ? "0" : s.substr(p, answerEndIndex - p);
		}
	};

	void Test_0402_Remove_K_Digits()
	{
		Solution3 solu;
		string s;
		int k;
		while (true)
		{
			s.clear();
			cout << "nums: ";
			getline(cin, s);
			if (s.size() == 0) break;
			cout << "k: ";
			cin >> k;
			cin.ignore();
			cout << solu.removeKdigits(s, k) << "\n\n";
		}
	}
}