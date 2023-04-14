#include "LibHeaders.h"

namespace _2179_Count_Good_Triplets_in_Array {

	using namespace std;

	/*
	* There is a way to view the two arrays alternatively.
	* For example:
	* nums1: 2 0 1 3
	* nums2: 0 1 2 3
	* 
	* I can just treat these numbers as symbols, for example, I can say nums1 is this:
	*        2          0        1         3
	*     triangle   square    circle    cross
	* 
	* These 4 symbols have their indexes in nums1:
	* symbol  |   index in nums1
	* --------+-----------------
	* triangle        0
	* square          1
	* circle          2
	* cross           3
	* 
	* These 4 symbols are re-arranged in nums2:
	* 
	* square    circle      triangle     cross
	* 
	* If I put the symbols' indexes in the order as they appear in nums2, I get this:
	* 
	* 1, 2, 0, 3
	* 
	* So in this way, I can always view nums1 as a perfectly sorted ascending array from 0 to n-1.
	* And I can view nums2 as a re-arrangement of nums1's indexes.
	* In this particular case, nums1 becomes: 0, 1, 2, 3, nums2 becomes: 1, 2, 0, 3
	* And the task becomes picking three out of nums2 so that their values (indexes of nums1) increase
	* If I view them in this way, once I have converted nums2, nums1 is not important any more.
	* And I call nums2 the "scrambled" array. This is why the brute force solution has this terminology
	* 
	*/

	/*
	* The core principle of these Solutions is all similar.
	* I first treat nums1 and nums2 as the "perfectly sorted in ascending order" array and the "scrambled" array.
	* So how did I get this "scrambled" array?
	* Imagine I have a bag of numbers from 0 to n-1. I draw one at a time, then put the number I just drew on
	* a line one after one. Every time I draw one out, I want to know: among the ones I drew before this one,
	* how many of them are less than this one.
	* I can calculate the numbers that are greater than this one and that are still in the bag. They will be
	* drawn out later and placed on the line after this one.
	* The ones already on the line and less than the current one plus the current one plus the greater numbers
	* still in the bag can form good triplets.
	* 
	* So the key is to count the numbers that are already drawn out before and are less than the current number.
	*/

	class Solution
	{
	public:
		long long goodTriplets(vector<int>& nums1, vector<int>& nums2)
		{
			int len = nums1.size();
			set<int> appearedSoFar;
			vector<int> indexes(len);  // indexes[5] tells me which cell of nums1 is the number '5' in?
			for (int i = 0; i < len; i++)
			{
				int val = nums1[i];
				indexes[val] = i;
			}
			long long goodTripletsCount = 0;
			for (int i = 0; i < len; i++)
			{
				int val = nums2[i];
				int index = indexes[val];
				if (appearedSoFar.size() == 0)
				{
					// nothing to do here, the element will be added to the set later
				}
				else
				{
					int sizeOfCurrentlyInSet = appearedSoFar.size();
					auto it = appearedSoFar.lower_bound(index);
					int lessThanAmongCurrentlyInSet = distance(appearedSoFar.begin(), it);  // this solution is no good, because this "distance" call takes O(current set size)
					int greaterThanAmongCurrentlyInSet = sizeOfCurrentlyInSet - lessThanAmongCurrentlyInSet;
					int originallyGreaterThan = len - 1 - index;
					int greaterThanAmongFutureAdditions = originallyGreaterThan - greaterThanAmongCurrentlyInSet;
					goodTripletsCount += lessThanAmongCurrentlyInSet * greaterThanAmongFutureAdditions;

				}
				appearedSoFar.emplace(index);
			}
			return goodTripletsCount;
		}
	};


	class Solution2
	{
	
		void addToBIT(vector<int>& bit, int pos, int val)
		{
			while (pos < bit.size())
			{
				bit[pos] += val;
				pos = pos + (pos & (-pos));
			}
		}
		int checkBIT(vector<int>& bit, int pos)
		{
			int sum = 0;
			while (pos > 0)
			{
				sum += bit[pos];
				pos = pos - (pos & (-pos));
			}
			return sum;
		}

	public:

		long long goodTriplets(vector<int>& nums1, vector<int>& nums2)
		{
			int len = nums1.size();
			vector<int> bit(len + 1);
			vector<int> symbolsIndexesInNums1(len);
			for (int i = 0; i < len; i++)
			{
				int symbol = nums1[i];
				symbolsIndexesInNums1[symbol] = i;  // each symbol has an index from 0 to len-1 in nums1
				                                    // a symbol with index 5 is actually the 6Th element,
													// so its position in the binary-index-tree is in cell#6 (cell#0 is left empty)
			}

			long long goodTripletsCount = 0;
			for (int i = 0; i < len; i++)
			{
				int symbol = nums2[i];
				int symIdx = symbolsIndexesInNums1[symbol];
				int currentLessThan = checkBIT(bit, symIdx + 1);
				int currentGreaterThan = i - currentLessThan;
				int totalGreaterThan = len - symIdx - 1;
				int greaterThanAmongFutureAdditions = totalGreaterThan - currentGreaterThan;
				goodTripletsCount += currentLessThan * greaterThanAmongFutureAdditions;
				addToBIT(bit, symIdx + 1, 1);
			}
			return goodTripletsCount;
		}
	};


	class Solution3
	{
		/*
		* This solution has the potential to be generalized to
		* "find the k-number group that maintain relative order".
		* But the disadvantage of this solution, serving as generalized,
		* is that the memory cost of goes up as 'k' increases.
		*/

		void addToBIT(vector<int>& bit, int pos, int val)
		{
			while (pos < bit.size())
			{
				bit[pos] += val;
				pos = pos + (pos & (-pos));
			}
		}
		int checkBIT(vector<int>& bit, int pos)
		{
			int sum = 0;
			while (pos > 0)
			{
				sum += bit[pos];
				pos = pos - (pos & (-pos));
			}
			return sum;
		}
	public:

		long long goodTriplets(vector<int>& nums1, vector<int>& nums2)
		{
			int len = nums1.size();
			vector<int> symbolsIndexesInNums1(len);
			for (int i = 0; i < len; i++)
			{
				int symbol = nums1[i];
				symbolsIndexesInNums1[symbol] = i;
			}

			vector<int> recordOfSingleNumberInOrder(len + 1);
			vector<int> recordOfDoubleNumberInOrder(len + 1);

			long long goodTriplets = 0;
			for (int i = 0; i < len; i++)
			{
				int symbol = nums2[i];
				int symIdx = symbolsIndexesInNums1[symbol];
				int symPosInBit = symIdx + 1;
				int a = checkBIT(recordOfSingleNumberInOrder, symPosInBit);
				int b = checkBIT(recordOfDoubleNumberInOrder, symPosInBit);
				goodTriplets += b;
				addToBIT(recordOfSingleNumberInOrder, symPosInBit, 1);
				addToBIT(recordOfDoubleNumberInOrder, symPosInBit, a);
			}
			return goodTriplets;
		}
	};

	class Solution4
	{
	public:
		long long goodTriplets(vector<int>& nums1, vector<int>& nums2)
		{
			int len = nums1.size();
			vector<int> pos(len + 1, 0);
			vector<int>presum(len + 1, 0);
			vector<int>before;

			for (int i = 0; i < len; i++)
			{
				pos[nums1[i]] = i;
			}

			for (int i : nums2)
			{
				int j = pos[i] + 1;
				int sum = 0;
				while (j)
				{
					sum += presum[j];
					j -= j & -j;
				}
				before.push_back(sum);
				j = pos[i] + 1;
				while (j < presum.size())
				{
					presum[j]++;
					j += j & -j;
				}
			}

			long long res = 0;
			presum.assign(len + 1, 0);
			for (int k = 0; k < len; k++)
			{
				int i = nums2[k];
				int j = pos[i] + 1;
				int sum = 0;
				while (j)
				{
					sum += presum[j];
					j -= j & -j;
				}

				res += sum;
				j = pos[i] + 1;
				while (j < presum.size())
				{
					presum[j] += before[k];
					j += j & -j;
				}
			}
			return res;
		}
	};

