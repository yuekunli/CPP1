#include "LibHeaders.h"

namespace _0715_Range_Module {

	using namespace std;

#define DEBUG

	class RangeModule
	{
		map<int, int>m;
	public:
		RangeModule()
		{

		}

		void addRange(int left, int right)
		{
			if (m.size() == 0)
			{
				m[left] = right;
				return;
			}
			if (queryRange(left, right))
				return;

			auto it = m.upper_bound(left);
			auto it2 = m.upper_bound(right);

			map<int, int>::iterator start, end;  // intervals covered by this new interval

			if (it == m.end() && m.rbegin()->second < left)
			{
				m[left] = right;
				return;
			}
			else if (it2 == m.begin())
			{
				m[left] = right;
				return;
			}
			else if (it == it2 && prev(it)->second < left)
			{
				m[left] = right;
				return;
			}

			if (it == m.begin() || (prev(it))->second < left)
			{
				start = it;
			}
			else if ((prev(it))->second >= left)
			{
				start = prev(it);
			}
			// if (    it != m.begin()    &&    (prev(it))->second >= left    )
			//     start = prev(it);


			end = it2;

			int intervalEnd = max(right, prev(end)->second);
			int intervalStart = min(left, start->first);

			auto i = start;
			while (i != end)
			{
				auto j = next(i);
				m.erase(i);
				i = j;
			}

			m[intervalStart] = intervalEnd;
		}

		bool queryRange(int left, int right)
		{
			if (m.size() == 0) return false;

			auto it = m.upper_bound(left);

			if (it == m.end())
			{
				if (m.rbegin()->second <= left)
					return false;
				else if (m.rbegin()->second >= right)
					return true;
				else
					return false;
			}
			else if (it == m.begin())
			{
				return false;
			}
			else
			{
				auto a = prev(it);
				if (a->second <= left)
					return false;
				else if (a->second >= right)
					return true;
				else
					return false;
			}
		}

		void removeRange(int left, int right)
		{
			if (m.size() == 0) return;

			auto it = m.upper_bound(left);
			auto it2 = m.upper_bound(right);

			if (it2 == m.begin() || (it == it2 && prev(it)->second < left))
			{
				return;
			}

			if (it != m.begin() && (prev(it))->second >= left)
			{
				auto a = prev(it);

				if (a->first == left)
				{
					if (a->second <= right)
						m.erase(a);
					else
					{
						int intervalStart = right;
						int intervalEnd = a->second;
						m.erase(a);
						m[intervalStart] = intervalEnd;
						return;
					}
				}
				else
				{
					int oldEnd = a->second;
					a->second = left;
					if (oldEnd > right)
					{
						m[right] = oldEnd;
						return;
					}
				}
			}

			auto i = it;
			while (i != it2)
			{
				if (i->first >= right)
					return;
				else if (i->second <= right)
				{
					auto j = next(i);
					m.erase(i);
					i = j;
				}
				else
				{
					int intervalStart = right;
					int intervalEnd = i->second;
					m.erase(i);
					m[intervalStart] = intervalEnd;
					return;
				}
			}
		}

		void display()
		{
			for (auto& p : m)
				cout << p.first << "  " << p.second << "\n";
			cout << "\n";
		}
	};

	static void Test1()
	{
		RangeModule rm;
		rm.addRange(10, 20);
		rm.removeRange(14, 16);
		cout << boolalpha << rm.queryRange(10, 14) << '\n';
		cout << boolalpha << rm.queryRange(13, 15) << '\n';
		cout << boolalpha << rm.queryRange(16, 17) << '\n';
	}

	static void Test2()
	{
		RangeModule rm;

		cout << boolalpha << rm.queryRange(1,4) << '\n';
		cout << boolalpha << rm.queryRange(6,10) << '\n';
		rm.addRange(2, 6);
		rm.addRange(2, 8);
		rm.addRange(4, 7);
		cout << boolalpha << rm.queryRange(2,5) << '\n';
		rm.removeRange(1, 10);
		rm.removeRange(3, 5);
		rm.removeRange(1, 2);

	}

	static void Test3()
	{
		RangeModule r;
		r.addRange(14, 100); //r.display();
		r.removeRange(1, 8); //r.display();
		r.queryRange(77, 80);
		r.queryRange(8, 43);
		r.queryRange(4, 13);
		r.removeRange(3, 9); 
		r.removeRange(45, 49); r.display(); // [14,45], [49,100]
		r.removeRange(41, 90); r.display(); // [14,41], [90,100]
		r.addRange(58, 79); r.display();    // [14,41], [58,79], [90,100]
		r.addRange(4, 83); r.display();     // [4,83], [90,100]
		r.addRange(34, 39); r.display();    // [4,83], [90,100]
		r.removeRange(84, 100); r.display();// [4,83]
		r.addRange(8, 9); r.display();      // [4,83]
		cout<<boolalpha<<r.queryRange(32, 56)<<"\n";
	}

	void Test_0715_Range_Module()
	{
		Test3();
	}

}