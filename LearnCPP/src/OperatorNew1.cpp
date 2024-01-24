#include<iostream>

namespace OperatorNew1 {

void Test_OperatorNew1()
{
	int* ptr = (int*)operator new[](sizeof(int) * 10);

	ptr[0] = 29;
	ptr[1] = 39;

	std::cout << "ptr 0 is " << ptr[0] << "\n\n";
	std::cout << "ptr 1 is " << ptr[1] << "\n\n";
	std::cout << "ptr 2 is " << ptr[2] << "\n\n";
}

}