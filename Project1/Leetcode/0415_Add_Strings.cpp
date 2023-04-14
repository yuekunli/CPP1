#include<iostream>
#include<string>
#include<algorithm>

namespace _0415_Add_Strings {

	using namespace std;

	class Solution1
	{
	public:

		std::string addStrings(std::string num1, std::string num2)
		{
			std::size_t L1 = num1.size();
			std::size_t L2 = num2.size();

			std::string result;

			if (L1 > L2)
			{
				result = addStrings2(num1, num2);
			}
			else
			{
				result = addStrings2(num2, num1);
			}

			return result;
		}

		std::string addStrings2(std::string ls, std::string ss) // ls: long string;  ss: short string
		{
			typename std::string::reverse_iterator it_s = ss.rbegin();
			typename std::string::reverse_iterator it_l = ls.rbegin();

			std::string result;

			int carry = 0;
			int n1, n2, n;

			for (; it_s < ss.rend(); it_s++, it_l++)
			{
				n1 = (int)*it_s - '0';
				n2 = (int)*it_l - '0';
				n = n1 + n2 + carry;
				if (n >= 10)
				{
					carry = 1;
					n = n - 10;
				}
				else
				{
					carry = 0;
				}
				result.push_back((char)(n + '0'));
			}

			for (; it_l < ls.rend(); it_l++)
			{
				n = (int)*it_l - '0';
				n = n + carry;
				if (n > 10)
				{
					carry = 1;
					n = n - 10;
				}
				else
				{
					carry = 0;
				}
				result.push_back((char)(n + '0'));
			}

			std::reverse(result.begin(), result.end());
			return result;
		}
	};


	class Solution2
	{
	public:
		struct AddOneDigitOp  // add two digits together
		{
			int carry = 0;

			char operator()(const char& lhs, const char& rhs)
			{
				int lnum = (int)lhs - '0';
				int rnum = (int)rhs - '0';
				int result_num = lnum + rnum + carry;
				if (result_num >= 10)
				{
					carry = 1;
					result_num -= 10;
				}
				else
				{
					carry = 0;
				}
				char result_ch = (char)(result_num + '0');
				return result_ch;
			}
		};

		struct AddOneDigitOp2   // add the long numbers extra digit, just the digit and the carry
		{
			int carry;
			AddOneDigitOp2(int carry_) :carry(carry_) {}
			char operator()(char const& a)
			{
				int n = a - '0';
				n = n + carry;
				if (n >= 10)
				{
					carry = 1;
					n -= 10;
				}
				else
					carry = 0;
				return (char)('0' + n);
			}
		};

		// I may have counted every digit from both numbers, the a carry can still create a new digit
		//     999
		//  +   75
		//------------
		//    1074

		string addStrings(string num1, string num2)
		{
			if (num1.size() > num2.size())   // make sure num1 is the shorter one
				return addStrings(num2, num1);

			AddOneDigitOp op{};
			string result;
			transform(num1.rbegin(), num1.rend(), num2.rbegin(), back_inserter(result), std::ref(op));  // the key is to use std::ref()
			
			auto it = num2.rbegin() + num1.size();  // make "it" point to the first digit in num2 that is over the length of num1
			if (it != num2.rend())
			{
				AddOneDigitOp2 op2{ op.carry };
				transform(it, num2.rend(), back_inserter(result), std::ref(op2));
				// I'm done processing num2's extra digits, but there can still be a carry
				if (op2.carry > 0)
					result.push_back((char)(op2.carry + '0'));
			}
			else
			{
				// num1 and num2 are equally long, but there can be a carry
				if (op.carry > 0)
					result.push_back((char)(op.carry+'0'));
			}

			reverse(result.begin(), result.end());
			return result;
		}
	};

	void Test_0415_Add_Strings()
	{
		Solution2 solu;

		while (true)
		{
			string s1;
			string s2;
			cout << "nums1: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			cout << "nums2: ";
			cin >> s2;
			cin.ignore();
			cout << solu.addStrings(s1, s2) << '\n';
		}
	}

}