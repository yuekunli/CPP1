#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<numeric>

namespace _0833_Find_And_Replace_in_String {

	using namespace std;

	class Solution
	{
	public:
		string findReplaceString(string s, vector<int>& indices, vector<string>& sources, vector<string>& targets)
		{
			size_t k = indices.size();
			
			vector<int>indicesOfIndices(k, 0);

			/*
			* indices:            13       52        8         44          38          21
			* indicesOfIndices:    0        1        2          3           4           5    <-- initialized with iota
			* 
			* indicesOfIndices:    2(8)     0(13)    5(21)      4(38)       3(44)       1(52)
			* 
			* indicesOfIndices[2] ---> 5       the 3rd smallest index is the on in cell#5
			* indices[5]          ---> 21      "21" is the one in cell#5
			* "21" is the index into the original string
			*/


			iota(indicesOfIndices.begin(), indicesOfIndices.end(), 0);
			sort(indicesOfIndices.begin(), indicesOfIndices.end(),
				[&](int const& a, int const& b)
				{
					return indices[a] < indices[b];
				});
			
			int offset = 0;
			for (size_t i = 0; i < k; i++)
			{
				int m = indicesOfIndices[i];
				int startIndex = indices[m] + offset;
				if (equal(sources[m].begin(), sources[m].end(), s.begin() + startIndex))
				{
					size_t sourceLength = sources[m].size();
					s.replace(s.begin() + startIndex, s.begin() + startIndex + sourceLength, targets[m].begin(), targets[m].end());
					offset += (int)targets[m].size() - (int)sourceLength;
				}
			}
			return s;
		}
	};




	void Test_0833_Find_And_Replace_in_String()
	{
		Solution solu;
		string tmp;
		vector<string>tmp2;

		string s;
		vector<int>indices;
		vector<string>sources;
		vector<string>targets;
		while (true)
		{
			tmp.clear();
			cout << "all inputs: ";
			getline(cin, tmp);
			if (tmp.size() == 0) break;

			stringstream ss(tmp);
			tmp2.clear();
			copy(istream_iterator<string>(ss), istream_iterator<string>(), back_inserter(tmp2));

			size_t sz = tmp2.size();
			size_t k = (sz - 1) / 3;
			indices.clear();
			sources.clear();
			targets.clear();
			s = tmp2[0];
			int tmp2Index = 1;
			for (int i = 0; i < k; i++)
			{
				indices.emplace_back(stoi(tmp2[tmp2Index]));
				tmp2Index++;
			}

			for (int i = 0; i < k; i++)
			{
				sources.emplace_back(tmp2[tmp2Index]);
				tmp2Index++;
			}

			for (int i = 0; i < k; i++)
			{
				targets.emplace_back(tmp2[tmp2Index]);
				tmp2Index++;
			}

			cout << solu.findReplaceString(s, indices, sources, targets) << "\n\n";
		}
	}
}