#include<memory>
#include<iostream>
using namespace std;

namespace UniquePtrInitializeAndCopy1 {

	static void wrong_use()
	{
		int a = 22;
		int* p1 = &a;

		std::unique_ptr<int> up = std::unique_ptr<int>(p1);  // up is an object

		std::unique_ptr<int>& r1 = up;  // r1 is another name of the object whose original name is up

		cout << "p1: " << *p1 << ", up: " << *up << ", r1: " << *r1 << endl;

		r1 = std::make_unique<int>(55);  // change the content of the object whose original name is up, r1 still bound to that object
		// operator= will call reset and release first, the deleter is going to be invoked, but p1 doesn't point at something on the head, 'a' is on the stack.
		// delete operator used on a variable on the stack causes an exception.

		cout << "p1: " << *p1 << ", up: " << *up << ", r1: " << *r1 << endl;
	}

	static void right_use()
	{
	
		int* p1 = new int;

		std::unique_ptr<int> up = std::unique_ptr<int>(p1);  // up is an object

		std::unique_ptr<int>& r1 = up;  // r1 is another name of the object whose original name is up

		cout << "p1: " << *p1 << ", up: " << *up << ", r1: " << *r1 << endl;

		r1 = std::make_unique<int>(55);  // change the content of the object whose original name is up, r1 still bound to that object
		// operator= will call reset and release first, the deleter is going to be invoked, but p1 doesn't point at something on the head, 'a' is on the stack.
		// delete operator used on a variable on the stack causes an exception.

		cout << "p1: " << *p1 << ", up: " << *up << ", r1: " << *r1 << endl;
	}

	void Test_UniquePtrInitializeAndCopy1()
	{
		right_use();

		wrong_use();
	}

}