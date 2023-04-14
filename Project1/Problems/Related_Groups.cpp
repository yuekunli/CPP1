#include <vector>
#include <string>
#include <iostream>
#include <sstream>

/*
* Can't really recall what the problem description is.
* Speculation:
* Input:
* related:   10100  01001  10100  00010  01001
* output:
* 3 groups
* person 1 and 3 are in the same group.
* person 2 and 5 are in the same group.
* person 4 is in a group.
*/


namespace Related_Groups {

    using namespace std;

	class Solution1
	{
	public:
        size_t n;
        void dfs(size_t person, vector<bool>& visited, vector<string>& related)
        {
            string& s = related[person];

            for (size_t i = person+1; i < n; i++) // only need to start looking within persons after current person
            {
                if (s[i] == '1' && !visited[i] && i != person)
                {
                    visited[i] = true;
                    dfs(i, visited, related);
                    break; // by the time I come back here, all the persons after current person should've been examined
                }
            }
        }

        int countGroups(vector<string> related) {
            n = related.size();
            vector<bool>visited(n, false);
            int groups = 0;
            for (size_t i = 0; i < n; i++)
            {
                if (!visited[i])
                {
                    groups++;
                    visited[i] = true;
                    dfs(i, visited, related);
                }
            }
            return groups;
        }
	};

    void Test_Related_Groups()
    {
        Solution1 solu;
        string s;
        vector<string> related;

        while (true)
        {
            s.clear();
            related.clear();

            cout << "input: ";
            getline(cin, s);
            if (s.size() == 0) break;

            stringstream ss(s);
            copy(istream_iterator<string>(ss), istream_iterator<string>(), back_inserter(related));

            cout << "The input I got: ";
            copy(related.begin(), related.end(), ostream_iterator<string>(std::cout, " "));

            cout << "\n\n" << "Related groups: " << solu.countGroups(related) << "\n";
        }
    }
}