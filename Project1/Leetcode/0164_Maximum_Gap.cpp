#include<vector>
#include<iostream>
#include<string>
#include<sstream>
#include<algorithm>

namespace _0164_Maximum_Gap {

	using namespace std;

	class Solution
	{
		/*
		* memory limit exceeded
		*/
	public:
		int maximumGap(vector<int>& nums)
		{
			size_t n = nums.size();
			if (n == 1) return 0;
			int maxElement = *max_element(nums.begin(), nums.end());
			vector<int>a(maxElement + 1, 0);
			for (int& c : nums)
			{
				a[c] = 1;
			}
			int previous = -1;
			int maxGap = 0;
			for (int i = 0; i <= maxElement; i++)
			{
				if (a[i] == 1)
				{
					if (previous != -1)
					{
						if (i - previous > maxGap)
							maxGap = i - previous;
					}
					previous = i;
				}
			}
			return maxGap;
		}
	};


	class Solution2
	{
		/*
		* using bit map
		*/
		/*
		* When the input number are scattered far apart, this exceeds time limit.
		* too many time spent on checking zero bits.
		*/
	public:
		int maximumGap(vector<int>& nums)
		{
			size_t n = nums.size();
			if (n == 1) return 0;
			int maxElement = *max_element(nums.begin(), nums.end());
			int minElement = *min_element(nums.begin(), nums.end());
			int bitsNeeded = maxElement - minElement + 1;
			int intNeeded = bitsNeeded / 32 + 1;
			
			vector<int32_t>a(intNeeded, 0);
			for (int& c : nums)
			{
				int diff = c - minElement;
				int whichInt = diff / 32;
				int whichBit = diff - (whichInt * 32);
				int32_t oneBit = 1;
				a[whichInt] = a[whichInt] | (oneBit << whichBit);
			}
			// find the first non-zero bit;
			int i = 0;
			while (a[i] == 0)
				i++;
			int shift = 0;
			while ((a[i] & (1 << shift)) == 0)
				shift++;
			int previous = 32 * i + shift;

			
			int maxGap = 0;
			while(i<intNeeded)
			{
				int shift = 0;
				while (shift < 32)
				{
					if ((a[i] & (1 << shift)) != 0)
					{
						int number = 32 * i + shift;
						int gap = number - previous;
						maxGap = max(gap, maxGap);
						previous = number;
					}
					shift++;
				}
				i++;
			}
			return maxGap;
		}
	};

	class Solution3
	{
		/*
		* bucket sort
		*/
		void putElementInBucket(vector<int>& bucket, int element)
		{
			bucket.push_back(element);
			if (bucket.size() == 1)
				return;
			int i = (int)(bucket.size() - 2);
			while (i >= 0)
			{
				if (bucket[i] > element)
					bucket[i + 1] = bucket[i];
				i--;
			}
			bucket[i + 1] = element;
		}
	public:
		int maximumGap(vector<int>& nums)
		{
			size_t n = nums.size();
			if (n == 1) return 0;
			int maxElement = *max_element(nums.begin(), nums.end());
			int minElement = *min_element(nums.begin(), nums.end());

			int gap = (maxElement - minElement) / (int)(n + 1);       // Bucket sort formular ???
			int buckets = (maxElement - minElement) / (gap + 1);
			vector<vector<int>> bucketsArray(buckets, vector<int>{});
			for (auto const& c : nums)
			{
				int bucket = (c - minElement) / gap;
				putElementInBucket(bucketsArray[bucket], c);
			}

			// find the first non-empty bucket
			size_t i = 0;
			while (bucketsArray[i].size() == 0)
				i++;

			int previous = bucketsArray[i][0];
			int maxGap = 0;
			while (i < buckets)
			{
				if (bucketsArray[i].size() > 0)
				{
					for (int const& c : bucketsArray[i])
					{
						int gap = c - previous;
						maxGap = max(gap, maxGap);
						previous = c;
					}
				}
				i++;
			}
			return maxGap;
		}
	};

	void Test_0164_Maximum_Gap()
	{
		Solution3 s;
		string s1;
		vector<int>nums;
		while (true)
		{
			s1.clear();
			cout << "nums: ";
			getline(cin, s1);
			if (s1.size() == 0)break;
			stringstream ss(s1);
			nums.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << s.maximumGap(nums) << "\n\n";
		}
	}
}