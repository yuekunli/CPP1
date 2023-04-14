#include "LibHeaders.h"

namespace AssignToPointerRef1 {

void Test_AssignToPointerRef1()
{
	int* p1 = nullptr;

	std::unique_ptr<int> up = std::unique_ptr<int>(p1);  // up is an object


	std::unique_ptr<int>& r1 = up;  // r1 is another name of the object whose original name is up

	r1 = std::make_unique<int>(55);  // change the content of the object whose original name is up, r1 still bound to that object

	int x = 10;
	int* p2 = &x;

	int y = 20;

	int*& r2 = p2;

	r2 = &y;   // change p2's value, r2 is still bound to p2.

	*r2 = 30;  

}



}