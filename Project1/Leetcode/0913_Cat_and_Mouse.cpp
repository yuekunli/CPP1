#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<deque>
#include<unordered_set>
#include<iomanip>

namespace _0913_Cat_and_Mouse {

	using namespace std;


	/*
	* Mouse's best chance to win is just go straight to the hole by shortest path.
	* Cat's strategy is to get to the node that is one hop away from the hole and is on mouse's best-chance path
	* Whoever gets to that one-hop-away node sooner wins.
	* For example, there are 3 different shortest paths for mouse to reach the hole. There will be 3 different
	* nodes that are one-hop away from the hole that can be reached by the mouse. Cat must have a way to reach
	* each of those 3 one-hop nodes sooner than mouse can. This condition determines whether mouse can win or
	* the game ends in tie.
	* What is the condition for cat to win?
	* Between node 1 to the hole, there is a node that is the choke point between single path and multi path,
	* if the cat can reach that point sooner than the mouse can, it can trap the mouse.
	*                     1
	*                     |
	*                     a
	*                     |
	*                     x
	*              +------+------+
	*              b             c
	*             ....          ...
	*                +           |
	*                |           |
	*                0-----------+
	* node x is the choke point in this case, if cat can reach x faster, it can trap the mouse within 1 to x.
	*/

	/*
	* "Mouse's best chance to win is just go straight to the hole by shortest path."
	* 
	* No, this statement is wrong!
	* 
	*       1--a--b--c--d
	*       |           |
	*    2--x           e
	*       |           |
	*       0--f--g--h--i
	* 
	* Obviously, mouse can win by going along the longer route to hole.
	* And mouse will be caught if it follows the shortest path to hole.
	* 
	*/

	/*
	*      1-------3
	*      |       |
	*      2-------4
	*       \     /
	*        \   /
	*          0
	* 
	*  1st call: mouseAt 1, catAt 2
	*  2nd call: mouseAt 3, catAt 4
	*  3rd call: mouseAt 1, catAt 2
	*  ...
	*  How do I get out of this dead loop?
	* 
	*/

#define DEBUG 0
	
	class Solution
	{
		/*
		* this solution uses depth-first-search
		* It always consider the "state" of the program from mouse's perspective.
		* In another word, the "state" of the program is: mouse's and cat's positions *when it's mouse's turn*.
		* The obvious question is why don't I consider things when it's cat's turn?
		* If I have more dimensions to define the state of the program, I'll use more memory,
		* but the available transitions when I'm at a certain state is probably easier to figure out.
		* On the other hand, if I have fewer dimensions to define a state, I save some
		* memory but the logic becomes complicated.
		* The biggest problem of this solution is to identify the "states" I have visited.
		* I certainly need to consider both cat's position and mouse's position, so I make a
		* hash based on these two numbers (cat's and mouse's positions). There are at mose 50
		* nodes in the graph (constraints), so if I use a 4-digit number to represent both players'
		* position, that should create a unique number wherever they are.
		* 
		* There is a question about this solution.
		* For example, there are a few states of the program:
		*            state A   -->  ...... -->  state C
		*               ^                           |
		*               |----------------------------
		* (Although I draw arrows, it's an undirected graph)
		* I visite state A first then eventually visit state C. 
		* By the time I examine state C, state A is still "under examination".
		* This is just the nature of depth-first-search, there can be back edges.
		* So I don't know the outcome of state A. Let's say state C has 5 "next" states.
		* One of the 5 is state A and state A's outcome is undetermined. Do I say one of the 5 "next" states
		* of C is a DRAW (i.e. state A)?
		* Looks like I have to do so, because what else can I do? state A depends on state C and vice versa.
		* Neither one can be decided. So mouse and cat can keep changing their position so that the state of
		* the program keeping alternating between A and C. That is exactly a DRAW situation.
		*/

	private:

		unordered_set<int> visited; // hash of mouse's and cat's positions

