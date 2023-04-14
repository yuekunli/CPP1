#include "LibHeaders.h"

namespace _2096_Step_Direction_Binary_Tree_Node_To_Another {

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
		enum class Found
		{
			Start = 0,
			Dest = 1,
			Both = 2,
			Neither = 3
		};

		int _startValue;
		int _destValue;

		Found dfs(TreeNode* n, Found status, string& s)
		{
			if (status == Found::Start)
			{
				if (n->val == _destValue)
					return Found::Dest;
				else
				{
					if (n->left)
					{
						Found result = dfs(n->left, status, s);
						if (result == Found::Dest)
						{
							s.append("L");
							return Found::Dest;
						}
					}
					if (n->right)
					{
						Found result = dfs(n->right, status, s);
						if (result == Found::Dest)
						{
							s.append("R");
							return Found::Dest;
						}
					}
				}
				return Found::Neither;
			}
			else if (status == Found::Dest)
			{
				if (n->val == _startValue)
					return Found::Start;
				else
				{
					if (n->left)
					{
						Found result = dfs(n->left, status, s);
						if (result == Found::Start)
						{
							s.append("L");
							return Found::Start;
						}
					}
					if (n->right)
					{
						Found result = dfs(n->right, status, s);
						if (result == Found::Start)
						{
							s.append("R");
							return Found::Start;
						}
					}
				}
				return Found::Neither;
			}
			else if (status == Found::Neither)
			{
				if (n->val == _startValue)
				{
					if (n->left)
					{
						Found result = dfs(n->left, Found::Start, s);
						if (result == Found::Dest)
						{
							s.append("L");
							reverse(s.begin(), s.end());
							return Found::Both;
						}
					}
					if (n->right)
					{
						Found result = dfs(n->right, Found::Start, s);
						if (result == Found::Dest)
						{
							s.append("R");
							reverse(s.begin(), s.end());
							return Found::Both;
						}
					}
					return Found::Start;
				}
				else if (n->val == _destValue)
				{
					if (n->left)
					{
						Found result = dfs(n->left, Found::Dest, s);
						if (result == Found::Start)
						{
							string a(s.size()+1, 'U');
							s = a;
							return Found::Both;
						}
					}
					if (n->right)
					{
						Found result = dfs(n->right, Found::Dest, s);
						if (result == Found::Start)
						{
							string a(s.size() + 1, 'U');
							s = a;
							return Found::Both;
						}
					}
					return Found::Dest;
				}
				else
				{
					Found resultLeft = Found::Neither, resultRight = Found::Neither;
					string toStart, toDest;
					if (n->left)
					{
						resultLeft = dfs(n->left, Found::Neither, s);
						if (resultLeft == Found::Start)
						{
							s.append("L");
							toStart = s;
							s.clear();
						}
						else if (resultLeft == Found::Dest)
						{
							s.append("L");
							toDest = s;
							s.clear();
						}
						else if (resultLeft == Found::Both)
						{
							return Found::Both;
						}
					}
					if (n->right)
					{
						resultRight = dfs(n->right, Found::Neither, s);
						if (resultRight == Found::Start)
						{
							s.append("R");
							toStart = s;
							s.clear();
						}
						else if (resultRight == Found::Dest)
						{
							s.append("R");
							toDest = s;
							s.clear();
						}
						else if (resultRight == Found::Both)
						{
							return Found::Both;
						}
					}
					if (resultLeft != Found::Neither && resultRight != Found::Neither)
					{
						reverse(toDest.begin(), toDest.end());
						string startToPivot(toStart.size(), 'U');
						s = startToPivot + toDest;
						return Found::Both;
					}
					else if (resultLeft != Found::Neither || resultRight != Found::Neither)
					{
						s = toStart.size() > 0 ? toStart : toDest;
						return toStart.size() > 0 ? Found::Start:Found::Dest;
					}
					else
					{
						return Found::Neither;
					}
				}
			}
			return Found::Neither;
		}

