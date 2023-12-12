#include "LibHeaders.h"

namespace AggregatedSmallExamples1 {

/*-----------------------------------------------------------------------------------*/
std::string itoa(int n, int base)
{
	std::string buffer;
	std::div_t dv {};
	dv.quot = n;

	do
	{
		dv = std::div(dv.quot, base);
		buffer += "0123456789abcdef"[std::abs(dv.rem)];
	} while (dv.quot);

	if (n < 0)
	{
		buffer += '-';
	}

	return { buffer.rbegin(), buffer.rend() };  // reverse begin and reverse end
	                                            // the return type of this function is std::string
	                                            // this return statement constructs a std::string object by using braced-init-list
	                                            // one of the constructors of std::basic_string is to use two iterators
	//return buffer;
}


void Test_std_div()
{
	std::cout << itoa(12345, 10) << std::endl
		<< itoa(-12345, 10) << std::endl
		<< itoa(42, 2) << std::endl
		<< itoa(65535, 16) << std::endl;
}

/*-----------------------------------------------------------------------------------*/

void Test_sizet()
{
	std::array<std::size_t, 10> a;
	for (std::size_t i = 0; i != a.size(); ++i)
	{
		a[i] = i;
	}
	for (std::size_t i = a.size() - 1; i < a.size(); --i)
		// dead loop if I use i < 0 here, because size_t is unsigned, it will never become negative.
	{
		std::cout << a[i] << " ";
	}
	std::cout << "\n";
}

/*-----------------------------------------------------------------------------------*/
class S1;
void Test_Macro_NULL()
{
	int* p = NULL;
	int* p2 = static_cast<std::nullptr_t>(NULL);

	void(*f)(int) = NULL;

	int S1::* mp = NULL;    // a pointer to a member of class S1 and this member must be of integer type

	void (S1:: * mfp)(int) = NULL;

	if (std::is_same_v<decltype(NULL), std::nullptr_t>)
	{
		std::cout << "NULL implemented with type std::nullptr" << std::endl;
	}
	else
	{
		std::cout << "NULL implemented using an integral type" << std::endl;
	}
}

/*-----------------------------------------------------------------------------------*/

void Test_string_push_back()
{
	std::string s;
	s.reserve(5);

	s.push_back('a');

	for (auto t : s)
	{
		std::cout << t << "  ";
	}

	std::cout << std::endl;


	s.push_back('b');

	for (auto t : s)
	{
		std::cout << t << "  ";
	}

	std::cout << std::endl;


	std::string s2;

	// without reserve

	s2.push_back('c');

	s2.push_back('d');

	std::cout << s2 << std::endl;

}

/*-----------------------------------------------------------------------------------*/

void Test_char_unsigned_char()
{
	// en.cppreference.com/w/cpp/language/types
	// "
	// For every value of type unsigned char in range [0, 255], coverting the value to char and then back to unsigned char produces the original value.
	// "
	// 
	// This doesn't mean that char variable in the middle has the same value.
	// This below case 'a' and 'c' are unsigned chars and 'b' is char.
	// 'a' and 'c' are guaranteed to have the same value, there is no guarantee to make 'b' have the same value.
	// In fact, depending on the compiler, if the signedness of char is signed, 'b' must be different than 'a' and 'c'.


	unsigned char a = 199;

	std::cout << "a:  " << (int) a << std::endl;

	char b = a;

	std::cout << "b:  " << (int) b << std::endl;

	unsigned char c = b;

	std::cout << "c:  " << (int) c << std::endl;
}

/*-----------------------------------------------------------------------------------*/

typedef int (*FP1)(int, int);

int func1(int a, int b)
{
	return a + b;
}

int func2(int a, int b)
{
	return a - b;
}

void Test_typedef_function_pointer()
{
	FP1  fp_1 = func1;
	FP1  fp_2 = &func2;   // either use the name of the function or take its address is OK

	int a = 10, b = 3;

	std::cout << fp_1(a, b) << std::endl;
	std::cout << (*fp_2)(a, b) << std::endl;  // either user the name of the pointer or de-reference it is OK

}


/*-----------------------------------------------------------------------------------*/

struct S2
{

public:
	int a;
	S2(int _a) : a(_a)
	{
		std::cout << a << "     value constructor" << std::endl;
	}
	S2(S2& s)
	{
		a = s.a;
		std::cout << a << "     non-const copy construcotr" << std::endl;
	}
	S2(S2 const& s)
	{
		a = s.a;
		std::cout << a << "     const copy constructor" << std::endl;
	}
	S2(S2&& s) noexcept
	{
		a = s.a;
		std::cout << a << "     move constructor" << std::endl;
	}
};

void Test_struct_init_in_vector_braced_init_list()
{
	std::vector<S2> v = { {1},{2},{3} };  // this will construct S2 twice for each object, value constructor then const copy constructor


	std::vector<S2> v2;   // if I don't reserve memory, at some point the container will have to re-size and move objects using move constructor
	v2.emplace_back(21);  // If I only talk about emplace_back, it only calls value constructor
	v2.emplace_back(22);
	v2.emplace_back(23);


	std::vector<S2> v3;
	v3.reserve(3);           // this makes sure the container won't need re-size
	v3.emplace_back(31);     // emplace_back only calls value constructor
	v3.emplace_back(32);
	v3.emplace_back(33);

	std::vector<S2> v4(5, {40});

}


/*-----------------------------------------------------------------------------------*/



#if 0
void copyt(int n)
{
	typedef int T[n];  // does not compile

	n += 1;

	T a;
	int b[n];  // does not compile
	for (int i = 1; i < n; i++)
	{
		a[i - 1] = b[i];
	}
}
#endif



/*-----------------------------------------------------------------------------------*/

struct S3
{
public:
	int a;
	S3(int _a) :a(_a)
	{
	}
};


static struct
{
public:
	int b;
} S4;

typedef S3 ts3;

typedef struct
{
public:
	int c;
} ts4;

struct S5
{
public:
	int a;
	struct S5* p;   // it's also OK to not have the keyword "struct" at the beginning
	S5* q;
};


void Test_struct_declare()
{
	S3 obj_S3 { 20 };  // S3 is a typename
	obj_S3.a = 1;

	struct S3 obj_ss3{30};  // it's OK to have or not have the keyword "struct" at the beginning.
	obj_ss3.a = 5;

	S4.b = 100;  // S4 is the name of an object

	ts3 obj_ts3 { 40 };
	obj_ts3.a = 2;

	ts4 obj_ts4;
	obj_ts4.c = 15;

	S5 obj_S5;
	obj_S5.a = 60;
	obj_S5.p = nullptr;
	obj_S5.q = &obj_S5;

}





/*-----------------------------------------------------------------------------------*/

/* this requirement is to only use operator<, must not use operator==*/
static bool implement_include()
{
	auto const v1 = { 1,2,3,4,5,6,7,8,9 }, v2 = { 4,6,10 };

	auto i2 = v2.begin();
	for (auto i1 = v1.begin(); i1 != v1.end() && i2!=v2.end(); i1++)
	{
		if (*i1 < *i2)
		{
			continue;
		}
		else
		{
			if (!(*i1 < *i2) && !(*i2 < *i1))  // i1 is not less than i2, i2 is not less than i1 either, they must be equal
			{
				i2++;
				continue;  // this continue will increase i1. so if v1 = {1,2,3} and v2 = {2,2}, i1 will point to 3 and i2 will point to the 2nd 2, this is deemed as "v1 not including v2"
			}
			else if (*i2 < *i1)  // i1 has jumped over i2, i2 must be in between two adjacent numbers in v1
			{
				return false;
			}
		}
	}
	return i2 == v2.end() ? true:  false;
}

void Test_implement_include()
{
	bool ret = implement_include();
	std::cout << std::boolalpha << ret << '\n';
}


/*-----------------------------------------------------------------------------------*/

void Test_init_list_variable()
{
	int n = 10;

	std::vector<int> v { n }; // use a variable instead of const or literal in side list-initialization

	std::vector<int> v1 { 1,2,3,n,4,5,6 };

	for (auto const& i : v)
		std::cout << i << ' ';
	std::cout << '\n';

	for (auto const& i : v1)
		std::cout << i << ' ';
	std::cout << '\n';
}




/*-----------------------------------------------------------------------------------*/


template <class Os, class V>
Os& operator<=(Os& os, V const& v)
{
	os << "####   ";
	for (auto& x : v)
		os << x << ' ';
	return os << '\n';
}

void Test_operator_overload()
{
	std::vector<int> v1 = { 1,6,3,9,6,3,10 };

	std::cout <= v1;

	std::vector<int> v12 = { 12,62,32,92,62,32,102 };

	std::cout <= v12;


	auto const v2 = { 2,9,19,78,45 };

	std::cout <= v2;

	auto const v3 = { 4,90,2,82,56 };

	std::cout <= v3;

	//auto const v4 { 5,3,10 };  //must have the '=' 
	//std::cout <= v4;


	//std::cout <= v3 << " ----- Hello World\n";  // binary '<<': 'const std::initialize_list<int>' does not define this operator or a conversion to a type acceptable to the predefined operator
	// why is this an error?  "std::cout <= v3" should be evaluated first and return a reference to the ostream. Then the next part is just "ostream << string literal". what is wrong with that?

	(std::cout <= v3) << " --- Hello World\n"; // this works, must enclose the first part with parentheses, so that it's evaluated first as a whole

}

/*-----------------------------------------------------------------------------------*/

void Test_setw()
{
	std::cout << "[" << 42 << "]\n"
		<< "[" << std::setw(6) << 42 << "]\n"
		<< "[" << 89 << std::setw(6) << 12 << 34 << "]\n";

	std::istringstream is { "12345678 9" };
	char sp[9] = "aaaaaaaa";  // sp is an array, its pre-specified size has be to at least 9, otherwise code won't compile

	char sp2[8] = { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' };

	is >> std::setw(6) >> sp;   // it takes 5 from the input stream and sets the 6th character to '\0'
	std::cout << std::setw(10) << is.str() << "\n"
		<< std::setw(10) << sp << "\n";

	//is >> std::noskipws >> sp2;    // the space in an input stream affects how it copies characters to a string or an array, noskipws or skipws don't matter
	//std::cout << std::setw(10) << sp2 << '\n';

	std::string s2;
	std::getline(is, s2);
	std::cout << std::setw(10) << s2 << '\n';
}

void Test_setw2()
{
	std::cout << std::setw(5) << "123456789"<<'\n';   
	           // it will print the full string is the set width is shorter than the string
	           // how to truncate? -- I can't truncate by manipulating the ostream
	           // I have to truncate the string directly then give the ostream the already truncated string.

	std::cout << std::setfill('*') << std::left << std::setw(15) << "123456789" << '\n';
	std::cout << std::setfill('*') << std::right<< std::setw(15) << "123456789" << '\n';
}


/*-----------------------------------------------------------------------------------*/


namespace StreamIterWithManipulator {
// only overload the operator<<
std::ostream& operator << (std::ostream& os, int n)
{
	return os << std::right << std::setfill('!') << std::setw(8) << std::to_string(n);
}

void Test_ostream_iter_with_manipulator()
{
	std::vector<int> v(10, 2);
	std::partial_sum(v.begin(), v.end(), std::ostream_iterator<int>(std::cout<<std::right<<std::setw(8), "")); // this won't call the overloaded operator<<
	                                                                /*only works for the first print*/
}

}

namespace StreamIterWithManipulator2 {
// wrap the value in a custom-defined struct/class, then overload operator<< for that struct/class
struct S1
{
	int a;
	S1(int _a) :a(_a)
	{
	}
	friend std::ostream& operator<<(std::ostream& os, const S1& s)   // with "friend" keyword, this function is not a member of this class, when it's called, system won't pass in "this" pointer as the first argument
	{
		return os << std::right << std::setfill('0') << std::setw(8) << s.a << '\n';
	}
};


void Test_ostream_iter_with_manipulator2()
{
	std::vector<S1>v { {2},{2},{2},{2},{2},{2},{2},{2},{2},{2} };
	std::partial_sum(v.begin(), v.end(), std::ostream_iterator<S1>(std::cout, ""), [](S1 const& a, S1 const& b)
	{
		return std::move(S1 { a.a + b.a });
	});
}

}

namespace StreamIterWithManipulator3 {
// augment ostream_iterator, keep using std::cout as the underlying ostream object
struct YOStreamIter : std::ostream_iterator<int>
{
	std::ostream& _os;


	YOStreamIter(std::ostream& os): std::ostream_iterator<int>(os, ""), _os(os)
	{

	}

    YOStreamIter& operator=(const int& n)
	{
		_os << "    " << n << '\n';
		return *this;
	}

	YOStreamIter& operator=(YOStreamIter& a)  // these seemingly trivial functions are necessary, they are all called by partial_sum,
	{                                         // these implementations keep the object at this child level instead of parent's level.
		return *this;
	}
	YOStreamIter& operator*()
	{
		return *this;
	}
};

void Test_ostream_iter_with_manipulator3()
{
	std::vector<int> v(10, 2);
	std::partial_sum(v.begin(), v.end(), YOStreamIter(std::cout));
}

}


namespace StreamIterWithManipulator4 {
// augment the ostream, keep using the std::ostream_iterator to wrap it
struct YOStream : std::ostream
{
	YOStream():std::ostream(std::cout.rdbuf())
	{
	}
	YOStream& operator<<(const int& n)  // why can't this overload be called?
	{
		std::cout << "     " << n << '\n';
		return *this;
	}
};

void Test_ostream_iter_with_manipulator4()
{
	YOStream yostr1 {};
	std::ostream_iterator<int> yostriter1(yostr1, "");  // this will cast yostr1 to std::basic_ostream, so the overload of operator<< defined in YOStream won't be called

	std::vector<int>v(10, 2);
	std::partial_sum(v.begin(), v.end(), yostriter1);
}
// essentially this approach is incorrect. If I keep using ostream_iterator, I must supply an object of std::basic_ostream, and I can't change an existing class' implementation of a function
// I can use an augmented iterator because partial_sum doesn't demand the exact type of the iterator.
//
// partial_sum   ---needs--->   iterator   ---needs--->   stream
//
// I can either "use standard iterator with customized stream" or "use standard stream with customized iterator"
// partial_sum only requires the iterator to be LegacyOutputIterator but not a specific class type
// This is why only 1 of those 2 approaches can work.
}

/*-----------------------------------------------------------------------------------*/


struct S6
{
	int a { 0 };
	template<typename T>
	void f1(T t)
	{
		std::cout << std::left << std::setw(10) << "S6" << t << "\n";
	}
};

struct S62 : S6
{
	void f1(int n)
	{
		std::cout << std::left << std::setw(10) << "S62" << n << '\n';
	}
};


void Test_override_function()
{
	S62 a;

	a.f1(10);

	((S6) a).f1(40);
}

/*-----------------------------------------------------------------------------------*/


void Test_execution_policy_overload_condition()
{
	// a lot of library functions have overloads that take an execution policy,
	// but such overload won't participate in resolution unless this evaluation returns true
	// The evaluation tests the "typename" of the execution policy
	// For example, a function takes an argument '5', but 5's typename is "int".
	// An execution policy can be "std::execution::par". This is the name of an object.
	// The name of the class where this object comes from is "std::execution::parallel_policy"

	bool a = std::is_execution_policy_v<std::decay_t<std::execution::parallel_policy>>;
	std::cout << std::boolalpha << a;
}


/*-----------------------------------------------------------------------------------*/

void Test_begin_end_on_primitive_array()
{
	int a[10] = { 1,2,3,4,5,6,7,8,9,10 };
	for (auto i = std::begin(a); i != std::end(a); i++)
		std::cout << *i << ' ';
	std::cout << '\n';

	//std::array<int, 10> b = a;  // this doesn't work
	
}



/*-----------------------------------------------------------------------------------*/

template<class T, class U>
static void addToSet(T& s, U n)  // how do I enforce that type 'U' is the parameter of 'T'?
{
	// for example if 'T' is a std::set<std::string>, how do I make sure 'U' really is a std::string?
	// addToSet(T<U>&s, U n) doesn't work

	s.emplace(n);
}

void Test_template()
{
	std::set<int> s;
	addToSet(s, 10);

	std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, "  "));

