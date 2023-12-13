#include<iostream>

namespace _0124_Binary_Tree_Maximum_Path_Sum {
	using namespace std;

	struct TreeNode {
		int val;
		TreeNode* left;
		TreeNode* right;
		TreeNode() : val(0), left(nullptr), right(nullptr) {}
		TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
		TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
	};

	class Solution
	{
		int bestSoFar;

		int maxPathSum2(TreeNode* n)
		{
			int maxSumOfPathFromRootToLeft = INT_MIN;
			int maxSumOfPathFromRootToRight = INT_MIN;

			if (n->left == NULL && n->right == NULL)
			{
				if (n->val > bestSoFar)
					bestSoFar = n->val;
				return n->val;
			}

			if (n->left)
			{
				maxSumOfPathFromRootToLeft = maxPathSum2(n->left);
			}
			if (maxSumOfPathFromRootToLeft < 0) // if I get a negative number from subtree, I would rather take none from subtree, the effect of taking none is to set max sum from subtree to 0
			{
				maxSumOfPathFromRootToLeft = 0;
			}
			if (n->right)
			{
				maxSumOfPathFromRootToRight = maxPathSum2(n->right);
			}
			if (maxSumOfPathFromRootToRight < 0)
			{
				maxSumOfPathFromRootToRight = 0;
			}
			int a = maxSumOfPathFromRootToLeft + maxSumOfPathFromRootToRight + n->val;
			if (a > bestSoFar)
				bestSoFar = a;

			return max(maxSumOfPathFromRootToLeft, maxSumOfPathFromRootToRight) + n->val;
		}

	public:
		int maxPathSum(TreeNode* root)
		{
			bestSoFar = INT_MIN;
			maxPathSum2(root);
			return bestSoFar;
		}
	};

	void Test_0124_Binary_Tree_Maximum_Path_Sum()
	{
		Solution so;

		TreeNode a[5];
		a[0].val = 9;
		a[1].val = -10;
		a[1].left = &a[0];
		a[1].right = &a[2];
		a[2].val = 20;
		a[2].left = &a[3];
		a[2].right = &a[4];
		a[3].val = 15;
		a[4].val = 7;

		cout << so.maxPathSum(&a[1]) <<"\n\n";
	}
}