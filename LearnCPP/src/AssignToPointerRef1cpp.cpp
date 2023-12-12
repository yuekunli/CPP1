#include<memory>
#include<iostream>
using namespace std;

namespace AssignToPointerRef1 {

	void Test_AssignToPointerRef1()
	{
		int a = 22;
		int* p1 = &a;

		std::unique_ptr<int> up = std::unique_ptr<int>(p1);  // up is an object

		std::unique_ptr<int>& r1 = up;  // r1 is another name of the object whose original name is up

		cout << "p1: " << *p1 << ", r1: " << *r1 << endl;

		r1 = std::make_unique<int>(55);  // change the content of the object whose original name is up, r1 still bound to that object
		// operator= will call reset and release first

		cout << "p1: " << *p1 << ", r1: " << *r1 << endl;


		int x = 10;

		int* p2 = &x;

		int y = 20;

		int*& r2 = p2;

		r2 = &y;   // change p2's value, r2 is still bound to p2.

		cout << "x: " << x << ", p2: " << *p2 << ", y: " << y << ", r2: " << *r2 << endl;

		*r2 = 30;  

		cout << "x: " << x << ", p2: " << *p2 << ", y: " << y << ", r2: " << *r2 << endl;
	}
}