#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#pragma warning(disable: 4267)
namespace _0135_Candy {

	using namespace std;

	class Solution // this is accepted by leetcode, but the logic is a little too entangled.
	{
		int calculateHill(int upLen, int downLen)
		{
			int total = 0;
			total += ((1 + upLen)*upLen/2);
			total += ((1 + downLen) * downLen / 2);
			total += max(upLen, downLen) + 1;  // the peak value has to be 1 greater than its bigger neighbor

			return total;
		}
	public:
		int candy(vector<int>& ratings)
		{
			size_t n = ratings.size();
			if (n == 1) return 1;

			int total = 0;

			// in case there are a few equal values at the beginning
			int a = ratings[0];
			size_t i = 1;
			while (i < n && ratings[i] == a)
				i++;

			if (i == n) return n;  // every child has equal rating, everyone gets 1 candy

			/*
			*  5  5  5  5  8
			*              |
			*              i is here
			* calculate the first 3 5's, the 4th '5' is part of a hill
			*/
			total += (i-1);

			bool up = ratings[i] > a ? true : false;

			int upTrendLength = 0;
			int downTrendLength = 0;
			int duplicateValleyPoint = 0;
			if (up)
			{
				upTrendLength = 2;
			}
			else
			{
				downTrendLength = 1;
			}
			i++;
			while (i < n)
			{
				if (up && ratings[i - 1] < ratings[i])
				{
					upTrendLength++;
					i++;
				}
				else if (!up && ratings[i - 1] > ratings[i])
				{
					downTrendLength++;
					i++;
				}
				else if (up && ratings[i - 1] > ratings[i])
				{
					up = false;
					upTrendLength--; // minus the peak point
					downTrendLength = 1;
					i++;
				}
				else if (!up && ratings[i - 1] < ratings[i])
				{
					total += calculateHill(upTrendLength, downTrendLength);
					up = true;
					upTrendLength = 2;
					duplicateValleyPoint++;
					i++;
				}
				else if (up && ratings[i - 1] == ratings[i])
				{
					up = false;
					upTrendLength--;
					total += calculateHill(upTrendLength, 0);

					int a = ratings[i - 1];
					int equalPoints = 0;
					while (i < n && ratings[i] == a)
					{
						equalPoints++;
						i++;
					}
					if (i == n)
					{
						return total + equalPoints - duplicateValleyPoint;
					}
					else
					{
						total += (equalPoints - 1);
						if (ratings[i] > a)
						{
							up = true;
							upTrendLength = 2;
							i++;
						}
						else
						{
							up = false;
							upTrendLength = 0;
							downTrendLength = 1;
							i++;
						}
					}
				}
				else if (!up && ratings[i - 1] == ratings[i])
				{
					total += calculateHill(upTrendLength, downTrendLength);

					int a = ratings[i - 1];
					int equalPoints = 0;
					while (i < n && ratings[i] == a)
					{
						equalPoints++;
						i++;
					}
					if (i == n)
					{
						return total + equalPoints - duplicateValleyPoint;
					}
					else
					{
						/*
						*  9  5  5  5  5  5  8
						*     |  \_______/
						*     |   these are counted as equalPoints, but the last '5' should be counted as next hill
						*    the end of previous hill
						*/
						total += (equalPoints-1);
						if (ratings[i] > a)
						{
							up = true;
							upTrendLength = 2;
							i++;
						}
						else
						{
							up = false;
							upTrendLength = 0;
							downTrendLength = 1;
							i++;
						}
					}
				}
			}

			if (up)
			{
				total += ((1+upTrendLength)*upTrendLength/2);
			}
			else
			{
				total += calculateHill(upTrendLength, downTrendLength);
			}

			total -= duplicateValleyPoint;

			return total;
		}
	};


