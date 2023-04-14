#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<map>
#include<functional>

namespace _0336_Palindrome_Pairs {
	
	using namespace std;

	class Solution 
	{
		/*
		* This solution is a brute force solution.
		* It examines one word at a time, find all the possible strings that can form a palindrome
		* with this word in such fashion: candidate + word. Then check if such a candidate is in
		* the given word list.
		* 
		* word: abcdefg
		* 
		* if "a" is a palindrome, then "gfedcb" is a candidate
		* if "ab" is a palindrome, then "gfedc" is a candidate
		* if "abc" is a palindrome, then "gfed" is a candidate
		* ...
		* if "abcdefg" is a palindrome, then empty string is a candidate
		*/

	private:

		static bool is_palindrome(const std::string_view& s)
		{
			return std::equal(s.begin(), s.begin() + s.size() / 2, s.rbegin());
		}

		/*
		* Find all the strings that can form a palindrome with s in this order: candidate + s
		* Basically if s is "abcdefg", reverse of s is a candidate for sure.
		* gfedcba + abcdefg
		* 
		* Then check each prefix of s. for example, if "abcde" is already a palindrome,
		* the reverse of the last 2 letters can be a candidate.
		* gf + abcdefg
		*      |   |
		*      this part is already a palindrome    
		*/
		vector<string> findCandidates(string const& s)
		{
			if (s.size() == 0)
				return vector<string>{};

			string scopy = s;
			reverse(scopy.begin(), scopy.end());
			vector<string> candidates;
			candidates.push_back(scopy);  // at least the reverse of entire 's' is a candidate
			size_t len = s.size();
			for (int i = 0; i < len; i++)
			{
				// test if s[0] to s[i] is palindrome
				// the distance between i and the last element is len-1-i
				int currentPrefixLen = i + 1;
				bool isPalin = is_palindrome(string_view(s.data(), currentPrefixLen));
				
				if (isPalin)  // if yes, the reverse of [i+1,end) is a candidate
				{
					if (i == len - 1)  // entire s is a palindrome
						candidates.push_back("");
					else
					{
						int candidateLen = len - i - 1;
						string candidate = scopy.substr(0, candidateLen);
						candidates.push_back(candidate);
					}
				}
			}
			return candidates;
		}

	public:

		// there needs some special processing if there is an empty string in "words"
		// any palindrome + "" is still a palindrome
		// If there is an empty string in "words", I need to identify the pre-existing palindromes in words

		vector<vector<int>> palindromePairs(vector<string>& words)
		{
			map<string, int> m;
			int index = 0;
			for (auto& w : words)
			{
				m[w] = index;
				index++;
			}
			vector<vector<int>>answer;
			vector<int> preexistPalin;
			int emptyStrIdx = -1;   // what if there are more than 1 empty string?
			for (int i = 0; i < words.size(); i++)
			{
				string const& w = words[i];
				if (w == "")
				{
					emptyStrIdx = i;
					continue;
				}
				vector<string>const& candidates = findCandidates(w);
				for (auto& candi : candidates)
				{
					if (m.find(candi) != m.end() && m[candi] != i)
					{
						// remember the candidates we found can form palindromes with 's' in this order: candidate + word
						answer.emplace_back(std::initializer_list<int>{m[candi], i}); // found one pair as an answer
					}
					if (candi == "")
					{
						// this word has an empty string as one of its candidates, this word itself must be a palindrome
						preexistPalin.emplace_back(i);
					}
				}
			}

			/*
			* If the "words" list is ["aba", "txt", ""]
			* by now I should have [2,0], [2,1] is the answer.
			* basically "empty_string + pre-exist_palindrome" is in the answer
			* but I still need "pre-exist_palindrome + empty_string"
			*/

			if (emptyStrIdx != -1 && preexistPalin.size() != 0)
			{
				for (auto& i : preexistPalin)
				{
					answer.emplace_back(std::initializer_list<int>{i, emptyStrIdx});
				}
			}

			return answer;
		}
	};



