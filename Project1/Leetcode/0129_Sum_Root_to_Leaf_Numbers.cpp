
namespace _0129_Sum_Root_to_Leaf_Numbers {

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
		void sumNumbersRecur(TreeNode* n, int sumOfThisPath, int& sum)
		{
			if (n->left == nullptr && n->right == nullptr)
			{
				sum += (sumOfThisPath * 10 + n->val);
				
			}
			if (n->left)
				sumNumbersRecur(n->left, (sumOfThisPath * 10 + n->val), sum);
			if (n->right)
				sumNumbersRecur(n->right, (sumOfThisPath * 10 + n->val), sum);
		}

	public:
		int sumNumbers(TreeNode* root)
		{
			int sum = 0;
			sumNumbersRecur(root, 0, sum);
			return sum;
		}
	};
}