#include "LibHeaders.h"

namespace _1320_Minimum_Distance_to_Type_a_Word_Using_Two_Fingers {

using namespace std;

class Solution
{
public:


	array<pair<int, int>, 26> m { {   
			           // array initialization must use double-braces or '=', 
			           // but '=' sign doesn't work here, probably becasue the pairs need braces to initialize too.
			           // so even if I change to '=' followed by single-brace, the compiler still thinks I'm using
			           // double-braces?
		{0,0},
		{0,1},
		{0,2},
		{0,3},
		{0,4},
		{0,5},
		{1,0},
		{1,1},
		{1,2},
		{1,3},
		{1,4},
		{1,5},
		{2,0},
		{2,1},
		{2,2},
		{2,3},
		{2,4},
		{2,5},
		{3,0},
		{3,1},
		{3,2},
		{3,3},
		{3,4},
		{3,5},
		{4,0},
		{4,1}
	} };

	int minDist_Recur(string&w, int ***t, int idx1, int idx2, int printedLen)
	{
		if (t[idx1][idx2][printedLen] != -1)
			return t[idx1][idx2][printedLen];

		int len = w.size();
		if (printedLen == len)
			return 0;

		if (idx1 > idx2)
			swap(idx1, idx2);   // why did I do this swap? Tested with this swap, the two leetcode examples produce the same results

		char nextletter = w[printedLen];
		char f1Current = w[idx1];
		char f2Current = w[idx2];

		int usef1 = 0;
		int usef2 = 0;

		usef1 = abs(m[nextletter - 'A'].first - m[f1Current - 'A'].first) + abs(m[nextletter - 'A'].second - m[f1Current - 'A'].second)
			+ minDist_Recur(w, t, printedLen, idx2, printedLen + 1);

		usef2 = abs(m[nextletter - 'A'].first - m[f2Current - 'A'].first) + abs(m[nextletter - 'A'].second - m[f2Current - 'A'].second)
				+ minDist_Recur(w, t, idx1, printedLen, printedLen + 1);
		
		t[idx1][idx2][printedLen] = min({ usef1, usef2 });
		return t[idx1][idx2][printedLen];
	}

	int minimumDistance(string word)
	{
		int len = word.size();

		int*** t = new int** [len];
		for (int i = 0; i < len; i++)
		{
			t[i] = new int* [len];
			for (int j = 0; j < len; j++)
			{
				t[i][j] = new int[len + 1];
				for (int k = 0; k <= len; k++)
				{
					t[i][j][k] = -1;
				}
			}
		}

		int minDist = std::numeric_limits<int>::max();
		// one of the two fingers has to start on the first letter, why don't I just designate that finger as finger#1
		for (int idx2 = 1; idx2 < len; idx2++)
		{
			int dist = minDist_Recur(word, t, 0, idx2, 0);
			if (dist < minDist)
				minDist = dist;
		}

		for (int i = 0; i < len; i++)
		{
			for (int j = 0; j < len; j++)
			{
				delete[] t[i][j];
			}
			delete[] t[i];
		}
		delete[] t;

		return minDist;
	}
};

class Solution2
{
	/*
	* small improvement over Solution1. Don't even try the finger that is waiting on a letter with
	* index greater than the current letter to be printed.
	* This helps at the beginning. If I decide to have the 2nd finger wait on the 5th letter, then
	* I essentially have decided to have 1st finger type letters at index 0 to 4.
	*/
public:
	array<pair<int, int>, 26> m{ {
			// array initialization must use double-braces or '=', 
			// but '=' sign doesn't work here, probably becasue the pairs need braces to initialize too.
			// so even if I change to '=' followed by single-brace, the compiler still thinks I'm using
			// double-braces?
	{0,0},
	{0,1},
	{0,2},
	{0,3},
	{0,4},
	{0,5},
	{1,0},
	{1,1},
	{1,2},
	{1,3},
	{1,4},
	{1,5},
	{2,0},
	{2,1},
	{2,2},
	{2,3},
	{2,4},
	{2,5},
	{3,0},
	{3,1},
	{3,2},
	{3,3},
	{3,4},
	{3,5},
	{4,0},
	{4,1}
	} };

	/*
	* If printedLen is 5, the index of the letter I'm about to print is also 5.
	* Let's say I want to print the letter at index 5, and 2nd finger is initially put on the letter at index 7.
	* This means I have decide to use the 1st finger to type letters at index 0 to 6. Therefore, when I'm in
	* that range, don't even try using 2nd finger.
	* If I'm about to print the letter at index 7, and if both fingers are resting on letters with indexes less than 7,
	* then I can consider both fingers.
	*/
	int minDist_Recur(string& w, int*** t, int idx1, int idx2, int printedLen)
	{
		if (t[idx1][idx2][printedLen] != -1)
			return t[idx1][idx2][printedLen];

		int len = w.size();
		if (printedLen == len)
			return 0;

		char nextletter = w[printedLen];
		char f1Current = w[idx1];
		char f2Current = w[idx2];

		int usef1 = numeric_limits<int>::max();
		int usef2 = numeric_limits<int>::max();

		if (idx1 <= printedLen)
			usef1 = abs(m[nextletter - 'A'].first - m[f1Current - 'A'].first) + abs(m[nextletter - 'A'].second - m[f1Current - 'A'].second)
				+ minDist_Recur(w, t, printedLen, idx2, printedLen + 1);

		if (idx2 <= printedLen)
			usef2 = abs(m[nextletter - 'A'].first - m[f2Current - 'A'].first) + abs(m[nextletter - 'A'].second - m[f2Current - 'A'].second)
				+ minDist_Recur(w, t, idx1, printedLen, printedLen + 1);

		t[idx1][idx2][printedLen] = min({ usef1, usef2 });
		return t[idx1][idx2][printedLen];
	}

	int minimumDistance(string word)
	{
		int len = word.size();

		int*** t = new int** [len];
		for (int i = 0; i < len; i++)
		{
			t[i] = new int* [len];
			for (int j = 0; j < len; j++)
			{
				t[i][j] = new int[len + 1];
				for (int k = 0; k <= len; k++)
				{
					t[i][j][k] = -1;
				}
			}
		}

		int minDist = std::numeric_limits<int>::max();
		// one of the two fingers has to start on the first letter, why don't I just designate that finger as finger#1
		for (int idx2 = 1; idx2 < len; idx2++)
		{
			int dist = minDist_Recur(word, t, 0, idx2, 0);
			if (dist < minDist)
				minDist = dist;
		}

		for (int i = 0; i < len; i++)
		{
			for (int j = 0; j < len; j++)
			{
				delete[] t[i][j];
			}
			delete[] t[i];
		}
		delete[] t;

		return minDist;
	}
};


void Test_1320_Minimum_Distance_to_Type_a_Word_Using_Two_Fingers()
{
	Solution2 s;

	while (true)
	{
		string word;
		cout << "input string: ";
		getline(cin, word);
		if (word.size() == 0) break;
		cout << s.minimumDistance(word) << '\n';
	}
}

}