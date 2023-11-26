#include <vector>
#include <random>
#include <numeric>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>

namespace _0041_First_Missing_Positive {
	using namespace std;

	class Solution
	{
		/*
		* Jump back and forth in the array
		* If the array's length is 10, an extreme case is that I have number 1 to 10,
		* if I sort them, number x should be at index x-1.
		* So let me loop through the array, whenever I see a number between 1 to 10,
		* let me put it where it belongs. For example, I see number '5' in cell#0,
		* let me put '5' into cell#4. And if cell#4 has another number between 1 to 10,
		* for example '7', then I put '7' into the cell where '7' belongs.
		* Continue such process until there is an "open" cell.
		* A cell is considered "open" if it has a negative number or zero or a number greater than 10.
		*/
	public:
		int firstMissingPositive(vector<int>& nums)
		{
			size_t len = nums.size();  // if len is 10, if all positive numbers are present, I should have 1 to 10 inclusively
			for (size_t i = 0; i < len; i++)
			{
				if (nums[i] > len)
					nums[i] = -1; // set to 0 or any negative number is OK
				else if (0 < nums[i] && nums[i] <= len && nums[i] != i+1)
				{
					int n = nums[i];
					size_t currentIndex = i;
					size_t shouldBeIndex = nums[i] - 1;
					nums[currentIndex] = 0; // set to 0 or any negative number is OK

					while (currentIndex != shouldBeIndex)
					{
						if (nums[shouldBeIndex] <= 0 || nums[shouldBeIndex] > len)
						{
							nums[shouldBeIndex] = n;
							break;
						}
						else
						{
							swap(n, nums[shouldBeIndex]);
							currentIndex = shouldBeIndex;
							shouldBeIndex = n - 1;
						}
					}
				}
			}
			int expect = 1;
			for (int i = 0; i < len; i++)
			{
				if (nums[i] != expect)
					return expect;
				else
					expect++;
			}
			return expect;
		}
	};


	class AutoTest
	{
		random_device rd;
		mt19937 gen;
		uniform_int_distribution<> udi;

		Solution s;

		vector<int> v;

	public:

		AutoTest() :gen(rd()), udi(-(1 << 10), ((1 << 10) - 1)) {}    // binary:1000 is 8 = 2^3   1000 is also 1 right-shift 3 times

		void generateVector(vector<int>& v)
		{
			int len = 2000;
			v.resize(2000);
			generate(v.begin(), v.end(), bind(udi, ref(gen)));
		}

		int bruteForce(vector<int>& v)
		{
			sort(v.begin(), v.end());

			int expectNext = 1;

			for (int i = 0; i < v.size(); i++)
			{
				if (v[i] > 0 && v[i] != expectNext)
					return expectNext;
				else if (v[i]>0)
					expectNext++;
			}
			return expectNext;
		}

		void runOneTest()
		{
			generateVector(v);
			
			int fastAnswer = s.firstMissingPositive(v);
			int bruteAnswer = bruteForce(v);

			cout << fastAnswer << "              " << bruteAnswer << '\n';
			if (fastAnswer != bruteAnswer)
				throw runtime_error("ERROR!!!\n");
		}

		void run(int numberOfTests)
		{
			cout << "fast answer: " << "  bruteAnswer: " << '\n';
			for (int i = 0; i < numberOfTests; i++)
			{
				runOneTest();
			}
		}
	};


	void Test_0041_First_Missing_Positive()
	{
		Solution solu;
		AutoTest ats;

		int option;

		while (true)
		{
			cout << "1: manual test;  2: auto test;  0: exit  > ";
			cin >> option;
			if (option == 0) break;
			if (option == 1)
			{
				string s;
				vector<int> nums;
				cin.ignore();
				cout << "nums: ";
				getline(cin, s);
				if (s.size() == 0) break;
				stringstream ss(s);
				copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
				cout << solu.firstMissingPositive(nums) << '\n';
			}
			if (option == 2)
			{
				ats.run(10);
			}
		}
	}
}