	class Solution2
	{
		/*
		* Use a trie. For every word in the given words' list, reverse it and add the reversed version to a trie.
		* Now I have a trie of all the reverses. Then examine each word in the given list again.
		* If a word is "abftf", search this word in the trie. If I find a path in the trie that looks like this:
		* root->a->b. That means there is a word in the words' list that reads "ba". abftf + ba = a palindrome.
		* 
		* It's also possible that when I examine a word, and search that word in the trie, I keep finding matching
		* letters all the way till I have exhausted letters in the word but the path has not ended.
		* for example:  two words in the input:  "abc"    "efecba"
		* 
		* The trie has:
		*                     root
		*                 a            c
		*                 b            b
		*                 c            a
		*                 e
		*                 f
		*                 e
		* 
		* I'm examining "abc", I'll find matching letters on the left path. When I arrive at 'c', I have run out
		* of letters in the word I'm examining, but the path in the trie has not ended. I can't just say OK I'll
		* go through the rest of the path and collect the letters and assemble a string (in this case, I get 'efe')
		* and test if this string is a palindrome. There can be many branches going out of node 'c', the subtree
		* rooted at node 'c' can be wide and deep.
		* Therefore I need to do two things:
		* (1) build a trie with every word's reverse, and search every word in this reverse trie.
		* (2) build a trie with every word, and serarch every word's reverse in this trie.
		* 
		* I must make sure which word's index is in front.
		* If I search a word in the reverse trie, an answer would be {word's index, reverse word's index (got from trie node)}
		* If I serach a word's reverse in the forward trie, an answer would be {forward word's index (got from trie node), reverse word's index}
		* 
		* One word can team up with more than one another word.
		* a b c d e d   +     c b a
		* a b c d e d   + e d c b a 
		* 
		* "cba" and "edcba" will share the first 3 nodes in the Trie
		*/

		static bool is_palindrome(string_view const& s)
		{
			return std::equal(s.begin(), s.begin() + s.size() / 2, s.rbegin());
		}

		struct Node
		{
			vector<Node*> children = vector<Node*>(26);
			int indexInInputList{ -1 };  // if this node is the ending letter of a word in the input list, 
			                             // what is that word's index in the list?
			                             // '-1' means it is not the ending letter
										// note that a node can be the ending letter of a word as well as an intermediate letter of another word
		};

		void addToTrie(string const& s, int index, Node* root)
		{
			Node* n = root;
			auto it = s.begin();
			while (it != s.end())
			{
				Node*& nextLevel = n->children[*it - 'a'];
				if (nextLevel == nullptr)
					nextLevel = new Node{};
				n = nextLevel;
				it = next(it);
			}
			n->indexInInputList = index;
		}

		void searchForPalindrome(string const& s, int index, Node* root, bool sInSecondPosInPair, vector<vector<int>>& answer)
		{
			Node* n = root;
			int charIndex = 0;
			// When I search a word, it can match the letters on a path all the way to the leaf node
			// This can happen in two cases:
			// (1). words: [abcd, dcba]   "abcd" is going to match the reverse of "dcba" and this is a legitimate match   
			// (2). when word is already a palindrome, it's going to match its own reverse
			// 
			while (charIndex < s.size())
			{
				Node* nextLevel = n->children[s[charIndex] - 'a'];
				if (nextLevel == nullptr)
					break;
				
				n = nextLevel;
				charIndex++;

				// make sure I'm not counting a word matching itself if the word is already a palindrome
				if (n->indexInInputList != -1 && n->indexInInputList != index)
				{
					// when charIndex == s.size() I have matched every letter in the word and I have just arrived at a node that represents a word
					if (charIndex == s.size() || is_palindrome(string_view(s.data() + charIndex)))
					{
						if (charIndex == s.size())
						{
							if (sInSecondPosInPair)
							{
								/*
								* 
								* If this is the full match case, and this is searching in the forward trie, do NOT record this answer!!
								* 
								* (first, I search a word in the reverse trie, secondly I search a word's reverse in the forward trie)
								* 
								* If two words fully match, they are going to match twice *just in the first search*, so I'll record
								* both answers in the first search, so do NOT record again in the second search.
								* 
								* example:  "abc" ,  "cba"
								* 
								* First:  reverse trie:
								*                       root
								*                   c           a
								*                   b           b
								*                   a           c
								*                    \           \
								*      (indexInInputList = 0)    (indexInInputList = 1)
								* 
								* search "abc", s is "abc", index is 0
								* I'll find "abc" in the trie, which represents "cba" in words' list. "cba"'s index is 1, so I have this pair (0,1) in answer
								* 
								* search "cba", s is "cba", index is 1
								* I'll find "cba" in the trie, which represents "abc" in words' list. "abc"'s index is 0, so I have this pair (1, 0) in answer
								* 
								* Now I have recorded both answers.
								* 
								* Second: forward trie:
								*                      root
								*                   a        c
								*                   b        b
								*                   c        a
								*                    \        \
								*     (indexInInputList = 0)   (indexInInputList = 1)
								* 
								* I search the reverse of "abc", i.e. "cba", s is "cba", index is 0
								* I'll find "cba" which represents "cba" in words' list. "cab"'s index is 1,
								* so if I do record, I put s's index in the 2nd position in the pair, I'll add pair (1, 0) to the answer
								* but obviously I should not do that.
								*/
							}
							else
							{
								answer.emplace_back(initializer_list<int>{index, n->indexInInputList});
							}
						}
						else
						{
							if (sInSecondPosInPair)
								answer.emplace_back(initializer_list<int>{n->indexInInputList, index});
							else
								answer.emplace_back(initializer_list<int>{index, n->indexInInputList});
						}
					}
				}
			}
		}

