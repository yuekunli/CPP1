#include <iostream>
#include <vector>
//#include <random>
//#include <algorithm>
//#include <numeric>
//#include <functional>
#include <iomanip>
#include <string>
#include <sstream>

namespace Fenwick_Tree_Postfix_Range {

	/*
	* Given a data array 'd', length n, implement two operations on this data array
	* (1). query
	* given x, return the information for range d[x] to d[last] inclusive
	* 
	* (2). update
	* given x and a value, update d[x] to the new value, make sure the subsequent query operations returned up-to-date information
	*/


	using namespace std;

	class Fenwick_Sum
	{
		vector<int> data;

		vector<int> fen;

		void build_fen_tree(vector<int>&data, vector<int>&fen)
		{
			size_t n = data.size();
			for (int i = 1; i <= n; i++)
			{
				fen[i] += data[n - i];
				int p = i + (i & (-i));
				if (p <= n)
					fen[p] += fen[i];
			}
		}

		int query_process_info(int x, int info)
		{
			return info + fen[x];
		}

		void update_process_info(int x, int info)
		{
			fen[x] += info;
		}

		int query_prefix(int x)
		{
			int info = fen[x];
			while (x > 0)
			{
				x = x - (x & (-x));
				info = query_process_info(x, info);
			}
			return info;
		}

		void update_prefix(int x, int v)
		{
			size_t n = fen.size();
			while (x < n)
			{
				update_process_info(x, v);
				x = x + (x & (-x));
			}
		}

	public:

		Fenwick_Sum(vector<int>& d)
		{
			data = d;
			fen = vector<int>(data.size() + 1);
			build_fen_tree(data, fen);
		}

		/*
		* 
		* data array index:   0   1   2   3   4   5   6   7   8
		*                                             |
		*                                            give '6' to query, I want data[6] + data[7] + data[8],
		*                                            result should be returned by query(3), because it's the sum of 3 elements
		*/


		/*
		* 'x' is the index of the original data array
		* This function returns the sum of range data[x] through data[last]
		*/
		int query(int x)
		{
			return query_prefix(data.size() - x);
		}
		
		/*
		* set the data[x] to 'v', update fenwick tree accordingly
		*/
		void update(int x, int v) // if caller can pass in delta, this class doesn't need to save a copy of data array
		{
			int delta = v - data[x];
			update_prefix(data.size() - x, delta);
		}
	};


	void Test_1()
	{
		// data               2,   8,   4,   9,   1,   20,   6,   14,   3,   7,   10
		// index              0    1    2    3    4    5     6    7     8    9    10
		// postfix sum:       84   82   74   70   61   60    40   34    20   17   10

		vector<int> data{2, 8, 4, 9, 1, 20, 6, 14, 3, 7, 10};
		int postfix_sum[11] = { 84,82,74,70,61,60,40,34,20,17,10 };

		Fenwick_Sum fen(data);
		
		for (int i = 10; i >= 0; i--)
		{
			if (fen.query(i) != postfix_sum[i])
			{
				cout << "ERROR" << "   i: " << i << "   fenwick sum: " << fen.query(i) << "   correct sum: " << postfix_sum[i] << "\n\n";
				return;
			}
		}
		cout << "PASS\n\n";
	}

	void Test_2()
	{
		// data               2,   8,   4,   9,   1,   20,   6,   14,   3,   7,   10
		// index              0    1    2    3    4    5     6    7     8    9    10
		// postfix sum:       84   82   74   70   61   60    40   34    20   17   10

		vector<int> data{ 2, 8, 4, 9, 1, 20, 6, 14, 3, 7, 10 };
		int postfix_sum[11] = { 84,82,74,70,61,60,40,34,20,17,10 };

		Fenwick_Sum fen(data);

		fen.update(10, 15);   // set data[10] to 15

		for (int i = 0; i < 11; i++)
			postfix_sum[i] += 5;

		for (int i = 10; i >= 0; i--)
		{
			if (fen.query(i) != postfix_sum[i])
			{
				cout << "ERROR" << "   i: " << i << "   fenwick sum: " << fen.query(i) << "   correct sum: " << postfix_sum[i] << "\n\n";
				return;
			}
		}
		cout << "PASS\n\n";
	}






