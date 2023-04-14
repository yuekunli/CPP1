#include "LibHeaders.h"

namespace _1310_XOR_Queries_of_a_Subarray {

	using namespace std;

	class Solution
	{
		/*
		* I want the XOR from 5 to 13, if I know the XOR from 1 to 4 and from 1 to 13,
		* I can quickly get the XOR from 5 to 13.
		* For a certain bit, if that bit is 1 in the XOR from 1 to 4 and that bit is 0
		* in the XOR from 1 to 13, this means there are overall even number of 1's on that
		* bit from 1 to 13 and there are odd number of 1's on that bit from 1 to 4,
		* so there must be another odd number of 1's on that bit from 5 to 13.
		*/
	public:

		vector<int> xorQueries(vector<int>& arr, vector<vector<int>>& queries)
		{
			vector<int> rollingXOR;

			partial_sum(arr.begin(), arr.end(), back_inserter(rollingXOR), bit_xor<int>{});
		
			vector<int> answers;

			for (vector<int>& q : queries)
			{
				int start = q[0];
				int end = q[1];
				if (start == 0)
					answers.emplace_back(rollingXOR[end]);
				else
					answers.emplace_back(rollingXOR[end] ^ rollingXOR[start-1]);
			}
			return answers;
		}
	};

	static void basic_test()
	{
		unsigned int a = 0b1000;
		unsigned int b = 0b1100;
		unsigned int c = 0b1110;

		std::cout << std::bitset<4>{a^ b^ c} << '\n';
		std::cout << std::bitset<4>{a& b& c} << '\n';
		std::cout << std::bitset<4>{a | b | c} << '\n';
	}

	auto print = [](vector<int> const & v)
	{
		for (auto& i : v)
			cout << i << "  ";
		cout << '\n';
	};

	static void Test1()
	{
		vector<int> arr { 1,3,4,8 };
		vector<vector<int>> queries { {0,1},{1,2},{0,3},{3,3} };
		Solution s;
		vector<int> const & answer = s.xorQueries(arr, queries);
		print(answer);

	}

	static void Test2()
	{
		vector<int>arr { 4,8,2,10 };
		vector<vector<int>>queries { {2,3},{1,3},{0,0},{0,3} };
		Solution s;
		vector<int> const & answer = s.xorQueries(arr, queries);
		print(answer);
	}

	void Test_1310_XOR_Queries_of_a_Subarray()
	{
		//basic_test();
		Test1();
		Test2();
	}
}