#include "LibHeaders.h"

namespace _1851_Minimum_Interval_Include_Query {
	using namespace std;

	class Solution
	{
		/*
		* The main idea is to sort the queries, but I can't do that because I need to remember
		* the order of each query because the answer needs to be in the same order.
		* So I create an array of queries' indexes, sort these indexes.
		* But I also make a copy of the queries, the real query numbers because I need to do
		* binary search into the sorted queries after all.
		*/
	public:
		vector<int>minInterval(vector<vector<int>>& intervals, vector<int>& queries)
		{
			sort(intervals.begin(), intervals.end(), [](vector<int>& a, vector<int>& b) {return (a[1] - a[0] + 1) < (b[1] - b[0] + 1); });

			int interLength = intervals.size();
			int qLength = queries.size();

			vector<int>queIndexes(qLength);
			iota(queIndexes.begin(), queIndexes.end(), 0);
			sort(queIndexes.begin(), queIndexes.end(), [&](int& a, int& b) {return queries[a] < queries[b]; });
			vector<int>queriesCopy(qLength, 0);
			transform(queIndexes.begin(), queIndexes.end(), queriesCopy.begin(), [&](int& a) {return queries[a]; });

			vector<int>answer(qLength, -1);

			for (int i = 0; i < interLength; i++)
			{
				int start = intervals[i][0];
				int end = intervals[i][1];

				auto it1 = lower_bound(queriesCopy.begin(), queriesCopy.end(), start, [&](int const & a, int const & b) {return a < b; });
				auto it2 = upper_bound(queriesCopy.begin(), queriesCopy.end(), end, [&](int const& a, int const& b) {return a < b; });

				auto it = it1;
				while (it != it2)
				{
					int d = distance(queriesCopy.begin(), it);
					int idxInOriginalQueryArray = queIndexes[d];
					if (answer[idxInOriginalQueryArray] == -1)
					{
						answer[idxInOriginalQueryArray] = end - start + 1;
					}
					it += 1;
				}
			}
			return answer;
		}
	};

	void Test_1851_Minimum_Interval_Include_Query()
	{
		Solution s;
		string s1;
		vector<vector<int>>intervals;
		vector<int>queries;
		vector<int>tmp;
		while (true)
		{
			s1.clear();
			cout << "intervals: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			tmp.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(tmp));
			intervals.clear();
			for (int i = 0; i <= tmp.size() - 2; i += 2)
			{
				intervals.emplace_back(initializer_list<int>{tmp[i], tmp[i + 1]});
			}
			s1.clear();
			cout << "queries: ";
			getline(cin, s1);
			stringstream ss2(s1);
			queries.clear();
			copy(istream_iterator<int>(ss2), istream_iterator<int>(), back_inserter(queries));
			vector<int>const& answer = s.minInterval(intervals, queries);
			copy(answer.begin(), answer.end(), ostream_iterator<int>(std::cout, " "));
			cout << "\n\n";
		}
	}
}