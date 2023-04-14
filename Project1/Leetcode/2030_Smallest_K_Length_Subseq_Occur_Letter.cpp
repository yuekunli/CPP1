#include "LibHeaders.h"

namespace _2030_Smallest_K_Length_Subseq_Occur_Letter {

	using namespace std;

	class Solution
	{
	public:
		string smallestSubsequence(string s, int k, char letter, int repetition)
		{
			int len = s.size();
			vector<char> answer;
			int totalTargetCount = count(s.begin(), s.end(), letter);
			int targetCountInAnswer = 0;
			int droppedTarget = 0;

			int i = 0;
			while (i < len)
			{
				if (answer.size() == 0)
				{
					// the very first iteration definitely comes in this "if" block, later iterations may come in here too
					// It's possible that I have added a few characters to the answer, but then I find a small character
					// I end up bumping all previously added characters out of answer, then next iteration I'll come in here
					answer.emplace_back(s[i]);
					if (s[i] == letter)
						targetCountInAnswer++;
					i++;
					continue;
				}
				else
				{
					char previous = answer.back();

					// consider dropping the previous char that is already in answer

					if (previous > s[i])
					{
						if (previous != letter)
						{
							answer.pop_back();
							continue; // don't increment 'i', but continue, 'i' will still point to the same char, but I'll examine one more char that is already in answer
						}
						else  // the "previous" in answer is a target, need more consideration
						{
							if (totalTargetCount - droppedTarget > repetition) // I haven't dropped a lot targets, there is still some leeway
							{
								answer.pop_back();
								droppedTarget++;
								targetCountInAnswer--;
								continue;
							}
						}
					}

					// consider adding this char pointed to by 'i'

					int nonTargetInAnswer = answer.size() - targetCountInAnswer;
					int availableSpotsInAnswer = k - answer.size();
					if (targetCountInAnswer >= repetition && availableSpotsInAnswer > 0)
					{
						answer.emplace_back(s[i]);
						if (s[i] == letter)
							targetCountInAnswer++;
						i++;
					}
					else if (targetCountInAnswer < repetition)
					{
						int targetsNeeded = repetition - targetCountInAnswer;
						if (s[i] == letter)
						{
							answer.emplace_back(s[i]);
							targetCountInAnswer++;
							i++;
						}
						else if (targetsNeeded < availableSpotsInAnswer)
						{
							answer.emplace_back(s[i]);
							if (s[i] == letter)
								targetCountInAnswer++;
							i++;
						}
						else
						{
							// I need more targets, this char is not a target, although I still have available spots, 
							// I need to reserve all of them for the targets I still need, so have to drop this char
							i++;
						}
					}
					else
					{
						// I already have enough targets, and there is no available spots, this doesn't mean the answer
						// won't change any more. If the target's value is big and there are more targets than I need,
						// a smaller char can still bump out a target later, but that is in a different code path.
						// I have to drop this char now, but I need to remember if this char is a target.
						if (s[i] == letter)
							droppedTarget++;
						i++;
					}
				}
			}
			return string(answer.begin(), answer.end());
		}
	};


	class BruteForceSolution
	{
		int n; // length of the input string

		bool nextSelection(int& selection, int n, int k)
		{
			bool openFound = false;
			int bitsToTheRight = 0;
			for (int i = 0; i < n; i++)
			{
				int bit = selection & (1 << i);
				if (bit && !openFound)
				{
					bitsToTheRight++;
					continue;
				}
				if (!bit && !openFound)
				{
					openFound = true;
					continue;
				}
				if (bit && openFound)
				{
					selection ^= (0b11 << (i - 1));
					int full = pow(2, n) - 1;
					int seg = pow(2, i - 1) - 1;
					selection &= (full ^ seg);
					int needed = pow(2, bitsToTheRight) - 1;
					needed = needed << (i - 1 - 1 - (bitsToTheRight - 1));
					selection |= needed;
					return true;
				}
			}
			return false;
		}

		void verify(string& s, int selection, int k, char letter, int repetition, string& answer)
		{
			int targetCount = 0;
			int base = 1 << (n-1);
			for (int i = 0; i < n; i++)
			{
				if (selection & (base >> i))
				{
					if (s[i] == letter)
						targetCount++;
				}
			}
			if (targetCount >= repetition) // note that there can be more than "repetition" targets
			{
				string potentialAnswer(k, ' ');
				int j = 0;
				for (int i = 0; i < n; i++)
				{
					if (selection & (base >> i))
					{
						potentialAnswer[j] = s[i];
						j++;
					}
				}
				if (answer.size() == 0)
					answer = potentialAnswer;
				else
					if (lexicographical_compare(potentialAnswer.begin(), potentialAnswer.end(), answer.begin(), answer.end()))
						answer = potentialAnswer;
			}
		}

	public:

		string smallestSubsequence(string s, int k, char letter, int repetition)
		{
			n = s.size();
			int needed = pow(2, k) - 1;
			int selection = needed << (n - k);
			string answer;
			do {
				verify(s, selection, k, letter, repetition, answer);
			} while (nextSelection(selection, n, k));
			return answer;
		}
	};


	class AutoTest
	{
	
		random_device rd;
		mt19937 rgen;
		
		string s = "aaaccptttxxxx";

		BruteForceSolution bfs;
		Solution solu;

	public:
		AutoTest() :rgen(rd()) {}

		bool runOneTest()
		{
			shuffle(s.begin(), s.end(), rgen);
			string bruteAnswer = bfs.smallestSubsequence(s, 5, 'x', 2);
			string fastAnswer = solu.smallestSubsequence(s, 5, 'x', 2);
			copy(s.begin(), s.end(), ostream_iterator<char>(std::cout, " ")); cout << '\n';
			if (bruteAnswer == fastAnswer)
			{
				cout << bruteAnswer << "\n-----------------------\n";
				return true;
			}
			else
			{
				cout << "fast answer: " << fastAnswer << "     brute answer: " << bruteAnswer << "\n-----------------------\n";
				return false;
			}
		}

		void run()
		{
			for (int i = 0; i < 5; i++)
				if (!runOneTest())
					break;
		}
	};


	void Test_2030_Smallest_K_Length_Subseq_Occur_Letter()
	{
		Solution solu;
		AutoTest ats;
		int option;
		string s;
		int k, repetition;
		char letter;
		while (true)
		{
			cout << "manual test: 1     auto test: 2      exit: 0   > ";
			cin >> option;
			cin.ignore();
			if (option == 0) break;
			else if (option == 1)
			{
				string everything;
				cout << "string,  k,  letter,  repetition: ";
				getline(cin, everything);
				if (everything.size() == 0) break;
				stringstream ss(everything);
				s.clear();
				ss >> s;
				ss >> k;
				ss >> letter;
				ss >> repetition;
				cout << solu.smallestSubsequence(s, k, letter, repetition) << '\n';
			}
			else if (option == 2)
			{
				ats.run();
			}
		}
	}
}