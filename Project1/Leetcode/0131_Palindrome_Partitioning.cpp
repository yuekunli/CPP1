#include<vector>
#include<iostream>
#include<string>

namespace _0131_Palindrome_Partitioning {

	using namespace std;

	class Solution
	{
		size_t n;

		vector<vector<string>> solve(string& s, vector<pair<bool, vector<vector<string>>>>& r, vector<vector<bool>>& isPalin, size_t index)
		{
			if (index == n)
			{
				return {};
			}
			if (r[index].first)
				return r[index].second;

			vector<vector<string>>answerAtIndex;
			for (size_t i = index; i < n; i++)
			{
				if (i == index || (i - index == 1 && s[i] == s[index]) || (s[i] == s[index] && isPalin[index + 1][i - 1]))
				{
					isPalin[index][i] = true;
					string a = s.substr(index, i - index + 1);

					if (i == n - 1)
					{
						answerAtIndex.emplace_back(initializer_list<string>({ a }));
					}
					else
					{
						vector<vector<string>>const& answerForTheRest = solve(s, r, isPalin, i + 1);
						for (vector<string>const& b : answerForTheRest)
						{
							vector<string> c;
							c.push_back(a);
							copy(b.begin(), b.end(), back_inserter(c));
							answerAtIndex.emplace_back(c);
						}
					}
				}
			}
			r[index].first = true;
			r[index].second = answerAtIndex;
			return r[index].second;
		}

	public:
		vector<vector<string>> partition(string s)
		{
			n = s.size();
			
			vector<pair<bool, vector<vector<string>>>>records(n, { false, vector<vector<string>>{} });
			// if I look at index 5, what is the possible partitions for the subarray [5, end)
			// records[5] gives me this answer. But I keep a pair at records[5], the first value is a boolean
			// which tells me whether this subarray [5, end) has been solved.

			vector<vector<bool>>isPalin(n, vector<bool>(n, false));
			// a 2-D array, isPalin[x][[y] tells me whether the subarray [x, y] is a palindrome

			for (size_t i = 0; i < n; i++)
				isPalin[i][i] = true;
			
			return solve(s, records, isPalin, 0);
		}
	};

	/*
	* is there another way?
	* for example I know [0--4] can be partitioned in a few ways, when I examine the 5th element,
	* just compare it with
	* 
	* a, b, c, d, c
	* a, b, cdc
	* 
	* Assume 5th element is 'b'
	* 
	* a b c d c b
	*       | | |
	*         compare with preview string and the one prior to it in this partial answer
	* 
	* a  b  cdc  b
	*    |  \_/  |
	*     compare with "cdc" and the 'b' prior to it.
	*/ 

	class Solution2
	{
		size_t n;

		vector<vector<string>> solve(string& s, vector<pair<bool, vector<vector<string>>>>& r, vector<vector<bool>>& isPalin, size_t index)
		{
			if (index == n)
			{
				return {};
			}
			if (r[index].first)
				return r[index].second;

			vector<vector<string>>answerAtIndex;
			for (size_t i = index; i < n; i++)
			{
				if (i == index || (i - index == 1 && s[i] == s[index]) || (s[i] == s[index] && isPalin[index + 1][i - 1]))
				{
					isPalin[index][i] = true;
					string a = s.substr(index, i - index + 1);
					
					if (i == n - 1)
					{
						answerAtIndex.emplace_back(initializer_list<string>({ a }));
					}
					else
					{
						vector<vector<string>>const& answerForTheRest = solve(s, r, isPalin, i + 1);
						for (vector<string>const& b : answerForTheRest)
						{
							// this part is slightly different than the first solution, logically the same.
							answerAtIndex.push_back(vector<string>{});
							answerAtIndex.rbegin()->push_back(a);
							copy(b.begin(), b.end(), back_inserter(*answerAtIndex.rbegin()));
						}
					}
				}
			}
			r[index].first = true;
			r[index].second = answerAtIndex;
			return r[index].second;
		}

	public:
		vector<vector<string>> partition(string s)
		{
			n = s.size();
			vector<pair<bool, vector<vector<string>>>>records(n, { false, vector<vector<string>>{} });
			vector<vector<bool>>isPalin(n, vector<bool>(n, false));
			for (size_t i = 0; i < n; i++)
				isPalin[i][i] = true;

			return solve(s, records, isPalin, 0);
		}
	};

	class Solution3
	{
		/*
		* Logically the same as the first two solutions, just simplify the records multi-dimension array
		* I don't need a boolean that tells me whether a subarray is solved already, just need to check its size.
		* It's impossible that a subarray is solved but the possible partition is none. At least I can partition it
		* by separating every individual letter.
		*/
		size_t n;

		vector<vector<string>> solve(string& s, vector<vector<vector<string>>>& r, vector<vector<bool>>& isPalin, size_t index)
		{
			if (index == n)
			{
				return {};
			}
			if (r[index].size() > 0)
				return r[index];

			vector<vector<string>>answerAtIndex;
			for (size_t i = index; i < n; i++)
			{
				if (i == index || (i - index == 1 && s[i] == s[index]) || (s[i] == s[index] && isPalin[index + 1][i - 1]))
				{
					isPalin[index][i] = true;
					string a = s.substr(index, i - index + 1);
					
					if (i == n - 1)
					{
						answerAtIndex.emplace_back(initializer_list<string>({ a }));
					}
					else
					{
						vector<vector<string>>const& answerForTheRest = solve(s, r, isPalin, i + 1);
						for (vector<string>const& b : answerForTheRest)
						{
							answerAtIndex.push_back(vector<string>{});
							answerAtIndex.rbegin()->push_back(a);
							answerAtIndex.rbegin()->insert(answerAtIndex.rbegin()->end(), b.begin(), b.end());
						}
					}
				}
			}
			
			r[index] = answerAtIndex;
			return r[index];
		}

	public:
		vector<vector<string>> partition(string s)
		{
			n = s.size();
			vector<vector<vector<string>>>records(n, vector<vector<string>>{});
			vector<vector<bool>>isPalin(n, vector<bool>(n, false));
			for (size_t i = 0; i < n; i++)
				isPalin[i][i] = true;

			return solve(s, records, isPalin, 0);
		}
	};

	void Test_0131_Palindrome_Partitioning()
	{
		Solution3 s;
		string s1;
		while (true)
		{
			s1.clear();
			cout << "string: ";  // don't give spaces in input string
			getline(cin, s1);
			if (s1.size() == 0)break;
			vector<vector<string>>const& answer = s.partition(s1);
			for (auto& a : answer)
			{
				copy(a.begin(), a.end(), ostream_iterator<string>(std::cout, ", "));
				cout << "\n";
			}
			cout << "\n\n";
		}
	}
}