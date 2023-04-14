#include<iostream>
#include<string>
#include<vector>

namespace  _0440_Kth_Smallest_in_Lexicographical_Order {

	using namespace std;

	class Solution
	{
		/*
		* Time limit exceeded
		*/

		struct Node
		{
			vector<Node*> c = vector<Node*>(10, nullptr);
			int numberInPath{ 0 };
		};
		Node* root;
	

		void buildTrie(int n)
		{
			root = new Node();
			
			for (int i = 1; i <= n; i++)
			{
				Node* a = root;
				string s = to_string(i);
				for (char& d : s)
				{
					Node* b = a->c[d - '0'];
					if (b == nullptr)
					{
						b = a->c[d - '0'] = new Node();
					}
					b->numberInPath++;
					a = b;
				}
			}
		}

		void deleteTrie(Node* root)
		{
			Node* a = root;
			for (Node*& b : a->c)
			{
				if (b != nullptr)
					deleteTrie(b);
			}
			delete a;
		}

	public:

		int findKthNumber(int n, int k)
		{
			if (k == 1) return 1;

			buildTrie(n);

			Node* a = root;
			string pathSoFar;
			while (true)
			{
				if (k == 1 && a!= root)
				{
					break;
				}
				else if (a!= root)
				{
					k--;  //count the one number ending exactly at this node.
				}

				int index = 0;
				for (Node* b : a->c)
				{
					if (b == nullptr)
					{
						index++;
						continue;
					}

					if (b->numberInPath < k)
					{
						k = k - b->numberInPath;
						index++;
						continue;
					}
					else if (b->numberInPath >= k)
					{
						pathSoFar.append(to_string(index));
						a = b;
						break;
					}
					else
					{
						throw runtime_error("Error");
					}
				}
			}
			deleteTrie(root);
			return stoi(pathSoFar);
		}
	};




	/*
	*     n:    3   8   7   5   3
	* 
	*  r:     |___|___|___|___|___|___|
	*     0
	*     1             X
	*     2
	*
	* Meaning of value in cell X. Considering the subarray starting from '7', given the situation in the previous 2 digit,
	* How many valid arrangements can I have?
	* Note that each position can by empty, but once a position is empty, the positions behind it must also be empty.
	* 
	* What are the situations in the previous 2 digits.
	* (0):  selected 2 digits are less than "38"
	* (1):  selected 2 digits are "38"
	* (2):  selected 2 digits are greater than "38"
	* 
	* If it's case (0), the last 3 digits can be anything and it won't violate the limit.
	* For example if I have selected "25" as the first 2 digits, now the 3rd digit can be empty, which means
	* the full number is 25 which is a number in the range of [1,n].
	* 
	* If it's case (2), the first 2 digits are already greater than "38", for example I selected "71".
	* It can still produce valid numbers as long as it's not 5-digit long. For example "7162" is a valid number
	* which has the last position as empty.
	* 
	* 'r' must have 6 columns. length of 'n' plus 1.
	* This is because when I query this record, I would say if I have picked 5 digits, is it a valid number.
	* For example, if I have picked "32105", I have to know if it's a valid number, I would check r[5][0].
	* '5' means I'm starting on position #5 (0-indexed), which means I have picked the fisrt 5 positions.
	* '0' means the first 5 I picked is less than 'n'.
	* The value of r[5][0] would be either 0 or 1. If it's 1, it means there is 1 valid arrangement for position #5,
	* which is being empty, therefore it means the first 5 I picked is a valid number.
	* If it's 0, it means it's not. In fact, r[5][0] will always be '1'.
	* r[5][2] will always be '0', because if I have picked 5 digits and they are greater than 'n', that means
	* they are out of bounds, so there is no valid arrangement for position #5.
	* 
	* 
	* When searching for kth number, it's similar to searching something in a binary tree with rank information.
	* I try to put '1' on the first digit, I check how many valid arrangements I can get from the last 4 digits
	* if I do put '1' on the first, let's say I can 127 valid arrangements, if k <= 127, which means the target
	* is within those 127 valid arrangements, so I should put '1' on the first position.
	* If k > 127, which means I should next try to put '2' on the first position. If k = 201, than I should find
	* the 94th number given that the first position has '2'. 94 = 201 - 127.
	*/

	class Solution2
	{
		size_t len;

