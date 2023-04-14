#include "LibHeaders.h"

namespace _1808_Maximize_Number_Nice_Divisors {

	using namespace std;

	class Solution
	{
		/*
		* It doesn't matter what 'n' is, it doesn't matter what the prime factors are.
		* All it matters is to evenly distribute the prime factors.
		* For example: if there are 5 prime factors, the best result is achieved with
		* such combination of prime factors:
		* [2,2,2,3,3]
		* Because the a nice divisor must be divisible by all prime factors, it must have
		* at least one '2' and one '3'.
		* How many 2's can it have? It can have 1, 2, or 3.
		* How many 3's can it have? it can have 1 or 2.
		* The problem basically becomes picking one or a few from one group, and picking
		* one or a few from another group, how many ways to pick and combine?
		* 2         *  3
		* 2         *  3 * 3
		* 2 * 2     *  3
		* 2 * 2     *  3 * 3
		* 2 * 2 * 2 *  3
		* 2 * 2 * 2 *  3 * 3
		* 
		* 3 ways to pick 2, 2 ways to pick 3. Combinatorics tell me there are 6 ways to
		* form a product.
		* It doesn't matter what the prime factors are.
		* [2,2,2,7,7] this yields the same result.
		* Basically the prime factors can be divided into groups, of course the ones with
		* same value go to the same group.
		* The answer is just the product of the counts in each group.
		* How to maximize the product? The overall count is given, which is the total count
		* of prime factors. The more evenly they are divided into groups, the greater
		* the product can be.
		* Among all shapes with the same perimeter, which one has the greatest area?
		* Circle.
		* Among all rectangles with the same perimeter, which one has the greatest area?
		* Square.
		*/
		/*
		* However that is not enough.
		* If the total prime factors is 25, should I just have 5 kinds of factors and each
		* kind has 5 individuals? The number of nice divisors is 5 * 5 * 5 * 5 * 5
		* No.
		* Whenever I have something in a group and the size of the group is 5, I can break
		* them to 2 in one group and 3 in another group. Because in that way these two smaller
		* groups can contribute '6' to the overall product. If they stay in the same group,
		* the contribution is '5'.
		* Whenever I have 3 groups each of which has 2 individuals, I should convert them to
		* 2 groups each of which has 3 individuals, because 2 * 2 * 2 = 8 whereas 3 * 3 = 9
		* However this doesn't mean 3 is always more preferable than 2.
		* 2 groups each of which has 2 individuals can contribute 2 * 2 = 4
		* whereas a group of 3 individuals and a group of 1 individual can only contribute 3
		*/
	public:
		int maxNiceDivisors(int primeFactors)
		{
			int a = primeFactors / 3;
			int b = primeFactors % 3;
			if (b == 0)
				return (int)pow(3, a) % 1'000'000'007;
			if (b == 2)
				return (int)(pow(3, a)) * 2 % 1'000'000'007;
			if (b==1)
				return (int)pow(3, a-1) * 4 % 1'000'000'007;
		}
	};

	class Solution2
	{
		/*
		* last solution shows the idea, but "primeFactors" can be as big as 10^9
		* So 'a' in that case can be very big, I must break down the exponential
		* operation and do the modulus operation along the way
		*/
		/*
		* For example, if I want to calculate 3^210
		* 210 = 2^7 + 2^6 + 2^4 + 2^1   (basically convert to binary number)
		* 3^210 = 3^128 * 3^64 * 3^16 * 3^2
		* The length of the recordbook doesn't have to 210
		* It can be 7.
		*/
		/*
		* unsigned long long uses 64 bits.
		* The maximum of 64 bit unsigned integer is 1.84 * 10^19
		* 1'000'000'007 ^ 2 ~ 1 * 10^18
		* Therefore if two numbers are both within 1'000'000'007,
		* the product of these two numbers can fit in unsigned long long
		* 
		* a 32-bit signed integer can be as big as 2 * 10^9
		* The template stipulates function return type is int. So I can assume
		* the system uses 32 bits for "int". This means the result of a number
		* modulus 1'000'000'007 can fit in "int".
		*/
		int expWithMod(vector<int>& r, int base, int exp, int mod)
		{
			int k = log2(exp);
			int t = exp - pow(2, k);
			
			unsigned long long r1 = 0;
			int r2 = 0;
			if (r[k] != 0)
				r1 = r[k];
			else if (k == 0)  // this is asking base^1 because 2^0 = 1
			{
				r1 = base;
				r[k] = r1;
			}
			else
			{
				int r3 = expWithMod(r, base, pow(2, k - 1), mod);
				r1 = (unsigned long long)r3 * (unsigned long long)r3 % mod;	
				r[k] = r1;
			}
			
			//------------------------

			if (t > 0)
				r2 = expWithMod(r, base, t, mod);
			else
				r2 = 1;

			return r1 * r2 % mod;
		}
	public:
		int maxNiceDivisors(int primeFactors)
		{
			int a = primeFactors / 3;
			int b = primeFactors % 3;
			int exp = (b == 0 || b == 2) ? a : a - 1;
			int recordbookLength = log2(exp) + 1;
			vector<int>r(recordbookLength); // for example r[4] = 3^16
			
			int c = expWithMod(r, 3, exp, 1'000'000'007);
			if (b == 0)
				return c;
			if (b == 1)
				return c * 4 % 1'000'000'007;
			if (b == 2)
				return c * 2 % 1'000'000'007;

			return 0;
		}
	};

	void Test_1808_Maximize_Number_Nice_Divisors()
	{
		Solution2 s;
		int primeFactors;
		while (true)
		{
			cout << "prime factors:  (0 to exit) > ";
			cin >> primeFactors;
			if (primeFactors == 0) break;
			cout << s.maxNiceDivisors(primeFactors) << '\n';
		}
	}
}