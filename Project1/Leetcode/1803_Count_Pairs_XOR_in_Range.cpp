#include "LibHeaders.h"

namespace _1803_Count_Pairs_XOR_in_Range {

	using namespace std;

	class Solution
	{
		struct Node
		{
			int numbersOnThisPath;
			Node* nextLevel[2] = { nullptr, nullptr };
		};

		Node* root;

		void addToTrie(int a)
		{
			/*
			* constraint: each number <= 2 * 10 ^ 4 ~ 2 ^ 15 = 32768
			*    x   x   x   x   x    .    .   .   .   x    x
			*    |   |                                 |    |
			*  2^15  2^14                             2^1   2^0
			*    |   |
			*    |   this bit can be 1
			*   this bit won't be 1
			*/

			int base = 1 << 14;
			Node* n = root;
			for (int i = 0; i < 15; i++)
			{
				Node** nextNode;
				if (a & (base >> i))
				{
					nextNode = &(n->nextLevel[1]);
				}
				else
				{
					nextNode = &(n->nextLevel[0]);
				}
				if (*nextNode == nullptr)
				{
					*nextNode = new Node{ 1 };
				}
				else
				{
					(*nextNode)->numbersOnThisPath++;
				}
				n = *nextNode;
			}
		}

		enum class Flag
		{
			DownToFloor = 0,
			UpToCeiling = 1,
			FloorAndCeiling = 2
		};

		int checkTrie(Node* root, int a, int low, int high, int index, Flag f)
		{
			//Node* n = root;
			if (index == 15) return 1;

			int base = 1 << 14;
			
			int count = 0;
			if (f == Flag::FloorAndCeiling)
			{
				if (  (low & (base >> index)) && (high & (base >> index))  )
				{
					if (a & (base >> index))
					{
						if (root->nextLevel[0] != nullptr)
							count = checkTrie(root->nextLevel[0], a, low, high, index + 1, Flag::FloorAndCeiling);
					}
					else
					{
						if (root->nextLevel[1] != nullptr)
							count = checkTrie(root->nextLevel[1], a, low, high, index + 1, Flag::FloorAndCeiling);
					}
				}
				if (  !(low & (base >> index)) && !(high & (base >> index))  )
				{
					if (a & (base >> index))
					{
						if (root->nextLevel[1] != nullptr)
							count = checkTrie(root->nextLevel[1], a, low, high, index + 1, Flag::FloorAndCeiling);
					}
					else
					{
						if (root->nextLevel[0] != nullptr)
							count = checkTrie(root->nextLevel[0], a, low, high, index + 1, Flag::FloorAndCeiling);
					}
				}
				if (  !(low & (base >> index)) && (high & (base >> index))  )
				{
					if (a & (base >> index))
					{
						if (root->nextLevel[0] != nullptr)
							count += checkTrie(root->nextLevel[0], a, low, high, index + 1, Flag::UpToCeiling);
						if (root->nextLevel[1])
							count += checkTrie(root->nextLevel[1], a, low, high, index + 1, Flag::DownToFloor);
					}
					else
					{
						if (root->nextLevel[0] != nullptr)
							count += checkTrie(root->nextLevel[0], a, low, high, index + 1, Flag::DownToFloor);
						if (root->nextLevel[1])
							count += checkTrie(root->nextLevel[1], a, low, high, index + 1, Flag::UpToCeiling);
					}
				}
			}
			if (f == Flag::DownToFloor)
			{
				if (low & (base >> index))
				{
					if (a & (base >> index))
					{
						if (root->nextLevel[0])
							count = checkTrie(root->nextLevel[0], a, low, high, index + 1, Flag::DownToFloor);
					}
					else
					{
						if (root->nextLevel[1])
							count = checkTrie(root->nextLevel[1], a, low, high, index + 1, Flag::DownToFloor);
					}
				}
				else
				{
					if (a & (base >> index))
					{
						if (root->nextLevel[0])
							count += root->nextLevel[0]->numbersOnThisPath;
						if (root->nextLevel[1])
							count += checkTrie(root->nextLevel[1], a, low, high, index + 1, Flag::DownToFloor);
					}
					else
					{
						if (root->nextLevel[0])
							count += checkTrie(root->nextLevel[0], a, low, high, index + 1, Flag::DownToFloor);
						if (root->nextLevel[1])
							count += root->nextLevel[1]->numbersOnThisPath;
					}
				}
			}
			
			if (f == Flag::UpToCeiling)
			{
				if (high & (base >> index))
				{
					if (a & (base >> index))
					{
						if (root->nextLevel[0])
							count += checkTrie(root->nextLevel[0], a, low, high, index + 1, Flag::UpToCeiling);
						if (root->nextLevel[1])
							count += root->nextLevel[1]->numbersOnThisPath;
					}
					else
					{
						if (root->nextLevel[0])
							count += root->nextLevel[0]->numbersOnThisPath;
						if (root->nextLevel[1])
							count += checkTrie(root->nextLevel[1], a, low, high, index + 1, Flag::UpToCeiling);
					}
				}
				else
				{
					if (a & (base >> index))
					{
						if (root->nextLevel[1])
							count = checkTrie(root->nextLevel[1], a, low, high, index + 1, Flag::UpToCeiling);
					}
					else
					{
						if (root->nextLevel[0])
							count = checkTrie(root->nextLevel[0], a, low, high, index + 1, Flag::UpToCeiling);
					}
				}
			}
			return count;
		}



		void destroyTrie(Node* root)
		{
			if (root->nextLevel[0] != nullptr)
				destroyTrie(root->nextLevel[0]);
			if (root->nextLevel[1] != nullptr)
				destroyTrie(root->nextLevel[1]);
			delete root;
		}

	public:

		int countPairs(vector<int>& nums, int low, int high)
		{
			root = new Node{};
			for (int& n : nums)
			{
				addToTrie(n);
			}
			int nicePairsCount = 0;
			for (int& n : nums)
			{
				nicePairsCount += checkTrie(root, n, low, high, 0, Flag::FloorAndCeiling);
			}
			destroyTrie(root);
			return nicePairsCount/2;
		}
	};



	class BruteForceSolution
	{
	public:
		int countPairs(vector<int>& nums, int low, int high)
		{
			int n = nums.size();
			int count = 0;
			for (int i = 0; i < n; i++)
			{
				for (int j = i + 1; j < n; j++)
				{
					int a = nums[i] ^ nums[j];
					if (low <= a && a <= high)
						count++;
				}
			}
			return count;
		}
	};


	void Test_1803_Count_Pairs_XOR_in_Range()
	{
		Solution solu;
		BruteForceSolution bs;

		string everything;
		vector<int>parameters;
		
		while (true)
		{
			everything.clear();

			cout << "nums:   low:   high: >  ";
			getline(cin, everything);
			if (everything.size() == 0) break;
			parameters.clear();
			stringstream ss(everything);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(parameters));
			vector<int>nums(parameters.begin(), parameters.end() - 2);
			int low = *(parameters.rbegin() + 1);
			int high = *(parameters.rbegin());
			cout << "fast solution: " << solu.countPairs(nums, low, high) << "   brute answer: " << bs.countPairs(nums, low, high) << '\n';
		}
	}
}