		/*
		* 1: mouse win; 2: cat win; 0: draw
		*/
		int gameJudge(int** t, vector<vector<int>>& g, int mouseAt, int catAt)
		{
			if (t[mouseAt][catAt] != -1)
			{
				return t[mouseAt][catAt];
			}

			if (mouseAt == 0)
			{
				t[mouseAt][catAt] = 1;
				if (DEBUG) cout << '\n' << mouseAt << " " << catAt << " -- " << '1';
				return 1;
			}
			if (mouseAt == catAt)
			{
				t[mouseAt][catAt] = 2;
				if (DEBUG) cout << '\n' << mouseAt << " " << catAt << " -- " << '2';
				return 2;
			}

			vector<int>& mouseNext = g[mouseAt];
			vector<int>& catNext = g[catAt];

			/*
			* I don't really need to do these two special processings, let the program transition,
			* these two special cases will eventually be handled by "moustAt == 0" or "mouseAt == catAt"
			* 
			if (find(mouseNext.begin(), mouseNext.end(), 0) != mouseNext.end())  // if mouse can move to node#0 in its next move, it wins.
			{
				t[mouseAt][catAt] = 1;
				cout << '\n' << mouseAt << " " << catAt << " -- " << '1';
				return 1;
			}
			if (mouseNext.size() == 1 && mouseNext[0] == catAt) // if mouse is trapped, i.e. its only next move is to move in where cat is now
			{
				t[mouseAt][catAt] = 2;
				cout << '\n' << mouseAt << " " << catAt << " -- " << '2';
				return 2;
			}
			*/

			bool catAlwaysWin = true;
			bool mouseCanWin = false;
			for (int i = 0; i < mouseNext.size(); i++)
			{
				bool mouseAlwaysWin = true;
				bool catCanWin = false;
				bool canDraw = false;

				// consider the case in which cat doesn't move!!
				{
					if (mouseNext[i] == catAt)
					{
						catCanWin = true;
						goto TallyResultAfterCheckingEveryCatMove;
					}
				}
				
				for (int j = 0; j < catNext.size(); j++)
				{
					if (catNext[j] == 0)  // what if there is only one node connected to cat's current node, and that one node is #0, basically cat can't move
						continue;

					int hash = mouseNext[i] * 100 + catNext[j];
					if (visited.find(hash) != visited.end())
					{
						canDraw = true;
						continue;
					}

					visited.insert(hash);
					int outcome = gameJudge(t, g, mouseNext[i], catNext[j]);
					//visited.erase(hash);  // I must not erase the hash of the visited "states", this is not backtracking and testing one potential choice for a certain step

					if (outcome == 1) // mouse win
						continue;
					else if (outcome == 2) // cat win
					{
						catCanWin = true;
						break;
					}
					//else
						//canDraw = true;  // if I do call visited.erase(hash) on a few lines above, I must keep this "else" case
					                       // because I previously determined DRAW "state" is re-visited and quickly returned because of the memoization
					                       // but the execution will come to here, and I need to set the canDraw variable.
					                       // if I don't call visited.erase(hash), then I don't need this line here, because canDraw will be set
					                       // right after I found out that "visited" has the "state"
				}

			    TallyResultAfterCheckingEveryCatMove:
				// from cat's point of view, as long as there is one way for cat to win, cat will make that move.
				// as long as cat can win in this situation (i.e. mouse has made a certain move), we move on to
				// checking the next possible mouse move, the "catAlwaysWin" is set to true by default.
				if (catCanWin)
					continue;

				if (canDraw)
				{
					mouseAlwaysWin = false;
					catAlwaysWin = false;
					continue;
				}

				if (mouseAlwaysWin)
				{
					mouseCanWin = true;
				}
				
			    //TallyResultForOneParticularMouseMove:
				if (mouseCanWin)
				{
					t[mouseAt][catAt] = 1;
					if (DEBUG) cout << '\n' << mouseAt << " " << catAt << " -- " << '1';
					return 1;
				}
			}

			if (catAlwaysWin)
			{
				t[mouseAt][catAt] = 2;
				if (DEBUG) cout << '\n' << mouseAt << " " << catAt << " -- " << '2';
				return 2;
			}

			t[mouseAt][catAt] = 0;
			if (DEBUG) cout << '\n' << mouseAt << " " << catAt << " -- " << '0';
			return 0;
		}

