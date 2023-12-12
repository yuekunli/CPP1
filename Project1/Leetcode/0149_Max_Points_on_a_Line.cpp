#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<unordered_map>

namespace _0149_Max_Points_on_a_Line {

	using namespace std;

	class Solution1
	{
		/*
		* Fix on one point at a time, count the points that are on the same line slop.
		* For example, fix on point 'x', examine all other points, for a point 'y',
		* calculate the slop of the line that goes through 'x' and 'y'.
		* I don't have to worry about y-intercept, because point 'x' is fixed.
		* But actually when I fix on point 'x', I don't really need to consider *all*
		* other points, just the ones I haven't considered.
		* If a line is formed by point 'a' and 'b', when I fix on point 'a', I already
		* considered this line, when I fix on point 'b', I don't need to consider the
		* same line again. If the line formed by 'a' and 'b' is eventually the answer
		* and there are 5 points on that line in total, for example, 'a', 'b', 'c', 'd', 'e'
		* When I examine 'a', I would have counted all 5 points. When I examine 'b', I only
		* check points listed after 'b', I would find the same line, but I would only count
		* 4 points on it, but that is OK, because I already recorded a best answer of 5 when
		* I examined 'a'. But this is actually a waste of time, it would be good if there is
		* a way to skip 'c', 'd', 'e' when I examine 'b'.
		*/

		int gcd(int a, int b)
		{
			if (b == 0)
				return a;
			else
				return gcd(b, a % b);
		}
		
		struct PairHash
		{
			/*
			* range of the coordinators: -10^4 ~ 10^, biggest slop would be (2*10^4)/1
			* The numerator is basically a 5-digit number. The denominator can be 5-digit as well.
			* This hash function basically left shift the numerator 5 digits.
			* The xDelta (the 2nd number in the pair) can be negative, does this handle that case correctly?
			* Yes, it should work fine. A unique negative int will be cast to a unique unsigned long long,
			* yDelta is shifted, I just stick the xDelta to the last 5 digit.
			* I can change this code to doing binary shift, which is probably more convincing.
			* int is at most 32-bit, unsigned long long is at least 64-bit, 
			*/
			unsigned long long operator()(pair<int, int> const & slop)const
			{
				//return (unsigned long long)(slop.first) * 100'000ull + (unsigned long long)(slop.second);
				return (unsigned long long)(slop.first) << 32 + (unsigned long long)(slop.second); // this line is not tested, the above line is.
			}
		};

		size_t n;
		
		int countLinesThruOnePoint(vector<vector<int>>& points, size_t i, vector<bool>& duplicate)
		{
			unordered_map<pair<int, int>, int, PairHash> m;
			//            \____________/   |
			//                  |          value: number of points on the line with that slop
			//                 key: slop

			int maxCount = 0;
			int dup = 0;
			for (size_t j = i + 1; j < n; j++)
			{
				int x1 = points[i][0];
				int y1 = points[i][1];
				int x2 = points[j][0];
				int y2 = points[j][1];

				if (x1 == x2 && y1 == y2)
				{
					duplicate[j] = true;
					dup++;
					continue;
				}
				pair<int, int> slop{ };
				if (x1 == x2)
				{
					slop.first = 1;
					slop.second = 0;
				}
				else if (y1 == y2)
				{
					slop.first = 0;
					slop.second = 1;
				}
				else
				{
					int yDelta = y1 - y2;
					int xDelta = x1 - x2;
					if (yDelta < 0)
					{
						yDelta = -yDelta;
						xDelta = -xDelta;
					}
					int commonDiv = gcd(abs(yDelta), abs(xDelta));
					yDelta = yDelta / commonDiv;
					xDelta = xDelta / commonDiv;
					slop.first = yDelta;
					slop.second = xDelta;
				}
				if (m.count(slop) == 0)
					m[slop] = 2;
				else
					m[slop]++;

				if (m[slop] > maxCount)
					maxCount = m[slop];
			}
			return maxCount + dup;
		}