		void destroyTrie(Node* root)
		{
			Node* n = root;
			for (int i = 0; i < 26; i++)
			{
				if (n->children[i] != nullptr)
				{
					destroyTrie(n->children[i]);
				}
			}
			delete root;
		}

	public:

		vector<vector<int>> palindromePairs(vector<string>& words)
		{
			Node* forwardRoot = new Node{};
			Node* reverseRoot = new Node{};

			vector<int> emptyStringIndexes;
			vector<int> alreadyPalindromeStringIndexes;

			int index = 0;
			for (string& w : words)
			{
				if (w.size() == 0)
				{
					// I don't add empty string to the trie, I'll take care of "empty string + already palindrome" situation later
					emptyStringIndexes.emplace_back(index);
					index++;
					continue;
				}
				if (is_palindrome(string_view(w.data())))
				{
					alreadyPalindromeStringIndexes.emplace_back(index);
				}
				addToTrie(w, index, forwardRoot);
				addToTrie(string(w.rbegin(), w.rend()), index, reverseRoot);
				index++;
			}
			vector<vector<int>> answer;
			for (int i = 0; i < words.size(); i++)
			{
				searchForPalindrome(words[i], i, reverseRoot, false,  answer);
				searchForPalindrome(string(words[i].rbegin(), words[i].rend()), i, forwardRoot, true, answer);
			}

			if (emptyStringIndexes.size() > 0)
			{
				// if there are more than one empty string in "words", just these empty strings can spin out many combinations
				// the problem statement declares the input is a list of "unique" words, so this concern is probably unnecessary
				size_t len = emptyStringIndexes.size();
				// permutation of 2 out of x
				for (int i = 0; i < len; i++)
				{
					for (int j = i + 1; j < len; j++)
					{
						answer.emplace_back(initializer_list<int>{emptyStringIndexes[i], emptyStringIndexes[j]});
						answer.emplace_back(initializer_list<int>{emptyStringIndexes[j], emptyStringIndexes[i]});
					}
				}

				if (alreadyPalindromeStringIndexes.size() > 0)
				{
					size_t len2 = alreadyPalindromeStringIndexes.size();
					for (int i = 0; i < len; i++)
					{
						for (int j = 0; j < len2; j++)
						{
							answer.emplace_back(initializer_list<int>{emptyStringIndexes[i], alreadyPalindromeStringIndexes[j]});
							answer.emplace_back(initializer_list<int>{alreadyPalindromeStringIndexes[j], emptyStringIndexes[i]});
						}
					}
				}
			}

			destroyTrie(forwardRoot);
			destroyTrie(reverseRoot);

			return answer;
		}
	};


	static void Test1()
	{
		vector<string>words{ "abcd","dcba","lls","s","sssll" };
		Solution2 s;
		vector<vector<int>>const& answer = s.palindromePairs(words);
		for (auto& onePair : answer)
			cout << "[" << onePair[0] << ", " << onePair[1] << "]\n";
		cout << "-----------------------------------------------\n";
	}

	static void Test2()
	{
		vector<string>words{ "bat","tab","cat"};
		Solution2 s;
		vector<vector<int>>const& answer = s.palindromePairs(words);
		for (auto& onePair : answer)
			cout << "[" << onePair[0] << ", " << onePair[1] << "]\n";
		cout << "-----------------------------------------------\n";
	}

	static void Test3()
	{
		vector<string>words{ "a",""};
		Solution2 s;
		vector<vector<int>>const& answer = s.palindromePairs(words);
		for (auto& onePair : answer)
			cout << "[" << onePair[0] << ", " << onePair[1] << "]\n";
		cout << "-----------------------------------------------\n";
	}

	// other test cases:
	// 
    // xabac, abacabax, x
    // answer: [0,1], [2,1]

	void Test_0336_Palindrome_Pairs()
	{
		int option;
		Solution2 solu;

		function<void(void)>tests[] = { Test1, Test2, Test3 };

		while (true)
		{
			cout << "1: pre-defined tests;    2: new test;     0: exit  > ";
			cin >> option;
			if (option == 0) break;
			else if (option == 1)
			{
				cout << "test case: ";
				cin >> option;
				tests[option - 1]();
			}
			else if (option == 2)
			{
				string s;
				vector<string> words;
				cin.ignore();
				cout << "words list: ";
				getline(cin, s);
				stringstream ss(s);
				copy(istream_iterator<string>(ss), istream_iterator<string>(), back_inserter(words));
				vector<vector<int>>const& answer = solu.palindromePairs(words);
				for (auto& onePair : answer)
					cout << "[" << onePair[0] << ", " << onePair[1] << "]\n";
				cout << "-----------------------------------------------\n";
			}
		}
	}
}