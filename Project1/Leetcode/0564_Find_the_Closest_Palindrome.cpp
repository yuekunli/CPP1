#include<iostream>
#include<string>
#include<algorithm>

namespace _0564_Find_the_Closest_Palindrome {

	using namespace std;

	/*
	
	first half reverse < second half:
	  just simple reverse:
	    odd:
	      62530    simple reverse   62526  (diff  4)   increase middle: 62626  (diff 96)
	    even:
	      112599   simple reverse:  112211 (diff 388)  increase middle: 113311 (diff 712)
	      112399   simple reverse:  112211 (diff 188)  increase middle: 113311 (diff 912)
	
	  need to increase middle:
	    odd:
	      11299    simple reverse:  11211 (diff 88)  increase middle: 11311 (diff  12)
	      62579    simple reverse:  62526 (diff 53)    increase middle: 62626  (diff 47)
	    even:
	      112999   simple reverse:  112211 (diff 788)  increase middle:  113311 (diff 312)


	first half reverse > second half:
	  need to decrease middle:
	    odd:
	      99800    simple reverse: 99899    decrease middle: 99799
	    even:
	      998100   simple reverse: 998899   decrease middle: 997799
	  need not to decrease middle (still use simple reverse)
	    odd:
	      958          simple reverse: 959      decrease middle 949
	      111 4 100    simple reverse: 1114111  decrease middle 1113111
	    even:
	      123 76 321   simple reverse: 12377321 (diff 1000)  decrease middle: 12366321 (diff 10,000)
	      987 43 000   simple reverse: 98744789 (diff 1789)  decrease middle: 98733789 (diff 9211)




	
	If input is already palindrome, and length is odd, is it always enough to just change the middle digit?

	If input is already palindrome, and length is even, is it always enough to just change the middle 2 digits?

	



	If it's always enough to just change the middle 1 or 2 digits, is it always increase or decrease?
	
	If the middle is(are) 1 to 8, decreasing certainly wins, 
	diff will be the same for decrease and increase, but decreasing makes it smaller


	middle is zero
	 
	two or more zeros in the middle, decreasing wins:
	 
	5 5 0 0 0 5 5    input
	5 5 0 1 0 5 5    increase middle, diff 1000
	5 4 9 9 9 4 5    decrease middle, but borrow from the left, then have to change the right accordingly, diff 110

	3 6 0 0 6 3   input
	3 6 1 1 6 3   increase, diff = 1100
	3 5 9 9 5 3   decrease, diff = 110

	one zero in the middle, increase wins
	
    4 0 4
	4 1 4   10
	3 9 3   11


	the middle is 9
	 
	one 9 in the middle, decreasing wins:
	 
	3 9 3  input
	3 8 3  decrease, diff = 10
	4 0 4  increase, diff = 11

	797  input
	808  increase, diff 11
	787  decrease, diff 10

	two 9's in the middle, increase wins:

	2 3 9 9 3 2
	2 4 0 0 4 2   110
	2 3 8 8 3 2   1100

	7 9 9 7
	8 0 0 8   11
	7 8 8 7   110


	-----------------------------------------------------------------------

	a special case when decreasing the middle:
	1 ........ 1     all zeros in the middle
	
	a special case when increasing the middle:
	9..........9   all 9's

	1 0 0 1   input
	0 9 9 0   this case is especially tricky, 0990 is not a valid number
	  9 9 9   correct way to decrease, diff = 2
	1 1 1 1   increase  diff = 110

	1 0 1
	  9 9

	These two special cases actually help each other.
	1000001   ----decrease---->  999999
	          <---increase-----

	If the input is one of these two special cases, then the answer is exactly the other special case.


	1 1 0 0 1 1    if there are more than one '1' on both side, it's not special any more
	1 0 9 9 0 1

	9 9 9 9 8 9  this is not special case, this is solved by (1) reverse first half, (2) decrease middle
	9 8 9 9 9 9  this is not special either, this is solved by (1) reverse first half, (2) increase middle

	As long as the input is not all 9's, it's not special case.

    */




	class Solution {
	public:

		string getFirstHalfRe(string& n)
		{
			size_t len = n.size();

			string firstHalf = n.substr(0, len / 2);
			string firstHalfRe = firstHalf;
			reverse(firstHalfRe.begin(), firstHalfRe.end());
			return firstHalfRe;
		}