		int backtrack_collect_record(string& s, vector<vector<int>>& r, size_t index, int previousCompare) // 0: selected numbers already less than 'n'; 1: equal; 2: already greater than 'n'
		{
			if (index == len && (previousCompare == 0 || previousCompare == 1))
			{
				r[index][previousCompare] = 1;
				return 1;
			}
			else if (index == len)
			{
				r[index][previousCompare] = 0;
				return 0;
			}
			if (r[index][previousCompare] != -1)
				return r[index][previousCompare];

			int total = 0;
			int numberLimit = s[index] - '0';

			if (previousCompare == 0)
			{
				total++; // this digit can be empty
				for (int i = 0; i <= 9; i++)
				{
					total += backtrack_collect_record(s, r, index + 1, 0);					
				}
			}
			else if (previousCompare == 1)
			{
				if (index == 0)
				{
					// the first digit can't be empty or 0
					for (int i = 1; i <= 9; i++)
					{
						if (i < numberLimit)
						{
							total += backtrack_collect_record(s, r, index + 1, 0);
						}
						else if (i == numberLimit)
						{
							total += backtrack_collect_record(s, r, index + 1, 1);
						}
						else
						{
							total += backtrack_collect_record(s, r, index + 1, 2);
						}
					}
				}
				else // index > 0
				{
					total++; // can be empty
					for (int i = 0; i <= 9; i++)
					{
						if (i < numberLimit)
						{
							total += backtrack_collect_record(s, r, index + 1, 0);
						}
						else if (i == numberLimit)
						{
							total += backtrack_collect_record(s, r, index + 1, 1);
						}
						else
						{
							total += backtrack_collect_record(s, r, index + 1, 2);
						}
					}
				}
			}
			else // previousCompare == 2
			{
				total++; // can be empty
				for (int i = 0; i <= 9; i++)
				{
					total += backtrack_collect_record(s, r, index + 1, 2);
				}
			}

			r[index][previousCompare] = total;
			return total;
		}

	public:
		int findKthNumber(int n, int k)
		{
			string s = to_string(n);
			len = s.size();
			vector<vector<int>>r = vector<vector<int>>(len+1, vector<int>(3, -1));

			backtrack_collect_record(s, r, 0, 1);

			vector<char> answer;
			int previousCompare = 1;
			size_t index = 0;
			int numberLimit;
			int total;
			int previousTotal;

			while (k > 0)
			{
				numberLimit = s[index] - '0';

				if (index == 0)
				{
					total = previousTotal = 0;
					for (int i = 1; i <= 9; i++)
					{
						previousTotal = total;
						
						if (i < numberLimit)
							total += r[index+1][0];
						else if (i == numberLimit)
							total += r[index+1][1];
						else
							total += r[index+1][2];

						if (total >= k)
						{
							k -= previousTotal;
							answer.push_back('0' + i);

							if (i < numberLimit)
								previousCompare = 0;
							else if (i == numberLimit)
								previousCompare = 1;
							else
								previousCompare = 2;
							
							index++;
							break;
						}
					}
				}
				else
				{
					if (k == 1)
					{
						// if k is 1, that means I just need to make this digit empty, then I'll get the target,
						// since I'm making this digit empty, I don't need to add anything to "answer",
						// so just break out, whatever that are already in "answer" is the answer.
						break;
					}
					else
						k--;  // this digit can be empty, so that counts for 1 possibility. 
								// if k is 2, it will become 1 after this line, then when I check r[index+1][x]
								// in the for loop, I'll get at least 1 possible arrangement, then total > k
								// evaluates to true, I'll put that digit to answer and then finish this iteration
								// then the next iteration comes in, k == 1 evaluates to true, it will break out.

					total = previousTotal = 0;
					for (int i = 0; i <= 9; i++)
					{
						previousTotal = total;

						if (previousCompare == 0)
							total += r[index+1][0];
						else if (previousCompare == 1 && i < numberLimit)
							total += r[index+1][0];
						else if (previousCompare == 1 && i == numberLimit)
							total += r[index+1][1];
						else if (previousCompare == 1 && i > numberLimit)
							total += r[index+1][2];
						else if (previousCompare == 2)
							total += r[index+1][2];

						if (total >= k)
						{
							k -= previousTotal;

							answer.push_back('0' + i);

							if (previousCompare == 1 && i < numberLimit)
								previousCompare = 0;
							else if (previousCompare == 1 && i > numberLimit)
								previousCompare = 2;

							index++;
							break;
						}
					}
				}
			}
			string a = string(answer.begin(), answer.end());
			return stoi(a);
		}
	};



	void Test_0440_Kth_Smallest_in_Lexicographical_Order()
	{
		Solution2 s;

		int n, k;

		while (true)
		{
			cout << "n: ";
			cin >> n;
			if (n == 0) break;
			cout << "k: ";
			cin >> k;
			cout << s.findKthNumber(n, k) << '\n';
		}
	}
}