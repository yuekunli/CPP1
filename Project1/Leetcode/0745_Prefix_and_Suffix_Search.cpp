#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

namespace _0745_Prefix_and_Suffix_Search {

	using namespace std;

	class WordFilter  // 17/17 test cases passed, but memory limit exceeded
	{
		struct Node
		{
			vector<Node*> c = vector<Node*>(26, nullptr); // children
			vector<int> indexes;
		};

		Node* pRoot = nullptr; // prefix trie root
		Node* sRoot = nullptr; // suffix trie root


		void addToTrie(string& s, int index, Node* root)
		{
			Node* n = root;
			for (char& letter : s)
			{
				Node* next = n->c[letter - 'a'];
				if (next == nullptr)
				{
					next = new Node();
					n->c[letter - 'a'] = next;
				}
				next->indexes.push_back(index);
				n = next;
			}
		}

		vector<int> findInTrie(string const & s, Node* root)
		{
			Node* n = root;
			
			for (char const& letter : s)
			{
				Node* next = n->c[letter - 'a'];
				if (next == nullptr)
					return {};
			
				n = next;
			}
			return n->indexes;
		}

		// post order traverse and delete
		void cleanup(Node* root)
		{
			Node* n = root;
			for (Node*& next : n->c)
				if (next != nullptr)
					cleanup(next);
			delete n;
		}

	public:

		WordFilter(vector<string>& words)
		{
			pRoot = new Node();
			sRoot = new Node();

			int index = 0;
			for (string& w : words)
			{
				addToTrie(w, index, pRoot);
				string wReverse(w.rbegin(), w.rend());
				addToTrie(wReverse, index, sRoot);
				index++;
			}
		}

		~WordFilter()
		{
			cleanup(pRoot);
			cleanup(sRoot);
		}

		int f(string prefix, string suffix)
		{
			vector<int>const& indexesMatchPrefix = findInTrie(prefix, pRoot);
			vector<int>const& indexesMatchSuffix = findInTrie(string(suffix.rbegin(), suffix.rend()), sRoot);
			int i = (int)indexesMatchPrefix.size() - 1, j = (int)indexesMatchSuffix.size() - 1;
			while (i >= 0 && j >= 0)
			{
				if (indexesMatchPrefix[i] == indexesMatchSuffix[j])
					return indexesMatchPrefix[i];
				else if (indexesMatchPrefix[i] > indexesMatchSuffix[j])
					i--;
				else
					j--;
			}
			return -1;
		}
	};



	
	class WordFilter2 // time limit exceeded
	{
		struct Node
		{
			vector<Node*> c = vector<Node*>(26, nullptr); // children
			int index{ -1 };
		};

		Node* pRoot = nullptr; // prefix trie root
		Node* sRoot = nullptr; // suffix trie root


		void addToTrie(string& s, int index, Node* root)
		{
			Node* n = root;
			for (char& letter : s)
			{
				Node* next = n->c[letter - 'a'];
				if (next == nullptr)
				{
					next = new Node();
					n->c[letter - 'a'] = next;
				}
				n = next;
			}
			n->index = index;
		}

		void collectIndexes(Node* n, vector<int>& a)
		{
			if (n->index >= 0)
			{
				a.push_back(n->index);
			}

			for (Node* const& next : n->c)
			{
				if (next != nullptr)
					collectIndexes(next, a);
			}
		}
		vector<int> findInTrie(string const& s, Node* root)
		{
			Node* n = root;

			for (char const& letter : s)
			{
				Node* next = n->c[letter - 'a'];
				if (next == nullptr)
					return {};

				n = next;
			}
			vector<int> a;
			collectIndexes(n, a);
			return a;
		}

		// post order traverse and delete
		void cleanup(Node* root)
		{
			Node* n = root;
			for (Node*& next : n->c)
				if (next != nullptr)
					cleanup(next);
			delete n;
		}

	public:

		WordFilter2(vector<string>& words)
		{
			pRoot = new Node();
			sRoot = new Node();

			int index = 0;
			for (string& w : words)
			{
				addToTrie(w, index, pRoot);
				string wReverse(w.rbegin(), w.rend());
				addToTrie(wReverse, index, sRoot);
				index++;
			}
		}

		~WordFilter2()
		{
			cleanup(pRoot);
			cleanup(sRoot);
		}