		string getSecondHalf(string& n)
		{
			size_t len = n.size();

			string secondHalf = n.substr(len - len/2, len / 2);

			return secondHalf;
		}

		string reverseFirstHalf(string& n)
		{
			/*
			if length = 7, 
			0 1 2   3   4 5 6
			    |
			    2 is the 3rd element  7/2 = 3
			
			if length = 8
			0 1 2 3  4 5 6 7
			      |
			      3 is the 4th element  8/2 = 4
			*/

			size_t len = n.size();

			string firstHalf = n.substr(0, len / 2);
			string firstHalfRe = firstHalf;
			reverse(firstHalfRe.begin(), firstHalfRe.end());
			
			if (len % 2 == 1)
				return firstHalf + n[len / 2] + firstHalfRe;
			else
				return firstHalf + firstHalfRe;
		}

		string increaseMiddle(string& n)
		{
			size_t len = n.size();
			bool isAll9 = true;
			for_each(n.begin(), n.end(), [&](char& c) {
				if (c != '9')
					isAll9 = false;
				});
			if (isAll9)
			{
				string a(len + 1, '0');
				a[0] = a[len] = '1';
				return a;
			}

			if (n[len / 2] != '9')
			{
				string a(n);
				if (len % 2 == 0)
				{
					a[(len - 1) / 2] = a[len / 2] = n[len / 2] + 1;
				}
				else
				{
					a[(len - 1) / 2] = n[len / 2] + 1;
				}
				return a;
			}
			else
			{
				string a(n);

				int i;
				for (i = (int)((len - 1) / 2); i >= 0; i--)
					if (a[i] != '9') break;
				int j = (int)(len - 1 - i);     // example:  0 1 2 3 4 5 6 7 8 9
					                            //                 i     j           len=10  6=10-1-3
				a[i]++;
				a[j]++;
				for (int k = i + 1; k <= j - 1; k++)
					a[k] = '0';
				return a;
			}
		}

		string decreaseMiddle(string& n)
		{
			size_t len = n.size();

			if (n[0] == '1' && n[len - 1] == '1')
			{
				bool isAll0 = true;
				for (size_t i = 1; i < len - 2; i++)
				{
					if (n[i] != '0')
					{
						isAll0 = false;
						break;
					}
				}
				if (isAll0)
				{
					return string(len - 1, '9');
				}
			}

			if (n[len / 2] != '0')
			{
				string a(n);
				if (len % 2 == 0)
				{
					a[(len - 1) / 2] = a[len / 2] = n[len / 2] - 1;
				}
				else
				{
					a[(len - 1) / 2] = n[len / 2] - 1;
				}
				return a;
			}
			else
			{
				string a(n);

				int i;
				for (i = (int)((len - 1) / 2); i >= 0; i--)
					if (a[i] != '0') break;
				int j = (int)(len - 1 - i);     // example:  0 1 2 3 4 5 6 7 8 9
										        //                 i     j           len=10  6=10-1-3
				a[i]--;
				a[j]--;
				for (int k = i + 1; k <= j - 1; k++)
					a[k] = '9';
				return a;
			}
		}

