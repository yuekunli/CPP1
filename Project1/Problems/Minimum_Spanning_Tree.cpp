#include<vector>
#include<string>
#include<iostream>


using namespace std;

namespace Minimum_Spanning_Tree {

	struct Node
	{
		int id;
		string name;
	};

	struct Edge
	{
		struct Node* u;
		struct Node* v;
		int weight;
	};



	struct HeapElementData;

	struct HeapElement
	{
		struct HeapElementData* cannonicalPos;
		int key;
	};

	struct HeapElementData
	{
		struct HeapElement* he;
		size_t index_in_heap;
		Node* data;
		Node* tree_edge_source;
	};



	struct AdjInfoNode
	{
		struct Node* n;
		int weight;
		struct AdjInfoNode* next;

		AdjInfoNode(Node* _n, int _weight, AdjInfoNode* _next) : n(_n), weight(_weight), next(_next) {}
	};



	class Minimum_Spanning_Tree
	{

		void swap_elements(vector<HeapElement>& h, size_t a, size_t b)
		{
			HeapElement temp(h[a]);

			HeapElementData* a_cannonicalPos = h[a].cannonicalPos;
			HeapElementData* b_cannonicalPos = h[b].cannonicalPos;

			a_cannonicalPos->he = &h[b];
			b_cannonicalPos->he = &h[a];
			swap(a_cannonicalPos->index_in_heap, b_cannonicalPos->index_in_heap);
			h[a] = h[b];
			h[b] = temp;
		}


		/*
		*                 0
		*        1                2
		*    3       4        5       6
		* 
		*  child index : x
		*  parent index: (x-1) / 2
		* 
		*  parent index: x
		*  left child: 2x + 1
		*  right child: 2x + 2
		*/

		void min_heap_decrease_key(vector<HeapElement>& h, size_t i, int newKey)
		{
			h[i].key = newKey;
			if (i == 0)
				return;

			size_t p_index = (i - 1) / 2;

			while (p_index > 0 && h[p_index].key > h[i].key)
			{
				swap_elements(h, p_index, i);
				i = p_index;
				p_index = (i - 1) / 2;

			}
			if (p_index == 0 && h[p_index].key > h[i].key)
				swap_elements(h, p_index, i);
		}

		HeapElement remove_min(vector<HeapElement>& h, size_t heap_length)
		{
			HeapElement ret = h[0];
			
			if (heap_length == 1)
				return ret;

			swap_elements(h, 0, heap_length - 1);
			heap_length--;

			size_t last_valid = heap_length-1;

			size_t i = 0,  left = 1, right = 2;
			
			while (left <= last_valid && right <= last_valid && (h[i].key > h[left].key || h[i].key > h[right].key))
			{
				if (h[i].key > h[left].key && h[right].key >= h[left].key) // left is the least
				{
					swap_elements(h, i, left);
					i = left;
					
				}
				else if (h[i].key > h[right].key && h[left].key >= h[right].key) // right is the least
				{
					swap_elements(h, i, right);
					i = right;
				}
				left = 2 * i + 1;
				right = 2 * i + 2;
			}

			if (left <= last_valid && right > last_valid && h[i].key > h[left].key)
				swap_elements(h, i, left);

			return ret;
		}

	public:

		void trim_down(vector<Node> & nodes, vector<AdjInfoNode*>& adj, vector<Edge>& tree)
		{
			size_t len = adj.size();
			
			vector<HeapElement> h(len);

			vector<HeapElementData> a(len);  // cannonically ordered nodes

			for (size_t i = 0; i < len; i++)
			{
				h[i].key = i == 0 ? 0 : INT_MAX;
				h[i].cannonicalPos = &a[i];
				a[i].data = &nodes[i];
				a[i].tree_edge_source = NULL;
				a[i].he = &h[i];
				a[i].index_in_heap = i;
			}

			//vector<Edge> tree;

			while (true)
			{
				HeapElement min_element = remove_min(h, len);
				
				if (min_element.key == INT_MAX)
					break;

				Node* n = (Node*)min_element.cannonicalPos->data;
				Node* p = (Node*)min_element.cannonicalPos->tree_edge_source;
				
				tree.emplace_back(p, n, min_element.key);

				len--;

				if (len == 0)
					break;

				AdjInfoNode* adj_node = adj[n->id];
				while (adj_node != NULL)
				{
					int adj_node_id = adj_node->n->id;
					if (a[adj_node_id].index_in_heap < len)
					{
						int new_distance_to_tree = adj_node->weight;
						int old_distance_to_tree = a[adj_node_id].he->key;
						if (new_distance_to_tree < old_distance_to_tree)
						{
							min_heap_decrease_key(h, a[adj_node_id].index_in_heap, new_distance_to_tree);
							a[adj_node_id].tree_edge_source = n;
						}
					}
					adj_node = adj_node->next;
				}
			}
			//return tree;
		}