	public:

		int catMouseGame(vector<vector<int>>& graph)
		{
			visited.clear();

			size_t len = graph.size();
			int** t = new int* [len];
			for (int i = 0; i < len; i++)
			{
				t[i] = new int[len];
				for (int j = 0; j < len; j++)
					t[i][j] = -1;
			}

			int hash = 1 * 100 + 2;
			visited.insert(hash);
			int outcome = gameJudge(t, graph, 1, 2);

			if (DEBUG)
			{
				cout << "\n\n";
				cout << setw(3) << " ";
				for (int i = 0; i < len; i++)
					cout << setw(3) << i;
				cout << '\n';
			}
			for (int i = 0; i < len; i++)
			{
				if (DEBUG)
				{
					cout << setw(3) << i;
					for (int j = 0; j < len; j++)
						cout << setw(3) << (t[i][j] == -1 ? "." : to_string(t[i][j]));
					cout << '\n';
				}
				delete[] t[i];
			}
			delete[] t;

			return outcome;
		}
	};


	class Solution2  // accepted
	{
		/*
		* leetcode solution
		* a backward breadth-first-search approach.
		* Find the a few states that are clearly mouse's wins or cat's wins by definition.
		* Tracing from these clear states backward to the starting position.
		* Mark each state's outcome along the way.
		* 
		* 
		* The key is that the the nodes in the BFS tree are not the nodes in the input graph.
		* A node in the BFS tree represents a "state" of the program's execution/progression.
		* And a "state" of the progression is a composite of information from a few "dimensions"
		* For example, the node where mouse is at is one of the dimensions. And whose turn it is
		* is also a dimension of a "state". 
		*/
	public:

		enum class Outcome
		{
			Draw = 0,
			MouseWin = 1,
			CatWin = 2,
		};

		struct ProgramState
		{
			/*
			* It feels like I don't need the three pieces of basic info (mouse's position, cat's position,
			* and "whose turn") because all the objects of this struct are stored in a 3-dimension array.
			* The position of a particular object in the 3-D array has three pieces of information already.
			* (i.e. the coordinates of the 3 dimensions)
			* But I need them because when they are put in the deque, their dimensional coordinates are lost,
			* they have to carry those three pieces of data by themselves.
			*/
			int mousePosition;
			int catPosition;
			int turn;  // 0: mouse's turn    1: cat's turn
			Outcome outcome{Outcome::Draw};
			int undecidedNextStates;
			bool bfsVisited{ false };
		};

