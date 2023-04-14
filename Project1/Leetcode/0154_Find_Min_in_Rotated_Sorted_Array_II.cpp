#include "LibHeaders.h"

namespace _0154_Find_Min_in_Rotated_Sorted_Array_II{

	using namespace std;

	// 1 5 5 5 5
	// 5 5 5 5 7 1

	class Solution
	{
		size_t n;

		int solve(vector<int>& nums, size_t lo, size_t hi)
		{
			if (lo == hi && lo != n - 1) return nums[lo + 1];

			int a = (hi - lo) / 2 + lo;
			if (a != n - 1)
			{
				if (nums[0] <= nums[a] && nums[a] > nums[a + 1])
					return nums[a + 1];

				else if (nums[0] <= nums[a] && nums[a] < nums[a + 1])
					return solve(nums, a + 1, hi);

				else if (nums[0] < nums[a] && nums[a] <= nums[a + 1])  // must not combine this and previous situation, nums[0] == nums[a] == nums[a+1] doesn't belong to either case
					return solve(nums, a + 1, hi);

				// basically if there is difference between nums[0], nums[a], nums[a+1], and it's increasing
				// index 'a' is in the part that has been rotated to the front, so the original minimum point
				// is to the right of this. What if the rotation just ends with the same original array?
				// nums[0], nums[a], nums[a+1] meet such increasing condition, but minimum point is obviously
				// to the left of 'a'. It's Ok because I eventually will reach the end of the array and will
				// wrap around and return nums[0]

				else if (nums[0] > nums[a + 1] && nums[a] <= nums[a + 1])
					return solve(nums, lo, a - 1);

				else if (nums[0] >= nums[a + 1] && nums[a] < nums[a + 1])
					return solve(nums, lo, a - 1);

				else // nums[0] == nums[a] == nums[a+1]  I don't know if I should go to the left or the right, What do I do? ---  Just solve both sides, take the smaller result.
				{
					int b1 = a < hi ? solve(nums, a + 1, hi) : nums[a];
					if (b1 < nums[0])
						return b1;
					else
					{
						int b2 = lo < a ? solve(nums, lo, a - 1) : nums[a];
						return min(b1, b2);
					}
				}
			}
			else
			{
				return nums[0];
			}
		}

	public:
		int findMin(vector<int>& nums)
		{
			n = nums.size();
			return solve(nums, 0, n - 1);
		}
	};



	/*
	* The decision is driven by the relation between nums[0] nums[a] and nums[a+1]
	* Can I alternatively using nums[0], nums[a], and nums[end-1] to make decision?
	*/



	class AutoTest
	{
		random_device rd;
		mt19937 gen;
		uniform_int_distribution <int> uid;
		Solution s;
	public:
		AutoTest() :gen(rd()), uid(1, 2) {}
		void generateArray(vector<int>& arr)
		{
			arr.assign(10,0);
			generate(arr.begin(), arr.end(), std::bind(uid, ref(gen)));
			sort(arr.begin(), arr.end());
			int ro = gen() % 10;
			rotate(arr.begin(), arr.begin() + ro, arr.end());
		}

		void run()
		{
			vector<int> a;
			int i = 0;
			while (i < 3000)
			{
				generateArray(a);
				int correct = *min_element(a.begin(), a.end());
				int test = s.findMin(a);
				if (test != correct)
				{
					copy(a.begin(), a.end(), ostream_iterator<int>(std::cout, " "));
					cout << "\n";
					cout << test << "\n\n";
					return;
				}
				i++;
			}
			cout << "all pass\n\n";
		}
	};

	void Test_0154_Find_Min_in_Rotated_Sorted_Array_II()
	{
		Solution s;
		AutoTest ats;
		string s1;
		vector<int>nums;
		int option;
		while (true)
		{
			cout << "1: manual test    2: auto test    0: exit  > ";
			cin >> option;
			cin.ignore();
			if (option == 1)
			{
				s1.clear();
				cout << "array: ";
				getline(cin, s1);
				stringstream ss(s1);
				nums.clear();
				copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
				cout << s.findMin(nums) << "\n\n";
			}
			else if (option == 2)
			{
				ats.run();
			}
			else if (option == 0)
				break;
		}
	}
}