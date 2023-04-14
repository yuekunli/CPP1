#include "LibHeaders.h"

namespace _1316_Distinct_Echo_Substrings {

	using namespace std;

	class Solution
	{
		/*
		*  Use the KMP string matching algorithm's pre-processing.
		*  If text is "abcdabcdef", the first 8 characters: "abcdabcd" can match a prefix of 4
		*  a b c d a b c d
		*          a b c d
		* 
		*  8 == 4 * 2    So I have found an echo substring.
		*  But where do I start the KMP pre-processing?
		*  What if the input is " x y z a b c d a b c d e f"
		*                               |
		*                             I have to start from here, as if the input just starts from here.
		* 
		*/

	public:

		unordered_set<string> echos;


		void checkSubstring(string&& s)
		{
			int len = s.size();

			if (len == 1) return;

			int* prefixSuffix = new int[len];
			for (int i = 0; i < len; i++)
				prefixSuffix[i] = 0;
			// the length of the prefix that can match a certain suffix
			// initializing every cell to 0 is good enough

			int i = 1, k = 0;

			while (i < len)
			{
				if (s[i] == s[k])
				{
					prefixSuffix[i] = k + 1;
					if (i + 1 == 2 * prefixSuffix[i])
					{
						string a = s.substr(0, i+1);
						if (echos.find(a) == echos.end())
						{
							echos.insert(a);
						}
					}
					i++;
					k++;
				}
				else
				{
					if (k == 0)
						i++;
					else
						k = prefixSuffix[k - 1];
				}
			}

			delete[] prefixSuffix;
			return;
		}

		int distinctEchoSubstrings(string text)
		{
			echos.clear();
			for (int i = 0; i < text.size() - 1; i++)
				checkSubstring(text.substr(i));
			return echos.size();

		}
	};


	class Solution2
	{
		//http://leetcode.ca/2019-07-08-1316-Distinct-Echo-Substrings/
		// rolling hash

	public:

		int distinctEchoSubstrings(string s)
		{
			int N = s.size();
			int mod = 100'000'000 + 7;
			int cnt = 0;
		
			vector<long> v(N);
			// v[9] is the hash of substring s[6,9] (s[6] and s[9] are included) when len == 4
			// When I get the value of v[9], which means I just computed the hash for substring s[6,9]
			// I need to compare it to v[5] which is the hash for substring s[2,5]

			for (int len = 1; len <= N / 2; ++len) // only need to check substrings whose length <= (input.length / 2)
			{
				long hash = s[0] - 'a', p = 1;
				unordered_set<string> seen;
				for (int i = 1; i < N; i++)
				{
					if (i < len)
					{
						// I'm building up the hash for the very first substring of that length
						// if len == 4, I'm within 0 to 3, the first substring of length 4
						hash = (hash * 26 + s[i] - 'a') % mod;
						p = (p * 26) % mod;  // p is the weight of the most significant digit
											 // when i == 1, I've considered two digits, the 1st digit (i.e. s[0]) has a weight of 26
					}
					else
					{
						// I'm out of the boundary of the very 1st substring, I need to start computing rolling hash

						hash = ((hash - (s[i - len] - 'a') * p) * 26 + s[i] - 'a') % mod;  // i-len is the index of the most significant digit of the previous substring
						if (hash < 0)
							hash += mod;   // this is important, because frequent modulus operation, "hash" can be less than (the previous significant digit * p)
						if (i - 2 * len + 1 >= 0 && v[i - len] == hash) // if len==4, the earliest point I can see an echo is when i==7, in another word i >= 2*len-1
						{
							// an echo substring can appear multiple times, for example: a b a b x y a b a b
							auto a = s.substr(i - 2 * len + 1, len); // a is half of the echo substring
							if (seen.count(a))  // if the half echo is already registed, I must have seen the entire echo
								continue;
							seen.insert(a);
							if (a == s.substr(i - len + 1, len))  // after all, all I've done is to compare the modulus of the hash, I need to compare the real substring
								cnt++;

							// I probably need to do this comparison earlier than checking whether 'a' is registered in "seen"
						}
					}
					v[i] = hash;
				}
			}
			return cnt;
		}
	};


	class Solution3
	{
		/*
		* still use rolling hash algorithm, but with better variable naming and cleaner code than Solution2
		*/
	public:

		/*
		* compare [s1, s1+length) and [s1+length, s1+2*length)
		*/
		bool twoSubstringsEqual(string& text, int s1, int length)
		{
			return equal(text.begin() + s1, text.begin() + s1 + length, text.begin() + s1 + length);
		}

		int distinctEchoSubstrings(string text)
		{
			int len = text.size();
			int m = 1'000'000'007;
			vector<unsigned long long> hash(len);
			unordered_set<string> echoSubstrings;
		
			// half of an echo substring can be at least 1 element and at most len/2 elements
			// if len is 11, an echo substring can be at most 10-element long, half of that is 5
			for (int halfLength = 1; halfLength <= len / 2; halfLength++)
			{
				/*
				*   a  b  c  d  e  f  g  h  i  j  k  l  m  n  p  q
				* 
				* for example, if halfLength is 4, calculate the hash for "ghij"
				* then compare it with "cdef".
				* I still need to calculate hash for "lmnp", "klmn", and "jklm", even though they won't
				* be the 1st half of an echo substring, they can still be the 2nd half of a echo substring
				*/
				unsigned long long h = 0;
				unsigned long long highDigitWeight = static_cast<unsigned long long>(pow(26, halfLength - 1));
				for (int start = 0; start <= len - halfLength; start++)  // if len is 11, when checking halfLength 3, I can start at 8
				{
					if (start == 0)
					{
						for (int i = 0; i < halfLength; i++)
						{
							h *= 26;  // if h is signed long, a number less than 1'000'000'007 multiple 26 can go over the upper limit of signed long
							h += text[i] - 'a';
							h %= m;
						}
					}
					else
					{
						unsigned long long prevHighDigitContribution = (text[start - 1] - 'a') * highDigitWeight;
						if (h < prevHighDigitContribution)
						{
							prevHighDigitContribution %= m;
							// h can still be less than prevHighDigitContribution even though they are both in range [0, 1'000'000'007)
							if (h < prevHighDigitContribution)
								h += m;
						}
						h -= prevHighDigitContribution;
						h *= 26;
						h += text[start + halfLength - 1] - 'a';
						h %= m;
					}

					if (start >= halfLength)
					{
						// if halfLength is 4, when start is 4, I compare with the substring in this range [0,3] whose hash is stored at hash[0] 
						if (h == hash[start - halfLength] && twoSubstringsEqual(text, start - halfLength, halfLength))
						{
							echoSubstrings.insert(text.substr(start-halfLength, 2*halfLength));
						}
						hash[start] = h;
					}
					else
					{
						// I'm still calculating hash for the first a few half length substrings
						// there is nothing to compare with.
						hash[start] = h;
					}
				}
			}
			return echoSubstrings.size();
		}
	};


	void Test_1316_Distinct_Echo_Substrings()
	{
		Solution3 s;

		while (true)
		{
			string text;
			cout << "input string: ";
			getline(cin, text);
			if (text.size() == 0) break;
			cout << s.distinctEchoSubstrings(text)<<'\n';
		}
	}

}