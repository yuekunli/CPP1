#include<iostream>
#include<iomanip>

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