		string getDiff(string& a, string& b)
		{
			string result;

			/*
			auto big = std::ref(a);
			auto sma = std::ref(b);
			if (a.size() < b.size() || (a.size() == b.size() && a[0] < b[0]))
			{
				big = std::ref(b);
				sma = std::ref(a);
			}
			*/

			string* hi;
			string* lo;
			int cmpResult = cmpTwoStr(a, b);
			if (cmpResult == 1)
			{
				hi = &a;
				lo = &b;
			}
			else if (cmpResult == 2)
			{
				hi = &b;
				lo = &a;
			}
			else
			{
				return "0";
			}


			char m[10] = {'0', '1', '2','3','4','5','6','7','8','9'};
			size_t hilen = (*hi).size();
			size_t lolen = (*lo).size();
			bool borrow = false;
			size_t i = 1;
			for (i = 1; i <= lolen; i++)
			{
				size_t j = lolen - i;
				size_t k = hilen - i;
				if ((*hi)[k] > (*lo)[j])
				{
					if (!borrow)
					{
						int d = (*hi)[k] - (*lo)[j];
						result.push_back(m[d]);
					}
					else
					{
						int d = (*hi)[k] - (*lo)[j] - 1;
						result.push_back(m[d]);
						borrow = false;
					}
				}
				else if ((*hi)[k] == (*lo)[j])
				{
					if (!borrow)
					{
						result.push_back('0');
					}
					else
					{
						result.push_back('9');
					}
				}
				else
				{
					if (!borrow)
					{
						int d = (*hi)[k] - '0' + 10 - ((*lo)[j] - '0');
						result.push_back(m[d]);
						borrow = true;
					}
					else
					{
						int d = (*hi)[k] - '0' + 10 - ((*lo)[j] - '0') - 1;
						result.push_back(m[d]);
					}
				}

			}
			if (i < hilen)
			{
				for (; i <= hilen; i++)
				{
					size_t k = hilen - i;
					if (borrow)
					{
						if ((*hi)[k] == '0')
						{
							result.push_back('9');
						}
						else
						{
							result.push_back((*hi)[k] - 1);
							borrow = false;
						}
						continue;
					}
					result.push_back((*hi)[k]);
				}
			}
			reverse(result.begin(), result.end());

			int leadingZero = 0;
			for (size_t i = 0; i < result.size(); i++)
			{
				if (result[i] == '0')
					leadingZero++;
				else
					break;
			}
			if (leadingZero != 0)
				return result.substr(leadingZero);
			else
				return result;
		}

		int cmpTwoStr(string& a, string& b)
		{
			if (a.size() > b.size()) return 1;
			if (a.size() < b.size()) return 2;

			size_t len = a.size();
			for (size_t i = 0; i < len; i++)    // this should have a simple way
			{
				if (a[i] > b[i]) return 1;
				if (a[i] < b[i]) return 2;
			}
			return 0;
		}

		string nearestPalindromic(string n)
		{
			if (n.size() == 1)
				return to_string((char)(n[0] - '1'));

			string firstHalfRe = getFirstHalfRe(n);
			string secondHalf = getSecondHalf(n);

			if (cmpTwoStr(firstHalfRe, secondHalf) == 1)  // first half reverse > second half
			{
				string afterReverse = reverseFirstHalf(n);
				string afterDecrease = decreaseMiddle(afterReverse);
				string diff_rever_origin = getDiff(n, afterReverse);
				string diff_decre_origin = getDiff(n, afterDecrease);
				int a = cmpTwoStr(diff_rever_origin, diff_decre_origin);
				if (a == 1)
					return afterDecrease;
				else if (a == 2)
					return afterReverse;
				else
					return afterDecrease;
			}
			else if (cmpTwoStr(firstHalfRe, secondHalf) == 2) // first half reverse < second half
			{
				string afterReverse = reverseFirstHalf(n);
				string afterIncrease = increaseMiddle(afterReverse);
				string diff_rever_origin = getDiff(n, afterReverse);
				string diff_incre_origin = getDiff(n, afterIncrease);
				int a = cmpTwoStr(diff_rever_origin, diff_incre_origin);
				if (a == 1)
					return afterIncrease;
				else if (a == 2)
					return afterReverse;
				else
					return afterReverse;
			}
			else
			{
				size_t len = n.size();
				if (n[len / 2] == '0' || n[len / 2] == '9')
				{
					string afterIncrease = increaseMiddle(n);
					string afterDecrease = decreaseMiddle(n);
					string diff_incre_origin = getDiff(n, afterIncrease);
					string diff_decre_origin = getDiff(n, afterDecrease);
					int a = cmpTwoStr(diff_incre_origin, diff_decre_origin);
					if (a == 1)
						return afterDecrease;
					else if (a == 2)
						return afterIncrease;
					else //(a == 0)
						return afterDecrease;
				}
				else
				{
					string afterDecrease = decreaseMiddle(n);
					return afterDecrease;
				}
			}
		}
	};


	void Test_0564_Find_the_Closest_Palindrome()
	{
		Solution solu;
		while (true)
		{
			string number;
			cout << "number: ";
			getline(cin,number);
			if (number.size() == 0) break;
			cout << solu.nearestPalindromic(number) << '\n';
		}
	}
}