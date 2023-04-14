#include<iostream>
#include<string>
#include<sstream>
#include<algorithm>
#include<vector>
#include<bitset>

namespace _0691_Stickers_to_Spell_Word {

	using namespace std;

	class Solution
	{
		/*
		* If there are 100 stickers, at each step of the backtracking process, there are 100 possible choices.
		* some of these 100 won't work because they don't provide the letters needed. Terminate a search path
		* if the length of the path is already longer than shortest path found so far.
		* But there is no caching, no dynamic programming in this solution. It works but slow.
		*/
		unsigned long allMatchStateValue;
		int currentBest;
		void backtrackSolve(string& target, vector<string>&stickers, bitset<16>&state, int currentPathLength)
		{
			unsigned long st = state.to_ulong();
			if (st == allMatchStateValue && currentPathLength < currentBest)
			{
				currentBest = currentPathLength;
				return;
			}
			if (currentPathLength == currentBest && st != allMatchStateValue)
				return;

			bitset<16> oldState = state;

			for (string& sk : stickers)
			{
				if (isStickerUseful(target, sk, state))
				{
					applySticker(target, sk, state);
					backtrackSolve(target, stickers, state, currentPathLength + 1);
					state = oldState;
				}
			}
		}

		void applySticker(string& target, string& sticker, bitset<16>& state)
		{
			for (char& c1 : sticker)
			{
				for (size_t i = 0; i < target.size(); i++)
				{
					if (target[i] == c1 && !state[i])
					{
						state[i] = true;
						break;
					}
				}
			}
		}

		bool isStickerUseful(string& target, string& sticker, bitset<16>&state)
		{
			for (size_t i = 0; i < target.size(); i++)
			{
				if (!state[i])
				{
					char targetChar = target[i];
					auto it = find(sticker.begin(), sticker.end(), targetChar);
					if (it != sticker.end())
						return true;
				}
			}
			return false;
		}

		void transformTarget(string& target, int* targetArray)
		{
			for (char& c : target)
			{
				targetArray[c - 'a']++;
			}
		}
		vector<string> transformStickers(vector<string>& stickers, int* targetArray)
		{
			vector<string>shortStickers;
			for (string& st : stickers)
			{
				string shortSticker;
				for (char& c : st)
				{
					if (targetArray[c - 'a'] != 0)
						shortSticker += c;
				}
				if (shortSticker.size() > 0)
					shortStickers.push_back(shortSticker);
			}
			return shortStickers;
		}
	public:
		int minStickers(vector<string>& stickers, string target)
		{
			size_t len = target.size();
			allMatchStateValue = (1ul << len) - 1;
			bitset<16>state(0);
			currentBest = numeric_limits<int>::max();

			int targetArray[26];
			memset(targetArray, 0, sizeof(targetArray));
			transformTarget(target, targetArray);
			vector<string> shortStickers = transformStickers(stickers, targetArray);
			
			backtrackSolve(target, shortStickers, state, 0);

			return currentBest != numeric_limits<int>::max() ? currentBest : -1;
		}
	};


	class Solution2 // accepted
	{
		/*
		* Assume there are 100 stickers.
		* Backtracking:
		* When I stand on a node along a search path, theoratically there are 100 possibilities for the next node,
		* but I abandon some possibilities because some stickers can't contribute the letters I need at that moment.
		* When I stand on a node, I must get the result of that subtree. I must not terminate because the path is
		* already too long and return without a result.
		* 
		* Dynamic programming:
		* I always get the result for a search subtree even if I know it's already not optimal.
		*/
		unsigned long allMatchStateValue;
		
		signed long long backtrackSolve(vector<signed long long>& r, string& target, vector<string>& stickers, bitset<16>& state)
		{
			unsigned long stateValue = state.to_ulong();
			if (r[stateValue] != -1)
				return r[stateValue];

			bitset<16> oldState = state;
			signed long long minResult = INT32_MAX;
			for (string& sk : stickers)
			{
				if (isStickerUseful(target, sk, state))
				{
					applySticker(target, sk, state);
					signed long long result = 1 + backtrackSolve(r, target, stickers, state);
					minResult = min(minResult, result);
					state = oldState;
				}
			}
			r[stateValue] = minResult;
			return minResult;
		}

		void applySticker(string& target, string& sticker, bitset<16>& state)
		{
			for (char& c1 : sticker)
			{
				for (size_t i = 0; i < target.size(); i++)
				{
					if (target[i] == c1 && !state[i])
					{
						state[i] = true;
						break;
					}
				}
			}
		}

		bool isStickerUseful(string& target, string& sticker, bitset<16>& state)
		{
			for (size_t i = 0; i < target.size(); i++)
			{
				if (!state[i])
				{
					char targetChar = target[i];
					auto it = find(sticker.begin(), sticker.end(), targetChar);
					if (it != sticker.end())
						return true;
				}
			}
			return false;
		}

		void transformTarget(string& target, int* targetArray)
		{
			for (char& c : target)
			{
				targetArray[c - 'a']++;
			}
		}
		vector<string> transformStickers(vector<string>& stickers, int* targetArray)
		{
			vector<string>shortStickers;
			for (string& st : stickers)
			{
				string shortSticker;
				for (char& c : st)
				{
					if (targetArray[c - 'a'] != 0)
						shortSticker += c;
				}
				if (shortSticker.size() > 0)
					shortStickers.push_back(shortSticker);
			}
			return shortStickers;
		}
	public:
		int minStickers(vector<string>& stickers, string target)
		{
			size_t len = target.size();
			allMatchStateValue = (1ul << len) - 1;
			bitset<16>state(0);
			
			vector<signed long long>r(allMatchStateValue + 1, -1ll);
			r[allMatchStateValue] = 0;
			int targetArray[26];
			memset(targetArray, 0, sizeof(targetArray));
			transformTarget(target, targetArray);
			vector<string> shortStickers = transformStickers(stickers, targetArray);

			int result = (int)backtrackSolve(r, target, shortStickers, state);
			return result == INT32_MAX ? -1 : result;
		}
	};


	void Test_0691_Stickers_to_Spell_Word()
	{
		Solution2 so;
		string s;
		vector<string>stickers;
		string target;
		while (true)
		{
			cout << "stickers:  ";
			s.clear();
			getline(cin, s);
			if (s.size() == 0) break;
			stringstream ss(s);
			stickers.clear();
			copy(istream_iterator<string>(ss), istream_iterator<string>(), back_inserter(stickers));
			cout << "target:  ";
			cin >> target;
			cin.ignore();
			cout << so.minStickers(stickers, target) << "\n\n";
		}
	}
}