	class BruteForceSolution
	{
		/*
		* Select 4 out of 10
		*    0   1   2   3   4   5   6   7   8   9
		*    |           |                   |   |   <- these 4 bits are set right now
		* 
		*  when i == 6, 1 << 6 is:
		*    0   1   2   3   4   5   6   7   8   9
		*    0   0   0   1   0   0   0   0   0   0
		*  
		*  I now find the selected bit that can be moved to its right
		* 
		*	 0   1   2   3   4   5   6   7   8   9
		*    1   0   0   1   0   0   0   0   1   1     these 4 are set right now
		* 
		*    0   0   0   1   0   0   0   0   0   0      1 << 6
		* 
		*    0   0   0   1   1   0   0   0   0   0   0b11 << 5
		* 
		*    1   0   0   0   1   0   0   0   1   1   after clearing #3 and set#4, but I still need to move the bits at #8 and #9 to #5 and #6
		* 
		*    1   1   1   1   1   1   1   1   1   1   pow(2, n) - 1 = full
		* 
		*    0   0   0   0   0   1   1   1   1   1   pow(2, (i-1)) - 1     (note that i right now is 6)
		* 
		*    1   1   1   1   1   0   0   0   0   0   XOR the last two sequences, use this to clear #5 to #9
		* 
		*    1   0   0   0   1   0   0   0   0   0   after clearing #5 to #9
		*
		*                                    1   1    I need two bits
		*                        1   1   0   0   0    shift to left by 3 hops.
		*                                             i == 6 right now, clear the bit at 6-hop position (#3), set the bit at 5-hop position (#4)
		*                                             set the bit at 4-hop position, because I'm setting two at once, I just need 3 hops
		*                                             3 = i - 1 - 1 - (2 - 1)
		*   
		*/

		bool nextSelection(int& selection, int n, int k)
		{
			bool openFound = false;
			int bitsToTheRight = 0;
			for (int i = 0; i < n; i++)
			{
				int bit = selection & (1 << i);
				if (bit && !openFound)
				{
					bitsToTheRight++;
					continue;
				}
				if (!bit && !openFound)
				{
					openFound = true;
					continue;
				}
				if (bit && openFound)
				{
					selection ^= (0b11 << (i-1));
					int full = pow(2, n) - 1;
					int seg = pow(2, i-1) - 1;
					selection &= (full ^ seg);
					int needed = pow(2, bitsToTheRight) - 1;
					needed = needed << (i - 1 - 1 - (bitsToTheRight-1));
					selection |= needed;
					return true;
				}
			}
			return false;
		}
		bool isValid(vector<int>& scrambledArray, int selection)
		{
			int len = scrambledArray.size();
			int previous = len;
			for (int i = 0; i < len; i++)
			{
				if (selection & (1 << i))
				{
					int idx = scrambledArray[len-i-1];
					if (idx > previous)
						return false;
					previous = idx;
				}
			}
			return true;
		}
	public:
		long long goodKlets(vector<int>& nums1, vector<int>& nums2, int k)
		{
			int len = nums1.size();
			vector<int> indexesInNums1(len);
			for (int i = 0; i < len; i++)
			{
				int symbol = nums1[i];
				indexesInNums1[symbol] = i;
			}
			// re-write nums2
			for (int i = 0; i < len; i++)
			{
				int symbol = nums2[i];
				int index = indexesInNums1[symbol];
				nums2[i] = index;
			}
			
			int goodKlets = 0;
			int needed = pow(2, k) - 1;
			int selection = needed << (len - k);
			do {
				if (isValid(nums2, selection))
					goodKlets++;
			} while (nextSelection(selection, len, k));
			return goodKlets;
		}
	};

	void Test_2179_Count_Good_Triplets_in_Array()
	{
		vector<int> nums1;
		vector<int> nums2;
		Solution3 s;
		
		while (true)
		{
			string s1;
			string s2;
			cout << "nums1: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			cout << "nums2: ";
			getline(cin, s2);
			nums1.clear();
			nums2.clear();
			stringstream ss1(s1);
			copy(istream_iterator<int>(ss1), istream_iterator<int>(), back_inserter(nums1));
			stringstream ss2(s2);
			copy(istream_iterator<int>(ss2), istream_iterator<int>(), back_inserter(nums2));
			cout << s.goodTriplets(nums1, nums2) << "\n\n";
			//cout << s.goodKlets(nums1, nums2, 3) << "\n\n";
		}
	}
}