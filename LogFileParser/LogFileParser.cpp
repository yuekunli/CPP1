#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

namespace ParseDownloadRange {

    // (1). have a WindowsPatching.log file
    // (2). need to manually pick out all the lines concerning the same download file, put all the picked lines in a separate file
    // (3). then this code finds every line that has this part "for range[57671680 - 58720255]", for example, this line:
    // 2023 - 09 - 13 03:26 : 54, 018 - INFO - Handling partial GET request[File Size : 84252255, File Hash : 7pwct7a5htzt4249ss9cpb6q4lf2ew9e, File Name : windows10.0 - kb4584642 - x86_c48447626303bee7f9e5ce4a4971487e8fe488a9.cab] for range[57671680 - 58720255](cV: 7 / scmd7wr0 + eZxXF.81.0.0.6.2.6.1.1.22) - WUAHttpServer - TID = 19054, I / O - dispatch - 1
    // (4). pick out the range, convert them to a pair, and sort these pairs and print out sorted result
    // Note that the example.log is an original WindowsPatching.log file, step#2 is not done.

    void parseRange()
    {

        std::ifstream inputFile("example.log");

        vector<pair<int, int>> r;

        string line;

        while (getline(inputFile, line))
        {
            string::size_type n = line.find("for range");
            if (n != string::npos)
            {
                string fromOpenBracketToEnd = line.substr(n + 10);
                string::size_type closeBracket = fromOpenBracketToEnd.find("]");
                //string::size_type l1 = closeBracket - 
                // [123-456]
                // |       |
                // index 0 |
                //         index 8
                // the length of the numberic part is 7

                string startAndEndNumbers = fromOpenBracketToEnd.substr(1, closeBracket-1);

                string::size_type dashPos = startAndEndNumbers.find("-");
                string start = startAndEndNumbers.substr(0, dashPos);
                string end = startAndEndNumbers.substr(dashPos + 1);

                int startValue = stoi(start);
                int endValue = stoi(end);

                r.emplace_back(startValue, endValue);
            }
        }

        stable_sort(r.begin(), r.end(),
            [](pair<int, int>const & a, pair<int, int>const & b) {
                return a.first < b.first;
            });

        size_t rangesCount = r.size();
        for (size_t i = 0; i < rangesCount; i++)
        {
            cout << setw(9) << r[i].first << " - " << setw(9) << r[i].second << "     " << setw(7) << r[i].second - r[i].first + 1 << "    ";

            if (i != 0)
            {
                if (r[i-1].second + 1 != r[i].first)
                {
                    if (r[i-1].second + 1 < r[i].first)
                        cout << "gap";
                    else if (r[i-1].first == r[i].first && r[i-1].second == r[i].second)
                        cout << "repeat";
                    else if (r[i-1].first == r[i].first && r[i-1].second != r[i].second)
                        cout << "repeat different ending";
                }
            }
            cout << endl;
        }
    }
}


int main()
{
    ParseDownloadRange::parseRange();
}