	std::cout << '\n';

	std::unordered_set<char> s2;
	addToSet(s2, 'a');
	std::copy(s2.begin(), s2.end(), std::ostream_iterator<char>(std::cout, "  "));

}


/*-----------------------------------------------------------------------------------*/


static void increaseBy1(std::string& s)
{
	for (auto c = s.rbegin(); c != s.rend(); c++)
	{
		if (*c != 'z')
		{
			(*c)++;
			break;
		}
		else
			*c = 'a';
	}
}

void Test_string_iterator_access_each_char()
{
	std::string s { "abzx" };
	
	for (int i = 0; i < 5; i++)
	{
		increaseBy1(s);
		std::cout << s << '\n';
	}
	std::cout << '\n';
}



/*-----------------------------------------------------------------------------------*/


void Test_stringstream1()
{
	std::stringstream ss1;

	int n = 32837;
	float f = 3.9f;

	std::string s1;

	ss1 << n;
	ss1 >> s1;
	std::cout << s1 << '\n';

	ss1.clear();  // reset eof flags and other stuff
	ss1.str("");  // clear whatever was in the buffer

	ss1 << f;
	ss1 >> s1;
	std::cout << s1 << '\n';  

	ss1 >> s1;
	std::cout << s1 << '\n'; 

	std::string s2 = ss1.str();
	std::cout << s2 << '\n';   

	ss1.clear();
	ss1.str("");

	ss1 << "Hello";
	std::cout << ss1.str() << '\n';  

}



/*-----------------------------------------------------------------------------------*/


void hourly_annual_salary()
{
	for (int i = 50; i <= 100; i++)
	{
		std::cout << std::setw(3) << i << "       " << 2080 * i << "\n";
	}
	std::cout << '\n';
}


}