#include<vector>
#include<string>
#include<unordered_set>
#include<list>
#include<iostream>

namespace _0140_Word_Break_II {

	using namespace std;

	class Solution
	{
		/*
		* For example, at index 5, there are 3 ways to form the a valid word, each of these 3 ways also has 3 ways to further form valid strings.
		* Do I really want to record 3*3=9 lists at index 5? If I do that, when next time I arrive at index 5, I can get all 9 different formations quickly
		* but I still need to examine all 9 formations to assemble the answers. If I only record 3 valids cuts at index 5 for the next word, I can go through
		* 9 formations with a few recursive calls, the time spent is still in proportion with 9, but this saves a lot of memory.
		*/

		list<list<size_t>> wordBreakRecur(string& s, unordered_set<string>& dict, vector<list<list<size_t>>>& r, list<size_t>& breaks, vector<string>& answers, size_t i)
		{
			if (i == s.size() && !breaks.empty())
			{
				string oneAnswer = "";
				size_t previous_segment_end = -1;
				for (size_t& a : breaks)
				{
					
					oneAnswer += s.substr(previous_segment_end + 1, a - previous_segment_end);
					oneAnswer += " ";
					previous_segment_end = a;
				}
				answers.emplace_back(oneAnswer);
				return list<list<size_t>>();
			}

			if (!r[i].empty())
			{
				list<list<size_t>>& answersForTheSubseq = r[i];
				for (list<size_t>& oneAnswerForTheSubseq : answersForTheSubseq)
				{
					list<size_t>fullAnswer(breaks);
					for (size_t& i : oneAnswerForTheSubseq)
						fullAnswer.emplace_back(i);

					string oneAnswer = "";
					size_t previous_segment_end = -1;
					for (size_t& i : fullAnswer)
					{
						oneAnswer += s.substr(previous_segment_end + 1, i - previous_segment_end);
						oneAnswer += " ";
						previous_segment_end = i;
					}
					answers.emplace_back(oneAnswer);

				}
				return r[i];
			}

			for (size_t j = i + 1; j < s.size(); j++)
			{
				string segment = s.substr(i, j - i + 1);
				if (dict.contains(segment))
				{
					breaks.push_back(j);
					list<list<size_t>> breaksForTheSubsequent = wordBreakRecur(s, dict, r, breaks, answers, j + 1);
					if (j == s.size() - 1)
					{
						r[i].emplace_back(list<size_t>{j});
					}
					else
					{
						for (list<size_t>& oneAnswerForTheSubsequent : breaksForTheSubsequent)
						{
							oneAnswerForTheSubsequent.emplace_front(j);
							r[i].push_back(oneAnswerForTheSubsequent);
						}
					}
					breaks.pop_back();
				}
			}
		}

	public:
		vector<string> wordBreak(string s, vector<string>& wordDict)
		{
			vector<string>answers;
			unordered_set<string>dict(wordDict.begin(), wordDict.end());
			vector<list<list<size_t>>> r = vector<list<list<size_t>>>(s.size(), list<list<size_t>>());
			list<size_t> breaks;
			wordBreakRecur(s, dict, r, breaks, answers, 0);
		}
	};


	class Solution2 // accepted
	{
		void wordBreakRecur(string& s, unordered_set<string>& dict, vector<list<size_t>>& r, list<size_t>& breaks, vector<string>& answers, size_t i)
		{
			if (i == s.size() && !breaks.empty())
			{
				string oneAnswer = "";
				size_t previous_segment_end = -1;
				for (size_t& a : breaks)
				{
					oneAnswer += s.substr(previous_segment_end + 1, a - previous_segment_end);
					oneAnswer += " ";
					previous_segment_end = a;
				}
				answers.emplace_back(oneAnswer.substr(0, oneAnswer.size()-1)); // I added one extra space in the for loop.
				return;
			}

			if (!r[i].empty() && r[i].front() != -1)
			{
				list<size_t>& answersForNextValidWord = r[i];
				for (size_t& oneAnswerForNextValidWord : answersForNextValidWord)
				{
					breaks.push_back(oneAnswerForNextValidWord);
					wordBreakRecur(s, dict, r, breaks, answers, oneAnswerForNextValidWord + 1);
					breaks.pop_back();
				}
				return;
			}

			// if r[i] is empty how do I know whether it's because I haven't explored it or I have done it but it doesn't have a valid next word

			if (!r[i].empty() && r[i].front() == -1)
				return;

			list<size_t> validIndicesForNextWord;
			bool foundAtLeastOne = false;
			for (size_t j = i; j < s.size(); j++)
			{
				string segment = s.substr(i, j - i + 1);
				if (dict.contains(segment))
				{
					foundAtLeastOne = true;
					validIndicesForNextWord.push_back(j);
					breaks.push_back(j);
					wordBreakRecur(s, dict, r, breaks, answers, j + 1);
					breaks.pop_back();
				}
				
			}
			if (foundAtLeastOne)
				r[i] = validIndicesForNextWord;
			else
				r[i].push_back(-1); // poison this index in the record book, indicating there is no valid word starting at this index.
		}

	public:
		vector<string> wordBreak(string s, vector<string>& wordDict)
		{
			vector<string>answers;
			unordered_set<string>dict(wordDict.begin(), wordDict.end());
			vector<list<size_t>> r = vector<list<size_t>>(s.size(), list<size_t>());
			list<size_t> breaks;
			wordBreakRecur(s, dict, r, breaks, answers, 0);
			return answers;
		}
	};


	void Test_0140_Word_Break_II()
	{
		Solution2 solu;
		string s1 = "catsanddog";
		vector<string> wordDict1{ "cat", "cats", "and", "sand", "dog" };
		vector<string>const & answers1 = solu.wordBreak(s1, wordDict1);
		for (string const & a : answers1)
			cout << a << endl;
		cout << "\n\n\n";

		string s2 = "pineapplepenapple";
		vector<string> wordDict2{ "apple", "pen", "applepen", "pine", "pineapple" };
		vector<string>const& answers2 = solu.wordBreak(s2, wordDict2);
		for (string const& a : answers2)
			cout << a << endl;
		cout << "\n\n\n";

		string s3 = "catsandog";
		vector<string>wordDict3{ "cats", "dog", "sand", "and", "cat" };
		vector<string>const& answers3 = solu.wordBreak(s3, wordDict3);
		for (string const& a : answers3)
			cout << a << endl;
		cout << "\n";
	}
}