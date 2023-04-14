#include<iostream>
#include<string>
#include<vector>


namespace _1092_Shortest_Common_Supersequence {

	using namespace std;

	class Solution
	{
	public:
		string shortestCommonSupersequence(string str1, string str2)
		{
			size_t l1 = str1.size();
			size_t l2 = str2.size();

			vector<vector<signed char>> reconstruct(l1, vector<signed char>(l2, 0)); // 1: from topleft; 2: from top; 3: from left
			vector<int> r(l2, 0);  // length of longest-common-subsequence, 1 dimension array is enough

			r[0] = str1[0] == str2[0] ? 1 : 0;
			for (size_t i = 1; i < l2; i++)
			{
				if (str1[0] == str2[i])
					r[i] = 1;
				else
					r[i] = r[i - 1];
			}
			for (size_t i = 1; i < l1; i++)
			{
				int previousStep = str1[i] == str2[0] ? 1 : r[0];
				for (size_t j = 1; j < l2; j++)
				{
					int thisStep;
					if (str1[i] == str2[j])
					{
						thisStep = r[j - 1] + 1;
						reconstruct[i][j] = 1;
						
					}
					else if (r[j] > previousStep)
					{
						thisStep = r[j];
						reconstruct[i][j] = 2;
					}
					else
					{
						thisStep = previousStep;
						reconstruct[i][j] = 3;
					}
					r[j - 1] = previousStep;
					previousStep = thisStep;
				}
				r[l2 - 1] = previousStep;
			}

			string shcosup; // shortest common supersequence

			int i = (int)l1 - 1, j = (int)l2 - 1;
			while (i > 0 && j > 0)
			{
				if (reconstruct[i][j] == 1)
				{
					shcosup.push_back(str1[i]);
					i--; j--;
				}
				else if (reconstruct[i][j] == 2)
				{
					shcosup.push_back(str1[i]);
					i--;
				}
				else
				{
					shcosup.push_back(str2[j]);
					j--;
				}
			}

			/*
			* the row#0 and column#0 of the reconstruct 2-D array just have the default value
			*/

			if (str1[i] == str2[j])
			{
				shcosup.push_back(str1[i]);
				i--; j--;
			}
			while (i >= 0)
			{
				shcosup.push_back(str1[i]);
				i--;
			}
			while (j >= 0)
			{
				shcosup.push_back(str2[j]);
				j--;
			}

			reverse(shcosup.begin(), shcosup.end());
			return shcosup;
		}
	};


	class Solution2
	{
		/*
		* Everything is the same as Solution, except some minor change around the reconstruct 2-D array
		* Fill in the 1st row and 1st column so that I don't need special processing when I reach
		* top row or left-most column.
		*/
	public:
		string shortestCommonSupersequence(string str1, string str2)
		{
			size_t l1 = str1.size();
			size_t l2 = str2.size();

			vector<vector<signed char>> reconstruct(l1, vector<signed char>(l2, 0)); // 1: from topleft; 2: from top; 3: from left
			vector<int> r(l2, 0);  // length of longest-common-subsequence, 1 dimension array is enough

			r[0] = str1[0] == str2[0] ? 1 : 0;
			for (size_t i = 1; i < l2; i++)
			{
				if (str1[0] == str2[i])
					r[i] = 1;
				else
					r[i] = r[i - 1];
			}
			
			for (size_t i = 0; i < l1; i++)
			{
				if (str1[i] == str2[0])
					reconstruct[i][0] = 1; // the '1' here doesn't necessarily mean it comes from top-left
				                           // because there is no top-left as this is alredy the left-most column
				                           // the '1' here just means when I reconstruct and if I arrive at this cell
				                           // I should take this letter and decrease *both* indexes
				else
					reconstruct[i][0] = 2;
			}

			for (size_t j = 0; j < l2; j++)
			{
				if (str1[0] == str2[j])
					reconstruct[0][j] = 1;
				else
					reconstruct[0][j] = 3;
			}
			
			for (size_t i = 1; i < l1; i++)
			{
				int previousStep = str1[i] == str2[0] ? 1 : r[0];
				for (size_t j = 1; j < l2; j++)
				{
					int thisStep;
					if (str1[i] == str2[j])
					{
						thisStep = r[j - 1] + 1;
						reconstruct[i][j] = 1;

					}
					else if (r[j] > previousStep)
					{
						thisStep = r[j];
						reconstruct[i][j] = 2;
					}
					else
					{
						thisStep = previousStep;
						reconstruct[i][j] = 3;
					}
					r[j - 1] = previousStep;
					previousStep = thisStep;
				}
				r[l2 - 1] = previousStep;
			}

			string shcosup; // shortest common supersequence

			int i = (int)l1 - 1, j = (int)l2 - 1;
			while (i>=0 || j>=0)
			{
				if (i < 0)
				{
					shcosup.push_back(str2[j]);
					j--;
					continue;
				}

				if (j < 0)
				{
					shcosup.push_back(str1[i]);
					i--;
					continue;
				}

				if (reconstruct[i][j] == 1)
				{
					shcosup.push_back(str1[i]);
					i--; j--;
				}
				else if (reconstruct[i][j] == 2)
				{
					shcosup.push_back(str1[i]);
					i--;
				}
				else
				{
					shcosup.push_back(str2[j]);
					j--;
				}
			}

			reverse(shcosup.begin(), shcosup.end());
			return shcosup;
		}
	};


