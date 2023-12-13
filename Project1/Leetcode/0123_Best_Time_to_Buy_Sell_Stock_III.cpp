#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#pragma warning(disable: 4267 4244)

namespace _0123_Best_Time_to_Buy_Sell_Stock_III {

	using namespace std;

	class Solution
	{
		/*
		* the strategy is to find the max subarray of the diff array and
		* find another max subarray in front of the real max subarray or after it.
		* Or sometimes I need to split the real max subarray.
		* When looking for secondary max subarray after the real max subarray,
		* I decide to jump over any lingering positive numbers and start searching
		* when I see a zero. but this doesn't work in this case: 1 7 2 4
		*/


		/*
		* prices:      2    1    2    0    1
		* diff:            -1    1   -2    1
		* subarrSums:       0    1    0    1
		*/

		/*
		* prices:      6    1    3    2    4    7
		* diff:            -5    2   -1    2    3
		* subarrSums:       0    2    1    3    6
		*/

		/*
		* prices:      1    2    4    2    5    7    2    4    9    0
		* diff:             1    2   -2    3    2   -5    2    5   -9
		* subarrSums:       1    3    1    4    6    1    3    8    0
		*/

		/*
		* prices:      1    7    2    4
		* diff:             6   -5    2
		* subarrSums:       6    1    3
		*/


	public:
		int maxProfit(vector<int>& prices)
		{
			int n = prices.size();
			if (n == 1) return 0;
			vector<int>a(n - 1);
			for (int i = 1; i < n; i++)
			{
				a[i - 1] = prices[i] - prices[i - 1];
			}

			vector<int>subarrSums(n - 1);
			int sum = 0;
			for (int i = 0; i < n - 1; i++)
			{
				sum += a[i];
				sum = sum > 0 ? sum : 0;
				subarrSums[i] = sum;
			}

			int start1, end1;

			auto it = max_element(subarrSums.begin(), subarrSums.end());
			end1 = distance(subarrSums.begin(), it);
			int i;
			for (i = end1 - 1; i >= 0; i--)
			{
				if (subarrSums[i] == 0)
					break;
			}
			start1 = i + 1;

			int secondaryMax = 0;

			if (start1 > 0)
			{
				it = max_element(subarrSums.begin(), subarrSums.begin() + start1);
				secondaryMax = *it;
			}
			if (end1 < subarrSums.size() - 1)
			{
				/*
				* If the subarrSums is like this: 3 5 0 0 6 9 4 0 0 2 1
				*                                             |
				*                                            this '4' is after '9', this means there is a negative number after a big number, so the sum becomes smaller.
				*                                            If I want to find the max-sum subarray within the subarray after end1, I must not include that negative number
				*                                            Therefore, I should start my search after at least a '0'.
				*/
				int j = end1 + 1;
				while (j < subarrSums.size() && subarrSums[j] > 0)
					j++;

				if (j < subarrSums.size())
				{
					for (; j < subarrSums.size(); j++)
					{
						secondaryMax = max(secondaryMax, subarrSums[j]);
					}
				}
			}

			int schema1 = subarrSums[end1] + secondaryMax;

			if (end1 - start1 > 1)
			{
				int schema2 = 0;
				for (int j = end1-1; j > start1; j--)
				{
					if (subarrSums[j-1] + subarrSums[end1] - subarrSums[j] > schema2)
					{
						schema2 = subarrSums[j - 1] + subarrSums[end1] - subarrSums[j];
					}
				}
				return schema1 > schema2 ? schema1 : schema2;
				
			}

			return schema1;
		}
	};