		int catMouseGame(vector<vector<int>>& graph)
		{
			size_t len = graph.size();
			vector<vector<vector<ProgramState>>> states(len, vector<vector<ProgramState>>(len, vector<ProgramState>(2)));
			// I usually don't need to pre-allocate every possible "state" when using breadth-first-search.
			// But this problem is a little unusual. Because this is a backward BFS, if a certain state has 5 children,
			// it can be visited 5 times from 5 different children. So I need the object for this state to be saved
			// somewhere outside the BFS queue.

			// there are 3 dimensions that define the "state" of the program's progression.
			// 1. moust's position, 2. cat's position, 3. whose turn it is. So I need 3-D array

			deque<ProgramState*> q;

			// initialize the basic information of each ProgramState objects
			for (int i = 0; i < len; i++)
			{
				for (int j = 0; j < len; j++)
				{
					for (int k = 0; k < 2; k++)
					{
						states[i][j][k].mousePosition = i;
						states[i][j][k].catPosition = j;
						states[i][j][k].turn = k;

						// if mouse is at node#3, cat is at node#7, it's mouse's turn
						// if the input graph has nodes #5 #8 #2 connected to node#3
						// this "state" can transit to "mouse at #5, cat at #7, cat's turn"
						// "moust at #8, cat at #7, cat's turn", and "moust at #2, cat at #7, cat's turn"
						if (k == 0)  // it's mouse's turn, which node mouse is at now matters
							states[i][j][k].undecidedNextStates = (int)graph[i].size();
						else
						{
							// it's cat's turn, which node cat is at now matters
							// if '0' is one of the nodes connected to where cat is now,
							// cat must not go to that node, so the next states of this state
							// have to exclude the situation where cat going to node#0
							if (find(graph[j].begin(), graph[j].end(), 0) != graph[j].end())
								states[i][j][k].undecidedNextStates = (int)graph[j].size() - 1;
							else
								states[i][j][k].undecidedNextStates = (int)graph[j].size();

							// precisely count undecided next states is very important
							// the way I initialize special states is also very important
							// for example I assume cat can never be in node#0 so I don't
							// initialize states[0][0][0] or states[0][0][1]. This decision
							// must go hand in hand with the decision that I exclude the cat going
							// to node#0 when counting undecided next states.
						}
					}
				}
			}

			// initialize the outcome of some special ProgramState objects
			for (int j = 1; j < len; j++)  // cat can never be in node#0
			{
				for (int k = 0; k < 2; k++)
				{
					states[0][j][k].outcome = Outcome::MouseWin;
					//     |
					//     as long as mouse's position is 0, the other two dimensions don't matter, it's mouse win.
					states[0][j][k].bfsVisited = true;
					q.emplace_back(&states[0][j][k]);
				}
			}

			for (int i = 1; i < len; i++) // if both in the same node, cat wins, but both can't be in node#0 simultaneously
			{
				for (int k = 0; k < 2; k++)
				{
					states[i][i][k].outcome = Outcome::CatWin;
					states[i][i][k].bfsVisited = true;
					q.emplace_back(&states[i][i][k]);
				}
			}

			while (q.size() > 0)
			{
				ProgramState* ps = q.front();
				q.pop_front();

				if (ps->outcome == Outcome::MouseWin && ps->turn == 1) // it's cat's turn now, but it's a mouse's win state
																		// mouse made a move from a parent of this state to this state
				{
					// set the parents of this "state" to be mouse win
					vector<ProgramState*>const& parents = getParents(states, ps, graph);
					
					for (ProgramState* const& parent : parents)
					{
						parent->outcome = Outcome::MouseWin;
						parent->bfsVisited = true;
						q.emplace_back(parent);
					}
				}
				else if (ps->outcome == Outcome::CatWin && ps->turn == 0) // it's mouse's turn now, but it's a cat's win state
				{
					// set the parents of this "state" to be cat's win
					vector<ProgramState*>const& parents = getParents(states, ps, graph);
					for (ProgramState* const& parent : parents)
					{
						parent->outcome = Outcome::CatWin;
						parent->bfsVisited = true;
						q.emplace_back(parent);
					}
				}
				else if (ps->outcome == Outcome::MouseWin && ps->turn == 0)
				{
					// it's mouse win state but it's also mouse's turn
					// this means the previous move was made by cat
					// why would the cat make a move to a mouse's win state?
					// because the cat has no other options, that parent state is surrounded by mouse's win states
					vector<ProgramState*>const& parents = getParents(states, ps, graph);
					for (ProgramState* const& parent : parents)
					{
						parent->undecidedNextStates--;
						// What if the parent's undecided-next-states count was also decremented by some cat's win child state?
						// Don't worry. If "ps" is a "cat's win, mouse's turn" state, I don't even check its parent's
						// undecided-next-states count, I go straight to setting its parent's outcome to cat's win.
						// The parent is a cat's-turn state, it only has mouse's-turn child states.
						// And if this parent's outcome is still Draw, which is default, which means it hasn't been set,
						// which further means it doesn't have a "cat's win, mouse's turn" child.
						if (parent->outcome == Outcome::Draw && parent->undecidedNextStates == 0)
						{
							parent->outcome = Outcome::MouseWin;
							parent->bfsVisited = true;
							q.emplace_back(parent);
						}
					}
				}
				else if (ps->outcome == Outcome::CatWin && ps->turn == 1)
				{
					vector<ProgramState*>const& parents = getParents(states, ps, graph);
					for (ProgramState* const& parent : parents)
					{
						parent->undecidedNextStates--;
						if (parent->outcome == Outcome::Draw && parent->undecidedNextStates == 0)
						{
							parent->outcome = Outcome::CatWin;
							parent->bfsVisited = true;
							q.emplace_back(parent);
						}
					}
				}
			}
			return static_cast<int>(states[1][2][0].outcome);
		}

