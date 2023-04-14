#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <sstream>

namespace Parse_Integer_String {

    /*
    * The requirement is to parse a string of integers delimited by commas. Discard negative numbers.
    * But the input may not be well formatted.
    * However it's ambiguous as to what type of ill-formated input I can get.
    * A few cases I can think:
    * (1). 1, 3, 4, 5        best format, comma always follow number and no comma after the last number
    * (2). 1, 4  ,  6  ,9 ,  comma not always follow number, even right before next number, and there can be extra comma at the end.
    */

    using namespace std;

    /*
    * Well formatted case, comma follows numbers
    */
    void f1()
    {
        string s;
        vector<int>v;
        while (true)
        {
            s.clear();
            getline(cin, s);
            if (s.size() == 0) break;
            stringstream ss(s);
            v.clear();
            for (int i; ss >> i;)
            {
                if (i >= 0)
                    v.emplace_back(i);
                if (ss.peek() == ',')  // this requires comma must follow number with no space in between
                    ss.ignore();
            }
            copy(v.begin(), v.end(), ostream_iterator<int>(std::cout, " ")); cout << "\n\n";
        }
    }

    /*
    * This is the correct solution
    */
    void f2()
    {
        string s;
        vector<int>v;
        while (true)
        {
            s.clear();
            getline(cin, s);
            if (s.size() == 0) break;
            stringstream ss(s);
            v.clear();
            for (int i; ss >> i;)
            {
                if (i >= 0)
                    v.emplace_back(i);
                while (ss.peek() == ',' || ss.peek() == ' ')
                    ss.ignore();
            }
            copy(v.begin(), v.end(), ostream_iterator<int>(std::cout, " ")); cout << "\n\n";
        }
    }

    /*
    * The intention is to read the comma, and abandon it, but this doesn't work
    * For this input: "1, 2 ,4"
    * read in '1', give it to 'i'
    * read in ',', give it to 's2'
    * read in '2', give it to 'i'
    * read in ',4', give it to 's2'
    * ss >> i returns EOF, for loop terminates, '4' is lost
    */
    void f3()
    {
        string s;
        vector<int>v;
        while (true)
        {
            s.clear();
            getline(cin, s);
            if (s.size() == 0) break;
            stringstream ss(s);
            v.clear();
            for (int i; ss >> i;)
            {
                v.emplace_back(i);
                string s2; 
                ss >> s2; // intention is to read the comma, but if comma is right before next number, this is going to read in the comma and the next number
            }
            copy(v.begin(), v.end(), ostream_iterator<int>(std::cout, " ")); cout << "\n\n";
        }
    }


    /*
    * This is correct
    */
    void f4()
    {
        string s;
        vector<int>v;
        while (true)
        {
            s.clear(); v.clear();
            getline(cin, s);
            if (s.size() == 0) break;

            size_t n = s.size();
            int i = 0;
            int numberStart = -1;
            while (i < n)
            {
                if (s[i] == '-' || (s[i] >= '0' && s[i] <= '9'))
                {
                    if (numberStart == -1)
                        numberStart = i;
                }
                if (i == n-1 || s[i] == ',' || s[i] == ' ')
                {
                    if (numberStart > -1)
                    {
                        int length = i == n-1 ? i - numberStart + 1 : i - numberStart;
                        string numberString = s.substr(numberStart, length);
                        stringstream ss(numberString);
                        int k;
                        ss >> k;
                        if (k >= 0)
                            v.emplace_back(k);
                        numberStart = -1;
                    }
                }
                i++;
            }

            copy(v.begin(), v.end(), ostream_iterator<int>(std::cout, " ")); cout << "\n\n";
        }
    }


    /*
    * The reason why I can use the return value of ss >> i as the loop's condition:
    * Search: std::basic_ios<CharT, Traits>:: operator bool
    * Putting a stream object inside "while" or "for" clause as loop termination condition
    * implicitly applies the bool operator onto the stream object, which checks whether
    * the stream has error or not.
    * If the stream's underlying string has a few letters but I want it to parse some numbers,
    * there will be error.
    */


    void Test_Parse_Integer_String()
    {
        f2();
    }
}