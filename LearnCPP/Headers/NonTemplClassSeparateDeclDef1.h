#pragma once
namespace NonTemplClassSeparateDeclDef1 {

class A
{
public:
	A(int);
	int size();

private:
	int _val;
};

}
