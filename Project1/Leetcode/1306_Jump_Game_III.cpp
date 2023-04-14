#include "LibHeaders.h"

namespace _1306_Jump_Gamp_III {

	class Solution
	{
	public:

		std::unordered_set<int> visited;

		bool canReach_DFS(std::vector<int>&arr, int start)
		{
			int n = arr[start];
		
			if (n == 0)
				return true;

			int nextIndex = start - n;
			if (nextIndex >= 0 && visited.find(nextIndex) == visited.end())
			{
				// every number in arr is non-negative, if start is valid, nextIndex won't go beyond arr.size()
				visited.insert(nextIndex);
				if (canReach_DFS(arr, nextIndex))
					return true;
			}
			nextIndex = start + n;
			if (nextIndex < arr.size() && visited.find(nextIndex) == visited.end())
			{
				visited.insert(nextIndex);
				return canReach_DFS(arr, nextIndex);
			}
			return false;
		}

		bool canReach(std::vector<int>& arr, int start)
		{
			visited.clear();
			return canReach_DFS(arr, start);
		}
	};

	static void Test1(Solution& s)
	{
		std::vector<int> arr { 4,2,3,0,3,1,2 };
		std::cout << std::boolalpha << s.canReach(arr, 5) << '\n';
	}

	static void Test2(Solution& s)
	{
		std::vector<int> arr { 4,2,3,0,3,1,2 };
		std::cout << std::boolalpha << s.canReach(arr, 0) << '\n';
	}

	static void Test3(Solution& s)
	{
		std::vector<int> arr { 3,0,2,1,2 };
		std::cout << std::boolalpha << s.canReach(arr, 2) << '\n';
	}

	void Test_1306_Jump_Gamp_III()
	{
		Solution s;
		Test1(s);
		Test2(s);
		Test3(s);
	}
}