	class Solution2
	{
		/*
		* This solution is correct, but is slow.
		*/
		int maxProfit2Transactions(vector<int>const & lowsHighs)
		{
			// Note that this function assumes that lowsHighs must end with a high point.
			// 
			// what is the max profit with 1 transaction in a subarray of lowsHighs?
			// for example, lows highs are like this:
			//            hi1          hi2           hi3
			//     lo1           lo2           lo3
			//     |
			//    what is the best I can do when I look at a subarray starting at lo1?
			// (1). If I buy at lo1 and find a best point to sell
			// (2). If I don't buy at lo1, then the best I can do is the same as the best
			// I can do when I look at the subarray starting at lo2
			//
			size_t n = lowsHighs.size();
			vector<int> oneTranMax(n, 0);
			size_t i = n;
			if (n >= 2)
			{
				// 'i' is of size_t which is unsigned, I want to go backward,
				// I have to use a do-while loop, and make sure the very first iteration
				// is valid, i.e. if n == 1, then i - 2 becomes 1-2.
				do {
					i = i - 2;
					int buy = lowsHighs[i];
					int maxProfitWithBuy = 0;
					for (size_t j = i + 1; j < n; j = j + 2)
					{
						int profit = lowsHighs[j] - buy;
						maxProfitWithBuy = max(maxProfitWithBuy, profit);
					}
					int profit_notBuy_on_i = i < n - 2 ? oneTranMax[i + 2] : 0;
					oneTranMax[i] = max(profit_notBuy_on_i, maxProfitWithBuy);
				} while (i > 0);
			}

			int maxProfit = 0;
			for (size_t i = 0; i < n; i = i + 2)
			{
				int buy = lowsHighs[i];
				for (size_t j = i + 1; j < n; j = j + 2)
				{
					int firstTranProfit = lowsHighs[j] - buy;
					int secondTranProfit = j < n - 1 ? oneTranMax[j + 1] : 0;
					maxProfit = max(maxProfit, firstTranProfit + secondTranProfit);
				}
			}

			return maxProfit;
		}

		vector<int> findLowsAndHighs(vector<int>& prices) // this implementation doesn't feel 100% correct
		{
			size_t n = prices.size();

			vector<int>lowsHighs;
			bool isUp = false;
			// the goal is to find the first low point, that is a potential buy point.
			// If the prices start with a downward trend, find the low point, remember it.
			// If the prices start with a upward trend, then prices[0] is the first low point.
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
			for (i = 2; i < n; i++)  // this doesn't feel correct, why do I start from 2? I may have moved past 2 in my effort to find the first low point.
			{
				if (isUp)
				{
					if (prices[i - 1] > prices[i])
					{
						lowsHighs.push_back(prices[i - 1]); // found a high point
						isUp = false;
					}
				}
				else
				{
					if (prices[i - 1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]); // found a low point
						isUp = true;
					}
				}
			}
			if (isUp)
				lowsHighs.push_back(prices[i - 1]);

			return lowsHighs;
		}

		vector<int> findLowsAndHighs2(vector<int>& prices)
		{
			vector<int> lowsHighs;
			bool isUp = false;;
			lowsHighs.push_back(prices[0]);
			/*
			* Just put the first price value in lowsHighs and say we're in a downward trend.
			* Why does this work?
			* If the next one is higher, then the first price value is kept and the next
			* price value is also put in and we keep looking for if the next after that is
			* even higher. Basically the first value is a low point and is recorded.
			* 
			* If the next one is lower, the first value in lowsHighs will be replaced.
			*/
			for (size_t i = 1; i < prices.size(); i++)
			{
				if (isUp)
				{
					if (*lowsHighs.rbegin() <= prices[i])
						*lowsHighs.rbegin() = prices[i];
					else
					{
						lowsHighs.push_back(prices[i]);
						isUp = false;
					}
				}
				else
				{
					if (*lowsHighs.rbegin() >= prices[i])
						*lowsHighs.rbegin() = prices[i];
					else
					{
						lowsHighs.push_back(prices[i]);
						isUp = true;
					}
				}
			}
			// if the last few prices values are in a downward trend, I don't need
			// to record a low point at the end of the lowsHighs array, because I'm not
			// going to sell on that low point for sure, and I can't buy on that low
			// point either, because there isn't another high point after it.
			if (!isUp)
			{
				lowsHighs.erase(lowsHighs.rbegin().base(), lowsHighs.end());
			}
			return lowsHighs;
		}

