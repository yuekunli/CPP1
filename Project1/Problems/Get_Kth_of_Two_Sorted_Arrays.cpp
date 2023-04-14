#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>
#include <string>
#include <sstream>

namespace Get_Kth_of_Two_Sorted_Arrays {

	/*
	* Leetcode problem 4
	*/

	using namespace std;

	class Solution
	{
	public:
		virtual int getKth(std::vector<int>& v1, std::vector<int>& v2, size_t target)
		{
			throw std::runtime_error("Not implemented!!");
		}
	};

	class Solution1 : public Solution
	{
	public:
		int getKth(std::vector<int>& v1, std::vector<int>& v2, size_t target)
		{
			std::vector<int>::iterator p1;
			std::vector<int>::iterator p2;

			size_t v1_start = 0;              // start and end of the searching range
			size_t v1_end = v1.size() - 1;

			while (true)
			{
				if (v1_start > v1_end)
				{
					// in a normal binary search, this can happen anywhere
					// but in this application, this only happens at the beginning of v1
					// taking just the 1st of v1 is still too many from v1
					// when both v1_start and v1_end are at the end of the array, we're taking the entire v1
					// that case is dealt with by checking c1 == v1.size()
					return *(v2.begin() + target - 1);
				}

				size_t i1 = (v1_end - v1_start) / 2 + v1_start;  // median index or the lower median index, include this in the first half
				size_t c1 = i1 + 1;  // number of elements I'm taking from v1, either exactly half or 1 fewer than the other half
				p1 = v1.begin() + c1 - 1;

				size_t c2 = target - c1;

				if (c2 <= 0)
				{
					if (c1 == target)
					{
						// for example, get 3rd, the target is small, half of v1 can well exceed the target,
						// so decrease the number taken from v1, decrease all the way down to taking
						// just 3 from v1, but 3rd of v1 may not be the 3rd overall
						// so I can't just return the 3rd of v1, if the first 3 of v1 are all less than v2,
						// then I can return 3rd of v1
						if (*p1 <= v2[0])
						{
							return *p1;
						}
						else
						{
							v1_end = i1 - 1;
							continue;
						}
					}
					else
					{
						// taking too many from v1
						v1_end = i1 - 1;
						continue;
					}
				}

				if (c2 > v2.size())
				{
					// taking too few from v1
					v1_start = i1 + 1;
					continue;
				}


				p2 = v2.begin() + c2 - 1;

				//if (c1 == v1.size() && c2 == v2.size())
				//{
				//	return std::max(*p1, *p2);
				//}  // this special case actually falls in the 3rd scenario in below section


				if ((p1 + 1) < v1.end() && *p2 > *(p1 + 1))  // I took too many from v2
				{
					v1_start = i1 + 1;
				}
				else if ((p2 + 1) < v2.end() && *p1 > *(p2 + 1))  // I took too many from v1
				{
					v1_end = i1 - 1;
				}
				else
				{
					return std::max(*p1, *p2);
				}
			}
		}
	};

	class Solution2 : public Solution
	{
	public:

		int getKth(vector<int>& v1, vector<int>& v2, size_t k)
		{
			size_t start = 0, end = v1.size() - 1;
			size_t i1, c1, i2, c2;

			while (start < end)
			{
				i1 = start + (end - start) / 2;
				c1 = i1 + 1;
				c2 = k - c1;
				i2 = c2 - 1;

				// what are the obvious conditions that c1 and c2 are incorrect?

				if (c1 > k)
				{
					end = i1 - 1;
					continue;
				}

				// c2 will never be greater than k, becuase c2 = k - c1, but c2 can be greater than v2.size()
				if (c2 > v2.size())
				{
					start = i1 + 1;
					continue;
				}

				// what are the conditions that I have found the correct i1, c1, i2, c2?
				// the big premises here is that start < end, which means I'm legitmately
				// taking some from v1, so c1 won't be 0.
				if (c2 == 0 && c1 == k)
				{
					if (v1[i1] <= v2[0])
						return v1[i1];
					else
					{
						end = i1 - 1;
						continue;
					}
				}

				if (c1 > 0 && c2 > 0)
				{
					if (i1 < v1.size() - 1 && i2 == v2.size() - 1 && v2[i2] <= v1[i1 + 1])
						return max(v1[i1], v2[i2]);

					if (i1 == v1.size() - 1 && i2 < v2.size() - 1 && v1[i1] <= v2[i2 + 1])
						return max(v1[i1], v2[i2]);

					if (i1 < v1.size() - 1 && i2 < v2.size() - 1 && v1[i1] <= v2[i2 + 1] && v2[i2] <= v1[i1 + 1])
						return max(v1[i1], v2[i2]);

					if (i1 < v1.size() - 1 && v2[i2] > v1[i1 + 1])
					{
						start = i1 + 1;
						continue;
					}

					if (i2 < v2.size() - 1 && v1[i1] > v2[i2 + 1])
					{
						end = i1 - 1;
						continue;
					}
				}
			}

			c1 = start + 1;
			c2 = k - c1;
			i2 = c2 - 1;

			if (start == 0 && c2 < v2.size() && v1[start] > v2[i2 + 1]) // don't take anything from v1
				return v2[k - 1];
			if (c1 == k)  // don't take anything from v2
				return v1[start];
			else
				return max(v1[start], v2[i2]);
		}
	};

