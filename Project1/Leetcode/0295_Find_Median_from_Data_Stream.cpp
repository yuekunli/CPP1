#include<iostream>
#include<set>
#include<random>

namespace _0295_Find_Median_from_Data_Stream {

	using namespace std;

	class MedianFinder
	{
		multiset<int> data;
		multiset<int>::iterator p1 = data.end(), p2 = data.end();

		/*
		*  Total count odd:   0, 1, 2, 3, 4, 5, 6
		*                              |  |
		*                             p1  p2
		*
		* Total count even:  0, 1, 2, 3, 4, 5, 6, 7
		*                             |  |
		*                            p1  p2
		*
		*/


	public:
		MedianFinder()
		{
			data.clear();
			p1 = p2 = data.end();

		}

		void addNum(int num)
		{
			size_t sz = data.size();
			if (sz == 0)
			{
				data.insert(num);
				p1 = data.begin();
				return;
			}
			if (sz == 1)
			{
				data.insert(num);
				if (num < *p1)
				{
					p2 = p1;
					p1 = prev(p2);
				}
				else
				{
					p2 = next(p1);
				}
				return;
			}

			if (sz % 2 == 0)
			{
				data.insert(num);
				if (num < *p1)
				{
					// do nothing
				}
				else if (num >= *p1 && num < *p2)
				{
					p1 = next(p1);
				}
				else
				{
					p1 = p2;
					p2 = next(p1);
				}
			}
			else
			{
				data.insert(num);
				if (num < *p1)
				{
					p2 = p1;
					p1 = prev(p2);
				}
				else if (num >= *p1 && num < *p2)
				{
					p2 = next(p1);
				}
				else
				{
					// do nothing
				}
			}

		}

		double findMedian()
		{
			if (data.size() % 2 == 0)
			{
				return ((double)(*p1 + *p2)) / 2.0;
			}
			else
				return static_cast<double>(*p1);
		}
	};


	class AutoTest
	{
		double findMedianBruteForce(vector<int>& a)
		{
			size_t len = a.size();
			if (len % 2 == 0)
			{
				// if len = 10, I want a[4] and a[5]
				return (double(a[len / 2 - 1] + a[len / 2])) / 2.0;
			}
			else
			{
				return (double)a[(len - 1) / 2];
			}
		}

		void addNum(vector<int>& a, int n)
		{
			auto i = lower_bound(a.begin(), a.end(), n);
			if (i != a.end())
				a.insert(i, n);
			else
				a.push_back(n);
		}

	public:
		bool run()
		{
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<int> dist(-10000, 10000);

			MedianFinder f{};
			vector<int> a;

			int addOrFind = 0; // 0: add,   1: find

			for (int i = 0; i < 400; i++)
			{
				addOrFind = i == 0 ? 0 : abs(dist(gen)) % 2;
				if (addOrFind == 0)
				{
					int n = dist(gen);
					f.addNum(n);
					addNum(a, n);
				}
				else
				{
					if (f.findMedian() != findMedianBruteForce(a))
						return false;
					else
						i % 40 == 0 ? (cout << "\n.") : (cout << '.');
				}
			}
			return true;
		}
	};


	void Test_0295_Find_Median_from_Data_Stream()
	{
		/*
		MedianFinder f{};
		f.addNum(1);
		f.addNum(2);
		cout << f.findMedian() << '\n';
		f.addNum(3);
		cout << f.findMedian() << '\n';
		*/

		AutoTest atest{};
		cout << "running auto test:\n";
		cout<<boolalpha<<atest.run()<<'\n';
	}
}