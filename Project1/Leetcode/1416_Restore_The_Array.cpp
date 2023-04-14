#include "LibHeaders.h"

namespace _1416_Restore_The_Array {

	using namespace std;

	class Solution
	{
	public:

		int numberOfArrays(string s, int k)
		{
			int len = s.size();
			vector<int> rb(len, 0); // record book

			if (stoi(s.substr(len - 1, 1)) != 0 && stoi(s.substr(len - 1, 1)) <= k)
				rb[len - 1] = 1;

			for (int i = len - 2; i >= 0; i--)
			{
				int soFarTotal = 0;
					
				int j = 0;

				// . . . . . a b c d
				//           |   |
				//           i   j (j is 2 in this example)
				// i: current digit I'm considering
				// j: how many digits after i are being considered

				while (i + j < len)
				{
					int part = stoi(s.substr(i, j + 1));
						
					if (part == 0) break;
						
					if (part <= k)
					{
						if (i + j + 1 == len)
							soFarTotal++;
						else
							soFarTotal += rb[i + j + 1];
						j++;
					}
					else
						break;
				}
				rb[i] = soFarTotal;				
			}
			return rb[0];
		}
	};


	class Solution2
	{
		// save space, record book can be shorter
		// a b c d e f g h i j k l m n
		//         |       |
		// I'm considering 'e' right now, let me take 4 letters from its right, i.e. I'm taking "fghi"
		// I put together 'e' and 'fghi', I get 'efghi', if 'efghi' is valid, I want to know how many
		// possibilities "jklmn" presents. rb[4] means taking 4 out of the substring on the right, the
		// number of possibilities presented by the rest of the substring.

	public:

		int numberOfArrays(string s, int k)
		{
			int len = s.size();
			vector<int> rb(10, 0);  // 1 <= k <= 10^9, k is at most a 10-digit number

			if (stoi(s.substr(len - 1, 1)) != 0 && stoi(s.substr(len - 1, 1)) <= k)
				rb[0] = 1;

			for (int i = len - 2; i >= 0; i--)
			{
				int soFarTotal = 0;

				int j = 0;

				// . . . . . a b c d
				//           |   |
				//           i   j (j is 2 in this example)
				// i: current digit I'm considering
				// j: how many digits after i are being considered

				while (i + j < len && j < 10)
				{
					int part = stoi(s.substr(i, j + 1));

					if (part == 0) break; // if 'a' is '0', I break out immediately, I won't consider 'ab', or 'abc', or anything, because they all have leading 0

					if (part <= k)
					{
						if (i + j + 1 == len)
							soFarTotal++;
						else
							soFarTotal += rb[j];
						j++;
					}
					else
						break;
				}
				rotate(rb.begin(), prev(rb.end()), rb.end()); // if rb was "25, 19, 11, 4", I want to drop '4', right-shift by 1, change it to "__, 25, 19, 11"
				rb[0] = soFarTotal;
			}
			return rb[0];
		}
	};


	void Test_1416_Restore_The_Array()
	{
		string input;
		int k;
		Solution2 s;

		while (true)
		{
			input.clear();
			cout << "input: ";
			getline(cin, input);
			if (input.size() == 0) break;
			cout << "k: ";;
			cin >> k;
			cin.ignore();
			cout << s.numberOfArrays(input, k) << '\n';
		}
	}
}