	class Solution3 : public Solution
	{
	public:

		int getKth(vector<int>& v1, vector<int>& v2, size_t k)
		{
			size_t start = 0, end = v1.size() - 1;
			size_t i1, c1, i2, c2;

			while (start <= end) // <== key difference between Solution2 and Solution3 is here
			{
				i1 = start + (end - start) / 2;
				c1 = i1 + 1;
				c2 = k - c1;
				i2 = c2 - 1;

				// what are the obvious conditions that c1 and c2 are incorrect?

				if (c1 > k)
				{
					end = i1 - 1;
					continue;
				}

				// c2 will never be greater than k, becuase c2 = k - c1, but c2 can be greater than v2.size()
				if (c2 > v2.size())
				{
					start = i1 + 1;
					continue;
				}

				// what are the conditions that I have found the correct i1, c1, i2, c2?
				// the big premises here is that start < end, which means I'm legitmately
				// taking some from v1, so c1 won't be 0.
				if (c2 == 0 && c1 == k)
				{
					if (v1[i1] <= v2[0])
						return v1[i1];
					else
					{
						if (i1 == 0)
							break;
						end = i1 - 1;
						continue;
					}
				}

				if (c1 > 0 && c2 > 0)
				{
					if (i1 == v1.size() - 1 && i2 == v2.size() - 1)
						return max(v1[i1], v2[i2]);

					if (i1 < v1.size() - 1 && i2 == v2.size() - 1 && v2[i2] <= v1[i1 + 1])
						return max(v1[i1], v2[i2]);

					if (i1 == v1.size() - 1 && i2 < v2.size() - 1 && v1[i1] <= v2[i2 + 1])
						return max(v1[i1], v2[i2]);

					if (i1 < v1.size() - 1 && i2 < v2.size() - 1 && v1[i1] <= v2[i2 + 1] && v2[i2] <= v1[i1 + 1])
						return max(v1[i1], v2[i2]);

					if (i1 < v1.size() - 1 && v2[i2] > v1[i1 + 1])
					{
						start = i1 + 1;
						continue;
					}

					if (i2 < v2.size() - 1 && v1[i1] > v2[i2 + 1])
					{
						
						// when both start and end are already pointing to 0, which means taking
						// 1 element from v1, but if it's still too many from v1, execution will
						// reach here, moving end to the left, which crosses over start, end should
						// become -1, so that start <= end condition fails, loop terminates. However
						// start and end are size_t which is unsigned, when i1 is 0, i1 - 1 becomes
						// a very large positive number.
						if (i1 == 0)
							break;
						end = i1 - 1;
						continue;
					}
				}
			}

			/*
			*     OK      X      NOT_OK
			*            / \
			*       start   end
			*
			* If 'X' is OK, start moves to the right, loop terminates, start now points to the first that is not OK.
			* If 'X' is not OK, end moves to the left, loop terminates, start now points to the first that is not OK.
			*
			*     OK     X     Y    NOT_OK
			*            |     |
			*         start    end
			* If 'X' is not OK, end moves to the left, loop terminates, start points to the first NOT OK.
			*
			* Therefore, when execution reaches here, start always points to the first that is not OK.
			* So the last OK would be start-1, but if start is already 0, that means even taking 1 from
			* v1 is too many for v1.
			*/

			if (start == 0)
			{
				// I must take none from v1
				return v2[k - 1];
			}
			else
			{
				i1 = start - 1;  // start - 1 is the last OK
				c1 = i1 + 1;
				c2 = k - c1;
				i2 = c2 - 1;
				if (c1 == k)  // don't take anything from v2
					return v1[i1];
				else
					return max(v1[i1], v2[i2]);
			}
		}
	};

	class AutoTest
	{
		std::random_device rd;
		std::mt19937 mt;
		std::uniform_int_distribution<> uid;
		int const maxArraySize = 50;

	public:
		Solution* solutions[3];
		int numberOfTests{ 0 };

		AutoTest() : rd{}, mt{ rd() }, uid{ 1,30 } //, solutions{ Solution1{}, Solution2{}, Solution3{} }
		{
			solutions[0] = new Solution1{};
			solutions[1] = new Solution2{};
			solutions[2] = new Solution3{};
		}
		~AutoTest()
		{
			for (size_t i = 0; i < 3; i++)
				delete solutions[i];
		}