	public:
		int maxPoints(vector<vector<int>>& points)
		{
			n = points.size();
			vector<bool>duplicate(n, false);
			int maxPoints = 0;
			for (size_t i = 0; i < n; i++)
			{
				if (!duplicate[i])
				{
					int maxPointsThruOnePoint = countLinesThruOnePoint(points, i, duplicate);
					maxPoints = max(maxPointsThruOnePoint, maxPoints);
				}
			}
			return maxPoints;
		}
	};


	class Solution2
	{
		/*
		* Solution1 uses a vector to track duplicate points, but the constraints say all
		* points are unique, so this solution simplify a little.
		* Also need to handle a corner case: there is only 1 point in the "points" array.
		*/
		int gcd(int a, int b)
		{
			if (b == 0)
				return a;
			else
				return gcd(b, a % b);
		}

		struct PairHash
		{
			/*
			* range of the coordinators: -10^4 ~ 10^, biggest slop would be (2*10^4)/1
			* The numerator is basically a 5-digit number. The denominator can be 5-digit as well.
			* This hash function basically left shift the numerator 5 digits.
			*/
			unsigned long long operator()(pair<int, int>const& slop)const
			{
				return (unsigned long long)(slop.first) * 100'000ull + (unsigned long long)(slop.second);
			}
		};

		size_t n;

		int countLinesThruOnePoint(vector<vector<int>>& points, size_t i)
		{
			unordered_map<pair<int, int>, int, PairHash> m;
			int maxCount = 0;
			for (size_t j = i + 1; j < n; j++)
			{
				int x1 = points[i][0];
				int y1 = points[i][1];
				int x2 = points[j][0];
				int y2 = points[j][1];

				pair<int, int> slop{ };
				if (x1 == x2)
				{
					slop.first = 1;
					slop.second = 0;
				}
				else if (y1 == y2)
				{
					slop.first = 0;
					slop.second = 1;
				}
				else
				{
					int yDelta = y1 - y2;
					int xDelta = x1 - x2;
					if (yDelta < 0)
					{
						yDelta = -yDelta;
						xDelta = -xDelta;
					}
					int commonDiv = gcd(abs(yDelta), abs(xDelta));
					yDelta = yDelta / commonDiv;
					xDelta = xDelta / commonDiv;
					slop.first = yDelta;
					slop.second = xDelta;
				}
				if (m.count(slop) == 0)
					m[slop] = 2;
				else
					m[slop]++;

				if (m[slop] > maxCount)
					maxCount = m[slop];
			}
			return maxCount > 0 ? maxCount : 1;
			// I should never return 0 from this function, if the point I'm examining is not the last point in "points" array
			// the return value should at least be 2, because any two points can form a line.
			// If the point under examination is the last point, I should return 1, because a line can go through that point.
		}

	public:
		int maxPoints(vector<vector<int>>& points)
		{
			n = points.size();
			int maxPoints = 0;
			for (size_t i = 0; i < n; i++)
			{				
				int maxPointsThruOnePoint = countLinesThruOnePoint(points, i);
				maxPoints = max(maxPointsThruOnePoint, maxPoints);	
			}
			return maxPoints;
		}
	};


	void Test_0149_Max_Points_on_a_Line()
	{
		Solution2 s;
		string s1;
		vector<int>tmp;
		vector<vector<int>>points;
		while (true)
		{
			s1.clear();
			cout << "points: ";
			getline(cin, s1);
			if (s1.size() == 0)break;
			stringstream ss(s1);
			tmp.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(tmp));
			size_t sz = tmp.size();
			points.clear();
			for (size_t i = 0; i <= sz - 2; i++)
			{
				points.emplace_back(initializer_list<int>{tmp[i], tmp[i + 1]});
			}
			cout << s.maxPoints(points) << "\n\n";
		}
	}
}