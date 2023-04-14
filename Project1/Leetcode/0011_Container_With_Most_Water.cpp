#include <iostream>
#include <sstream>
#include <vector>


namespace _0011_Container_With_Most_Water {

	using namespace std;

	class Solution
	{
		/*
		*   a   b   c    d    e    f    g
		*   |                           |
		*   p1                          p2
		* 
		*  if 'a' is shorter than 'g', the volumn of the container "ag" is limited by 'a'.
		*  moving p2 toward the middle won't help. if 'f' is taller than 'g', container "af" is still
		*  limited by 'a'. if 'f' is shorter than 'a', than the container just gets even smaller.
		*  The only chance to get a bigger container is to move p1 toward the middle.
		* 
		*  When there is a tie.
		*    8    4       12   8
		*    |                 |
		*    p1                p2
		* 
		*  if I move p2, then I get container "8--12", then p1 becomes the shorter end, I'll then move p1.
		*  If I move p1 to begin with, I get container "4--8", if there is never a taller bar on p1's way
		*  toward the middle, I'll never get a bigger container. so "8--8" is the answer. If there is ever
		*  a bar taller than '8' on p1's way, I'll move p2 after p1 arrives at it.
		*  In another word, when there is a tie, if every bar in between is shorter, then the optimal answer
		*  is that tie, then moving which one doesn't matter. If there is taller bars in the middle, moving
		*  which one don't matter either, because when one pointer arrives at the tall bar, the other will
		*  start moving as well.
		*  
		*/
	public:
		int maxArea(vector<int>& height)
		{
			size_t len = height.size();
			size_t i = 0, j = len - 1;
			int maxVol = 0;
			while (i < j)
			{
				int vol = static_cast<int>((j - i)) * min(height[i], height[j]);
				maxVol = max(maxVol, vol);
				if (height[i] < height[j])
					i++;
				else if (height[i] > height[j])
					j--;
				else
					i++; // Why I can just move either the left or right pointer in case of a tie?
			}
			return maxVol;
		}
	};


	void Test_0011_Container_With_Most_Water()
	{
		Solution solu;
		string s;
		vector<int>height;
		while (true)
		{
			s.clear();
			cout << "heights: ";
			getline(cin, s);
			if (s.size() == 0) break;
			stringstream ss(s);
			height.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(height));
			cout << solu.maxArea(height) << "\n\n";
		}
	}
}