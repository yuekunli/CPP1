#include<vector>
#include<string>
#include<numeric>
#include<forward_list>
#include<iostream>

using namespace std;

namespace _0127_Word_Ladder {

	class Solution
	{
		vector<forward_list<size_t>> m;
		size_t beginWordIdx = -1;
		size_t endWordIdx = -1;

		int diffLetterCount(string& s1, string& s2)
		{
			return inner_product(s1.begin(), s1.end(), s2.begin(), 0, plus<int>{},
				[](auto& a, auto& b)
				{
					return a == b ? 0 : 1;
				});
		}

		void buildAdjacency(string& beginWord, string& endWord, vector<string>& wordList)
		{
			m = vector<forward_list<size_t>>(wordList.size());

			for (size_t i = 0; i < wordList.size(); i++)
			{
				if (beginWordIdx == -1 && (wordList[i] == beginWord))
					beginWordIdx = i;
				if (endWordIdx == -1 && (wordList[i] == endWord))
					endWordIdx = i;

				for (size_t j = i + 1; j < wordList.size(); j++)
				{
					if (diffLetterCount(wordList[i], wordList[j]) == 1)
					{
						m[i].push_front(j);
						m[j].push_front(i);
					}
				}
			}
		}
	public:
		int ladderLength(string beginWord, string endWord, vector<string>& wordList)
		{
			buildAdjacency(beginWord, endWord, wordList);

			vector<bool> seen = vector<bool>(wordList.size(), false);
			forward_list<size_t> v[2];
			unsigned char qswitch = 0;
			if (beginWordIdx != -1)
			{
				for (size_t& i : m[beginWordIdx])
				{
					v[0].push_front(i);
					seen[i] = true;
				}
			}
			else
			{
				for (size_t i = 0; i < wordList.size(); i++)
				{
					if (diffLetterCount(wordList[i], beginWord) == 1)
					{
						v[0].push_front(i);
						seen[i] = true;
					}
				}
			}
			forward_list<size_t>* q = &v[0];
			int ladderLength = 1;
			while (!q->empty())
			{
				ladderLength++;
				qswitch = (qswitch + 1) % 2;
				forward_list<size_t>* nextQ = &v[qswitch];
				for (auto& i : *q)
				{
					if (i == endWordIdx)
					{
						return ladderLength;
					}

					for (auto& adjIdx : m[i])
					{
						if (!seen[adjIdx])
						{
							nextQ->push_front(adjIdx);
							seen[adjIdx] = true;
						}
					}
				}
				q->clear();
				q = nextQ;
				nextQ = nullptr;
			}
			return 0;
		}
	};



	class Solution2
	{
		/*
		* Try not building an adjacency list at the beginning.
		*/

		int diffLetterCount(string& s1, string& s2)
		{
			return inner_product(s1.begin(), s1.end(), s2.begin(), 0, plus<int>{},
				[](auto& a, auto& b)
				{
					return a == b ? 0 : 1;
				});
		}

	public:
		int ladderLength(string beginWord, string endWord, vector<string>& wordList)
		{

			vector<bool> seen = vector<bool>(wordList.size(), false);
			forward_list<size_t> v[2];
			unsigned char qswitch = 0;
			for (size_t i = 0; i < wordList.size(); i++)
			{
				if (diffLetterCount(wordList[i], beginWord) == 1)
				{
					v[0].push_front(i);
					seen[i] = true;
				}
			}
			forward_list<size_t>* q = &v[0];
			int ladderLength = 1;
			while (!q->empty())
			{
				ladderLength++;
				qswitch = (qswitch + 1) % 2;
				forward_list<size_t>* nextQ = &v[qswitch];
				for (auto& i : *q)
				{
					if (endWord == wordList[i])
					{
						return ladderLength;
					}

					for (size_t a = 0; a < wordList.size(); a++)
					{
						if ((!seen[a]) && diffLetterCount(wordList[i], wordList[a]) == 1)
						{
							nextQ->push_front(a);
							seen[a] = true;
						}
					}
				}
				q->clear();
				q = nextQ;
				nextQ = nullptr;
			}
			return 0;
		}
	};


	void Test_0127_Word_Ladder()
	{
		Solution solu;
		vector<string> wordList = vector<string>{ "hot", "dot", "dog", "lot", "log", "cog"};
		cout << solu.ladderLength("hit", "cog", wordList) << endl;
	}
}