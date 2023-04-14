#include "LibHeaders.h"

namespace _1302_Deepest_Leaves_Sum {

struct TreeNode
{
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode() : val(0), left(nullptr), right(nullptr) {}
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
	TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};


class Solution
{
	int max = 0;
	int sum = 0;

	void findDeepestLevel(TreeNode* node, int level)
	{
		if (node->left == nullptr && node->right == nullptr)
		{
			if (max < level)
				max = level;
		}
		if (node->left != nullptr)
			findDeepestLevel(node->left, level + 1);
		if (node->right != nullptr)
			findDeepestLevel(node->right, level + 1);
	}

	void computeSum(TreeNode* node, int level)
	{
		if (level == max)
			sum += node->val;  // I can explicitly call return right here, but it's fine this way, it's certain that the deepest level node has no children.
		if (node->left != nullptr)
			computeSum(node->left, level + 1);
		if (node->right != nullptr)
			computeSum(node->right, level + 1);
	}

public:

	int deepestLeavesSum(TreeNode* root)
	{
		max = sum = 0;

		findDeepestLevel(root, 0);
		computeSum(root, 0);
		return sum;
	}
};

static void Test1(Solution& s)
{
	std::vector<TreeNode> v(8);
	iota(v.begin(), v.end(), 1);

	v[0].left = &v[1];
	v[0].right = &v[2];
	v[1].left = &v[3];
	v[1].right = &v[4];
	v[3].left = &v[6];
	v[2].right = &v[5];
	v[5].right = &v[7];

	std::cout << s.deepestLeavesSum(&v[0]) << '\n';
}

/*
*             6 (0)
*         +---+---------+
*         7 (1)         8 (2)
*     +---+---+      +--+--+
*     2 (3)   7(4)   1(5)  3(6)
*  +--+--+ +--+--+         +---+
*  9(7)    1(8)  4(9)          5(10)
*/
static void Test2(Solution& s)
{
	std::vector<TreeNode> v(11);
	v[0].left = &v[1];
	v[0].right = &v[2];
	v[0].val = 6;

	v[1].left = &v[3];
	v[1].right = &v[4];
	v[1].val = 7;

	v[2].left = &v[5];
	v[2].right = &v[6];
	v[2].val = 8;

	v[3].left = &v[7];
	v[3].val = 2;

	v[4].left = &v[8];
	v[4].right = &v[9];
	v[5].val = 7;

	v[5].val = 1;

	v[6].right = &v[10];
	v[6].val = 3;

	v[7].val = 9;

	v[8].val = 1;

	v[9].val = 4;

	v[10].val = 5;

	std::cout << s.deepestLeavesSum(&v[0]) << '\n';
}

/*
* both test cases are from leetcode
*/
void Test_1302_Deepest_Leaves_Sum()
{
	Solution s;
	Test1(s);
	Test2(s);
}
}