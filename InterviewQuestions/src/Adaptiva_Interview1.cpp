#include<iostream>
#include<vector>
#include<map>
#include<unordered_map>
namespace Adaptiva_Interview1 {

	using namespace std;



	/*
	* A number of cities are on a cartesion coordinates system.
	* vector 'c' has each city's name.
	* vector 'x' has each city's x-coordinate.
	* vector 'y' has each city's y-coordinate.
	* 
	* A query gives a city, the goal is to find a city that shares either the x-coornidate or the y-coornidate,
	* and the distance is the shortest. If there is a tie on distance, use city name's lexicographical order as tie breaker.
	*/

	class NearestCity_Solution
	{
		/*
		*   |
		*   |        a
		*   |      c x b
		*   |        d
		*   |
		* --+--------------------
		* 
		* Each city can have at most 4 candidates just be measuring the distance. They can come from top, down, left, or right.
		* In that case the lexicographically smallest one should be the answer.
		*/

	public:
		vector<string> nearestCity(vector<string>& c, vector<int>& x, vector<int>& y, vector<string>& q)
		{
			unordered_map<int, map<int, int>>xmap;
			unordered_map<int, map<int, int>>ymap;
			unordered_map<string, int>cmap;

			size_t len = c.size();

			vector<string>answer(q.size());

			for (int i = 0; i < len; i++)
			{
				int xcoor = x[i];
				int ycoor = y[i];
				string& name = c[i];
				xmap[xcoor].insert({ycoor, i});
				ymap[ycoor].insert({ xcoor, i });
				cmap[name] = i;
			}

			size_t i = 0;
			for (string& city : q)
			{
				int cityIndex = cmap[city];

				int xcoor = x[cityIndex];
				int ycoor = y[cityIndex];

				map<int, int>& sameX = xmap[xcoor];
				map<int, int>& sameY = ymap[ycoor];

				int sameXClosestCityIndex = -1;
				int sameYClosestCityIndex = -1;

				if (sameX.size() > 1)
				{
					auto it = sameX.lower_bound(ycoor);
					if (it == sameX.begin())
					{
						auto it2 = next(it);
						sameXClosestCityIndex = it2->second;
					}
					else if (next(it) == sameX.end())
					{
						auto it2 = prev(it);
						sameXClosestCityIndex = it2->second;
					}
					else
					{
						auto it2 = prev(it);
						auto it3 = next(it);
						if (abs(it2->first - ycoor) < abs(it3->first - ycoor))
						{
							sameXClosestCityIndex = it2->second;
						}
						else if (abs(it2->first - ycoor) < abs(it3->first - ycoor))
						{
							sameXClosestCityIndex = it3->second;
						}
						else
						{
							int a = c[it2->second].compare(c[it3->second]);
							if (a < 0)
								sameXClosestCityIndex = it2->second;
							else
								sameXClosestCityIndex = it3->second;
						}
					}
				}


				if (sameY.size() > 1)
				{
					auto it = sameY.lower_bound(xcoor);
					if (it == sameY.begin())
					{
						auto it2 = next(it);
						sameYClosestCityIndex = it2->second;
					}
					else if (next(it) == sameY.end())
					{
						auto it2 = prev(it);
						sameYClosestCityIndex = it2->second;
					}
					else
					{
						auto it2 = prev(it);
						auto it3 = next(it);
						if (abs(it2->first - xcoor) < abs(it3->first - xcoor))
						{
							sameYClosestCityIndex = it2->second;
						}
						else if (abs(it2->first - xcoor) < abs(it3->first - xcoor))
						{
							sameYClosestCityIndex = it3->second;
						}
						else
						{
							int a = c[it2->second].compare(c[it3->second]);
							if (a < 0)
								sameYClosestCityIndex = it2->second;
							else
								sameYClosestCityIndex = it3->second;
						}
					}
				}


				if (sameXClosestCityIndex == -1 && sameYClosestCityIndex == -1)
				{
					answer[i] = "NONE";
				}
				else if (sameXClosestCityIndex == -1)
				{
					answer[i] = c[sameYClosestCityIndex];
				}
				else if (sameYClosestCityIndex == -1)
				{
					answer[i] = c[sameXClosestCityIndex];
				}
				else
				{
					if (abs(xcoor - x[sameYClosestCityIndex]) < abs(ycoor - y[sameXClosestCityIndex]))
					{
						answer[i] = c[sameYClosestCityIndex];
					}
					else if (abs(xcoor - x[sameYClosestCityIndex]) > abs(ycoor - y[sameXClosestCityIndex]))
					{
						answer[i] = c[sameXClosestCityIndex];
					}
					else
					{
						int a = c[sameXClosestCityIndex].compare(c[sameYClosestCityIndex]);
						if (a < 0)
							answer[i] = c[sameXClosestCityIndex];
						else
						{
							answer[i] = c[sameYClosestCityIndex];
						}
					}
				}
				i++;
			}
			return answer;
		}
	};

