#include "LibHeaders.h"

namespace _0366_Find_Leaves_Binary_Tree {

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
		int dfs(TreeNode* n, unordered_map<int, vector<TreeNode*>>& m, int depth)
		{
			int leftDepth = depth;
			int rightDepth = depth;
			if (n->left)
			{
				leftDepth = dfs(n->left, m, depth + 1);
			}
			if (n->right)
			{
				rightDepth = dfs(n->right, m, depth + 1);
			}
			int deepest = max(leftDepth, rightDepth);
			int height = deepest - depth;
			m[height].emplace_back(n);
			return deepest;
		}

	public:
		vector<vector<int>>findLeaves(TreeNode* root)
		{
			unordered_map<int, vector<TreeNode*>>m;
			int maxHeight = dfs(root, m, 0); // the deepest depth is also the height of the root
			vector<vector<int>>answer(maxHeight+1);
			for (auto& a : m)
			{
				int h = a.first;
				auto& nodes = a.second;
				answer[h].assign(nodes.size(), 0);
				transform(nodes.begin(), nodes.end(), answer[h].begin(), [&](TreeNode*& a) {return a->val; });
			}
			return answer;
		}
	};


	class Solution2
	{
		/*
		* Don't even need the unordered_map.
		*/

		int dfs(TreeNode* n, vector<vector<int>>&answer, int depth)
		{
			int leftDepth = depth;
			int rightDepth = depth;
			if (n->left)
			{
				leftDepth = dfs(n->left, answer, depth + 1);
			}
			if (n->right)
			{
				rightDepth = dfs(n->right, answer, depth + 1);
			}
			int deepest = max(leftDepth, rightDepth);
			int height = deepest - depth;
			if (height < answer.size())
			{
				answer[height].emplace_back(n->val);
			}
			else
			{
				/*
				* if "answer" right now has 3 vectors, then level with height 0, 1, and 2 are present
				* "height" in this case must be 3, so after adding one more vector, I can directly find
				* the new vector using "height" as index.
				*/
				answer.emplace_back();
				answer[height].emplace_back(n->val);
			}
			return deepest;
		}

	public:
		vector<vector<int>>findLeaves(TreeNode* root)
		{
			vector<vector<int>>answer;
			(void) dfs(root, answer, 0); // the deepest depth is also the height of the root

			return answer;
		}
	};

	static void Test1()
	{
		TreeNode a[6];
		a[1].val = 1;
		a[2].val = 2;
		a[3].val = 3;
		a[4].val = 4;
		a[5].val = 5;
		a[1].left = &a[2];
		a[1].right = &a[3];
		a[2].left = &a[4];
		a[2].right = &a[5];

		Solution2 s;
		vector<vector<int>>const& answer = s.findLeaves(&a[1]);
		for (vector<int>const& a : answer)
		{
			copy(a.begin(), a.end(), ostream_iterator<int>(std::cout, " "));
			cout << '\n';
		}
		cout << '\n';
	}

	void Test_0366_Find_Leaves_Binary_Tree()
	{
		Test1();
	}
}