	public:
		int maxProfit(vector<int>& prices)
		{
			if (prices.size() < 2)
				return 0;

			vector<int>const& lowsHighs = findLowsAndHighs2(prices);

			return maxProfit2Transactions(lowsHighs);
		}
	};




	class Solution3
	{
		/*
		 * when I reach t1's sell point, I must have recorded the lowest point -- among all points in front of t1's sell point -- as t1's cost.
		 * It may not be the lowest point among the entire array, but it must be the lowest among all points precedes t1's sell point.
		 * Likewise, when I reach t2's sell point, t2Cost must have been recorded with the lowest (prices[i] - t1Profit) value among all such values
		 * precedes t2's sell point.
		 */


		/*
		* Is it possible that I recorded a t2cost at some price point
		* that is between t1's buy and sell points? i.e. I buy t2 before selling
		* t1? No it's not possible. For example, these are prices:
		* a, b, c, d, e, f, g, h, i, j, k, m, n
		* 
		* Let's say 'c' is a low price, I record t1cost and t2cost at 'c'.
		* As long as I don't sell t1, t1profit will be 0, "price - t1profit" will
		* just be price.
		* As soon as I sell t1, for example I sell it at 'f'. There is a t1profit
		* which is "f - c". Then I try to see if I should buy it back immediately
		* at 'f'. Actually I will try to do just that, because at that point,
		* "price - t1profit", it's still 'c', but it won't violate the not-holding-2-shares-simultaneously
		* rule because I would pretend to sell and immediately buy back on the same day.
		* Then if the very next day's price is lower, I won't change where I sell t1,
		* but "price - t1profit" becomes lower, so I would change where I buy t2.
		* 
		* Therefore, before I find a valid t1 selling point, i.e. if the first a few
		* price points are in downward trend, the 3rd and 4th lines inside the for loop
		* actually don't matter.
		* 
		*/
	public:
		int maxProfit(vector<int>& prices) {
			int t1Cost = INT_MAX,
				t2Cost = INT_MAX;
			int t1Profit = 0,
				t2Profit = 0;

			for (int price : prices) {
				// the maximum profit if only one transaction is allowed
				t1Cost = min(t1Cost, price);
				t1Profit = max(t1Profit, price - t1Cost);
				// re-invest the gained profit in the second transaction
				t2Cost = min(t2Cost, price - t1Profit);
				t2Profit = max(t2Profit, price - t2Cost);
			}

			return t2Profit;
		}
	};



	class Solution5
	{
		/*
		* Using the same principle as the classic stock profit problem.
		* First turn the every day price to price deltas.
		* Go through the array of deltas as if I'm looking for a max profit for 1 trade.
		* for example, this is the deltas' array:
		* a  b  c  d  e  f  g  h  i  j  k
		*                         |
		*                       for example if I want to use 'i' as the 2nd trade, I should use
		* the best trade among 'a' to 'h' as the 1st trade. That is also the max profit for a
		* single trade among 'a' to 'h'. So I just keep a record of the max number as I move
		* forward, when I arrive at 'i', that max number is the best first trade. I try to
		* use a 2-trade strategy, see what I can get by make that best first trade and the 2nd
		* trade at 'i'. If that yields a better outcome for a 2-trade strategy, record that.
		* By the end of the array, I have the max number of the array which is the max profit by
		* a single trade. I also tried all the possible 2-trade scenarios.
		* The key is that when I record a better single trade, I need to reset the accumulation
		* for the 2nd trade. For example, if I'm at 'e' and I find that is a better sell point
		* for a single trade, I record that as the profit for a single trade. At that point, I've
		* sole the stock and in order to do the 2nd trade, the accumulation has to start at 0.
		*/
	public:
		int maxProfit(vector<int> prices)
		{
			size_t len = prices.size();
			vector<int> deltas = vector<int>(len - 1, 0);
			for (size_t i = 1; i < len; i++)
			{
				deltas[i - 1] = prices[i] - prices[i - 1];
			}

			deltas[0] = deltas[0] > 0 ? deltas[0] : 0;
			int firstTradeMax = deltas[0];
			int secondTradeMax = 0;
			int twoTradesMax = 0;
			int accum1 = deltas[0], accum2 = 0;
			for (size_t i = 1; i < len - 1; i++)
			{
				if (deltas[i] + accum1 > 0)
					accum1 += deltas[i];
				else
					accum1 = 0;

				if (deltas[i] + accum2 > 0)
				{
					accum2 += deltas[i];
					if (twoTradesMax < firstTradeMax + accum2)
						twoTradesMax = firstTradeMax + accum2;
				}
				else
					accum2 = 0;

				if (firstTradeMax < accum1)
				{
					firstTradeMax = accum1;
					accum2 = 0;
				}
			}
			return max(firstTradeMax, twoTradesMax);

		}
	};

	class Solution6
	{
		/*
		* This is the same principle as solution 5.
		* Upon further inspection of solution 5, I don't really need to create an array of deltas.
		* When I examine an element in the delta's array, I only look at that element, not the ones
		* in front of it or behind it. So I can calculate the delta as I go. Just iterate the price
		* array and calculate the delta when I move each step forward.
		*/
	public:
		int maxProfit(vector<int> prices)
		{
			if (prices.size() == 1)
				return 0;

			int firstTradeMax = 0;
			int secondTradeMax = 0;
			int twoTradesMax = 0;
			
			int firstDelta = prices[1] - prices[0];
			firstTradeMax = firstDelta > 0 ? firstDelta : 0;
			int accum1 = firstTradeMax, accum2 = 0;

			size_t len = prices.size();
			for (size_t i = 2; i < len; i++)
			{
				int delta = prices[i] - prices[i - 1];
				if (delta + accum1 > 0)
					accum1 += delta;
				else
					accum1 = 0;

				if (delta + accum2 > 0)
				{
					accum2 += delta;
					if (twoTradesMax < firstTradeMax + accum2)
						twoTradesMax = firstTradeMax + accum2;
				}
				else
					accum2 = 0;

				if (firstTradeMax < accum1)
				{
					firstTradeMax = accum1;
					accum2 = 0;
				}
			}
			return max(firstTradeMax, twoTradesMax);
		}
	};

	// 2, 1, 4, 5, 2, 9, 7      11
	// 6  1  3  2  4  7         7

	class Solution7
	{
		
	public:
		int maxProfit(vector<int> prices)
		{
			if (prices.size() == 1)
				return 0;

			int firstTradeMax = 0;
			int secondTradeMax = 0;
			int twoTradesMax = 0;

			int firstDelta = prices[1] - prices[0];
			firstTradeMax = firstDelta > 0 ? firstDelta : 0;
			int accum1 = firstTradeMax, accum2 = 0;

			size_t len = prices.size();
			for (size_t i = 2; i < len; i++)
			{
				int delta = prices[i] - prices[i - 1];
				if (delta + accum1 > 0)
					accum1 += delta;
				else
					accum1 = 0;

				if (delta + accum2 > 0)
				{
					accum2 += delta;
					if (twoTradesMax + delta < firstTradeMax + accum2)
						twoTradesMax = firstTradeMax + accum2;
					else
						twoTradesMax += delta;
				}
				else
				{
					accum2 = 0;
					if (twoTradesMax + delta < 0)
						twoTradesMax = firstTradeMax;
				}

				if (firstTradeMax < accum1)
				{
					firstTradeMax = accum1;
					accum2 = 0;
				}
			}
			return max(firstTradeMax, twoTradesMax);
		}
	};







	void Test_0123_Best_Time_to_Buy_Sell_Stock_III()
	{
		Solution7 solu;
		string s;
		vector<int> prices;
		while (true)
		{
			s.clear();
			cout << "prices: ";
			getline(cin, s);
			if (s.size() == 0) break;
			stringstream ss(s);
			prices.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(prices));
			cout << solu.maxProfit(prices) << "\n\n";
		}
	}
}