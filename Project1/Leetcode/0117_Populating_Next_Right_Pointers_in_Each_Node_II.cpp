#include<iostream>
#include<functional>
#include<algorithm>

namespace _0117_Populating_Next_Right_Pointers_in_Each_Node_II {

	using namespace std;

	class Node
	{
	public:
		int val;
		Node* left;
		Node* right;
		Node* next;
		Node() : val{ 0 }, left{ nullptr }, right{ nullptr }, next{ nullptr }{}
		Node(int _val): val{_val},left{nullptr}, right{ nullptr }, next{ nullptr }{}
		Node(int _val, Node* _left, Node* _right, Node* _next) : val{ _val }, left{ _left }, right{ _right }, next{ _next }{}
	};

	class Solution
	{
	public:

		/*
		* Follow-up:
		* Constant extra space
		* stack space consumed by recursive calls is not counted as extra space
		* ---- Basically disallow breadth-first-search
		*/

		/*
		* Traverse one horizontal level at a time, when traversing on the nth level,
		* I link up the nodes horizontally on the (n+1)th level.
		* As long as I have nth level well linked up, I can link up (n+1)th level no problem.
		* Now the very first level is just the root and is easy to deal with.
		*/

		Node* connect(Node* root)
		{
			Node* n = root;

			Node* frontRunner;
			Node* nextLevelStart = nullptr;
			while (n != nullptr)
			{
				if (n->left != nullptr || n->right != nullptr)
				{
					if (nextLevelStart == nullptr)
					{
						// I'm on the left-most node of a new level
						if (n->left != nullptr)
							nextLevelStart = n->left;
						else
							nextLevelStart = n->right;
					}
					if (n->left != nullptr && n->right != nullptr)
						n->left->next = n->right;

					Node* lookingForNext;
					if (n->left != nullptr && n->right == nullptr)
						lookingForNext = n->left;
					else
						lookingForNext = n->right;

					frontRunner = n->next;
					Node* nextFound = nullptr;
					while (frontRunner != nullptr)
					{
						if (frontRunner->left != nullptr)
						{
							nextFound = frontRunner->left;
							break;
						}
						else if (frontRunner->right != nullptr)
						{
							nextFound = frontRunner->right;
							break;
						}
						else
							frontRunner = frontRunner->next;
					}
					lookingForNext->next = nextFound;
					if (frontRunner != nullptr)
					{
						n = frontRunner;
						continue;
					}
					else
					{
						n = nextLevelStart;
						nextLevelStart = nullptr;
						continue;
					}
				}
				n = n->next;
			}
			return root;
		}
	};

	class Solution2
	{
		/*
		* This solution derives from the same principles as Solution does.
		* Just trying to clean-up an tidy-up.
		*/
	public:
		Node* connect(Node* root)
		{
			Node* n = root;

			Node* nextLevelStart = nullptr;
			Node* lookingForNext = nullptr; // the node on the next level that is looking for its right side sibling

			while (n != nullptr)
			{
				if (lookingForNext != nullptr)
				{
					if (n->left)
						lookingForNext->next = n->left;
					else
						lookingForNext->next = n->right; // if n has no child, lookingForNext won't get a "next"
					                                     // but if n has no child, I won't change "lookingForNext" either
					                                     // until I find an 'n' that has child(children)
				}

				if (nextLevelStart == nullptr)
				{
					if (n->left)
						nextLevelStart = n->left;
					else
						nextLevelStart = n->right;  // by now nextLevelStart can still be null
				}

				if (n->left != nullptr && n->right != nullptr)
				{
					// In this case, I must have linked n->left to lookingForNext,
					// so it's time to move lookingForNext
					n->left->next = n->right;
					lookingForNext = n->right;
				}
				else if (n->left != nullptr && n->right == nullptr)
				{
					lookingForNext = n->left;
				}
				else if (n->left == nullptr && n->right != nullptr)
				{
					lookingForNext = n->right;
				}
				
				if (n->next == nullptr)
				{
					n = nextLevelStart;
					nextLevelStart = nullptr;
					lookingForNext = nullptr;
				}
				else
					n = n->next;
			}
			return root;
		}
	};


	void Test1()
	{
		/*
		*                     0
		*             +-------+--------+
		*             1                2
		*        +----+----+           +-----+
		*        3         4                 5
		*    +---+---+                       +-----+
		*    6       7                             8
		*         +--+--+
		*         9     10
		*/
		Node t[11];
		for (int i = 0; i < 11; i++)
			t[i].val = i;

		t[0].left = &t[1];
		t[0].right = &t[2];
		t[1].left = &t[3];
		t[1].right = &t[4];
		t[2].right = &t[5];
		t[3].left = &t[6];
		t[3].right = &t[7];
		t[5].right = &t[8];
		t[7].left = &t[9];
		t[7].right = &t[10];

		Solution2 s;
		s.connect(&t[0]);

		for (int i = 0; i < 11; i++)
		{
			cout << i << "   next: " << (t[i].next != nullptr ? t[i].next->val : -1) << "\n";
		}
		cout << "\n\n";
	}

	void Test2()
	{
		/*
		*                     0
		*                     +--------+
		*                              2
		*                        +----+-----+
		*                       3            5
		*                  +---+---+         +-----+
		*                  6       7               8
		*                       +--+--+
		*                       9     10
		*/
		Node t[11];
		for (int i = 0; i < 11; i++)
			t[i].val = i;

		t[0].right = &t[2];
		t[2].left = &t[3];
		t[2].right = &t[5];
		t[3].left = &t[6];
		t[3].right = &t[7];
		t[5].right = &t[8];
		t[7].left = &t[9];
		t[7].right = &t[10];

		Solution2 s;
		s.connect(&t[0]);

		for (int i = 0; i < 11; i++)
		{
			cout << i << "   next: " << (t[i].next != nullptr ? t[i].next->val : -1) << "\n";
		}
		cout << "\n\n";
	}


	void Test_0117_Populating_Next_Right_Pointers_in_Each_Node_II()
	{
		function<void(void)> testers[] = { Test1, Test2 };
		for_each(begin(testers), end(testers),
			[](function<void(void)>&f)
			{f(); });
	}
}