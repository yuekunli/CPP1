#include<iostream>
#include<iomanip>
#include<random>
#include<vector>
#include<algorithm>
#include<functional>  // std::bind

namespace Merge_Two_Sorted_Arrays {

	using namespace std;

	int* merge(int a[], size_t len1, int b[], size_t len2)
	{
		if (a == nullptr && b != nullptr)
			return b;
		if (a != nullptr && b == nullptr)
			return a;
		if (a == nullptr && b == nullptr)
			return nullptr;

		int* c = new int[len1+len2];

		size_t i = 0, j = 0, k = 0;
		while (i < len1 && j < len2)
		{
			if (a[i] <= b[j])
			{
				c[k] = a[i];
				i++;
			}
			else
			{
				c[k] = b[j];
				j++;
			}
			k++;
		}
		while (i < len1)
		{
			c[k] = a[i];
			i++;
			k++;
		}
		while (j < len2)
		{
			c[k] = b[j];
			j++;
			k++;
		}
		return c;
	}


	void Test_Merge_Two_Sorted_Arrays()
	{
		int a[] = { 1,4,5,8 };
		int b[] = { 3,7 };

		int* c = merge(a, sizeof(a)/sizeof(a[0]), b, sizeof(b)/sizeof(b[0]));

		size_t len = sizeof(c);
		size_t len1 = sizeof(a);
		size_t len2 = sizeof(b);

		cout << boolalpha << (len == len1 + len2) << '\n';

		for (size_t i = 0; i < 6; i++)
		{
			cout << c[i] << " ";
		}
		cout << "\n";
		delete[] c;
		int k;
		cin >> k;
	}
}

namespace Merge_Sort_Iterative {


	template<typename T>
	void merge(T* a, T* b, size_t start, size_t mid, size_t end)
	{
		size_t i = start, j = mid + 1, k = start;
		while (i <= mid && j <= end)
		{
			if (a[i] <= a[j])
			{
				b[k] = a[i];
				i++;
			}
			else
			{
				b[k] = a[j];
				j++;
			}
			k++;
		}
		while (i <= mid)
		{
			b[k] = a[i];
			i++; k++;
		}
		while (j <= end)
		{
			b[k] = a[j];
			j++;
			k++;
		}
		
		for (i = start; i <= end; i++)
			a[i] = b[i];
		
	}

	template<typename T>
	void merge_sort(T* a, size_t len)
	{
		if (len == 1)
			return;

		T* b = new T[len];

		size_t portion_len = 2; size_t tail_len = 0;
		while(len / portion_len >= 1)
		{
			size_t start = 0, end = start + portion_len - 1;
			while (end < len)
			{
				merge<T>(a, b, start, start + portion_len/2 - 1, end);
				start += portion_len;
				end += portion_len;
			}

			if (portion_len == 2 && start == len - 1)
			{
				portion_len *= 2;
				tail_len = 1;
				continue;
			}

			/*
			*   0  1  2  3  4
			*            |
			*           start of tail = 5 -2 = len - tail_len
			*/
			if (start < len - tail_len)
			{
				// merge half of a portion and the tail
				merge<T>(a, b, start, start + portion_len/2 - 1, len - 1);
				tail_len = portion_len / 2 + tail_len;
			}

			portion_len *= 2;
		}

		if (tail_len != 0)
		{
			merge(a, b, 0, portion_len/2 - 1, len - 1);
		}

		delete[] b;
		
		return;
	}


	void Test_Merge_Sort_Iterative_auto_test()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> intDist(1, 200);

		for (size_t len = 1; len <= 100; len++)
		{
			std::vector<int>arr(len);
			std::generate(arr.begin(), arr.end(), std::bind(intDist, std::ref(gen)));

			int* a = new int[len];
			int* original_copy = new int[len];
			for (size_t i = 0; i < len; i++)
			{
				a[i] = arr[i];
				original_copy[i] = arr[i];
			}
			std::sort(arr.begin(), arr.end());

			merge_sort<int>(a, len);

			for (size_t i = 0; i < len; i++)
			{
				if (arr[i] != a[i])
				{
					for (size_t i = 0; i < len; i++)
						std::cout << original_copy[i] << "  ";
					std::cout << std::endl;
					throw std::runtime_error("failed");
				}
			}
			
			delete[] a;
			delete[] original_copy;
		}
		std::cout << "all pass" << std::endl;
	}

	
	template<size_t N>
	void Test_Merge_Sort_Iterative_specific_case()
	{
		int a[N] = { 137, 96, 36 };
		merge_sort<int>(a, N);
		for (size_t i = 0; i < N; i++)
			std::cout << a[i] << "  ";

		std::cout << std::endl;
	}

	void Test_Merge_Sort_Iterative()
	{
		Test_Merge_Sort_Iterative_auto_test();
		//Test_Merge_Sort_Iterative_specific_case<3>();
	}
}


namespace Merge_In_Place {

	using namespace std;

	void test()
	{
		vector<int>a{ 1,2,3,4,5,6,7 };
		rotate(a.begin(), a.begin() + 2, a.end());
		for (size_t i = 0; i < 7; i++)
			cout << a[i] << "  ";
		cout << endl;
	}


