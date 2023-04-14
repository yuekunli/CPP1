#include<vector>
#include<string>
#include<iostream>
#include<sstream>

namespace _0335_Self_Crossing {

	using namespace std;


	// if two lines exactly overlap, is it a "cross"?  

	// example, 2, 1, 2, 1, 2, 1, 2, 1, ......
	//
	//            ^
	//            |
	//            |
	//      +-----+ 2
	//      |     |
	//      |     |
	//      |     |
	//      |     |
	//  ----+-----+------->
	//     -1     0
	//
	// leetcode example#3 has input [1,1,1,1] and calls this a "cross".

	// constraints:
	// 1 <= distances.length <= 10^5
	// distances is certainly a long array, even though all the leetcode examples give an array of size 4.



	class Solution
	{
	public:

		bool isSelfCrossing(vector<int>& d)
		{
			if (d.size() <= 3)  // it will never cross with 3 or fewer segments
				return false;

			int const top = 0;
			int const left = 1;
			int const bot = 2;
			int const right = 3;
			int bounds[4]{ 0,0,0,0 };  //top, left, bottom, right
			int previousBounds[4]{ 0,0,0,0 };

			size_t len = d.size();

			pair<int, int> currentCoor{ 0,0 };

			// initialize things with the first 2 distances
			previousBounds[top] = bounds[top] = d[0];
			currentCoor.second = d[0];

			previousBounds[left] = bounds[left] = -d[1];
			currentCoor.first = -d[1];

			previousBounds[bot] = previousBounds[right] = bounds[bot] = bounds[right] = 0; // these are initialized to '0' anyway

			// index in the distance array:

			// 0   1   2   3   4   5   6   7   8
			// ^   <   v   >   ^   <   v   >   ^

			bool goingSmallerCircle = false;

			for (int i = 2; i < len; i++)
			{
				int direction = i % 4;
				int endX = 0, endY = 0;

				if (goingSmallerCircle)
				{
					switch (direction)
					{
					case 0:  // up trip
						endY = currentCoor.second + d[i];
						if (endY >= bounds[top])
						{
							return true;
						}
						else
						{
							bounds[top] = currentCoor.second = endY;
						}
						break;
					case 1:  // left trip
						endX = currentCoor.first - d[i];
						if (endX <= bounds[left])
						{
							return true;
						}
						else
						{
							bounds[left] = currentCoor.first = endX;
						}
						break;
					case 2:  // down trip
						endY = currentCoor.second - d[i];
						if (endY <= bounds[bot])
						{
							return true;
						}
						else
						{
							bounds[bot] = currentCoor.second = endY;
						}
						break;
					case 3:  // right trip
						endX = currentCoor.first + d[i];
						if (endX >= bounds[right])
						{
							return true;
						}
						else
						{
							bounds[right] = currentCoor.first = endX;
						}
						break;
					}
				}
				else
				{
					switch (direction)
					{
					case 0:   // up trip
						endY = currentCoor.second + d[i];
						previousBounds[top] = bounds[top];
						
						currentCoor.second = endY;
						if (endY > bounds[top])
						{
							bounds[top] = endY;
						}
						else
						{
							/*
							* 1, 1, 3, 2, 1, 1
							* 
							*                 |
							*           +--<--+
							*           |     ^
							*           v     |
							* ----------|-----|--------
							*           v     ^
							*           |     |
							*           v     |--<-+
							*           |     |    |
							*           +-->--+-->-+
							*                 |
							* 
							*  When considering the 2nd up trip, the rectangle at that point is like this:
							*  +------+
							*  |      |
							*  |      |
							*  |
							*  |
							*  |
							*  +------------
							* 
							* It's not a complete rectangle, if the up trip doesn't reach x-axis,
							* I shouldn't re-assign left bound.
							* 
							*/
							bounds[top] = endY;
							if (i > 4 || (i == 4 && endY >= 0))
							{
								bounds[left] = previousBounds[right];    // not go up high enough, when turn left, previous right bound becomes new left bound
							}							
							goingSmallerCircle = true;
						}
						break;
					case 1:   // left trip
						endX = currentCoor.first - d[i];
						previousBounds[left] = bounds[left];
						
						currentCoor.first = endX;
						if (endX < bounds[left])
						{
							bounds[left] = endX;
						}
						else
						{
							bounds[left] = endX;
							bounds[bot] = previousBounds[top];  // not go left enough, when turn down, previous top bound becomes new bottom bound
							goingSmallerCircle = true;
						}
						break;
					case 2:   // down trip
						endY = currentCoor.second - d[i];
						previousBounds[bot] = bounds[bot];
						
						currentCoor.second = endY;
						if (endY < bounds[bot])
						{
							bounds[bot] = endY;
						}
						else
						{
							bounds[bot] = endY;
							if (i > 2)
							{
								bounds[right] = previousBounds[left];// not go down enough, when turn right, previous left bound becomes new right bound
								// but if it hasn't even finished its first cycle, this bound re-assign doesn't happen
							}
							goingSmallerCircle = true;
						}
						break;
					case 3:   // right trip
						endX = currentCoor.first + d[i];
						previousBounds[right] = bounds[right];
						currentCoor.first = endX;
						if (endX > bounds[right])
						{
							bounds[right] = endX;
						}
						else
						{
							bounds[right] = endX;
							/*
							*                 |
							*           +--<--+
							*           |     ^
							*           v     |
							* ----------|-----|--------
							*           v     ^
							*           |     |
							*           +-->--+
							*                 |
							* If the trip to right doesn't go over right bound, even if it's equal, it starts a smaller circle,
							* but when do I re-assign top bound? If this is not the first cycle, I certainly should re-assing.
							* If this is the first cycle, mostly I don't, because if it's the first cycle, the rectangle is
							* still bottomless. but if it's the first cycle and the trip ends exactly on the y-axis, then it
							* goes up, if it reaches (0,0), it's considered a self-crossing.
							*/
							if (i > 3 || (i == 3 && endX == 0))
							{
								bounds[top] = previousBounds[bot];// not go right enough, when turn up, previous bottom bound becomes new top bound
								// but if it hasn't even finished its first cycle, this bound re-assign doesn't happen
							}
							
							goingSmallerCircle = true;
						}
						break;
					}
				}
			}
			return false;
		}
	};

	void Test_0335_Self_Crossing()
	{
		Solution s;
		
		while (true)
		{
			vector<int> dist;
			string s1;
			cout << "distances: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(dist));
			cout << boolalpha << s.isSelfCrossing(dist) << '\n';
		}
	}
}