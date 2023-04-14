#include "LibHeaders.h"

namespace _2102_Sequentially_Ordinal_Rank_Tracker {

	using namespace std;

	class SORTracker
	{
		struct Place
		{
			string name;
			int score{ 0 };
		};

		struct PlaceComp
		{
			bool operator()(Place const & a, Place const & b) const  // three "const"s in this definition, none can be omitted
			{
				if (a.score != b.score)
					return a.score > b.score;
				else
					return lexicographical_compare(a.name.begin(), a.name.end(), b.name.begin(), b.name.end());  // higher score or smaller name
			}
		};

		set<Place, PlaceComp> data;
		set<Place, PlaceComp>::iterator p = data.end();
		bool pointAtNextGet = false;
		bool firstAdd = true;

	public:
		
		SORTracker(){}

		void add(string name, int score)
		{
			data.insert(Place(name, score));

			if (firstAdd)
			{
				p = data.begin();
				firstAdd = false;
				pointAtNextGet = true;
				return;
			}

			if (score > p->score || (score == p->score && lexicographical_compare(name.begin(), name.end(), p->name.begin(), p->name.end())))
			{
				if (pointAtNextGet)
				{
					p = prev(p);
				}
				else
					pointAtNextGet = true;
			}
		}

		string get()
		{
			if (pointAtNextGet)
			{
				pointAtNextGet = false;
				return p->name;
			}
			else
			{
				p = next(p);
				return p->name;
			}
		}
	};


	class SORTracker2
	{
		/*
		* The same logic as SORTracker 1, this one just cleans up and tidies up things.
		*/
		struct Place
		{
			string name;
			int score{ 0 };
		};

		struct PlaceComp
		{
			bool operator()(Place const& a, Place const& b) const  // three "const"s in this definition, none can be omitted
			{
				if (a.score != b.score)
					return a.score > b.score;
				else
					return lexicographical_compare(a.name.begin(), a.name.end(), b.name.begin(), b.name.end());  // higher score or smaller name
			}
		};

		set<Place, PlaceComp> data;
		set<Place, PlaceComp>::iterator p;

	public:

		SORTracker2(){	}

		void add(string name, int score)
		{
			bool firstAdd = false;
			if (data.size() == 0)
				firstAdd = true;

			data.insert(Place(name, score));
			if (firstAdd)
			{
				p = data.begin();
			}
			else
			{
				if (PlaceComp{}(Place{ name,score }, *p))
				{
					p = prev(p);
				}
			}
		}

		string get()
		{
			string a = p->name;
			p = next(p);
			return a;
		}
	};


	void Test_2102_Sequentially_Ordinal_Rank_Tracker()
	{
		SORTracker2 tracker{};

		tracker.add("bradford", 2);
		tracker.add("branford", 3);
		cout<<tracker.get()<<'\n';
		tracker.add("alps", 2);
		cout << tracker.get() << '\n';
		tracker.add("orland", 2);
		cout << tracker.get() << '\n';
		tracker.add("orlando", 3);
		cout << tracker.get() << '\n';
		tracker.add("alpine", 2);
		cout << tracker.get() << '\n';
		cout << tracker.get() << '\n';
	}
}