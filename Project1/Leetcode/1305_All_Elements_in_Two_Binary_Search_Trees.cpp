#include "LibHeaders.h"

namespace _1305_All_Elements_in_Two_Binary_Search_Trees {

	struct TreeNode
	{
		int val;
		TreeNode* left;
		TreeNode* right;
		TreeNode* parent;
		TreeNode() : val(0), left(nullptr), right(nullptr), parent(nullptr)
		{
		}
		TreeNode(int x) : val(x), left(nullptr), right(nullptr), parent(nullptr)
		{
		}
		TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right), parent(nullptr)
		{
		}
	};

	class Solution
	{
	public:
		virtual std::vector<int> getAllElements(TreeNode* root1, TreeNode* root2)
		{
			return std::vector<int>();
		}
	};

	class Solution1 : public Solution
	{
		/*
		* This solution works fine, but it does need extra memory in the order of O(number of nodes in the first tree)
		*/

		void getAllNodesInOneTree(TreeNode* node, std::vector<int>& v)  // THIS FUNCTION IS NOT IN USE
		{
			if (node->left != nullptr)
				getAllNodesInOneTree(node->left, v);

			v.emplace_back(node->val);

			if (node->right != nullptr)
				getAllNodesInOneTree(node->right, v);
		}

		/*
		* The normal merge process deals with two sequential containers.
		* I give a tree node and a vector to this function.
		* Just imagine the tree node is also a sequential container.
		* Now I'm dealing with two sequential containers.
		* 'i' tells me from which element in the vector I should start consider.
		* if 'i' is 5, I first consider the 0th in the tree and the 5th in the vecotr.
		* If this function returns 13, that means the 13th in the vector has not been merged.
		*/
		int mergeVectorAndTree(TreeNode* node, std::vector<int>& mergeWith, int i, std::vector<int>& merged)
		{
			if (node->left != nullptr)
				i = mergeVectorAndTree(node->left, mergeWith, i, merged);

			while (i >= 0 && i < mergeWith.size() && mergeWith[i] < node->val)
			{
				merged.emplace_back(mergeWith[i]);
				i++;
			}
			merged.emplace_back(node->val);

			if (node->right != nullptr)
				i = mergeVectorAndTree(node->right, mergeWith, i, merged);

			return i;
		}

	public:

		/*
		* merge the first tree with a dummy vector, then the merged becomes just the serialized first tree.
		* merge the second tree with the serialized first tree, if the return value is 15, that means the
		* 15th (and afterward) in the vector has not been merged, so I need to move all of them to the "merged".
		*/
		std::vector<int> getAllElements(TreeNode* root1, TreeNode* root2)
		{
			std::vector<int> dummy;
			std::vector<int> firstTree;
			std::vector<int> merged;
			int i;  // from which element in the vector should I start considering
	
			(void)mergeVectorAndTree(root1, dummy, -1, firstTree);
			i = mergeVectorAndTree(root2, firstTree, 0, merged);
			if (i < firstTree.size())  // the last a few elements in the "firstTree" are greater than everyone in the root2 tree, move them into "merged"
				std::copy(firstTree.begin() + i, firstTree.end(), std::back_inserter(merged));

			return merged;
		}
	};


	class Solution2 : public Solution
	{
		TreeNode* getNext(TreeNode* n)
		{
			if (n->right != nullptr)
				return getSmallest(n->right);

			while (n->parent != nullptr && n == n->parent->right)
				n = n->parent;

			return n->parent;
		}

		TreeNode* getSmallest(TreeNode* n)
		{
			while (n->left != nullptr)
				n = n->left;
			return n;
		}

	public:
		std::vector<int> getAllElements(TreeNode* root1, TreeNode* root2)
		{
			std::vector<int> merged;
			TreeNode* n1 = getSmallest(root1);
			TreeNode* n2 = getSmallest(root2);
			while (n1 != nullptr || n2 != nullptr)
			{
				if (n1 == nullptr)
				{
					merged.emplace_back(n2->val);
					n2 = getNext(n2);
					continue;
				}

				if (n2 == nullptr)
				{
					merged.emplace_back(n1->val);
					n1 = getNext(n1);
					continue;
				}

				if (n1->val <= n2->val)
				{
					merged.emplace_back(n1->val);
					n1 = getNext(n1);
					continue;
				}
				else
				{
					merged.emplace_back(n2->val);
					n2 = getNext(n2);
					continue;
				}
			}
			return merged;
		}
	};


	static void Test1(Solution& s)
	{
		std::vector<TreeNode> t {2,1,4,1,0,3};
		t[0].left = &t[1];
		t[0].right = &t[2];
		
		t[1].parent = &t[0];
		t[2].parent = &t[0];

		t[3].left = &t[4];
		t[3].right = &t[5];
		
		t[4].parent = &t[3];
		t[5].parent = &t[3];

		
		std::vector<int> const & result = s.getAllElements(&t[0], &t[3]);

		for (int const & i : result)
			std::cout << i << "   ";
		std::cout << '\n';
	}


	/*
		    8(0)
		 /     \
		4(1)     15(2)
	   /  \      /    \
	  2(3) 7(4) 10(5)  17(6)
		  /       \      \
		 5(7)      13(8)  20(9)
                            \
                             22(10)

			16(11)
		  /      \
		 3(12)   19(13)

	*/
	static void Test2(Solution& s)
	{
		std::vector<TreeNode> t { 8, 4, 15, 2, 7, 10, 17, 5, 13, 20, 22, 16, 3, 19 };
							  //  0  1  2   3  4  5   6   7  8   9   10  11  12 13
		t[0].left = &t[1];
		t[0].right = &t[2];
		t[1].left = &t[3];
		t[1].right = &t[4];
		t[1].parent = &t[0];
		t[2].left = &t[5];
		t[2].right = &t[6];
		t[2].parent = &t[0];
		t[3].parent = &t[1];
		t[4].left = &t[7];
		t[4].parent = &t[1];
		t[5].right = &t[8];
		t[5].parent = &t[2];
		t[6].right = &t[9];
		t[6].parent = &t[2];
		t[7].parent = &t[4];
		t[8].parent = &t[5];
		t[9].right = &t[10];
		t[9].parent = &t[6];
		t[10].parent = &t[9];
		t[11].left = &t[12];
		t[11].right = &t[13];
		t[12].parent = &t[11];
		t[13].parent = &t[11];

		
		std::vector<int> const & result = s.getAllElements(&t[0], &t[11]);

		for (int const & i : result)
			std::cout << i << "   ";
		std::cout << '\n';
	}

	void Test_1305_All_Elements_in_Two_Binary_Search_Trees()
	{
		Solution2 s;
		Test1(s);
		Test2(s);
	}
}