		int f(string prefix, string suffix)
		{
			vector<int> indexesMatchPrefix = findInTrie(prefix, pRoot);
			vector<int> indexesMatchSuffix = findInTrie(string(suffix.rbegin(), suffix.rend()), sRoot);
			
			if (indexesMatchPrefix.size() == 0 || indexesMatchSuffix.size() == 0)
				return -1;

			make_heap(indexesMatchPrefix.begin(), indexesMatchPrefix.end());
			make_heap(indexesMatchSuffix.begin(), indexesMatchSuffix.end());

			int i = (int)indexesMatchPrefix.size() - 1, j = (int)indexesMatchSuffix.size() - 1;
			pop_heap(indexesMatchPrefix.begin(), indexesMatchPrefix.end());
			pop_heap(indexesMatchSuffix.begin(), indexesMatchSuffix.end());

			while (true)
			{
				if (indexesMatchPrefix[i] == indexesMatchSuffix[j])
					return indexesMatchPrefix[i];
				
				else if (indexesMatchPrefix[i] > indexesMatchSuffix[j])
				{

					if (i == 0)
						break;
					pop_heap(indexesMatchPrefix.begin(), indexesMatchPrefix.begin() + i);
					i--;
				}
				else
				{

					if (j == 0)
						break;
					pop_heap(indexesMatchSuffix.begin(), indexesMatchSuffix.begin() + j);
					j--;
				}
			}

			return -1;
		}
	};


	class WordFilter3 // accepted
	{
		/*
		* Only build 1 trie. Find the indexes of words that match the prefix.
		* Then just use straightforward comparison to test each prefix-matching word see
		* whether each of them match the suffix as well.
		*/
		struct Node
		{
			vector<Node*> c = vector<Node*>(26, nullptr); // children
			vector<int> indexes;
		};

		Node* pRoot = nullptr; // prefix trie root


		void addToTrie(string& s, int index, Node* root)
		{
			Node* n = root;
			for (char& letter : s)
			{
				Node* next = n->c[letter - 'a'];
				if (next == nullptr)
				{
					next = new Node();
					n->c[letter - 'a'] = next;
				}
				next->indexes.push_back(index);
				n = next;
			}
		}

		vector<int> findInTrie(string const& s, Node* root)
		{
			Node* n = root;

			for (char const& letter : s)
			{
				Node* next = n->c[letter - 'a'];
				if (next == nullptr)
					return {};

				n = next;
			}
			return n->indexes;
		}

		// post order traverse and delete
		void cleanup(Node* root)
		{
			Node* n = root;
			for (Node*& next : n->c)
				if (next != nullptr)
					cleanup(next);
			delete n;
		}
		vector<string> dictionary;
	public:

		WordFilter3(vector<string>& words)
		{
			dictionary = words;
			pRoot = new Node();


			int index = 0;
			for (string& w : words)
			{
				addToTrie(w, index, pRoot);
				index++;
			}
		}

		~WordFilter3()
		{
			cleanup(pRoot);
		}

		bool isSuffixMatch(int index, string& suf)
		{
			string& word = dictionary[index];

			if (word.size() < suf.size())
				return false;
			
			int i = (int)suf.size() - 1;
			int j = (int)word.size() - 1;
			while (i >= 0)
			{
				if (word[j] != suf[i])
					return false;
				i--;
				j--;
			}
			return true;
		}

		int f(string prefix, string suffix)
		{
			vector<int>const& indexesMatchPrefix = findInTrie(prefix, pRoot);

			int i = (int)indexesMatchPrefix.size() - 1;
			while (i >= 0)
			{
				if (isSuffixMatch(indexesMatchPrefix[i], suffix))
					return indexesMatchPrefix[i];
				i--;
			}
			return -1;
		}
	};

	/*
	* Another idea to try is to not save indexes at every node, only save them at end node. (Solution2)
	* And build only 1 trie (Solution3)
	*/


	static void Test_1()
	{
		vector<string> words{ "abcdefgh", "abccef", "abbhij", "effggik", "abcdtth", "eefftth", "mpqtth" };

		WordFilter wf(words);
		cout << wf.f("abc", "gh") << '\n';
		cout << wf.f("abc", "tth") << '\n';
	}

	static void Test_2()
	{
		vector<string> words{ "apple" };
		WordFilter3 wf(words);
		cout << wf.f("a", "a") << '\n';
	}
	void Test_0745_Prefix_and_Suffix_Search()
	{
		Test_2();
	}

}