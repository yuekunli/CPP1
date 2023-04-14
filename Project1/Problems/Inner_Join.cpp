#include <iostream>
#include <vector>
#include <unordered_map>
#pragma warning(disable: 4267)
namespace Inner_Join {

	using namespace std;

    /*
    * Always want table1's columns precede table2's columns no matter who is the stream.
    * But I want to build the index map for the non-stream table because it uses less memory for the map.
    */
	class Solution
	{
	public:
        vector<vector<int>> innerJoin(vector<vector<int>>& t1, vector<vector<int>>& t2, bool isT1Stream)
        {
            vector<vector<int>>* p1;
            vector<vector<int>>* p2;
            if (isT1Stream)
            {
                p1 = &t1;
                p2 = &t2;
            }
            else
            {
                p1 = &t2;
                p2 = &t1;
            }
            size_t sz2 = p2->size();
            unordered_map<int, vector<int>>m;
            for (size_t i = 0; i < sz2; i++)
            {
                m[(*p2)[i][0]].emplace_back(i);   // indices of the entries in non-stream table. 
                /*
                * If the table has:
                * key1: value1;
                * key2: value2;
                * key1: value3;
                *
                * 'm' will have:
                * key1:  0, 2
                * key2:  1
                *
                */
            }

            size_t sz1 = p1->size();

            size_t col1 = (*p1)[0].size();
            size_t col2 = (*p2)[0].size();

            vector<vector<int>>answer;
            for (size_t i = 0; i < sz1; i++)
            {
                int key = (*p1)[i][0];
                if (m.count(key) > 0)
                {
                    vector<int>& index = m[key];
                    vector<int> a(col1 + col2 - 1);
                    for (int& b : index)
                    {
                        if (isT1Stream) // "index" map is built for table2, and table2 is referenced by p2
                        {
                            copy((*p1)[i].begin(), (*p1)[i].end(), a.begin());
                            copy((*p2)[b].begin()+1, (*p2)[b].end(), a.begin()+col1);
                        }
                        else // "index" map is build for table1, and table1 is referenced by p2
                        {
                            copy((*p2)[b].begin(), (*p2)[b].end(), a.begin());
                            copy((*p1)[i].begin() + 1, (*p1)[i].end(), a.begin() + col2);
                        }
                        answer.push_back(a);
                    }
                }
            }
            return answer;
        }
	};

    void Test_Inner_Join()
    {
        Solution s;
        vector<vector<int>>t1 = { {1,2,3}, {4,5,6}, {1,7,8} };
        vector<vector<int>>t2 = { {1,10,11}, {4,1,1}, {7,12,13} };
        vector<vector<int>>const& answer = s.innerJoin(t1, t2, true);
        for (auto& a : answer)
        {
            for (int const& b : a)
                cout << b << " ";
            cout << "\n";
        }
        cout << "\n\n";
        vector<vector<int>>const& answer2 = s.innerJoin(t1, t2, false);
        for (auto& a : answer2)
        {
            for (int const& b : a)
                cout << b << " ";
            cout << "\n";
        }
        cout << "\n\n";
    }
}