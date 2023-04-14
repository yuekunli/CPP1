#include "LibHeaders.h"

namespace _2172_Max_AND_Sum_of_Array {

	using namespace std;

	class Solution
	{
	public:
		// numSlots = 4,
		// 1  1  2  2  3  3  4  4
		// 0  1  2  3  4  5  6  7
		// slot value = (index + 2) / 2

		int maximumANDSum(vector<int>& nums, int numSlots)
		{
			int len = nums.size();
			bitset<32> assignment(0);
			int maxAssignmentState = pow(2, numSlots * 2) - 1;
			vector<vector<int>>r(len, vector<int>(maxAssignmentState + 1, -1));

			return maxANDSum_Recurse(nums, numSlots, r, 0, assignment, -1);
		}

		int maxANDSum_Recurse(vector<int>& nums, int numSlots, vector<vector<int>>& r, int index, bitset<32>& assignment, int hint)
		{
			int len = nums.size();

			if (index == len)
				return 0;

			int state = static_cast<int>(assignment.to_ulong());

			if (hint != -1)
			{
				r[index][state] = hint;
				return hint;
			}

			if (r[index][state] != -1)
				return r[index][state];

			int maxSum = 0;
			for (int i = 0; i <= numSlots * 2 - 1; i++)
			{
				if (assignment[i] == false)
				{
					int slotValue = (i + 2) / 2;
					int sumOfThisRun = nums[index] & slotValue;
					assignment[i] = true;
					int recursiveResult = maxANDSum_Recurse(nums, numSlots, r, index + 1, assignment, -1);
					assignment[i] = false;
					sumOfThisRun += recursiveResult;
					if (sumOfThisRun > maxSum)
						maxSum = sumOfThisRun;

					if (i % 2 == 0)
					{
						// 0 1    2 3     4 5
						// '4' and '5' both represent slot value '2'.
						if (assignment[i] == assignment[i + 1])
						{
							assignment[i + 1] = true;
							maxANDSum_Recurse(nums, numSlots, r, index + 1, assignment, recursiveResult);
							assignment[i + 1] = false;
							i++;
						}
					}
				}
			}
			r[index][state] = maxSum;
			return maxSum;
		}
	};



	class Solution2
	{
		/*
		* The same algorithm as Solution 1, just change the way representing the assignment of numbers
		* to slots. In this solution I use a vector with the same length as the slots. Each slot has
		* 3 states: 0, 1, 2. The advantage of this approach is that when convert this array of slots' states
		* into one single number, the number can be smaller. So the recordbook 2-D array can be smaller
		* because one of the two dimensions is the number representing the assignment state.
		* Use an extreme example, for example I have 10 objects and 1 slot. The slot can accomedate any
		* number of objects. I want to represent the number of objects assigned to this slot. If I just
		* use 1 number, the upper limit of this number will be just 10. If I break this 1 slot into 10,
		* and use a 10-digit binary number, the upper limit of the number representing the state will
		* be 2^10 - 1 which is obviously significantly greater than 10.
		* In the case, if there are 4 slots, using binary number to represent the assignment will need
		* a 8-digit binary number and the upper limit is 2^8 - 1 = 255.
		* Whereas if I just use a 4-digit base 3 number, the upper limit will be:
		* 2 2 2 2 (base 3) which is 3^4 - 1 = 80.
		*/

		int maxANDSum_Recurse(vector<int>& nums, int numSlots, vector<vector<int>>& r, int index, vector<short>& assignment)
		{
			int len = nums.size();

			if (index == len)
				return 0;

			int state = 0;
			for (int i = 0; i < numSlots; i++)
			{
				state = state * 3 + assignment[i];
			}
			
			if (r[index][state] != -1)
				return r[index][state];

			int maxSum = 0;
			for (int i = 0; i < numSlots; i++)
			{
				if (assignment[i] != 2)
				{
					int slotValue = i + 1;
					int sumOfThisRun = nums[index] & slotValue;
					assignment[i]++;
					int recursiveResult = maxANDSum_Recurse(nums, numSlots, r, index + 1, assignment);
					assignment[i]--;
					sumOfThisRun += recursiveResult;
					if (sumOfThisRun > maxSum)
						maxSum = sumOfThisRun;
				}
			}
			r[index][state] = maxSum;
			return maxSum;
		}

	public:

		int maximumANDSum(vector<int>& nums, int numSlots)
		{
			int len = nums.size();
			vector<short> assignment(numSlots, 0);
			int maxAssignmentState = pow(3, numSlots) - 1;
			vector<vector<int>>r(len, vector<int>(maxAssignmentState + 1, -1));

			return maxANDSum_Recurse(nums, numSlots, r, 0, assignment);
		}
	};

	void Test_2172_Max_AND_Sum_of_Array()
	{
		vector<int>nums;
		string numsInStr;
		int numSlots;
		Solution2 s;
		while (true)
		{
			numsInStr.clear();
			cout << "input array: ";
			getline(cin, numsInStr);
			if (numsInStr.size() == 0) break;
			istringstream ss(numsInStr);
			nums.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << "num slots: ";
			cin >> numSlots;
			cin.ignore();
			cout << s.maximumANDSum(nums, numSlots) << "\n";
		}
	}
}