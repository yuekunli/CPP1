#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <sstream>

namespace _2213_Longest_Substring_One_Repeating_Char {
	using namespace std;

	class Solution
	{
	public:

		struct Segment
		{
			int start;
			int length;
			char c;
		};

		struct SegComp
		{
			using is_transparent = void;

			bool operator()(Segment const& lhs, Segment const & rhs) const
			{
				return lhs.start < rhs.start;
			}

			bool operator()(Segment const& lhs, int rhs) const
			{
				return lhs.start < rhs;
			}

			bool operator()(int lhs, Segment const& rhs) const
			{
				return lhs < rhs.start;
			}
		};

		vector<int> longestRepeating(string s, string queryCharacters, vector<int>& queryIndices)
		{
			set<Segment, SegComp> segmentsSet;
			map<int, int> segLengthCount;  // segment length:  number of segments with that length
			size_t len = s.size();
			char current = s[0];
			int runLength = 0;
			int runStart = 0;
			for (int i = 0; i < len; i++)
			{
				if (s[i] == current)
					runLength++;
				else
				{
					Segment seg{ runStart, runLength, current };
					segmentsSet.insert(seg);
					segLengthCount[runLength]++;
					current = s[i];
					runStart = i;
					runLength = 1;
				}
			}
			Segment seg{ runStart, runLength, current };
			segmentsSet.insert(seg);
			segLengthCount[runLength]++;
			
			size_t qLen = queryCharacters.size();
			vector<int>answer(qLen);

			auto longest = prev(segLengthCount.end()); // longest points to a std::pair<key, value>

			for (int i = 0; i < qLen; i++)
			{
				int qIdx = queryIndices[i];
				char oldChar = s[qIdx];
				char newChar = queryCharacters[i];
				if (oldChar == newChar)
				{
					answer.push_back(longest->first);
					continue;
				}

				auto it = segmentsSet.lower_bound(qIdx);
				
				if (it->start > qIdx)
					it = prev(it);

				int segmentStartOfThisQuery = it->start;
				int segmentEndOfThisQuery = segmentStartOfThisQuery + it->length - 1;

				if (segmentStartOfThisQuery < qIdx && qIdx < segmentEndOfThisQuery)
				{
					// qIdx falls in the middle of a segment, it's going to break 1 segment into 3
					int oldLength = it->length;
					int count = segLengthCount[oldLength];
					count--;
					if (count == 0)
						segLengthCount.erase(oldLength);
					
					segmentsSet.erase(it);

					// 1st new segment:
					int newSegmentLength = qIdx-1 - segmentStartOfThisQuery + 1;  
					segLengthCount[newSegmentLength]++;
					segmentsSet.insert({ segmentStartOfThisQuery , newSegmentLength, oldChar });

					// 2nd new segment (just the single character that is being changed now)
					segLengthCount[1]++;
					segmentsSet.insert({qIdx, 1, newChar});

					// 3rd new segment:
					newSegmentLength = segmentEndOfThisQuery - qIdx;
					segLengthCount[newSegmentLength]++;
					segmentsSet.insert({qIdx+1, newSegmentLength, oldChar});

					//if (oldLength == longest->first && count==0)
					//    longest = prev(segLengthCount.end());
					//answer[i]=longest->first;
				}
				else if (segmentStartOfThisQuery == qIdx && qIdx < segmentEndOfThisQuery)
				{
					bool needToMergeWithPrev = false;
					typename set<Segment, SegComp>::iterator prevIt;
					if (it != segmentsSet.begin())
					{
						prevIt = prev(it);
						if (prevIt->c == newChar)
							needToMergeWithPrev = true;
					}

					Segment thisSeg = *it;
					
					if (needToMergeWithPrev)
					{
						Segment prevSeg = *prevIt;
						segmentsSet.erase(prevIt);

						int oldLength = prevSeg.length;
						int oldCount = segLengthCount[oldLength];
						segLengthCount[oldLength]--;
						if (oldCount == 1)
							segLengthCount.erase(oldLength);
						segLengthCount[oldLength + 1]++;
						//prevIt->length += 1;  // I can't do this because this is modifying the key of an element in a "set"

						prevSeg.length++;
						segmentsSet.insert(prevSeg);
					}
					else
					{
						// don't need to merge with previous, but this segment is broken into two
						segmentsSet.erase(it);
						
						// first deal with the newly created single-character segment
						segLengthCount[1]++;
						segmentsSet.insert({ qIdx, 1, newChar });	
					}

					int oldCount = segLengthCount[thisSeg.length];
					segLengthCount[thisSeg.length]--;
					if (oldCount == 1)
						segLengthCount.erase(thisSeg.length);
					segLengthCount[thisSeg.length - 1]++;
					segmentsSet.insert({ qIdx + 1, thisSeg.length - 1, thisSeg.c });
				}
				else if (segmentStartOfThisQuery < qIdx && qIdx == segmentEndOfThisQuery)
				{
					bool needToMergeWithNext = false;
					auto nextIt = next(it);
					if (nextIt != segmentsSet.end())
					{
						if (nextIt->c == newChar)
							needToMergeWithNext = true;
					}

					Segment thisSeg = *it;

					if (needToMergeWithNext)
					{
						Segment nextSeg = *nextIt;
						segmentsSet.erase(nextIt);

						int oldLength = nextSeg.length;
						int oldCount = segLengthCount[oldLength];
						segLengthCount[oldLength]--;
						if (oldCount == 1)
							segLengthCount.erase(oldLength);
						segLengthCount[oldLength + 1]++;
						//prevIt->length += 1;  // I can't do this because this is modifying the key of an element in a "set"

						nextSeg.start--;
						nextSeg.length++;
						segmentsSet.insert(nextSeg);
					}
					else
					{
						// don't need to merge with next, but this segment is broken into two
						segmentsSet.erase(it);

						// first deal with the newly created single-character segment
						segLengthCount[1]++;
						segmentsSet.insert({ qIdx, 1, newChar });
					}

					int oldCount = segLengthCount[thisSeg.length];
					segLengthCount[thisSeg.length]--;
					if (oldCount == 1)
						segLengthCount.erase(thisSeg.length);
					segLengthCount[thisSeg.length - 1]++;
					segmentsSet.insert({ thisSeg.start, thisSeg.length - 1, thisSeg.c });
				}
				else
				{
					// changing the only character in a single-character segment
					bool needToMergeWithPrev = false, needToMergeWithNext = false;

					typename set<Segment, SegComp>::iterator prevIt;
					if (it != segmentsSet.begin())
					{
						prevIt = prev(it);
						if (prevIt->c == newChar)
							needToMergeWithPrev = true;
					}

					auto nextIt = next(it);
					if (nextIt != segmentsSet.end())
					{
						if (nextIt->c == newChar)
							needToMergeWithNext = true;
					}

					Segment thisSeg = *it;

					if (needToMergeWithPrev && !needToMergeWithNext)
					{
						Segment prevSeg = *prevIt;

						segmentsSet.erase(prevIt);
						int oldCount = segLengthCount[prevSeg.length];
						segLengthCount[prevSeg.length]--;
						if (oldCount == 1)
							segLengthCount.erase(prevSeg.length);

						segmentsSet.erase(it);
						oldCount = segLengthCount[1];
						segLengthCount[1]--;
						if (oldCount == 1)
							segLengthCount.erase(1);

						prevSeg.length++;
						segmentsSet.insert(prevSeg);
						segLengthCount[prevSeg.length]++;
					}
					else if ((!needToMergeWithPrev) && needToMergeWithNext)
					{
						Segment nextSeg = *nextIt;

						segmentsSet.erase(nextIt);
						int oldCount = segLengthCount[nextSeg.length];
						segLengthCount[nextSeg.length]--;
						if (oldCount == 1)
							segLengthCount.erase(nextSeg.length);

						segmentsSet.erase(it);
						oldCount = segLengthCount[1];
						segLengthCount[1]--;
						if (oldCount == 1)
							segLengthCount.erase(1);

						nextSeg.length++;
						nextSeg.start--;
						segmentsSet.insert(nextSeg);
						segLengthCount[nextSeg.length]++;
					}
					else if (needToMergeWithPrev && needToMergeWithNext)
					{
						Segment prevSeg = *prevIt;
						Segment nextSeg = *nextIt;

						segmentsSet.erase(prevIt);
						segmentsSet.erase(nextIt);
						segmentsSet.erase(it);

						int oldCount = segLengthCount[prevSeg.length];
						segLengthCount[prevSeg.length]--;
						if (oldCount == 1)
							segLengthCount.erase(prevSeg.length);
						oldCount = segLengthCount[nextSeg.length];
						segLengthCount[nextSeg.length]--;
						if (oldCount==1)
							segLengthCount.erase(nextSeg.length);
						oldCount = segLengthCount[1];
						segLengthCount[1]--;
						if (oldCount == 1)
							segLengthCount.erase(1);

						prevSeg.length = prevSeg.length + 1 + nextSeg.length;
						segmentsSet.insert(prevSeg);
						segLengthCount[prevSeg.length]++;
					}
					else
					{
						segmentsSet.erase(it);
						thisSeg.c = newChar;
						segmentsSet.insert(thisSeg);
					}
				}
				longest = prev(segLengthCount.end());
                answer[i]=longest->first;
			}
			return answer;
		}
	};


	void Test_2213_Longest_Substring_One_Repeating_Char()
	{
		Solution solu;

		while (true)
		{
			string s;
			string queryCharacters;

			cout << "string: ";
			getline(cin, s);

			if (s.size() == 0)break;
			
			cout << "query characters: ";
			cin >> queryCharacters;

			vector<int>queryIndices;
			string queryIndicesString;

			cin.ignore();
			cout << "query indices: ";
			getline(cin, queryIndicesString);
			
			stringstream ss(queryIndicesString);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(queryIndices));
			
			vector<int>const& answer = solu.longestRepeating(s, queryCharacters, queryIndices);
			for (int const& i : answer)
				cout << i << " ";
			cout << '\n';
		}
	}
}