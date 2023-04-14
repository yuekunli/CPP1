#include "LibHeaders.h"

namespace _0218_The_Skyline_Problem {

	using namespace std;

	class Solution1
	{
	public:

		struct Segment
		{
			int x{ 0 };
			int y{ 0 };
			int length{ 0 };
		};

		int getFromRange(vector<int>& biTree, int index)
		{
			int maximum = 0;
			while (index > 0)
			{
				int k = biTree[index];
				if (maximum < k)
					maximum = k;
				index = index - (index & (-index));
			}
			return maximum;
		}

		void setInRange(vector<int>& biTree, int index, int value)
		{
			while (index < biTree.size())
			{
				if (value > biTree[index])
					biTree[index] = value;
				index = index + (index & (-index));
			}
		}
		struct segmentComp
		{
			bool operator()(Segment const & a, Segment const & b) const
			{
				return a.x < b.x;
			}
		};

		vector<vector<int>> getSkyline(vector<vector<int>>& buildings)
		{
			int len = buildings.size();
			vector<int> posInSorted(len);
			vector<vector<int>*> pointers(len, nullptr);
			for (int i = 0; i < len; i++)
			{
				pointers[i] = &buildings[i];
			}
			sort(pointers.begin(), pointers.end(),
				[&](auto & a, auto &b)
				{
					return (*a)[2] > (*b)[2];
				});

			for (int i = 0; i < len; i++)
			{
				int d = pointers[i] - &buildings[0];
				posInSorted[d] = i;
			}

			vector<int> biTree(len + 1);

			set<Segment, segmentComp> segs;  
			// don't really need an ordered set here, 
			// I can put every segment in a sequential data structure and sort them afterward

			for (int i = 0; i < len; i++)
			{
				int index = posInSorted[i] + 1;
				int farEnd = getFromRange(biTree, index);
				if (farEnd <= buildings[i][0])
				{
					segs.insert(Segment(buildings[i][0], buildings[i][2], buildings[i][1] - buildings[i][0]));
				}
				else if (farEnd > buildings[i][0] && farEnd < buildings[i][1])
				{
					segs.insert(Segment(farEnd, buildings[i][2], buildings[i][1] - farEnd));
				}
				else
				{
					// nothing to add
				}
				setInRange(biTree, index, buildings[i][1]);
			}

			vector<Segment> tempStack;
			int currentSegEnd = 0;

			vector<vector<int>>answer;

			auto i = segs.begin();
			currentSegEnd = i->x;  // if the first building doesn't start at (0,0), I must not have {0,0} in the answer

			while (i != segs.end())
			{
				if (currentSegEnd >= i->x)
				{
					answer.push_back({ i->x, i->y });
					currentSegEnd = i->x + i->length;
					tempStack.push_back(*i);
				}
				else
				{
					while (currentSegEnd < i->x && tempStack.size() > 0)
					{
						auto s = tempStack.end() - 1;
						if (s->x + s->length > currentSegEnd)
						{
							answer.push_back({ currentSegEnd, s->y });
							currentSegEnd = s->x + s->length;
							
						}
						else
						{
							tempStack.erase(s);
						}
					}
					if (tempStack.size() == 0 && currentSegEnd < i->x)
					{
						answer.push_back({ currentSegEnd, 0 });
					}
					answer.push_back({i->x, i->y});
					currentSegEnd = i->x + i->length;
					tempStack.push_back(*i);
				}

				i = next(i);
			}

			for_each(tempStack.rbegin(), tempStack.rend(),
				[&](Segment& s)
				{
					if (s.x + s.length > currentSegEnd)
					{
						answer.push_back({currentSegEnd, s.y});
						currentSegEnd = s.x + s.length;
					}
				});

			answer.push_back({ currentSegEnd, 0 });

			/*
			bool foundDuplicate = false;
			for (int i = 0; i < answer.size(); i++)
			{
				if (i != answer.size() - 1)
				{
					if (answer[i][1] == answer[i + 1][1])
					{
						answer[i + 1][0] = -1;
						foundDuplicate = true;
					}
				}
			}

			if (foundDuplicate)
			{
				vector<vector<int>>cleanAnswer;
				for (int i = 0; i < answer.size(); i++)
				{
					if (answer[i][0] != -1)
						cleanAnswer.push_back(answer[i]);
				}
				return cleanAnswer;
			}
			*/

			return answer;
		}
	};