	/*
	* a: x  x  x  x  x  x  x  x  x  x  x  x  x  x
	*    |        |        |     |              |
	*    start    |       mid    |              end
	*             |              |
	*             m1             m2
	* 
	* [start, mid] are sorted
	* (mid, end] are sorted
	* find the middle between start and mid, call it m1.
	* find the insertion point of m1 between mid and end, call it m2.
	*/
	template<typename T>
	void merge_in_place(vector<T>&a, size_t start, size_t mid, size_t end)
	{
		if (end - start == 0)
			return;

		if (end - start == 1)
		{
			if (a[start] <= a[end])
				return;
			swap(a[start], a[end]);
			return;
		}

		size_t m1 = start + (mid - start) / 2;

		// binary search m1 in (mid, end]

		size_t lo = mid + 1, hi = end, m2 = lo + (hi - lo) / 2;
		
		if (a[m1] <= a[mid + 1])
		{
			/*
			* a b c d e
			* |
			* start
			* mid
			* m1
			* 
			* start, mid, m1 all point to the same, i.e. the first part has only 1 element
			* I don't have anything in the first part that is after m1.
			* i.e. I don't have anyting in the first part which I need to merge with the 2nd part.
			*/
			if (m1 == mid)
			{
				return;
			}
			/*
			* "m1+1": must use +1, must not use "m1" instead.
			* 15,    17,    23,    28
			* |      |
			* start  mid
			* m1
			*
			* start, m1 both point to the same element.
			* If I use "m1" instead of "m1+1", recursion ends up in dead loop
			*/
			merge_in_place(a, m1+1, mid, end);
			return;
		}

		if (a[end] <= a[m1])
		{
			rotate(a.begin() + m1, a.begin() + mid + 1, a.begin() + end + 1);

			/*
			*   0 1 2 3 4 5 6 7 8 9 10 11
			*   |     |     |          |
			* start   m1   mid         end
			* 
			* 
			*   0 1 2 7 8 9 10 11 3 4 5 6
			*       |           |
			*      2 = 3-1      the index of this is 7 = start + m1 + (end - mid) - 1    
			* 
			*/

			/*
			* special case: start == mid == m1, the first part has only 1 element,
			* this element is rotated to the end, the job is done.
			*/
			if (m1 == start)
				return;

			merge_in_place(a, start, m1 - 1, m1-1 + (end - mid));
			return;
		}
		
		
		while (lo < hi)
		{
			if (a[m2] <= a[m1] && a[m1] <= a[m2 + 1])
				break;
			else if (a[m1] < a[m2])
			{
				hi = m2 - 1;
			}
			else // a[m2] < a[m1]
			{
				lo = m2 + 1;
			}
			m2 = lo + (hi - lo) / 2;
		}

		rotate(a.begin() + m1, a.begin() + mid + 1, a.begin() + m2 + 1);

		/*
		*  a b c d     e f g h i   j k l m n   p q r s t
		*  |           |       |           |           |
		*  start       m1     mid          m2          end
		*  0           4       8           13          18
		* 
		*  a b c d   j k l m n   e f  g  h  i   p  q  r  s  t
		*  0 1 2 3   4 5 6 7 8   9 10 11 12 13  14 15 16 17 18
		* 
		*/

		/*
		* handle special case where first part or second part has only 1 element.
		* 
		* if start, mid, m1 all point to the same element, that element will be placed in between 'n' and 'p'
		* after rotation. The first part, after rotation, only has [j,n], so I don't need to run another
		* merge_in_place for such first part.
		*/
		
		size_t first_part_end = (m1 - 1) + (m2 - mid);

		if (start < m1)
		{
			merge_in_place(a, start, m1 - 1, first_part_end);
		}

		/*
		* if mid+1 == m2 == end, this is actually the a[m1] greater than or less than the entire 2nd part case,
		* which should've been handled before.
		*/
		merge_in_place(a, first_part_end+1, first_part_end+(mid-m1)+1, end);
	}


	void auto_test()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> intDist(1, 200);

		for (size_t len = 1; len <= 100; len++)
		{
			std::vector<int>arr(len);
			std::generate(arr.begin(), arr.end(), std::bind(intDist, std::ref(gen)));
			std::vector<int>brr(arr);
			//std::vector<int>crr(arr);
			

			size_t mid = (len-1) / 2;
			std::sort(arr.begin(), arr.begin()+mid+1);
			std::sort(arr.begin() + mid + 1, arr.end());

			for (size_t i = 0; i < len; i++)
				std::cout << arr[i] << ", ";
			std::cout << std::endl;


			merge_in_place<int>(arr, 0, mid, len - 1);
			sort(brr.begin(), brr.end());

			
			for (size_t i = 0; i < len; i++)
			{
				if (arr[i] != brr[i])
				{

					throw std::runtime_error("error");
				}
			}
		}
		std::cout << "all pass" << std::endl;
	}

	void Test_Merge_In_Place()
	{
		//vector<int> a{ 10, 20, 30, 40, 50, 60, 13, 15, 22, 43, 47, 66 };
		//merge_in_place(a, 0, 5, 11);
		
		//vector<int> a{ 10, 20, 15, 17 };
		//merge_in_place(a, 0, 1, 3);

		//vector<int> a{ 30, 64, 99, 50, 69, 165 };
		//merge_in_place(a, 0, 2, 5);

		//vector<int> a{ 30, 44, 86, 170, 184, 27, 117, 150, 158, 164 };
		//merge_in_place(a, 0, 4, 9);

		auto_test();
	}
}