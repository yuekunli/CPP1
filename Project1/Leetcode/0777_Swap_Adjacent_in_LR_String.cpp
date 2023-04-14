#include<string>
#include<iostream>
#include<iomanip>

namespace _0777_Swap_Adjacent_in_LR_String {
	using namespace std;


	/*
	* The relative order of R's and L's can't change.
	* As if I delete all the X's, only keep R's and L's, compare if the two such "distilled" strings are equal.
	* But this is wrong idea!
	* XL can be transformed to LX, but LX cannot be transformed to XL.
	*/

	/*
	* Cut the start and end strings into segments. Each segment should be a mixture of R's and X's or L's and X's
	* A segment in start and end can be something like this:
	* start segment :    X X R X R X R X X
	* end   segment :    X X X R X X R R X
	* Basically, the R's can be pushed to the right.
	* 
	* Another example of segments:
	* start segment :  X X L X L X X X L
	* end   segment :  L X X L X X X X L
	* Basically the L's can be pushed to the left.
	* 
	* Another example:
	* start segment : X X X R R
	* end   segment : X X R X R
	* This transformation is impossible, R's can never be moved to their left.
	* Likewise, L's can never be moved to their right.
	* 
	* How to decide the start index and end index of a segment?
	* In the "start" array, whenever I see an 'R', find as many R's as possible without seeing an 'L'.
	* Find the same number of R's in "end" string, because those R's in "start" string could've been
	* shoved to their right.
	* 
	* If a segment is a mixture of X's and L's, get as many L's as possbile in the "start" string,
	* that should be the end of the segment, because the same amount L's should only appear to the left.
	* 
	* If "start" has a few X's at the end, 'i' will skip them to the end without forming a new segment.
	* But now I need to make sure "end" string also ends with a few X's without any R's or L's.
	* 
	*
	*/

	/*
	* start: "XLXRRXXRXX"
	* end:   "LXXXXXXRRR"
	* expected: true
	* 
	* start: "LLR"
	* end:   "RRL"
	*/
	
	class Solution // accepted
	{
		bool verifySegment(string& s1, string& s2, size_t startIndex, size_t endIndex, char type)
		{
			size_t i = startIndex, j = startIndex;
			if (type == 'R')
			{
				while (i <= endIndex && j <= endIndex)
				{
					if (s1[i] == 'X' && s2[j] == 'X')
					{
						i++;
						j++;
					}
					else if (s1[i] == 'R' && s2[j] == 'X')
					{
						j++;
					}
					else if (s1[i] == 'X' && s2[j] == 'R')
					{
						i++;
					}
					else if (s1[i] == 'R' && s2[j] == 'R' && i <= j)
					{
						i++;
						j++;
					}
					else  // This "else" is important, if start segment is "R X R" but "end" segment is "R X L" which is obviously invalid, but the first a few cases don't cover this scenario
						return false;
				}
				while (i <= endIndex)
				{
					if (s1[i] != 'X')
						return false;
					i++;
				}
				while (j <= endIndex)
				{
					if (s2[j] != 'X')
						return false;
					j++;
				}
				return true;
			}
			else
			{
				while (i <= endIndex && j <= endIndex)
				{
					if (s1[i] == 'X' && s2[j] == 'X')
					{
						i++;
						j++;
					}
					else if (s1[i] == 'L' && s2[j] == 'X')
					{
						j++;
					}
					else if (s1[i] == 'X' && s2[j] == 'L')
					{
						i++;
					}
					else if (s1[i] == 'L' && s2[j] == 'L' && i >= j)
					{
						i++;
						j++;
					}
					else
						return false;
				}
				while (i <= endIndex)
				{
					if (s1[i] != 'X')
						return false;
					i++;
				}
				while (j <= endIndex)
				{
					if (s2[j] != 'X')
						return false;
					j++;
				}
				return true;
			}
		}
	public:
		bool canTransform(string start, string end)
		{
			size_t len = start.size();

			size_t i = 0, j = 0;
			size_t segStart = 0, segEnd;
			while (i < len)
			{
				if (start[i] == 'X')
					i++;

				else if (start[i] == 'R')
				{
					int rCount = 0;
					while (i < len && start[i] != 'L')
					{
						if (start[i] == 'R')
							rCount++;
						i++;
					}
					i = segStart;
					while (i < len && rCount>0)
					{
						if (end[i] == 'R')
							rCount--;
						i++;
					}
					if (i == len && rCount > 0)
						return false;
					segEnd = i - 1;
					if (!verifySegment(start, end, segStart, segEnd, 'R'))
						return false;

					i = segStart = segEnd + 1;
				}
				else if (start[i] == 'L')
				{
					while (i < len && start[i] != 'R')
					{
						if (start[i] == 'L')
							segEnd = i;
						i++;
					}
					if (!verifySegment(start, end, segStart, segEnd, 'L'))
						return false;

					i = segStart = segEnd + 1;
				}
			}
			if (segStart < len)  // In case "start" ends with a few X's, they won't be put in a segment, I just need to make sure "end" also ends with X's
			{
				i = segStart;
				while (i < len)
				{
					if (end[i] != 'X')
						return false;
					i++;
				}
			}
			return true;
		}
	};



	class Solution2 //accepted
	{
		/*
		* Same idea as Solution1, but cleaner code
		* 
		* As long as I see an 'R' in "start" at position 'x', there must be a corresponding 'R' in "end".
		* Where is that corresponding 'R' in "end"? It must be either at position 'x' or to its right.
		* 
		* As long as I see an 'L' in "start" at position 'y', there must be a corresponding 'L' in "end".
		* Where is that corresponding 'L' in "end"? It must be either at position 'y' or to its left.
		*/
	public:
		bool canTransform(string start, string end)
		{
			size_t len = start.size();
			size_t i = 0, j = 0;
			while (i < len)
			{
				if (start[i] == 'L')
				{
					while (j < len)
					{
						if (end[j] == 'X')
							j++;
						else if (end[j] == 'R')
							return false;

						else if (end[j] == 'L' && j > i)
							return false;

						else // end[j] == 'L' && j <= i
							break;
					}
					if (j == len)
						return false;
					else
						j++;
				}
				else if (start[i] == 'R')
				{
					while (j < len)
					{
						if (end[j] == 'X')
							j++;
						else if (end[j] == 'L')
							return false;
						else if (end[j] == 'R' && j < i)
							return false;
						else // end[j] == 'R' && j>=i
							break;
					}
					if (j == len)
						return false;
					else
						j++;
				}
				i++; // if start[i] == 'X', I just advance 'i', if either the above two cases passes the examination, I also need to advance 'i'.
			}
			// what if start: XXXX  end: XRXL, obviously invalid, 'i' will quickly run over "start" and exit the loop, but I still need to check "end" string
			while (j < len)
			{
				if (end[j] != 'X')
					return false;
				j++;
			}
			return true;
		}
	};



	void Test_0777_Swap_Adjacent_in_LR_String()
	{
		Solution2 s;
		string start;
		string end;
		while (true)
		{
			start.clear();
			end.clear();
			cout << "start: ";
			cin >> start;
			cout << "end:   ";
			cin >> end;
			cout << boolalpha << s.canTransform(start, end) << '\n';
			int k;
			cout << "enter 0 to exit, enter any other number to run another test: ";
			cin >> k;
			if (k == 0) break;
		}
	}
}