	static void Test1()
	{
		/*
		* 
		*                       |
		*                       |
		*                    b1 |
		*           a1   a2  a3 | a4
		* ----------------------+---------------------
		*                b4  b2 |
		*                       |
		*                    b3 |
		*                       |
		* 
		*/
		vector<string> c{"a1", "a2", "a3", "a4", "b1", "b2", "b3", "b4"};
		vector<int> x   {-5,   -3,   -1,    2,    -1,   -1,   -1,   -3};
		vector<int> y   {2,     2,    2,    2,     4,   -1,   -4,   -1};
		vector<string> q{"a4", "a3", "a2", "a1", "b4", "b3", "b2", "b1"};
	// expected answer    a3    a2    a1    a2    b2    b2    b4    a3
		NearestCity_Solution s;

		vector<string>const& answer = s.nearestCity(c, x, y, q);

		for (string const& c : answer)
			cout << c << "  ";
		cout << "\n\n";
	}

	void Test_nearestCity()
	{
		Test1();
	}



	/*
	* VM Pricing
	* 
	* Users provision virtual machines. Determine the price per machine.
	* 
	* The price is determine based on some "historical" data.
	* 
	* Given a vector "instances" and a vector "price".
	* 
	* Historically, some user provisioned instances[i] number of machines,
	* and the price per machine is price[i].
	* 
	* Now if a user requests 'n' machines, here are the rules to determine the price:
	* 
	* (1). Plot the historical data points on a Cartesian coordinate sytem,
	* x-axim being the number of machines, y-axis being the price per machine.
	* 
	* (2). If 'n' falls between two historical data points x-coordinates, connect the
	* two historical data points to form a straight line, find the point on
	* that line with x-coordinate being 'n', the y-coordinate of that point shall be the price
	* 
	* (3). If 'n' falls the left of the left-most point or to the right of the right-most point,
	* connect the two closest points to form a line and find the point on that line
	* with x-coordinate being 'n', the y-coordinate of that point shall be the price.
	* 
	* 
	* values in "instances" are sorted in ascending order.
	* 
	* Some value pairs (e.g. instances[x] and price[x]) are invalid data points in price[x] is zero
	* or negative, invalid data point must not be taken into consideration.
	* 
	* 
	*/


	class VM_Pricing_Solution
	{
	public:
		string interpolate(int n, vector<int> instances, vector<float>price)
		{
			size_t len = instances.size();
			size_t i = 0, j = 0;

			// purge the invalid data points.
			// move valid data points to the front of the array.
			while (j < len)
			{
				if (price[j] > 0.0)
				{
					instances[i] = instances[j];
					price[i] = price[j];
					i++;
					j++;
				}
				else
					j++;
			}

			size_t effective_len = i;

			float finalPrice;

			if (effective_len == 1)
			{
				finalPrice = price[0];
			}
			else
			{
				int q1, q2;   // q: quantities
				float p1, p2; // p: prices
				bool haveFinalPrice = false;

				auto end_it = instances.begin() + effective_len;
				auto it = lower_bound(instances.begin(), end_it, n);

				if (it != end_it && *it == n)
				{
					size_t d = distance(instances.begin(), it);
					finalPrice = price[d];
					haveFinalPrice = true;
				}
				else if (it != instances.begin() && it != end_it)
				{
					auto itprev = prev(it);
					size_t d = distance(instances.begin(), itprev);
					q1 = instances[d];
					p1 = price[d];
					q2 = instances[d + 1];
					p2 = price[d + 1];

				}
				else if (it == instances.begin())
				{
					q1 = instances[0];
					q2 = instances[1];
					p1 = price[0];
					p2 = price[1];
				}
				else // it == end_it
				{
					q1 = instances[effective_len - 1];
					q2 = instances[effective_len - 2];
					p1 = price[effective_len - 1];
					p2 = price[effective_len - 2];
				}

				if (!haveFinalPrice)
				{
					float slop = (p2 - p1) / (float)(q2 - q1);
					// (finalPrice - p1) / (n - q1) = slop;
					finalPrice = slop * (float)(n - q1) + p1;
				}
			}

			char answer[6];  // if the price is a 2-digit integral part and 2-digit decimal places number,
							// it needs 5 elements to accomedate (the dot takes 1 cell) the meaningful data,
							// but it needs 1 more cell reserved for null terminator.
							// if the price is a multi-digit integral part number, it needs even more space.
							// so '6' is only enough for "12.34" type of decimals.

			sprintf_s(answer, sizeof(answer), "%.2f", finalPrice);
			string answer2(answer);
			return answer2;
		}
	};

	static void Test_VM_1()
	{
		vector<int> instances{20, 50, 100, 200};
		vector<float> price{73.88f, 61.05f, 50.12f, 37.59f};

		VM_Pricing_Solution s;

		cout << s.interpolate(18, instances, price) << "\n";
		cout << s.interpolate(33, instances, price) << "\n";
		cout << s.interpolate(75, instances, price) << "\n";
		cout << s.interpolate(119, instances, price) << "\n";
		cout << s.interpolate(306, instances, price) << "\n\n";
	}

	void Test_VM_Pricing()
	{
		Test_VM_1();
	}
}