	class Solution2
	{
		/*
		* Using a separate array "lowsHighs",
		* put the indices of low and high points in this array.
		* 
		* All the low points get 1 candy.
		* For a high point, based on distance between the high
		* and its left and right low neighbors, the high point
		* get appropriate candies.
		* 
		* for example:
		*              hi
		*      lo1            lo2
		* If there are 5 between lo1 and hi, and 3 between hi and lo2,
		* hi must get 7.
		* 
		* 1 2 3 4 5 6 7 4 3 2 1
		* |           |       |
		* lo1        hi       lo2
		* 
		* The key is how to deal with equal points.
		* Try to convert these flat regions to ups and downs:
		* 
		* \
		*  \
		*   - - -
		*         \
		*          \
		* 
		* 9 8 7 6 5 5 5 5 4 3 2    <-  ratings
		* 5 4 3 2 1 1 1 4 3 2 1    <-  candy
		* |       |   | |     |
		* hi     lo  lo hi    lo
		* 
		* hi      hi
		* \       /\           
		*  \     /  \          
		*   - - -    \        
		*  lo   lo    lo
		* 
		* I can pretend there is another hill after the flat region,
		* but the flat region itsefl is still there.
		* This is not a good strategem
		*/
	};


	class Solution3
	{
		/*
		* The reason I get so much trouble in the first two solutions is because
		* I can't find a unified way to handler flat regions. Flag regions basically get 1 candy each child.
		* 
		* First find the children that deserve 1 candy and set their ratings to 0.
		* Then every non-zero streak between two zeros must be a hill (not necessarily
		* have two slops though).
		*/

		int calculateSegment(vector<int>& ratings, size_t start, size_t end)
		{
			/*
			*    0  1  2  3  4  5  3  2  0   <-- ratings
			*       |                 |
			*    start index         end index
			* 
			* This function should work if the end index is the last index of ratings array and the end is the peak
			*/

			/*
			* Attention this function is not 100% correct!
			* Special case must be handled: flat peak
			* 
			* ratings:
			* 
			*              6    6
			*          4            3
			*       0  |            |  0
			*         start        end
			* peak index will be pointing to the first '6'.
			* distance from peak to end will be longer.
			* calculating the candies given to peak from right side,
			* I get the answer saying I need to give peak 4 candies,
			* however in fact, both peak points need just 3 candies.
			* 
			* There won't be more than 3 peak points because the ones in the middle
			* will be re-assigned 0 rating and the segment will be broken up.
			* for example:
			* 0  4  6  6  6  3  0  <-- ratings
			*          |
			*          this one will get 1 candy and get re-assigned 0 rating
			* So eventually there will be two segments.
			* 
			* Revised version of this function is in Solution4
			*/

			size_t peakIndex = start;
			for (size_t i = start+1; i <= end; i++)
			{
				if (ratings[i - 1] < ratings[i])
					peakIndex = i;
				else
					break;
			}
			
			size_t startPeakDistance = peakIndex - start;
			size_t peakEndDistance = end - peakIndex;
			int total = 0;
			if (startPeakDistance >= peakEndDistance)
			{
				int assignment = 2;
				for (size_t i = start; i <= peakIndex; i++)
				{
					total += assignment;
					assignment++;
				}
				assignment = 2;
				for (size_t i = end; i > peakIndex; i--)
				{
					total += assignment;
					assignment++;
				}
			}
			else
			{
				int assignment = 2;
				for (int i = (int)end; i >= (int)peakIndex; i--)  // if start is 0 and start is the peak, I must use signed integer as loop variable
				{
					total += assignment;
					assignment++;
				}
				assignment = 2;
				for (size_t i = start; i < peakIndex; i++)
				{
					total += assignment;
					assignment++;
				}
			}
			return total;
		}
	public:
		int candy(vector<int>& ratings)
		{
			int totalCandyForChildrenWithOneCandy = 0;

			int previousRating = ratings[0];
			if (ratings[0] <= ratings[1])
			{
				ratings[0] = 0;
				totalCandyForChildrenWithOneCandy++;
			}
			size_t i = 1;
			while (i < ratings.size() - 1)
			{
				if (previousRating >= ratings[i] && ratings[i] <= ratings[i + 1]) // as long as ratings[i] is not higher than either of its neighbors, it can be given just 1 candy.
				{
					previousRating = ratings[i];
					ratings[i] = 0;
					totalCandyForChildrenWithOneCandy++;
				}
				else
				{
					previousRating = ratings[i];
				}
				i++;
			}
			if (previousRating >= ratings[i])
			{
				ratings[i] = 0;
				totalCandyForChildrenWithOneCandy++;
			}
			
			
			// at this point, all the children that deserve 1 candy have their ratings set to 0

			size_t segmentStart = 0;
			bool segmentStarted = false;
			int total = 0;
			for (size_t i = 0; i < ratings.size(); i++)
			{
				if (segmentStarted && ratings[i] == 0)
				{
					total += calculateSegment(ratings, segmentStart, i - 1);
					segmentStarted = false;
				}
				else if (!segmentStarted && ratings[i] > 0)
				{
					segmentStart = i;
					segmentStarted = true;
				}
			}

			if (segmentStarted)
				total += calculateSegment(ratings, segmentStart, ratings.size() - 1);

			// what if the ratings array ends with a up trend?
			// 0 1 2 3 2 1 0 1 2 3 4 5
			// If ratings end with a down trend, the last child gets a 0 rating
			// the last segment won't end on the last index


			return total + totalCandyForChildrenWithOneCandy;
		}
	};


