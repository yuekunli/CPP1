#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>


namespace Package_Install_Dependency {

    /*

    Problem Description in Python:

    def get_install_order(packages: dict) -> List[str]:
        return

    react_packages = {
        'react': ['loose-envify', 'object-assign', 'prop-types'],
        'loose-envify': ['js-tokens'],
        'prop-types': ['loose-envify', 'object-assign', 'react-is']
    }

    other_packages = {
                'A': ['B', 'C', 'D'],
                'B': ['E', 'J'],
                'C': ['K'],
                'D': ['C'],
                'E': ['L', 'X'],
                'F': ['A'],
                'X': [],
                'Z': ['F', 'B']
            }

    print(get_install_order(other_packages))

    # Expected Result: L, X, E, J, B, K, C, D, A, F, Z

    There should be more than one correct answer?
    For example:
    X, L, K, E, J, B, C, D, A, F, Z

    */

    using namespace std;

    class Solution
    {
    public:
        void dfs(unordered_map<char, vector<char>>& m, char current, vector<char>& answer, unordered_set<char>& seen)
        {
            // Note that "current" may not even be one of the keys in "m"
            // for example, current can be 'L', but 'L' is not a key in "m"
            if (m.contains(current))
            {
                vector<char>& p = m[current];
                if (p.size() > 0)
                {
                    size_t sz = p.size();
                    for (size_t i = 0; i < sz; i++)
                    {
                        if (seen.count(p[i]) == 0)
                        {
                            seen.insert(p[i]);
                            dfs(m, p[i], answer, seen);
                        }
                    }
                }
            }
            answer.emplace_back(current);
        }

        vector<char> getInstallOrder(vector<vector<char>>& depen, vector<char>& pack)
        {
            unordered_map<char, vector<char>>m;
            size_t sz = pack.size();
            for (size_t i = 0; i < sz; i++)
            {
                char c = pack[i];
                if (m.contains(c))
                {
                    for (char const& package : depen[i])
                    {
                        m[c].emplace_back(package);
                    }
                }
                else
                {
                    m[c] = depen[i];
                }
            }

            vector<char> answer;
            unordered_set<char>seen;
            
            for (size_t i = 0; i < sz; i++)
            {
                if (seen.count(pack[i]) == 0)
                {
                    seen.insert(pack[i]);
                    dfs(m, pack[i], answer, seen);
                }
            }
            return answer;
        }
    };

    /*
    'A': ['B', 'C', 'D'],
    'B': ['E', 'J'],
    'C': ['K'],
    'D': ['C'],
    'E': ['L', 'X'],
    'F': ['A'],
    'X': [],
    'Z': ['F', 'B']
    'E': ['Q']
    */
    void Test_Package_Install_Dependency() {

        vector<char>pack{ 'A', 'B', 'C', 'D', 'E', 'F', 'X', 'Z', 'E'};
        vector<vector<char>>depen{ {'B', 'C', 'D'}, {'E', 'J'}, {'K'}, {'C'}, {'L', 'X'}, {'A'}, {}, {'F', 'B'}, {'Q'} };

        Solution s;

        vector<char>const& answer = s.getInstallOrder(depen, pack);
        for (char const& c : answer)
            cout << c << "  ";

        cout << "\n\n";
    }
}