	class Solution3  // accepted
	{
		/*
		* Same principles as the previous 2 solutions, just when reconstructing the supersequence,
		* use the already calculated longest-common-subsequence length to calculate the length of
		* the supersequence, and pre-allocate a string with that length, so that I don't have to
		* put letters into the supersequence in reverse order and run reverse at the end.
		*/
	public:
		string shortestCommonSupersequence(string str1, string str2)
		{
			size_t l1 = str1.size();
			size_t l2 = str2.size();

			vector<vector<signed char>> reconstruct(l1, vector<signed char>(l2, 0)); // 1: from topleft; 2: from top; 3: from left
			vector<int> r(l2, 0);

			r[0] = str1[0] == str2[0] ? 1 : 0;
			for (size_t i = 1; i < l2; i++)
			{
				if (str1[0] == str2[i])
					r[i] = 1;
				else
					r[i] = r[i - 1];
			}

			for (size_t i = 0; i < l1; i++)
			{
				if (str1[i] == str2[0])
					reconstruct[i][0] = 1; // see Solution2 for why setting this to '1'
				else
					reconstruct[i][0] = 2;
			}

			for (size_t j = 0; j < l2; j++)
			{
				if (str1[0] == str2[j])
					reconstruct[0][j] = 1;
				else
					reconstruct[0][j] = 3;
			}

			for (size_t i = 1; i < l1; i++)
			{
				int previousStep = str1[i] == str2[0] ? 1 : r[0];
				for (size_t j = 1; j < l2; j++)
				{
					int thisStep;
					if (str1[i] == str2[j])
					{
						thisStep = r[j - 1] + 1;
						reconstruct[i][j] = 1;

					}
					else if (r[j] > previousStep)
					{
						thisStep = r[j];
						reconstruct[i][j] = 2;
					}
					else
					{
						thisStep = previousStep;
						reconstruct[i][j] = 3;
					}
					r[j - 1] = previousStep;
					previousStep = thisStep;
				}
				r[l2 - 1] = previousStep;
			}

			size_t lcsLength = r[l2 - 1];

			size_t shortestCommonSupersequenceLength = (l1 - lcsLength) + (l2 - lcsLength) + lcsLength;

			string shcosup(shortestCommonSupersequenceLength, ' '); // shortest common supersequence

			int i = (int)l1 - 1, j = (int)l2 - 1, k = (int)shortestCommonSupersequenceLength - 1;
			while (i >= 0 || j >= 0)
			{
				if (i < 0)
				{
					shcosup[k] = str2[j];
					j--;
				}
				else if (j < 0)
				{
					shcosup[k] = str1[i];
					i--;
				}
				else if (reconstruct[i][j] == 1)
				{
					shcosup[k] = str1[i];
					i--; j--;
				}
				else if (reconstruct[i][j] == 2)
				{
					shcosup[k] = str1[i];
					i--;
				}
				else
				{
					shcosup[k] = str2[j];
					j--;
				}
				k--;
			}
			return shcosup;
		}
	};


	/*
	*  a b c b  d    a b
	*    b      d c  a b  a
	*    |      |    | |
	*    b      d    a b   <-- longest common subsequence
	* 
	* a  b c b  d c  a b  a  <-- shortest common supersequence
	*/
	
	void Test_1092_Shortest_Common_Supersequence()
	{
		string s1; string s2;
		Solution3 s;

		while (true)
		{
			s1.clear();
			s2.clear();
			cout << "give input 1: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			cout << "give input 2: ";
			cin >> s2;
			cin.ignore();
			cout << s.shortestCommonSupersequence(s1, s2) << "\n\n";
		}
	}
}