	class Solution4
	{
		int calculateSegment(vector<int>& ratings, size_t start, size_t end)
		{
			size_t peakIndex = start;
			bool doublePeak = false;
			size_t peakIndex2 = 0;
			for (size_t i = start + 1; i <= end; i++)
			{
				if (ratings[i - 1] < ratings[i])
					peakIndex = i;
				else if (ratings[i - 1] == ratings[i])
				{
					doublePeak = true;
					peakIndex2 = i;
				}
				else
					break;
			}

			int total = 0;

			if (doublePeak)
			{
				int assignment = 2;
				for (size_t i = start; i <= peakIndex; i++)
				{
					total += assignment;
					assignment++;
				}
				assignment = 2;
				for (size_t i = end; i >= peakIndex2; i--)
				{
					total += assignment;
					assignment++;
				}
			}
			else
			{
				// single peak
				int assignment = 2;
				int peakCandyFromStart = 0;
				int peakCandyFromEnd = 0;
				for (size_t i = start; i < peakIndex; i++)
				{
					total += assignment;
					assignment++;
				}
				peakCandyFromStart = assignment;

				assignment = 2;
				for (int i = (int)end; i > (int)peakIndex; i--)
				{
					total += assignment;
					assignment++;
				}
				peakCandyFromEnd = assignment;
				total += max(peakCandyFromStart, peakCandyFromEnd);
			}
			return total;
		}
	public:
		int candy(vector<int>& ratings)
		{
			if (ratings.size() == 1) // this corner case is tested in leetcode judging machine.
				return 1;

			int totalCandyForChildrenWithOneCandy = 0;

			int previousRating = ratings[0];
			if (ratings[0] <= ratings[1])
			{
				ratings[0] = 0;
				totalCandyForChildrenWithOneCandy++;
			}
			size_t i = 1;
			while (i < ratings.size() - 1)
			{
				if (previousRating >= ratings[i] && ratings[i] <= ratings[i + 1])
				{
					previousRating = ratings[i];
					ratings[i] = 0;
					totalCandyForChildrenWithOneCandy++;
				}
				else
				{
					previousRating = ratings[i];
				}
				i++;
			}
			if (previousRating >= ratings[i])
			{
				ratings[i] = 0;
				totalCandyForChildrenWithOneCandy++;
			}

			size_t segmentStart = 0;
			bool segmentStarted = false;
			int total = 0;
			for (size_t i = 0; i < ratings.size(); i++)
			{
				if (segmentStarted && ratings[i] == 0)
				{
					total += calculateSegment(ratings, segmentStart, i - 1);
					segmentStarted = false;
				}
				else if (!segmentStarted && ratings[i] > 0)
				{
					segmentStart = i;
					segmentStarted = true;
				}
			}

			if (segmentStarted)
				total += calculateSegment(ratings, segmentStart, ratings.size() - 1);

			return total + totalCandyForChildrenWithOneCandy;
		}
	};


	/*
	* Test case in leetcode Solution section:
	* 
	* ratings: 1 2 3 4 5 3 2 1 2 6 5 4 3 3 2 1 1 3 3 3 4 2
	* answer: 47
	* 
	*/
	void Test_0135_Candy()
	{
		Solution4 s;
		string s1;
		vector<int>ratings;
		while (true)
		{
			s1.clear();
			cout << "ratings: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			ratings.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(ratings));
			cout << s.candy(ratings) << "\n\n";
		}
	}
}