		void show(vector<Edge> const & tree)
		{
			for (Edge const& e : tree)
			{
				if (e.u != NULL)
					cout << e.u->id << "   " << e.v->id << "   " << e.weight << endl;
			}
		}
	};



	/*
	*                   0
	*                 / | \
	*                /  |  \
	*           13  /   |   \
	*              /    |    \  16
	*             /  15 |     \
    *            /      |      \
	*           1-------2-------3
	*           |   11      19  |
	*            \_____________/
	*                  27
	* 
	*   expected minimum spanning tree:
	* 
	*                    0
	*                   / \
	*                  /   \
	*                 /     \
	*                1--2    3
	* 
	*/
	void Test1()
	{
		vector<Node> nodes(4);
		for (size_t i = 0; i < 4; i++)
		{
			nodes[i].id = i;
		}

		AdjInfoNode *current, * p;

		vector<AdjInfoNode*> adj(4);

		p = new AdjInfoNode(&nodes[1], 13, NULL);
		adj[0] = p;
		current = p;
		p = new AdjInfoNode(&nodes[2], 15, NULL);
		current->next = p;
		current = p;
		p = new AdjInfoNode(&nodes[3], 16, NULL);
		current->next = p;

		p = new AdjInfoNode(&nodes[0], 13, NULL);
		adj[1] = p;
		current = p;
		p = new AdjInfoNode(&nodes[2], 11, NULL);
		current->next = p;
		current = p;
		p = new AdjInfoNode(&nodes[3], 27, NULL);
		current->next = p;

		p = new AdjInfoNode(&nodes[0], 15, NULL);
		adj[2] = p;
		current = p;
		p = new AdjInfoNode(&nodes[1], 11, NULL);
		current->next = p;
		current = p;
		p = new AdjInfoNode(&nodes[3], 19, NULL);
		current->next = p;
		
		p = new AdjInfoNode(&nodes[0], 16, NULL);
		adj[3] = p;
		current = p;
		p = new AdjInfoNode(&nodes[1], 27, NULL);
		current->next = p;
		current = p;
		p = new AdjInfoNode(&nodes[2], 19, NULL);
		current->next = p;

		Minimum_Spanning_Tree mst;
		vector<Edge> minimum_spanning_tree;
		mst.trim_down(nodes, adj, minimum_spanning_tree);
		mst.show(minimum_spanning_tree);


		current = adj[0];
		p = current->next;
		delete current;
		current = p;
		p = current->next;
		delete current;
		delete p;

		delete adj[1]->next->next->next;
		delete adj[1]->next->next;
		delete adj[1];

		delete adj[2]->next->next->next;
		delete adj[2]->next->next;
		delete adj[2];

		delete adj[3]->next->next->next;
		delete adj[3]->next->next;
		delete adj[3];
	}

	void Test2()
	{
		vector<Node> n(9);
		vector<AdjInfoNode*> a(9);
		string s[9] = { "a", "b", "c", "d", "e", "f", "g", "h", "i" };
		for (size_t i = 0; i < 9; i++)
		{
			n[i].id = i;
			n[i].name = s[i];
		}

		AdjInfoNode* c, * p;

		p = new AdjInfoNode(&n[1], 4, NULL);
		a[0] = p;
		c = p;
		p = new AdjInfoNode(&n[7], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[7], 11, NULL);
		a[1] = p;
		c = p;
		p = new AdjInfoNode(&n[0], 4, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[2], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[3], 7, NULL);
		a[2] = p;
		c = p;
		p = new AdjInfoNode(&n[5], 4, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[8], 2, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[1], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[5], 14, NULL);
		a[3] = p;
		c = p;
		p = new AdjInfoNode(&n[2], 7, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[4], 9, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[3], 9, NULL);
		a[4] = p;
		c = p;
		p = new AdjInfoNode(&n[5], 10, NULL);
		c->next = p;
		
		p = new AdjInfoNode(&n[6], 2, NULL);
		a[5] = p;
		c = p;
		p = new AdjInfoNode(&n[2], 4, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[4], 10, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[3], 14, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[7], 1, NULL);
		a[6] = p;
		c = p;
		p = new AdjInfoNode(&n[8], 6, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[5], 2, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[8], 7, NULL);
		a[7] = p;
		c = p;
		p = new AdjInfoNode(&n[6], 1, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[1], 11, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[0], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[2], 2, NULL);
		a[8] = p;
		c = p;
		p = new AdjInfoNode(&n[6], 6, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[7], 7, NULL);
		c->next = p;


		vector<Edge> mst;

		Minimum_Spanning_Tree MST;
		MST.trim_down(n, a, mst);
		MST.show(mst);

		for (size_t i = 0; i < 9; i++)
		{
			c = a[i];
			
			while (c != NULL)
			{
				p = c->next;
				delete c;
				c = p;
			}
		}
	}

	void Test_Minimum_Spanning_Tree()
	{
		//Test1();
		Test2();
	}

}