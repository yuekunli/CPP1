#include <iostream>
#include <sstream>
#include <vector>

namespace _0012_Integer_to_Roman {

	using namespace std;

	class Solution
	{
	public:
		static vector<string> const thou;
		static vector<string>  const hend;
		static vector<string>  const ten;
		static vector<string>  const indi;


		string intToRoman(int num)
		{
			string roman;
			int th = num / 1000;
			if (th > 0)
				roman += thou[th - 1];
			num = num - th * 1000;
			int he = num / 100;
			if (he > 0)
				roman += hend[he - 1];
			num = num - he * 100;
			int te = num / 10;
			if (te > 0)
				roman += ten[te - 1];
			num = num - te * 10;
			int in = num;
			if (in > 0)
				roman += indi[in - 1];

			return roman;
		}
	};
	
	vector<string> const Solution::thou{ "M", "MM", "MMM" };
	vector<string> const Solution::hend{ "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM" };
	vector<string> const Solution::ten{ "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC" };
	vector<string> const Solution::indi{ "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX" };

	void Test_0012_Integer_to_Roman()
	{
		int num;
		Solution s;
		while (true)
		{
			cout << "input number: (1 ~ 3999) (zero to exit)  > ";
			cin >> num;

			if (num <= 0) break;

			cout << s.intToRoman(num) << "\n\n";
		}
	}
}