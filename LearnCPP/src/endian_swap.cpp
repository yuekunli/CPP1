#include <cstdint>
#include <iostream>

using namespace std;

namespace EndianSwap {
	static uint32_t my_ntohl(uint32_t n)
	{
		unsigned char* np = (unsigned char*)&n;

		uint8_t b0 = np[0]; // bracket operator must give the required byte, e.g. [0] must give the first byte in memory
		uint8_t b1 = np[1];
		uint8_t b2 = np[2];
		uint8_t b3 = np[3];

		uint32_t c0 = (uint32_t)b0; // convert 1 byte to 4 byte, system's endianness decides where to put the 1 byte within the 4 byte space. The ultimate value of the variable must be preserved
		uint32_t c1 = (uint32_t)b1;
		uint32_t c2 = (uint32_t)b2;
		uint32_t c3 = (uint32_t)b3;

		uint32_t a0 = c0 << 24; // shift operator is logic, if I left shift 0000000000001, the value must increase, but system's endianness decides whether to actually left shift or right shift
		uint32_t a1 = c1 << 16;
		uint32_t a2 = c2 << 8;
		uint32_t a3 = c3;

		return a0 | a1 | a2 | a3;


		//return ((uint32_t)np[0] << 24) | ((uint32_t)np[1] << 16) | ((uint32_t)np[2] << 8) | ((uint32_t)np[3]);
	}

	void Test_EndianSwap()
	{
		uint32_t n = 274'742'287;
		uint32_t h = my_ntohl(n);
		cout << "n: " << n << ", h: " << h << endl;
	}
}