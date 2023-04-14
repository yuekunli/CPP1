#include "LibHeaders.h"

namespace _0642_Search_Autocomplete {

	using namespace std;

	class AutocompleteSystem
	{
		struct Node
		{
			vector<Node*>nextLevel = vector<Node*>(27, nullptr);
			int times = 0;
		};
		string currentBuildUp;
		Node* root;
		Node* currentNode;
		void addToTrie(Node* root, string& s, int times)
		{
			Node* n = root;
			for (char& c : s)
			{
				int index = 0;
				if (c != ' ')
					index = c - 'a' + 1;

				Node*& nextNode = n->nextLevel[index];
				if (nextNode == nullptr)
					nextNode = new Node();
				n = nextNode;
			}
			n->times = times;
		}

		struct A
		{
			string s;
			int times = 0;
			A(string& s_, int times_) :s(s_), times(times_) {}
		};

		void dfs(Node* n, string& buildUp, vector<A>& pa)
		{
			if (n->times > 0)
				pa.emplace_back( buildUp, n->times );
			for (int i = 0; i < 27; i++)
			{
				if (n->nextLevel[i] != nullptr)
				{
					string add;
					if (i == 0)
						buildUp.append(1, ' ');
					else
						buildUp.append(1, 'a' + i - 1);

					dfs(n->nextLevel[i], buildUp, pa);
					buildUp.erase(buildUp.end() - 1);
				}
			}
		}

		void destroyTrie(Node* n)
		{
			for (int i = 0; i < 27; i++)
			{
				if (n->nextLevel[i] != nullptr)
					destroyTrie(n->nextLevel[i]);
			}
			delete n;
		}

	public:
		AutocompleteSystem(vector<string>& sentences, vector<int>& times)
		{
			root = new Node();

			int index = 0;
			for (string& s : sentences)
			{
				addToTrie(root, s, times[index]);
				index++;
			}
			currentNode = root;
		}

		vector<string> input(char c)
		{
			if (c == '#')
			{
				currentNode->times++;
				currentNode = root;
				currentBuildUp.clear();
				return {};
			}
			int index = 0;
			if (c != ' ')
				index = c - 'a' + 1;

			Node*& nextNode = currentNode->nextLevel[index];
			if (nextNode == nullptr)
			{
				nextNode = new Node{};
				currentNode = nextNode;
				return {};
			}
			else
			{
				currentNode = nextNode;
				currentBuildUp = currentBuildUp.append(1,c);
			}
			vector<A>potentialAnswers;
			dfs(currentNode, currentBuildUp, potentialAnswers);

			// Technically speaking, I don't need to sort.
			// I just want the top 3 answers, so scanning the array
			// 3 times should give me the answers.
			sort(potentialAnswers.begin(), potentialAnswers.end(),
				[](A const& a, A const& b) 
				{
					if (a.times != b.times)
						return a.times > b.times;
					else
						return a.s.compare(b.s) < 0;
				});
			vector<string>answer;
			for (int i = 0; i < 3; i++)
			{
				if (i == potentialAnswers.size())
					break;
				answer.push_back(potentialAnswers[i].s);
			}
			return answer;
		}

		~AutocompleteSystem()
		{
			destroyTrie(root);
		}
	};

	void Test_0642_Search_Autocomplete()
	{
		vector<string> sentences{"i love you", "island", "iroman", "i love leetcode"};
		vector<int> times{ 5, 3, 2, 2 };
		AutocompleteSystem acs(sentences, times);
		vector<string>const& a1 = acs.input('i');
		copy(a1.begin(), a1.end(), ostream_iterator<string>(std::cout, "    "));
		cout << "\n";
		vector<string>const& a2 = acs.input(' ');
		copy(a2.begin(), a2.end(), ostream_iterator<string>(std::cout, "    "));
		cout << "\n";
		vector<string>const& a3 = acs.input('a');
		copy(a3.begin(), a3.end(), ostream_iterator<string>(std::cout, "    "));
		cout << "\n";
		vector<string>const& a4 = acs.input('#');
		copy(a4.begin(), a4.end(), ostream_iterator<string>(std::cout, "    "));
		cout << "\n";
	}
}