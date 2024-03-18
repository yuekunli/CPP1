
#include<iostream>

using namespace std;

namespace Specialized_Template1 {

    template<typename T>
    class Base
    {
    public:
        int a;
        Base(int _a) : a(_a) {}
        Base() : a(0) {}

        void foo()
        {
            cout << "Base foo" << "\n";
        }

        void bar()
        {
            foo();
        }
    };


    struct A
    {

    };

    struct B
    {

    };


    template<>
    class Base<A>
    {
        void foo()
        {
            std::cout << "A foo" << '\n';
        }
    };

    template<>
    void Base<B>::foo()
    {
        cout << "B foo" << "\n";
    }

    void Test_Specialized_Template1()
    {
        Base<A> objA;
        
        //objA.bar();
        // This doesn't compile, specialized template does not inherit 
        // members from non-specialized template.
        // But watch carefully the difference between Base<A> and Base<B>
        // Base<B> only specializes 1 member function instead of specializing entire template

        

        Base<B> objB(10);
        objB.bar();


        // Note that I can even call the non-default constructor on Base<B>.
        // Base<A> objA(10)   this doesn't compile, error is that Base<A> has no such constructor

    }
}