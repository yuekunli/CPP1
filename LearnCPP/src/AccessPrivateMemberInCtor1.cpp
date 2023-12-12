#include "LibHeaders.h"
namespace AccessPrivateMemberInCtor1 {


class A
{
public:
	A() :
		v(0)
	{
		std::cout << "defalut ctor" << std::endl;
	}

	A(int val) :
		v(val)
	{
		std::cout << "int ctor" << std::endl;
	}

	A(const A& a) :
		v(a.v)  // access a's private member, does it work?  --  Yes, it does
	{
		std::cout << "copy ctor" << std::endl;
	}

	A(A&& a) :
		v(std::move(a.v))  
	{
		std::cout << "move ctor" << std::endl;
	}

	A& operator=(const A& a)
	{
		std::cout << "copy =" << std::endl;
		v = a.v;
		return *this;
	}

	A& operator=(A&& a) noexcept
	{
		std::cout << "move =" << std::endl;
		v = std::move(a.v);
		return *this;
	}

	~A() = default;




	void F1(const A& a)
	{
		std::cout << "F1" << std::endl;
		v = a.v * 2;   // this works too??
		// We're inside the class definition, and we're accessing the private member of this class
		// I guess this is OK, even if that means the object on which this function is invoked is 
		// essentially accessing another object's private member.
	}

private:
	int v;
};



void Test_AccessPrivateMemberInCtor1()
{
	A obj1 { 30 };

	A obj2 = obj1;

	A obj3;

	obj3 = obj2;

	A obj4;
	obj4.F1(obj1);  // obj4 is essentially accessing obj1's private member. But this works

}


}