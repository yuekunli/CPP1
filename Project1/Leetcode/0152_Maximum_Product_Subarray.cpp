#include<vector>
#include<numeric>
#include<string>
#include<iostream>
#include<sstream>

namespace _0152_Maximum_Product_Subarray {

	using namespace std;

	class Solution
	{
	public:
		int maxProduct(vector<int>& n)
		{
			size_t i = 0;
			size_t len = n.size();
			bool hasZero = false;
			size_t segment_start, segment_end;
			bool newSegmentStarted = false;
			int maxProduct = INT_MIN;
			while (i < len)
			{
				if (n[i] == 0)
					hasZero = true;

				if (n[i] != 0 && !newSegmentStarted)
				{
					segment_start = i;
					newSegmentStarted = true;
				}
				if ((n[i] == 0 || i == len-1) && newSegmentStarted)
				{
					if (n[i] == 0)
					{
						segment_end = i - 1;
					}
					else // i is pointing at the last element
						segment_end = i;

					newSegmentStarted = false;

					//**** process a segment ****

					int negativeNumberCount = 0;
					size_t firstNegativeIndex = -1, lastNegativeIndex;
					for (size_t j = segment_start; j <= segment_end; j++)
					{
						if (n[j] < 0)
						{
							negativeNumberCount++;
							if (firstNegativeIndex == -1)
								firstNegativeIndex = j;
							lastNegativeIndex = j;
						}
					}
					if (negativeNumberCount % 2 == 0)
					{
						int product = accumulate(n.begin() + segment_start, n.begin() + segment_end + 1, 1, multiplies<int>());
						maxProduct = max(maxProduct, product);
					}
					else
					{
						/*
						* This segment is divided into 3 parts, the left and right part has all positive numbers,
						* the middle part has mixed positive and negative numbers.
						* Note that any of the 3 parts can be missing, for example, the first number in the segment is negative, there won't be a left part.
						* In extreme case, this segment only has 1 negative number, so all 3 parts are missing.
						*
						*      + + + + + + + + + +  -  . . . . . . .  . . . . . . .  - + + + + + + + + + + + +
						*      |<-- left part -->|  |  |<----   middle    -------->  | |<----   right     -->|
						*                           |                                |
						*                       first negative                      last negative
						*
						* 
						* basically there are 4 points to compare, namely seg_start, first_neg, last_neg, seg_end
						* 
						*/
						int leftProduct = 1, midProduct, rightProduct = 1;
						int maxProductOfThisSegment;

						if (segment_start<firstNegativeIndex)
							leftProduct = accumulate(n.begin() + segment_start, n.begin() + firstNegativeIndex, 1, multiplies<int>());
						if (firstNegativeIndex < lastNegativeIndex)
							midProduct = accumulate(n.begin() + firstNegativeIndex + 1, n.begin() + lastNegativeIndex, 1, multiplies<int>());
						if (lastNegativeIndex < segment_end)
							rightProduct = accumulate(n.begin() + lastNegativeIndex + 1, n.begin() + segment_end + 1, 1, multiplies<int>());


						if (firstNegativeIndex != lastNegativeIndex) // as long as there is middle part, I don't care left or right part, if they are missing, I can pretend their products are 1 respectively
							maxProductOfThisSegment = max(leftProduct * n[firstNegativeIndex] * midProduct, midProduct * n[lastNegativeIndex] * rightProduct);
						else if (segment_start < firstNegativeIndex && firstNegativeIndex == lastNegativeIndex && lastNegativeIndex < segment_end)  // no middle, but both left and right exist
							maxProductOfThisSegment = max(leftProduct, rightProduct);
						else if (segment_start == firstNegativeIndex && firstNegativeIndex == lastNegativeIndex && lastNegativeIndex < segment_end) // no left or middle, but right exists
							maxProductOfThisSegment = rightProduct;
						else if (segment_start < firstNegativeIndex && firstNegativeIndex == lastNegativeIndex && lastNegativeIndex == segment_end) // no middle or right, but left exists
							maxProductOfThisSegment = leftProduct;
						else if (segment_start == firstNegativeIndex && firstNegativeIndex == lastNegativeIndex && lastNegativeIndex == segment_end) // none of left, middle or right, just 1 negative number
							maxProductOfThisSegment = n[firstNegativeIndex];


						maxProduct = max(maxProduct, maxProductOfThisSegment);
					}
				}
				i++;
			}
			if (maxProduct < 0 && hasZero)  // example: [-2, 0, -1], the best answer that every segment can produce is negative, but there are zeros in the array.
											// or the array has all zeros, the majority of the code is always skipped, maxProduct is never changed, it remains INT_MIN, but there are zeros in the array
				return 0;
			else
				return maxProduct;
		}
	};

	void Test_0152_Maximum_Product_Subarray()
	{
		Solution solu;

		string s;
		vector<int>nums;

		while (true)
		{
			s.clear();
			cout << "array:  ";
			getline(cin, s);
			if (s.size() == 0)
				return;
			stringstream ss(s);
			nums.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << "answer: " << solu.maxProduct(nums) << "\n\n";
		}
	}
}