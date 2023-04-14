#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<deque>
#include<iostream>

namespace _0133_Clone_Graph {
	using namespace std;

    class Node {
    public:
        int val;
        vector<Node*> neighbors;
        Node() {
            val = 0;
            neighbors = vector<Node*>();
        }
        Node(int _val) {
            val = _val;
            neighbors = vector<Node*>();
        }
        Node(int _val, vector<Node*> _neighbors) {
            val = _val;
            neighbors = _neighbors;
        }
    };

	class Solution
	{
	public:
		Node* cloneGraph(Node* node)
		{
            deque<Node*> original;
            deque<Node*> clone;
            unordered_map<int, Node*> cloneMap;

            if (node == nullptr)
            {
                return nullptr;
            }

            Node* n = new Node{node->val};
            original.emplace_back(node);
            clone.emplace_back(n);

            while (original.size() > 0)
            {
                Node* n1 = original.front();
                Node* n2 = clone.front();

                for (Node*& k : n1->neighbors)
                {
                    if (cloneMap.count(k->val) == 1)
                    {
                        n2->neighbors.emplace_back(cloneMap[k->val]);
                    }
                    else
                    {
                        Node* t = new Node{ k->val };
                        cloneMap[k->val] = t;
                        n2->neighbors.emplace_back(t);
                        original.emplace_back(k);
                        clone.emplace_back(t);
                    }
                }
                original.pop_front();
                clone.pop_front();
            }
            return cloneMap[node->val];
		}
	};

    static void walkTwoGraphsRecursive(Node* n1, Node* n2, unordered_set<int>&visited)
    {
        cout << n1->val << "  " << n1 << "        ";
        copy(n1->neighbors.begin(), n1->neighbors.end(), ostream_iterator<void*>(std::cout, "  "));
        cout << "          " << n2->val << "  " << n2 << "        ";
        copy(n2->neighbors.begin(), n2->neighbors.end(), ostream_iterator<void*>(std::cout, "  "));
        cout << "\n\n";

        visited.insert(n1->val);
        size_t sz = n1->neighbors.size();
        for (size_t i = 0; i < sz; i++)
        {
            if (visited.count(n1->neighbors[i]->val) == 0)
            {
                walkTwoGraphsRecursive(n1->neighbors[i], n2->neighbors[i], visited);
            }
        }
    }

    /*
    * Attention, potential danger of this destroy function
    *         1
    *        / \
    *       2---3
    * start from '1', go to '2', go to '3', delete '3', return to '2', delete '2'
    * return to '1', now '1' still has a pointer pointing to '3', that pointer is now
    * invalid, but I have to dereference that pointer in order to read the value of
    * this neighboring node of '1', even though "visited" can tell me that that node
    * with value '3' is already visited (and deleted).
    * In another word, I need to dereference a neighbor's pointer to know that neighbor's value.
    * Changing to a 'breadth-first' strategy won't work either. After all the problem
    * is that there are multiple pointers pointing at one object, once that object
    * is deleted, all the pointers become invalid, and I have no way to know where
    * those pointers are when I delete the object.
    */

    /*
    * shared_ptr is probably designed for this purpose!
    */

    /*
    * For a graph like this:
    *    1 ------ 2
    *    |        |
    *    4 ------ 3
    * build a separate map
    * 1: [2, 3]
    * 2: [1, 3]
    * 3: [2, 4]
    * 4: [1, 3]
    * basically records of values of each node's neighbors.
    * The order of the values must be the same as the actual order of the neighbors in the graph.
    * So when I get back to '1', I check whether the value '3' is in "visited", it is, then skip it.
    * This way I don't need to dereference the neighbor's pointer, instead I read '3' from this
    * separate map.
    */

    static void buildNeighborValuesListRecursive(Node* n, unordered_set<int>& visited, unordered_map<int, vector<int>>& m)
    {
        visited.insert(n->val);
        for (Node*& k : n->neighbors)
        {
            m[n->val].push_back(k->val);
        }
        for (Node*& k : n->neighbors)
        {
            if (visited.count(k->val) == 0)
                buildNeighborValuesListRecursive(k, visited, m);
        }
    }


    static void destroyGraphRecursive(Node* n, unordered_set<int>& visited, unordered_map<int, vector<int>>& m)
    {
        visited.insert(n->val);

        size_t sz = n->neighbors.size();

        for (size_t i = 0; i < sz; i++)
        {
            if (visited.count(m[n->val][i]) == 0)
            {
                destroyGraphRecursive(n->neighbors[i], visited, m);
            }
        }
        delete n;
    }

    static void destroyGraph(Node* n)
    {
        unordered_set<int> visited;
        unordered_map<int, vector<int>>m;
        buildNeighborValuesListRecursive(n, visited, m);

        visited.clear();
        destroyGraphRecursive(n, visited, m);
    }

    void Test_0133_Clone_Graph()
    {
        /*
        *    1 ------ 2
        *    |        |
        *    4 ------ 3
        */
        
        Node g1[4];
        g1[0].val = 1;
        g1[1].val = 2;
        g1[2].val = 3;
        g1[3].val = 4;
        
        g1[0].neighbors.emplace_back(&g1[1]);
        g1[0].neighbors.emplace_back(&g1[3]);

        g1[1].neighbors.emplace_back(&g1[0]);
        g1[1].neighbors.emplace_back(&g1[2]);

        g1[2].neighbors.emplace_back(&g1[1]);
        g1[2].neighbors.emplace_back(&g1[3]);
        
        g1[3].neighbors.emplace_back(&g1[0]);
        g1[3].neighbors.emplace_back(&g1[2]);

        Solution s;

        Node* newGraph = s.cloneGraph(&g1[0]);

        unordered_set<int> visited;
        walkTwoGraphsRecursive(&g1[0], newGraph, visited); // for the repeatedly used data structure (visited in this case), I must create it outside the recursive function and pass it in.

        destroyGraph(newGraph);
    }
}