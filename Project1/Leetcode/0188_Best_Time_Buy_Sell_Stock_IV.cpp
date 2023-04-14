#include<iostream>
#include<string>
#include<sstream>
#include<vector>

namespace _0188_Best_Time_Buy_Sell_Stock_IV {

	using namespace std;

	/*
	* If k is '2', I can do "buy sell buy sell". A buy action and a sell action together are called 1 transaction
	*/

	class Solution
	{
		/*
		* Time limit exceeded
		*/

		size_t n;
		int solve(vector<int>& prices, size_t index, int k, vector<vector<int>>& r, int previousBuyPrice)
		{
			if (index == n)
				return 0;
			if (k == 0)
				return 0;

			if (previousBuyPrice == -1 && r[index][k] != -1)
				return r[index][k];

			int maxProfit = 0;
			if (previousBuyPrice >= 0)
			{
				for (size_t i = index; i < n; i++)
				{
					int profit = prices[i] - previousBuyPrice + solve(prices, i+1, k - 1, r, -1);
					maxProfit = max(maxProfit, profit);
				}
				return maxProfit;
			}
			else
			{
				for (size_t i = index; i < n; i++)
				{
					int profit = solve(prices, i + 1, k, r, prices[i]); // check the possibility of buying at each index
					maxProfit = max(maxProfit, profit);

					profit = solve(prices, i + 1, k, r, -1);// check the possibility of not doing anything at each index and looking at the rest of the array with a "fresh" start
					maxProfit = max(maxProfit, profit);
				}
				r[index][k] = maxProfit;
				return maxProfit;
			}
		}

	public:
		int maxProfit(int k, vector<int>& prices)
		{
			n = prices.size();
			vector<vector<int>>records(n, vector<int>(k + 1, -1));
			return solve(prices, 0, k, records, -1);
		}
	};


	class Solution2
	{
		/*
		* Still too slow
		*/

		size_t n;
		int solve(vector<int>& prices, vector<vector<vector<int>>>& recordWithStockBought, vector<vector<int>>&recordNoStock, size_t index, size_t previousBuyIndex, int k)
		{
			if (index == n)
				return 0;
			if (k == 0)
				return 0;

			if (previousBuyIndex == -1 && recordNoStock[index][k] != -1)
				return recordNoStock[index][k];

			if (previousBuyIndex > -1 && recordWithStockBought[index][previousBuyIndex][k] != -1)
				return recordWithStockBought[index][previousBuyIndex][k];

			int maxProfit = 0;
			if (previousBuyIndex >= 0)
			{
				for (size_t i = index; i < n; i++)
				{
					int profit = prices[i] - prices[previousBuyIndex] + solve(prices, recordWithStockBought, recordNoStock, i + 1, -1, k - 1);
					maxProfit = max(maxProfit, profit);
				}
				recordWithStockBought[index][previousBuyIndex][k] = maxProfit;
				return maxProfit;
			}
			else
			{
				for (size_t i = index; i < n; i++)
				{
					int profit = solve(prices, recordWithStockBought, recordNoStock, i + 1, i, k); // check the possibility of buying at each index
					maxProfit = max(maxProfit, profit);

					profit = solve(prices, recordWithStockBought, recordNoStock, i+1, -1, k);// check the possibility of not doing anything at each index and looking at the rest of the array with a "fresh" start
					maxProfit = max(maxProfit, profit);
				}
				recordNoStock[index][k] = maxProfit;
				return maxProfit;
			}
		}

	public:
		int maxProfit(int k, vector<int>& prices)
		{
			n = prices.size();
			vector<vector<int>>recordNoStock(n, vector<int>(k + 1, -1));
			vector<vector<vector<int>>>recordWithStockBought(n, vector<vector<int>>(n, vector<int>(k + 1, -1)));
			return solve(prices, recordWithStockBought, recordNoStock, 0, -1, k);
		}
	};