		void fillVectors(std::vector<int>& v1, std::vector<int>& v2, std::vector<int>& merged)
		{
			std::generate(v1.begin(), v1.end(), std::bind(uid, std::ref(mt)));
			std::generate(v2.begin(), v2.end(), std::bind(uid, std::ref(mt)));

			std::sort(v1.begin(), v1.end());
			std::sort(v2.begin(), v2.end());

			std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), merged.begin());
		}

		void determineArraySizeAndTarget(size_t& len1, size_t& len2, size_t& target)
		{
			do
			{
				len1 = mt() % maxArraySize;
			} while (len1 == 0);

			do
			{
				len2 = mt() % maxArraySize;
			} while (len2 == 0);

			target = mt() % (len1 + len2) + 1;
		}

		void checkAndPrint(int& guaranteed, int& under_test, std::vector<int>& v1, std::vector<int>& v2, size_t& target)
		{
			std::cout << std::left << std::setw(7) << guaranteed << std::left << std::setw(7) << under_test << '\n';

			if (guaranteed != under_test)
			{
				std::cout << "v1: ";
				std::copy(v1.begin(), v1.end(), std::ostream_iterator<int>(std::cout, " "));
				std::cout << '\n';
				std::cout << "v2: ";
				std::copy(v2.begin(), v2.end(), std::ostream_iterator<int>(std::cout, " "));
				std::cout << '\n';
				std::cout << "target: " << target << "\n";
				throw std::runtime_error("ERROR!!");
			}
		}

		void runOneTest(int solutionID)
		{
			size_t len1, len2, target;
			determineArraySizeAndTarget(len1, len2, target);
			std::vector<int> v1(len1);
			std::vector<int> v2(len2);
			std::vector<int> merged(len1 + len2);
			fillVectors(v1, v2, merged);
			int guaranteed = merged[target - 1];
			int under_test = solutions[solutionID]->getKth(v1, v2, target);

			checkAndPrint(guaranteed, under_test, v1, v2, target);
		}

		void run(int solutionID)
		{
			for (int i = 0; i < numberOfTests; i++)
				runOneTest(solutionID);
		}

		void runEdgeCaseOnce(int solutionID)
		{
			size_t len1, len2, target;
			determineArraySizeAndTarget(len1, len2, target);
			std::vector<int> v1(len1);
			std::vector<int> v2(len2);
			std::vector<int> merged(len1 + len2);
			fillVectors(v1, v2, merged);
		
			//reset target to 1
			target = 1;
			int guaranteed = merged[target - 1];
			int under_test = solutions[solutionID]->getKth(v1, v2, target);

			checkAndPrint(guaranteed, under_test, v1, v2, target);


			//reset target to the maximum
			target = v1.size() + v2.size();
			guaranteed = merged[target - 1];
			under_test = solutions[solutionID]->getKth(v1, v2, target);

			checkAndPrint(guaranteed, under_test, v1, v2, target);
		}

		void runEdgeCases(int solutionID)
		{
			for (int i = 0; i < numberOfTests; i++)
				runEdgeCaseOnce(solutionID);
		}
	};

	void Test_Get_Kth_of_Two_Sorted_Arrays()
	{
		AutoTest ats{};
		int input;
		while (true)
		{
			std::cout << "1: manual test;  2: automated tests;  3: edge cases;   4: exit  > ";
			std::cin >> input;

			if (input == 4) break;

			if (input == 2)
			{
				std::cout << "number of tests: ";
				std::cin >> input;
				int id;
				cout << "solution: ";
				cin >> id;
				ats.numberOfTests = input;
				ats.run(id);
			}

			if (input == 3)
			{
				std::cout << "number of tests: ";
				std::cin >> input;
				int id;
				cout << "solution: ";
				cin >> id;
				ats.numberOfTests = input;
				ats.runEdgeCases(id);
			}

			if (input == 1)
			{
				/*
				* 3,6,7,12,13 
				* 5,10,15,20,25 
				* get 8th, answer should be 15
				*/
				std::vector<int> v1;
				std::vector<int> v2;
				
				std::string s;

				cin.ignore();

				std::cout << "v1: ";
				getline(std::cin, s);
				std::stringstream ss(s);
				std::copy(std::istream_iterator<int>(ss), std::istream_iterator<int>(), std::back_inserter(v1));

				s.clear();

				std::cout << "v2: ";
				getline(std::cin, s);
				std::stringstream ss2(s);
				std::copy(std::istream_iterator<int>(ss2), std::istream_iterator<int>(), std::back_inserter(v2));

				int target;
				std::cout << "target: ";
				std::cin >> target;
				int id;
				cout << "solution: ";
				cin >> id;
				std::cout << ats.solutions[id]->getKth(v1, v2, target) << '\n';
			}
		}
	}
}