	public:
		string getDirections(TreeNode* root, int startValue, int destValue)
		{
			_startValue = startValue;
			_destValue = destValue;
			string s;
			dfs(root, Found::Neither, s);
			return s;
		}
	};



	class Solution2
	{
		/*
		* Exactly the same logic as Solution 1. Because the path to "start" is not important, it will eventually become a series of 'U'.
		* So don't record that path, just record its length.
		*/
		enum class Found
		{
			Start = 0,
			Dest = 1,
			Both = 2,
			Neither = 3
		};

		int _startValue;
		int _destValue;

		Found dfs(TreeNode* n, Found status, string& s, int& toStartPathLength)
		{
			if (status == Found::Start)
			{
				if (n->val == _destValue)
					return Found::Dest;
				else
				{
					if (n->left)
					{
						Found result = dfs(n->left, status, s, toStartPathLength);
						if (result == Found::Dest)
						{
							s.append("L");
							return Found::Dest;
						}
					}
					if (n->right)
					{
						Found result = dfs(n->right, status, s, toStartPathLength);
						if (result == Found::Dest)
						{
							s.append("R");
							return Found::Dest;
						}
					}
				}
				return Found::Neither;
			}
			else if (status == Found::Dest)
			{
				if (n->val == _startValue)
					return Found::Start;
				else
				{
					if (n->left)
					{
						Found result = dfs(n->left, status, s, toStartPathLength);
						if (result == Found::Start)
						{
							toStartPathLength++;
							return Found::Start;
						}
					}
					if (n->right)
					{
						Found result = dfs(n->right, status, s, toStartPathLength);
						if (result == Found::Start)
						{
							toStartPathLength++;
							return Found::Start;
						}
					}
				}
				return Found::Neither;
			}
			else if (status == Found::Neither)
			{
				if (n->val == _startValue)
				{
					if (n->left)
					{
						Found result = dfs(n->left, Found::Start, s, toStartPathLength);
						if (result == Found::Dest)
						{
							s.append("L");
							reverse(s.begin(), s.end());
							return Found::Both;
						}
					}
					if (n->right)
					{
						Found result = dfs(n->right, Found::Start, s, toStartPathLength);
						if (result == Found::Dest)
						{
							s.append("R");
							reverse(s.begin(), s.end());
							return Found::Both;
						}
					}
					return Found::Start;
				}
				else if (n->val == _destValue)
				{
					if (n->left)
					{
						Found result = dfs(n->left, Found::Dest, s, toStartPathLength);
						if (result == Found::Start)
						{
							s = string(toStartPathLength + 1, 'U');
							return Found::Both;
						}
					}
					if (n->right)
					{
						Found result = dfs(n->right, Found::Dest, s, toStartPathLength);
						if (result == Found::Start)
						{
							s = string(toStartPathLength + 1, 'U');
							return Found::Both;
						}
					}
					return Found::Dest;
				}
				else
				{
					Found resultLeft = Found::Neither, resultRight = Found::Neither;
					int _toStartPathLength = 0;
					if (n->left)
					{
						resultLeft = dfs(n->left, Found::Neither, s, toStartPathLength);
						if (resultLeft == Found::Start)
						{
							_toStartPathLength = toStartPathLength+1;
							toStartPathLength = 0;
						}
						else if (resultLeft == Found::Dest)
						{
							s.append("L");
						}
						else if (resultLeft == Found::Both)
						{
							return Found::Both;
						}
					}
					if (n->right)
					{
						resultRight = dfs(n->right, Found::Neither, s, toStartPathLength);
						if (resultRight == Found::Start)
						{
							_toStartPathLength = toStartPathLength + 1; // actually don't need these two lines here, they are necessary after searching the left subtree
							toStartPathLength = 0;
						}
						else if (resultRight == Found::Dest)
						{
							s.append("R");
						}
						else if (resultRight == Found::Both)
						{
							return Found::Both;
						}
					}
					if (resultLeft != Found::Neither && resultRight != Found::Neither)
					{
						reverse(s.begin(), s.end());
						s = string(_toStartPathLength, 'U') + s;
						return Found::Both;
					}
					else if (resultLeft != Found::Neither || resultRight != Found::Neither)
					{
						toStartPathLength = _toStartPathLength;
						/*
						* if I found "start" in the left subtree, then I wiped out the value in "toStartPathLength" before
						* searching the right subtree in order to not confuse myself when I'm in the right subtree.
						* If I have only found the "start" I need to restore the value in "toStartPathLength" so that the parent
						* nodes can keep incrementing that value.
						*/
						return _toStartPathLength > 0 ? Found::Start : Found::Dest;
					}
					else
					{
						return Found::Neither;
					}
				}
			}
			return Found::Neither;
		}

	public:
		string getDirections(TreeNode* root, int startValue, int destValue)
		{
			_startValue = startValue;
			_destValue = destValue;
			string s;
			int toStartPathLength = 0;
			dfs(root, Found::Neither, s, toStartPathLength);
			return s;
		}
	};


	static void Test1()
	{
		TreeNode j[7] = { {0},{1},{2},{3},{4},{5},{6} };
		j[5].left = &j[1];
		j[5].right = &j[2];
		j[1].left = &j[3];
		j[2].left = &j[6];
		j[2].right = &j[4];
		Solution2 s;
		cout << s.getDirections(&j[5], 3, 6) << "\n\n";
	}
	void Test_2096_Step_Direction_Binary_Tree_Node_To_Another()
	{
		Test1();
	}
}