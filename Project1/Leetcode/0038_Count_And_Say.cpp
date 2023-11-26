#include <string>
#include <iostream>
#include <unordered_map>
#include <iomanip>
#pragma warning(disable: 4267 4244)

namespace _0038_Count_And_Say {

	using namespace std;



	/*
	* An observation: I will never have same 4 letters in a row.
	* Informal proof:
	* How to get 4 same letters in a row?
	* for example 33'33
	* then the previous string must have 333'333
	* but 333'333 will become 63 in the next string
	*/

	/*
	* If at some point the string repeats an early result,
	* the subsequent results are going to be repetitions too.
	* For example:
	* n = 7 result: .... (string a)
	* n = 8 result: .... (string b)
	* n = 9 result: .... (string c)
	* ...
	* n = 31 result .... (string b)  <- string b appears again.
	* 
	* Without further computation, I can say that n=32 result will be string c
	* and n=33 result will be same as n=10
	* 
	* But how do I make use such pattern? I have to record every results as I progress,
	* but I don't know whent he repetition is going to happen, it may not happen before
	* I reach the target 'n'.
	* 
	* However, in reality, the length of the result strings grows almost exponentially!
	*/


	class Solution
	{
	public:

		unordered_map<string, string>m; // I wanted to use the map, but actually this solution doesn't add records into the map


		string countAndSay(int n)
		{
			string last = "1";
			for (int i = 2; i <= n; i++)
			{
				string a = "";
				int j = 0;
				while (j < last.size())
				{
					string part = last.substr(j);
					if (m.find(part) != m.end())
					{
						string part_a = m[part];
						a = a + part_a;
						break;
					}
					else
					{
						char current = last[j];
						int cnt = 0;
						while (j < last.size() && last[j] == current)
						{
							cnt++;
							j++;
						}
						a = a + std::to_string(cnt) + std::to_string(current);
					}
				}
				last = a;
			}
			return last;
		}
	};



	class Solution2
	{
		/*
		* Use a map to keep some records
		* For a certain string, it may re-appear
		* for example, at some point, the string is:
		* 1 3 2 4 2 2 1 1 3 1 1 2
		*       |---------------|
		*       maybe this part of the string appeared earlier
		*       if I have a record as to what this part truns into
		*       I can save some time.
		*/


		/*
		* The recursion in this solution focus on individual string not the series of strings from n=1 to n=target
		*/


	public:

		unordered_map<string, string>m;
		int useCache = 0;   // this counts how many times the map actually helped

		string solve_bruteForce(string a)
		{
			int i = 0;
			int cnt = 0;
			char current = a[0];
			string answer;
			while (i < a.size())
			{
				current = a[i];
				cnt = 0;
				while (i < a.size() && a[i] == current)
				{
					i++;
					cnt++;
				}
				answer = answer + to_string(cnt) + current;
			}
			return answer;
		}


		/*
		* take the first segment, convert it to count+say
		* then recursively solve the rest.
		* I think this is a typical tail-recursion.
		*/
		string solve_Recur(string a)
		{
			if (m.find(a) != m.end())
				return m[a];
			else
			{
				int j = 0;
				int cnt = 0;
				char current = a[0];
				while (j < a.size() && a[j] == current)
				{
					cnt++;
					j++;
				}
				string answer;
				if (j < a.size())
					answer = to_string(cnt) + current + solve_Recur(a.substr(j));
				else
					answer = to_string(cnt) + current;
				m[a] = answer;
				return answer;
			}
		}

		/*
		* This function tries to be smarter (but may not gain much on performance)
		* If the input string is long, cut it to half, solve each one (this can be a
		* performance improvement if using multi-thread). If the input is short, just
		* use brute force. If it's between "long" and "short" solve the first segment
		* then recursively solve the rest.
		*/
		string solve_Recur2(string a)
		{
			if (a.size() > 10)
			{
				size_t i = a.size() / 2;
				while (i < a.size() - 1 && a[i] == a[i + 1])
					i++;

				// I don't need to worry about the second half
				// being all same character, and not knowing
				// where this long streak actually starts,
				// because if a is longer than 10 characters,
				// half of it is at least 5, and there won't be
				// 4 same characters in a streak.

				string a1 = a.substr(0, i + 1);
				string a2 = a.substr(i + 1);
				return solve_Recur2(a1) + solve_Recur2(a2);
			}

			if (m.find(a) != m.end())
			{
				useCache++;
				return m[a];
			}

			if (a.size() <= 4)
			{
				m[a] = solve_bruteForce(a);
				return m[a];
			}

			// a's size is between 5 and 10

			int j = 0;
			int cnt = 0;
			char current = a[0];
			while (j < a.size() && a[j] == current)
			{
				cnt++;
				j++;
			}
			string answer;

			// don't need to worry about entire "a" has the same character
			answer = to_string(cnt) + current + solve_Recur2(a.substr(j));

			m[a] = answer;
			return answer;
		}


		string countAndSay(int n)
		{
			m.clear();
			useCache = 0;
			string last = "1";
			for (int i = 2; i <= n; i++)
			{
				last = solve_Recur2(last);
			}
			return last;
		}
	};


	class Solution3
	{
		/*
		* This is another way to do recursion
		* When given previous string, the next string is solved by brute force
		* but I use recursive to get the previous string.
		*/
	public:
		string countAndSay(int n)
		{
			if (n == 1) return "1";

			string prev = countAndSay(n - 1);

			int j = 0;
			string a; // this is the answer
			char current = prev[0];
			int cnt = 0;
			while (j < prev.size())
			{
				if (prev[j] == current)
				{
					cnt++;
					j++;
					continue;	
				}
				a = a + std::to_string(cnt) + std::to_string(current);

				current = prev[j];
				cnt = 1;
				j++;	
			}
			a = a + std::to_string(cnt) + std::to_string(current);
			return a;
		}
	};


	class Solution4  // Simple iteration + brute force, no cache
	{
	public:
		string countAndSay(int n)
		{
			string last = "1";
			for (int i = 2; i <= n; i++)
			{
				int j = 0;
				string answer = "";
				while (j < last.size())
				{
					int cnt = 0;
					char current = last[j];
					while (j < last.size() && last[j] == current)
					{
						cnt++;
						j++;
					}
					answer = answer + to_string(cnt) + current;
				}
				last = answer;
				//cout << setw(2) << i << "   " << last << '\n';
			}
			return last;
		}
	};

	void Test_0038_Count_And_Say()
	{
		int n;
		int sID;
		Solution s1;
		Solution2 s2;
		Solution3 s3;
		Solution4 s4;

		while (true)
		{
			cout << "input n:  ";
			cin >> n;
			
			if (n == 0) break;

			cout << "solution: ";
			cin >> sID;
			
			switch (sID)
			{
			case 1:
				cout<< s1.countAndSay(n) << '\n';
				break;
			case 2:
				cout << s2.countAndSay(n) << '\n'
					<< "Use cache " << s2.useCache << " times" << '\n'
					<< "Cache size " << s2.m.size() << '\n';
				break;
			case 3:
				cout << s3.countAndSay(n) << '\n';
				break;
			case 4:
				cout << s4.countAndSay(n) << '\n';
				break;
			}
		}
	}
}