#include<iostream>
#include<bitset>

using namespace std;
namespace BinaryDecimalConvert {
	/*
	* Principles:
	* (1). Given hex, octal, binary literal, the exact bits are stored in memory, 2's compliment algorithm decides what value that is
	* (2). Given decimal literal, the value is preserved, 2's compliment algorithm decides what bits to store
	* (3). When converting between types, value must be preserved, 2's compliment algorithm decides that bits to store
	*/
	void Test_BinaryDecimalConvert()
	{
		char a = 0x81;
		// I want 'a' to store the exact bits 0x81, as to what its value is, it depends on what algorithm the OS uses, 
		// in case of modern computer, it's 2's compliment algorithm, that determines the value is -127
		// 
		// this actually produces a C4309 "truncation of const value" warning. 
		// Because numeric literal is treated as an int which is 4-byte wide, suffix 'l', 'll', 'u' can make it long, longlong, unsigned,
		// but there is no suffix that can make it shorter

		unsigned char b = a; // b's value must be equal to a's.
		int c = a; // c's value also must be equal to a's. But what bits to store in 'c'? 2's compliment algorithm determines that.

		std::cout << b << ",  " << c << '\n';

		std::cout << std::bitset<8>(a) << std::endl;
		std::cout << std::bitset<8>(b) << std::endl;
		std::cout << std::bitset<32>(c) << std::endl;
		std::cout << std::bitset<8>(c) << std::endl;

		std::cout << "\n\n\n";

		char a2 = -127;
		b = a2;
		c = a2;

		std::cout << b << ",  " << c << '\n';

		std::cout << std::bitset<8>(a2) << std::endl;
		std::cout << std::bitset<8>(b) << std::endl;

		std::cout << "\n\n\n";

		unsigned char uc_array[3] = { -1, -127, -128 };

		for (int i = 0; i < 3; i++)
		{
			std::cout << std::bitset<8>(uc_array[i]) << std::endl;
		}
	}
}