	class Solution3
	{
		size_t n;
		int solve(vector<int>& prices, vector<vector<vector<int>>>& recordWithStockBought, vector<vector<int>>& recordNoStock, size_t index, size_t previousBuyIndex, int k)
		{
			if (index == n)
				return 0;
			if (k == 0)
				return 0;

			if (previousBuyIndex == -1 && recordNoStock[index][k] != -1)
				return recordNoStock[index][k];

			if (previousBuyIndex > -1 && recordWithStockBought[index][previousBuyIndex][k] != -1)
				return recordWithStockBought[index][previousBuyIndex][k];

			int maxProfit = 0;
			if (previousBuyIndex >= 0)
			{
				for (size_t i = index; i < n; i++)
				{
					if ((i < n - 1 && prices[i] > prices[i + 1] && prices[i] > prices[previousBuyIndex]) || (i == n - 1 && prices[i] > prices[previousBuyIndex]))
						              // only sell if i's price is higher than (i+1)'s price, otherwise, hold on and sell later
						              // actually if i's price is lower than (i-1)'s price, I shouldn't trying selling at i either, should've sold earlier.
					{
						int profit = prices[i] - prices[previousBuyIndex] + solve(prices, recordWithStockBought, recordNoStock, i + 1, -1, k - 1);
						maxProfit = max(maxProfit, profit);
					}
				}
				recordWithStockBought[index][previousBuyIndex][k] = maxProfit;
				return maxProfit;
			}
			else
			{
				for (size_t i = index; i < n; i++)
				{
					int profit = 0;

					if (i < n - 1 && prices[i] < prices[i + 1]) // don't buy if i's prices is not even lower than (i+1)'s price
					{
						profit = solve(prices, recordWithStockBought, recordNoStock, i + 1, i, k);
						maxProfit = max(maxProfit, profit);
					}
					maxProfit = max(maxProfit, profit);
				}
				recordNoStock[index][k] = maxProfit;
				return maxProfit;
			}
		}

	public:
		int maxProfit(int k, vector<int>& prices)
		{
			n = prices.size();
			vector<vector<int>>recordNoStock(n, vector<int>(k + 1, -1));
			vector<vector<vector<int>>>recordWithStockBought(n, vector<vector<int>>(n, vector<int>(k + 1, -1)));
			return solve(prices, recordWithStockBought, recordNoStock, 0, -1, k);
		}
	};



	class Solution4
	{
		/*
		* If at some point, I have 3 transactions left (k=3), and the "lowsHighs" array has 2 pairs of (low, high) combination,
		* I should stop trying because a better solution must have been reached when I had 3 pairs of (low, high) combination.
		* But be careful when the lowsHighs array doesn't even have that many pairs to begin with.
		* For example: prices: [2, 4, 1];  k = 2.  This prices array is converted to a lowsHighs array like this: [2, 4]
		* There is only 1 pair and k = 2.
		* But if there are 5 pairs in total, k= 3, I picked 1 pair, and skipped 2 pairs, now I'm looking at 2 final pairs,
		* but I just need 2 more pairs. 
		*/

