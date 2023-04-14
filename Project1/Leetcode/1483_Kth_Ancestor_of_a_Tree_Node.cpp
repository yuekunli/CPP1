#include "LibHeaders.h"

namespace _1483_Kth_Ancestor_of_a_Tree_Node {

	using namespace std;

	/*
	* The algorithm is called binary lifting.
	* If I forget what it is, read nodes on OneNote and cp-algorithm.
	* I won't be able to figure it out just by reading this code here.
	*/

	class TreeAncestor
	{
		vector<vector<int>> children;  // basically the adjacency list in graph's terminology, 
							   // I may change this to a unordered_map, because about 
							   // half the nodes may be at the bottom of the tree therefore without children
		vector<vector<int>>ancestors;
		int binaryJumpAncestorListLength{ 0 };

		void buildLeapAncestorList(int node, int parent)
		{
			ancestors[node][0] = parent;
			for (int i = 1; i < binaryJumpAncestorListLength; i++)
			{
				if (ancestors[node][i - 1] != -1)
					ancestors[node][i] = ancestors[ancestors[node][i - 1]][i - 1];
				else
					break;
			}
			for (int const& i : children[node])
			{
				buildLeapAncestorList(i, node);
			}
		}

		void buildChildrenList(int n, vector<int>const & parent)
		{
			int len = parent.size();
			for (int i = 0; i < len; i++)
			{
				int p = parent[i];
				if (p != -1)
					children[p].emplace_back(i);
			}
		}

	public:

		TreeAncestor(int n, vector<int>&& parent) : TreeAncestor(n, parent)
		{
			//TreeAncestor(n, parent);  // I must not do this call on this line. It must be done on the above line.
			                            // Otherwise it's treated as if I'm constructing *another* TreeAncestor object here
			                            // then this temporary object immediately goes out of scope and gets destructed.
			                            // But the actually object I wanted never gets constructed.
		}

		TreeAncestor(int n, vector<int>& parent) // the problem's template stipulates "parent" is not a const reference, if I do call this constructor with rvalue, I need a rvalue overload
		{
			binaryJumpAncestorListLength = static_cast<int>(log2(n)) + 1;
			ancestors.assign(n, vector<int>(binaryJumpAncestorListLength, -1));

			children.resize(n);

			buildChildrenList(n, parent);
			buildLeapAncestorList(0, -1);
		}

		int getKthAncestor(int node, int k)
		{
			if (k == 0) return node;
			int a = static_cast<int>(log2(k));
			while (static_cast<int>(pow(2, a)) < k)
			{
				node = ancestors[node][a];
				k = k - static_cast<int>(pow(2, a));
				a = static_cast<int>(log2(k));
			}
			return ancestors[node][a];
		}

		// just try a different way to lay out the binary search logic, no essential difference
		int getKthAncestor2(int node, int k)
		{
			int probeLengthInLogarithm = binaryJumpAncestorListLength - 1;
			int realProbeLength = static_cast<int>(pow(2, probeLengthInLogarithm));
			while (k != realProbeLength)
			{
				if (k > realProbeLength)
				{
					node = ancestors[node][probeLengthInLogarithm];
					if (node == -1) return -1;
					probeLengthInLogarithm--;
					k = k - realProbeLength;
				}
				else
				{
					probeLengthInLogarithm--;
				}
				realProbeLength = static_cast<int>(pow(2, probeLengthInLogarithm));
			}
			return ancestors[node][probeLengthInLogarithm];
		}
	};


	static void Test1()
	{
		TreeAncestor ta{ 7, {-1,0,0,1,1,2,2} };
		cout << ta.getKthAncestor2(3, 1) << '\n'
			<< ta.getKthAncestor2(5, 2) << '\n'
			<< ta.getKthAncestor2(6, 3) << '\n';
	}

	static void Test2()
	{
		// a tree with one single branch, '0' being the root
		// 0 -- 1 -- 2 -- 3 -- ... -- 25
		vector<int> parent(26);
		iota(parent.begin(), parent.end(), -1);
		TreeAncestor ta{ 26, parent };

		int nodeId;
		int kthAncestor;
		while (true)
		{
			cout << "node id: (-1 to exit) > ";
			cin >> nodeId;
			if (nodeId == -1) break;
			cout << "kth ancestor: ";
			cin >> kthAncestor;
			cout << "ancestor node ID: " << ta.getKthAncestor2(nodeId, kthAncestor) << '\n';
		}
	}

	void Test_1483_Kth_Ancestor_of_a_Tree_Node()
	{
		//Test1();
		Test2();
	}
}