	class Solution2
	{
	public:
		struct Segment
		{
			int x1;  // start
			int x2;  // end
			int y;   // height
		};
		struct ComboSegment
		{
			int start;
			int end;
			vector<Segment> s;
		};
		vector<vector<int>> getSkyline(vector<vector<int>>& buildings)
		{
			// sort buildings by height in descending order

			sort(buildings.begin(), buildings.end(),
				[](auto& a, auto& b)
				{
					return a[2] > b[2];
				});

			map<int, ComboSegment> m;

			for (auto& b : buildings)
			{
				int bstart = b[0];
				int bend = b[1];
				int bheight = b[2];

				/*
				* Theoretically I don't need to deal with m.size()==0 and m.size()==1 cases outside general logic,
				* but if I do, it will make the general logic simpler, because when there is none or just 1 element
				* in the ordered-map, the return value of lower_bound can be one of many scenarios. If I know for sure there
				* are at least 2 elements in the map, dealing with the return of lower_bound becomes easier.
				*/
				if (m.size() == 0)
				{
					ComboSegment cs{ bstart, bend, {{bstart, bend, bheight}} };
					m.insert({ bstart, cs });
					continue;
				}
				if (m.size() == 1)
				{
					auto it = m.begin();
					ComboSegment& cs = it->second;
					
					if (cs.start <= bend && bstart <= cs.end)
					{
						// the only combo segment and the new segment overlap
						bool keyChange = false;
						if (bstart < cs.start)  // does the new segment extend the combo on the left side?
						{
							if (bheight == cs.s.begin()->y)
								cs.s.begin()->x1 = bstart;
							else
								cs.s.insert(cs.s.begin(), {bstart, cs.start, bheight});

							cs.start = bstart;
							keyChange = true;
						}
						if (cs.end < bend)  // does the new segment extend the combo on the right side?
						{
							if (bheight == cs.s.rbegin()->y)
								cs.s.rbegin()->x2 = bend;
							else
								cs.s.push_back({ cs.end, bend, bheight });

							cs.end = bend;
						}
						if (keyChange)
						{
							ComboSegment newCS = cs;
							m.erase(it);
							m.insert({ newCS.start, newCS });
						}
					}
					else
					{
						// I'm going to add a new combo segment that has 1 segment which is the segment under consideration
						ComboSegment cs{ bstart, bend, {{bstart, bend, bheight}} };
						m.insert({bstart, cs});
					}
					continue;
				}


				// I want to find the first combo segment that overlaps with this segment
				auto it1 = m.lower_bound(bstart);  // it1's start index is greater than or equal to bstart
				if (it1 != m.begin())   // the key in the map is a combe segment's start index,
					                    // the previous combo segment may have it's ending part overlap with this segment
				{
					auto it0 = prev(it1);
					if (bstart <= it0->second.end)
						it1 = it0;
				}

				// now it1 should be the first combo segment that has contact with this segment
				// it1 can be m.end() though
				// |---combo 1---| |---combo 2---|  |--segment---|

				if (it1 == m.end())
				{
					ComboSegment cs{ bstart, bend, {{bstart, bend, bheight}} };
					m.insert({ bstart, cs });
					continue;
				}

				// now it's guaranteed there must be some overlapping with existing combo segment(s)

				int currentX = bstart;  // the entire length of this building needs to be accounted.
				                        // this "currentX" tells me till where I have accounted.
				ComboSegment newcs;
				newcs.start = bstart < it1->second.start ? bstart : it1->second.start;
				while (it1 != m.end() && bend >= it1->second.start)
				{                       // once bend < it1->second.start, there is no more overlapping
					ComboSegment& cs = it1->second;
					if (currentX < cs.start)  // this means this combo segment needs to be extended on its left side
					{
						// but the extended part may have equal height as the first segment in the original combo
						// but I'm not dealing with that situation here, I'll let the answer-asembleying process deal with it
						newcs.s.push_back({ currentX, cs.start, bheight });
					}
					copy(cs.s.begin(), cs.s.end(), back_inserter(newcs.s));
					if (bend <= cs.end)
					{
						// this building ends inside this combo segment, so I'm done processing this building
						currentX = bend;
						m.erase(it1);
						break;
					}
					else
					{
						currentX = cs.end;
						auto it2 = it1;
						it1 = next(it1);
						m.erase(it2);
					}
				}
				if (currentX < bend)  // this means this building goes beyond all the combo segments that overlap with it
					newcs.s.push_back({ currentX, bend, bheight });
				m.insert({newcs.start, newcs});
			}

			vector<vector<int>> answer;
			buildAnswer(m, answer);
			return answer;
		}

		void buildAnswer(map<int, ComboSegment>& m, vector<vector<int>>& a)
		{
			auto it = m.begin();
			while (it != m.end())
			{
				vector<Segment>& segs = it->second.s;
				int end = 0;
				for (auto& se : segs)
				{
					if (a.size() > 0)
					{
						if ((*a.rbegin())[1] == se.y)  // deal with the consecutive segment with the same height problem.
						{
							end = se.x2;
							continue;
						}
					}
					a.push_back({ se.x1, se.y });
					end = se.x2;
				}
				a.push_back({ end, 0 });
				it = next(it);
			}
		}

	};

	// There is probably an even more generalized approach.
	// Just consider the segments by themselves, no combo segments.
	// When a new building is under examination, just search the segments
	// that can potentially be affected by this new build.

	void Test_0218_The_Skyline_Problem()
	{
		vector<vector<int>>buildings[] = {

			{{2, 9, 10}, {3, 7, 15}, {5, 12, 12}, {15, 20, 10}, {19, 24, 8}},

			{ {0,2,3}, {2,5,3} },

			{ {0,20,5}, {2,24,3}, {7,13,10} },

			{{0,10,5},{1,3,7},{3,6,5}},

			{{0,4,6},{2,6,5},{3,7,8}}, // this case proves that Solution1 has flaws

			{{1,3,5}, {6,9,5}, {1,6,5}} // how Solution2 deal with equal height
		};

		Solution2 s;
		int test;

		while (true)
		{
			cout << "test case  (there are totally " << size(buildings)<< ")  (0 to exit) : ";
			cin >> test;
			if (test == 0) break;

			vector<vector<int>> const& answer = s.getSkyline(buildings[test-1]);
			for (auto& seg : answer)
			{
				cout << "[" << seg[0] << "," << seg[1] << "], ";
			}
			cout << '\n';
		}
	}
}