		size_t n2;
		int originalK;
		int solve(vector<int>& lowsHighs, vector<vector<int>>& r1, vector<vector<vector<int>>>& r2, size_t index, size_t buyIndex, int k)
		{
			if (index == n2) return 0;
			if (k == 0) return 0;

			if (buyIndex == -1 && r1[index][k] != -1)
				return r1[index][k];
			if (buyIndex > -1 && r2[index][buyIndex][k] != -1)
				return r2[index][buyIndex][k];

			int maxProfit = 0;
			if (buyIndex == -1) // no previous buy
			{
				for (size_t i = index; i < n2; i++)
				{
					if (i % 2 == 0) // I'm looking at a low point (low points have indices: 0, 2, 4, 6, ....)
					{
						if (n2/2 < originalK || (n2 - i) / 2 >= k) // There are more transactions allowed than I can do, so buy in of course
						{
							int profit = solve(lowsHighs, r1, r2, i + 1, i, k);
							maxProfit = max(maxProfit, profit);
						}
						// Why do I not consider when there are limited transactions allowed?
					}
				}
				r1[index][k] = maxProfit;
				return maxProfit;
			}
			else
			{
				for (size_t i = index; i < n2; i++)
				{
					if (i % 2 == 1 && lowsHighs[i] > lowsHighs[buyIndex])
					{
						int profit = lowsHighs[i] - lowsHighs[buyIndex] + solve(lowsHighs, r1, r2, i + 1, -1, k - 1);
						maxProfit = max(maxProfit, profit);
					}
				}
				r2[index][buyIndex][k] = maxProfit;
				return maxProfit;
			}
		}
	public:
		int maxProfit(int k, vector<int>& prices)
		{
			originalK = k;
			size_t n = prices.size();
			if (n < 2) return 0;

			vector<int>lowsHighs;
			bool isUp;
			if (prices[0] > prices[1])
				isUp = false;
			else if (prices[0] < prices[1])
			{
				lowsHighs.push_back(prices[0]);
				isUp = true;
			}
			else
			{
				size_t i = 1;
				while (i < n && prices[i] == prices[0])
					i++;
				if (i < n)
				{
					if (prices[i - 1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = true;
					}
					else
						isUp = false;
				}
			}

			size_t i;
			for (i = 2; i < n; i++)
			// if there are some equal values at the beginning of the array,
			// I should've checked them when I looked for the first low/high point,
			// but it's OK to start from i=2 here, if I already set "isUp" to true,
			// I won't do anything until I reach the first high point. Likewise if
			// I have set "isUp" to false.
			{
				if (isUp)
				{
					if (prices[i - 1] > prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = false;
					}
				}
				else
				{
					if (prices[i-1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = true;
					}
				}
			}
			if (isUp)
				lowsHighs.push_back(prices[i - 1]);
			
			n2 = lowsHighs.size();
			vector<vector<int>>r1(n2, vector<int>(k + 1, -1));
			vector<vector<vector<int>>>r2(n2, vector<vector<int>>(n2, vector<int>(k + 1, -1)));
			return solve(lowsHighs, r1, r2, 0, -1, k);
		}
	};


	class Solution5
	{
		/*
		* The big improvement from Solution4 to Solution5 is that
		* I don't really need to remember where I bought the stock.
		*/

		size_t n2;
		int originalK;
		int solve(vector<int>& lowsHighs, vector<vector<vector<int>>>& r, size_t index, int k, bool bought)
		{
			if (index == n2) return 0;
			if (k == 0) return 0;

			if (!bought && r[index][k][0] != -1)
				return r[index][k][0];
			if (bought && r[index][k][1] != -1)
				return r[index][k][1];

			int maxProfit = 0;
			if (!bought)
			{
				for (size_t i = index; i < n2; i++)
				{
					if (i % 2 == 0) // only try buying at a low point
					{
						if (n2 / 2 < originalK || (n2 - i) / 2 >= k)
						{
							int profit = solve(lowsHighs, r, i + 1, k, true) - lowsHighs[i]; // a call to "solve" with "true" as the last arg will return a result including the sell price
							maxProfit = max(maxProfit, profit);
						}
					}
				}
				r[index][k][0] = maxProfit;
				return maxProfit;
			}
			else  // bought stock before
			{
				for (size_t i = index; i < n2; i++)
				{
					if (i % 2 == 1)
					{
						int profit = lowsHighs[i] + solve(lowsHighs, r, i + 1, k - 1, false); // return the sell price and the optimal solution of the rest of the problem. The caller of this can add the sell price to its own optimal solution
						maxProfit = max(maxProfit, profit);
					}
				}
				r[index][k][1] = maxProfit;
				return maxProfit;
			}
		}
	public:
		int maxProfit(int k, vector<int>& prices)
		{
			originalK = k;
			size_t n = prices.size();
			if (n < 2) return 0;

			vector<int>lowsHighs;
			bool isUp;
			if (prices[0] > prices[1])
				isUp = false;
			else if (prices[0] < prices[1])
			{
				lowsHighs.push_back(prices[0]);
				isUp = true;
			}
			else
			{
				int i = 1;
				while (i < n && prices[i] == prices[0])
					i++;
				if (i < n)
				{
					if (prices[i - 1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = true;
					}
					else
						isUp = false;
				}
			}

			int i;
			for (i = 2; i < n; i++)
			{
				if (isUp)
				{
					if (prices[i - 1] > prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = false;
					}
				}
				else
				{
					if (prices[i - 1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = true;
					}
				}
			}
			if (isUp)
				lowsHighs.push_back(prices[i - 1]);

			n2 = lowsHighs.size();
			
			vector<vector<vector<int>>>r(n2, vector<vector<int>>(k+1, vector<int>(2, -1)));
			return solve(lowsHighs, r, 0, k, false);
		}
	};



	class Solution6 // this is accepted by leetcode
	{
		size_t n2;
		int originalK;
		int solve(vector<int>& lowsHighs, vector<vector<vector<int>>>& r, size_t index, int k, bool bought)
		{
			if (index == n2) return 0;
			if (k == 0) return 0;

			if (!bought && r[index][k][0] != -1)
				return r[index][k][0];
			if (bought && r[index][k][1] != -1)
				return r[index][k][1];

			int maxProfit = 0;
			if (!bought)
			{
				size_t i = index;
				int profit = 0;
				// It feels that the handling here is not clean, as to whether I should try buying or moving on or both. but this is accepted.
				// the next solution makes this part cleaner.
				if (i % 2 == 0) // only try buying at a low point
				{
					if (n2 / 2 < originalK || (n2 - i) / 2 >= k)
					{
						profit = solve(lowsHighs, r, i + 1, k, true) - lowsHighs[i]; // a call to "solve" with "true" as the last arg will return a result including the sell price
						maxProfit = max(maxProfit, profit);
					}
				}
				profit = solve(lowsHighs, r, i + 1, k, false); // looking at a high point and I didn't buy stock before, so just solve the rest of the array
				maxProfit = max(maxProfit, profit);
				r[index][k][0] = maxProfit;
				return maxProfit;
			}
			else  // bought stock before
			{
				size_t i = index;
				int profit = 0;
				if (i % 2 == 1)
				{
					// suppose I sell at this high point, return the price of this high point and the best result for the rest of the array
					int profit = lowsHighs[i] + solve(lowsHighs, r, i + 1, k - 1, false); // return the sell price and the optimal solution of the rest of the problem. The caller of this can add the sell price to its own optimal solution
					maxProfit = max(maxProfit, profit);
				}
				profit = solve(lowsHighs, r, i + 1, k, true);
				maxProfit = max(maxProfit, profit);
				r[index][k][1] = maxProfit;
				return maxProfit;
			}
		}
	public:
		int maxProfit(int k, vector<int>& prices)
		{
			originalK = k;
			size_t n = prices.size();
			if (n < 2) return 0;

			vector<int>lowsHighs;
			bool isUp;
			if (prices[0] > prices[1])
				isUp = false;
			else if (prices[0] < prices[1])
			{
				lowsHighs.push_back(prices[0]);
				isUp = true;
			}
			else
			{
				int i = 1;
				while (i < n && prices[i] == prices[0])
					i++;
				if (i < n)
				{
					if (prices[i - 1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = true;
					}
					else
						isUp = false;
				}
			}

			int i;
			for (i = 2; i < n; i++)
			{
				if (isUp)
				{
					if (prices[i - 1] > prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = false;
					}
				}
				else
				{
					if (prices[i - 1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = true;
					}
				}
			}
			if (isUp)
				lowsHighs.push_back(prices[i - 1]);

			n2 = lowsHighs.size();

			vector<vector<vector<int>>>r(n2, vector<vector<int>>(k + 1, vector<int>(2, -1)));
			return solve(lowsHighs, r, 0, k, false);
		}
	};



	class Solution7  // cleaner way to build the lowsHighs array
	{
		size_t lowHighCounts;
		int totalTransactionsAllowed;

		int solve(vector<int>& lowsHighs, vector<vector<vector<int>>>& r, size_t index, int k, bool bought)
		{
			if (index == lowHighCounts) return 0;
			if (k == 0) return 0;

			if (!bought && r[index][k][0] != -1)
				return r[index][k][0];
			if (bought && r[index][k][1] != -1)
				return r[index][k][1];

			int maxProfit = 0;
			if (!bought)
			{
				int profit = 0;
				if (index % 2 == 0) // only try buying at a low point
				{
					if (lowHighCounts / 2 < totalTransactionsAllowed || (lowHighCounts - index) / 2 <= k)
					{
						// I have more transaction quota than the trades left. So just buy at every low and sell at every high, maximize the profit for this subarray.
						for (size_t i = index+1; i < lowHighCounts; i=i+2)
						{
							maxProfit += (lowsHighs[i] - lowsHighs[i-1]);
						}
					}
					else
					{
						// I don't have that many transactions quota left, I need to try both strategies.
						{// buy
							profit = solve(lowsHighs, r, index + 1, k, true) - lowsHighs[index]; // a call to "solve" with "true" as the last arg will return a result including the sell price
							maxProfit = max(maxProfit, profit);
						}

						{// not buy
							profit = solve(lowsHighs, r, index + 1, k, false); // looking at a high point and I didn't buy stock before, so just solve the rest of the array
							maxProfit = max(maxProfit, profit);
						}
					}
				}
				else
				{
					// I'm looking at a high point, of course I should not buy, so just move on.
					profit = solve(lowsHighs, r, index + 1, k, false);
					maxProfit = max(maxProfit, profit);
				}
				
				r[index][k][0] = maxProfit;
				return maxProfit;
			}
			else  // bought stock before
			{
				int profit = 0;
				if (index % 2 == 1)
				{
					// suppose I sell at this high point, return the price of this high point and the best result for the rest of the array
					int profit = lowsHighs[index] + solve(lowsHighs, r, index + 1, k - 1, false); // return the sell price and the optimal solution of the rest of the problem. The caller of this can add the sell price to its own optimal solution
					maxProfit = max(maxProfit, profit);

					profit = solve(lowsHighs, r, index + 1, k, true);
					maxProfit = max(maxProfit, profit);
				}
				else
				{
					// looking at a low point, don't sell, move on.
					profit = solve(lowsHighs, r, index + 1, k, true);
					maxProfit = max(maxProfit, profit);
				}
				
				r[index][k][1] = maxProfit;
				return maxProfit;
			}
		}
	public:
		int maxProfit(int k, vector<int>& prices)
		{
			vector<int>lowsHighs;
			bool isUp;

			totalTransactionsAllowed = k;
			if (prices.size() < 2) return 0;

			lowsHighs.push_back(prices[0]);
			isUp = false; // I assume I start with a down trend, this way I won't record prices[0] if the prices array really starts with a down trend, which is what I want.
							// basically, if I assume I start with a down trend, I want to catch the first low point, if I assume I start with an up trend, I want to catch the first high point.
			size_t n = prices.size();
			for (size_t i = 1; i < n; i++)
			{
				if (isUp)
				{
					if (prices[i - 1] <= prices[i])
						*lowsHighs.rbegin() = prices[i];
					else
					{
						lowsHighs.push_back(prices[i]);
						isUp = false;
					}
				}
				else
				{
					if (prices[i-1] >= prices[i])
						*lowsHighs.rbegin() = prices[i];
					else
					{
						lowsHighs.push_back(prices[i]);
						isUp = true;
					}
				}
			}
			if (!isUp)
			{
				// the prices array ends with a down trend, I should've recorded the last prices as a low, but I don't need that.
				lowsHighs.erase(lowsHighs.end() - 1);
			}

			lowHighCounts = lowsHighs.size();

			vector<vector<vector<int>>>r(lowHighCounts, vector<vector<int>>(k + 1, vector<int>(2, -1)));
			return solve(lowsHighs, r, 0, k, false);
		}
	};


	void Test_0188_Best_Time_Buy_Sell_Stock_IV()
	{
		Solution6 s;
		string s1;
		int k;
		vector<int>prices;

		while (true)
		{
			s1.clear();
			cout << "prices: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			prices.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(prices));
			cout << "k: ";
			cin >> k;
			cin.ignore();
			cout << s.maxProfit(k, prices) << "\n\n";
		}
	}
}