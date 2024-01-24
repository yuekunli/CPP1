#include<cstring>
#include<iostream>


/*
* The Amazon-Corretto-Crypto-Provider project has one peculiar piece of code that caught my attention.
* https://github.com/corretto/amazon-corretto-crypto-provider/blob/0fdb5f7194815f95847a4ea49a277b4f4db7de76/csrc/util.h#L70
* The way it overloads operator* doesn't make sense to me.
* It returns the address of an array, yet it declares the return type is a reference to "type".
* type& operator*() { return &buf; }
* But how can such an obvious mistake, if it really is one, go undetected in an open source project for so long?
* Maybe my understanding is wrong?
* 
*/


using namespace std;

namespace OperatorOverload1 {


	static void secureZero(void* ptr, size_t size)
	{
		memset(ptr, 0, size);
	}

	class SecureBuffer1
	{
	public:
		char buf[100];

		SecureBuffer1()
		{
			secureZero(buf, sizeof(buf));
		}

		virtual ~SecureBuffer1()
		{
			zeroize();
		}

		operator char* ()
		{
			return buf;
		}

		char& operator*()
		{
			return *buf;  // If I do this: return &buf,   it doesn't compile.
		}

		char& operator[](size_t idx)
		{
			return buf[idx];
		}

		virtual void zeroize()
		{
			secureZero(buf, sizeof(buf));
		}
	};



	template <typename type, size_t size> class SecureBuffer {
	public:
		type buf[size];

		SecureBuffer() { secureZero(buf, sizeof(buf)); }
		virtual ~SecureBuffer() { zeroize(); }
		operator type* () { return buf; }
		operator const type* () const { return buf; }
		type& operator*() 
		{ 
			return *buf;  // If I keep this like the original open source code on github, return &buf, it doesn't compile.
		}
		const type& operator*() const { return &buf; }
		type& operator[](size_t idx) { return buf[idx]; }
		type& operator[](size_t idx) const { return buf[idx]; }
		virtual void zeroize() { secureZero(buf, sizeof(buf)); }
	};



	void Test_OperatorOverload1()
	{
		SecureBuffer<char, 3> sb1;

		sb1[0] = 'a';
		sb1[1] = 'b';
		sb1[2] = '\0';

		cout << (char*)sb1 << "\n";

		cout << *sb1 << "\n";
	}
}