#include<vector>
#include<algorithm>
#include<iostream>

namespace _0662_Maximum_Width_of_Binary_Tree {

	using namespace std;


	struct TreeNode
	{
		int val;
		TreeNode* left;
		TreeNode* right;
		TreeNode() :val(0), left(nullptr), right(nullptr) {}
		TreeNode(int x) :val(x), left(nullptr), right(nullptr) {}
		TreeNode(int x, TreeNode* left, TreeNode* right) :val(x), left(left), right(right) {}
	};

	class Solution
	{
		/*
		* This solution's memory consumption is not bad according to leetcode judge
		* less than 99.81% of submissions.
		* 
		* Is it possible to make an O(1) memory algorithm if the TreeNode struct allows a "nextRight" point?
		*/
	public:

		int widthOfBinaryTree(TreeNode* root)
		{
			vector<pair<TreeNode*, unsigned int>> q[2];
			int currentQ = 0;
			q[0].emplace_back(root, 0);
			int maxWidth = 0;
			while (q[currentQ].size() > 0)
			{
				int nextQ = (currentQ + 1) % 2;
				for_each(q[currentQ].begin(), q[currentQ].end(),
					[&](auto const& p)
					{
						if (p.first->left != nullptr)
						{
							int index = p.second * 2 + 1;
							q[nextQ].emplace_back(p.first->left, index);
						}
						if (p.first->right != nullptr)
						{
							int index = p.second * 2 + 2;
							q[nextQ].emplace_back(p.first->right, index);
						}
					});
				auto it1 = q[currentQ].begin();
				auto it2 = prev(q[currentQ].end());
				int width = it2->second - it1->second + 1;
				if (width > maxWidth)
					maxWidth = width;
				currentQ = (currentQ + 1) % 2;
				nextQ = (currentQ + 1) % 2;
				q[nextQ].clear();
			}
			return maxWidth;
		}
	};

	void Test1()
	{
		/*
		*            0
		*           / \
		*          1   2
		*         / \   \
		*        3  4    5
		*/

		TreeNode t[6];
		t[0].left = &t[1];
		t[0].right = &t[2];
		t[1].left = &t[3];
		t[1].right = &t[4];
		t[2].right = &t[5];

		Solution s;
		cout << s.widthOfBinaryTree(&t[0]) << '\n';
	}

	void Test2()
	{
		/*
		*       0
		*      /
		*     1
		*    / \
		*   2   3
		* 
		*/

		TreeNode t[4];
		t[0].left = &t[1];
		t[1].left = &t[2];
		t[1].right = &t[3];

		Solution s;
		cout << s.widthOfBinaryTree(&t[0]) << '\n';

	}

	void Test3()
	{
		/*
		*      0
		*    /   \
		*   1     2
		*  /
		* 3
		*/

		TreeNode t[4];
		t[0].left = &t[1];
		t[0].right = &t[2];
		t[1].left = &t[3];

		Solution s;
		cout << s.widthOfBinaryTree(&t[0]) << '\n';
	}

	void Test_0662_Maximum_Width_of_Binary_Tree()
	{
		Test1();
		Test2();
		Test3();
	}
}