	/*
	* index                    0   1   2   3   4   5   6   7
	* time slot                1   2   3   4   5   6   7   8
	* event deadline           3   3   4   5   5   8   8   9
	* slack                    2   1   1   1   0   2   1   1
	* smallest slack in range  0   0   0   0   0   1   1   1
	* 
	*/

	class Fenwick_Min
	{
		vector<int> data;
		vector<int> fen;

		void build_fen_tree(vector<int>& data, vector<int>& fen)
		{
			size_t n = data.size();
			for (int i = 1; i <= n; i++)
			{
				int slack = data[n - i] - (n - i + 1);
				
				if (fen[i] > slack)
					fen[i] = slack;

				int p = i + (i & (-i));
				if (p <= n)
				{
					if (fen[p] > fen[i])
						fen[p] = fen[i];
				}
			}
		}

		int query_process_info(int x, int info)
		{
			return min(fen[x], info);
		}

		void update_process_info(int x, int info)
		{
			if (fen[x] > info)
				fen[x] = info;
		}

		int query_prefix(int x)
		{
			int info = fen[x];
			while (x > 0)
			{
				x = x - (x & (-x));
				info = query_process_info(x, info);
			}
			return info;
		}

		void update_prefix(int x, int v)
		{
			size_t n = fen.size();
			while (x < n)
			{
				update_process_info(x, v);
				x = x + (x & (-x));
			}
		}

	public:

		Fenwick_Min(vector<int>& d)
		{
			data = d;
			fen = vector<int>(data.size() + 1, 100000);

			build_fen_tree(data, fen);
		}

		/*
		*
		* data array index:   0   1   2   3   4   5   6   7   8
		*                                             |
		*                                            give '6' to query, I want info for range: data[6], data[7], data[8],
		*                                            result should be returned by query(3), because it's the range of 3 elements
		*/


		/*
		* 'x' is the index of the original data array
		* This function returns the information for range data[x] through data[last]
		*/
		int query(int x)
		{
			return query_prefix(data.size() - x);
		}

		/*
		* set the data[x] to 'v', update fenwick tree accordingly
		*/
		void update(int x, int v) // if caller can pass in delta, this class doesn't need to save a copy of data array
		{
			int newdiff = v - (x + 1);
			update_prefix(data.size() - x, newdiff);
		}
	};





	void Test_3()
	{
		// time slot      1    2    3    4    5    6    7    8
		vector<int> data{ 3,   3,   4,   5,   5,   8,   8,   9 };
		// slack          2    1    1    1    0    2    1    1
		
		int min_slack[8] = { 0,   0,   0,   0,   0,   1,   1,   1 };

		Fenwick_Min fen(data);

		for (int i = 7; i >= 0; i--)
		{
			if (fen.query(i) != min_slack[i])
			{
				cout << "ERROR" << "   i: " << i << "   fenwick sum: " << fen.query(i) << "   minimum slack: " << min_slack[i] << "\n\n";
				return;
			}
		}
		cout << "PASS\n\n";
	}

	void Test_4()
	{
		// time slot      1    2    3    4    5    6    7    8
		vector<int> data{ 3,   3,   4,   5,   5,   8,   8,   9 };
		// slack          2    1    1    1    0    2    1    1

		int min_slack[8] = { 0,   0,   0,   0,   0,   1,   1,   1 };

		Fenwick_Min fen(data);

		fen.update(6, 7); // change the deadline of the event at index 6 to be 7 (it was 8), its slack becomes 0

		min_slack[5] = min_slack[6] = 0;

		for (int i = 7; i >= 0; i--)
		{
			if (fen.query(i) != min_slack[i])
			{
				cout << "ERROR" << "   i: " << i << "   fenwick info: " << fen.query(i) << "   minimum slack: " << min_slack[i] << "\n\n";
				return;
			}
		}
		cout << "PASS\n\n";
	}


	void Test_Fenwick_Tree_Postfix_Range()
	{
		Test_1();
		Test_2();
		Test_3();
		Test_4();
	}
}