		vector<ProgramState*> getParents(vector<vector<vector<ProgramState>>>& states, ProgramState* ps, vector<vector<int>>&graph)
		{
			vector<ProgramState*> answer;
			if (ps->turn == 0) // it's mouse's turn now, so last move was made by cat, but cat must not come from where the mouse is at right now
			{
				vector<int>& connectedNodes = graph[ps->catPosition];
				for (int& nodeId : connectedNodes)
				{
					if (nodeId != ps->mousePosition && nodeId != 0) // cat must not come from node#0
						if (!states[ps->mousePosition][nodeId][1].bfsVisited)
							answer.emplace_back(&states[ps->mousePosition][nodeId][1]);
				}
			}
			else
			{   // it's cat's turn now, so last move was made by mouse
				vector<int>& connectedNodes = graph[ps->mousePosition];
				for (int& nodeId : connectedNodes)
				{
					if (nodeId != ps->catPosition)
						if (!states[nodeId][ps->catPosition][0].bfsVisited)
							answer.emplace_back(&states[nodeId][ps->catPosition][0]);
				}
			}
			return answer;
		}

	};


	void Test_0913_Cat_and_Mouse()
	{
		Solution s;

		vector<vector<int>>graphs[7];

		graphs[0] = {{2, 5}, {3}, {0, 4, 5}, {1, 4, 5}, {2, 3}, {0, 2, 3}};
		graphs[1] = {{1, 3}, {0}, {3}, {0, 2}};

		/*
		*      1-------3
		*      |       |
		*      2-------4
		*       \     /
		*        \   /
		*          0
		*/
		graphs[2] = {{2,4},{2,3},{0,1,4},{1,4},{2,3,0}};


		/*
		* 
		*       1------4----5
		*       |           |
		*    2--3           |
		*       |           |
		*       0-----------6
		* 
		*/

		graphs[3] = {{3,6},{3,4},{3},{0,1,2},{1,5},{4,6},{0,5}};


		/*
		*      1-------3--5--7--9
		*      |       |  |  |  |
		*      2-------4--6--8--10
		*       \               | 
		*        \              |
		*          0------------+
		*/

		graphs[4] = {{2,10},{2,3},{0,1,4},{1,4,5},{2,3,6},{3,6,7},{4,5,8},{5,8,9},{6,7,10},{7,10},{0,8,9}};


		/*
		*      1
		*      |\
		*   2--3-4
		*      |/
		*      0
		*/
		graphs[5] = { {3,4}, {3,4}, {3}, {0,1,2,4}, {0,1} };

		/*
		*         1--
		*         |  \
		*     2---3---4
		*         |   |
		*         0---5
		*/

		graphs[6] = { {3,5}, {3,4}, {3}, {0,1,2,4}, {1,3,5}, {0,4} };


		 /* 
		 * 0: tie       0
		 * 1: mouse win 1
		 * 2: tie       0
		 * 3: mouse win 1
		 * 4: tie       0
		 * 5: mouse win 1
		 * 6: mouse win 1
		 */

		int test_case;
		while (true)
		{
			cout << "test case (0 ~ 6)   (-1 exit)  : ";
			cin >> test_case;
			if (test_case == -1) break;
			int game_outcome = s.catMouseGame(graphs[test_case]);
			cout << "test case: " << test_case << ",   outcome: " <<
				(game_outcome == 1 ? "mouse win" : (game_outcome == 2 ? "cat win" : "draw")) << "\n\n";
		}
	}
}