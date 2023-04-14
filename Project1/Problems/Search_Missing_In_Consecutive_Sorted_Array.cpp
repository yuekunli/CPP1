#include <iostream>

namespace Search_Missing_In_Consecutive_Sorted_Array {

	using namespace std;

    /*
    * Given an array of sorted consecutive positive integers.
    * There may be 1 missing, if there is one missing, return the value of the missing one
    * Return 0 otherwise.
    */

    /*
    *   v   v   v   v   v   x   x   x   x
    *                   |   |
    *                start  end
    *                   i
    *
    *  i's spot has the correct number, start moves to the right, start and end are equal
    *  loop terminates, the answer is the number pointed by start minus 1
    *
    *
    *   v   v   v   v   v   x   x   x   x
    *                   |   |   |
    *                start  |  end
    *                       i
    *
    *  i in the middle, i's spot has the wrong number, end moves to the left, start and end are equal
    *  loop terminates, the answer is the number pointed by start plus 1
    *
    *   v   x   x
    *   |       |
    *  start    end
    *
    *  i in the middle, i has the wrong number, end moves to the left,
    *  loop terminates, start right now is 0, the answer is a[start]+1
    *
    *  likewise, when loop terminates, start can point to the last element
    *
    *                          v   v   x
    *                          |       |
    *                       start      end
    *  i in the middle, i has the correct number, start moves to the right, the answer is a[start]-1
    *
    *  Therefore, every thing is possible when the loop terminates, I must test whether "start" is the
    *  last correct number or the first wront number, and must handle the case where "start" is 0 or size-1
    */

	class Solution1
	{
    public:

        int findMissing(int a[], int size)
        {
            int start = 0;
            int end = size - 1;
            while (start < end)
            {
                int i = start + (end - start) / 2;
                if (a[i] == a[0] + i)
                {
                    start = i + 1;
                }
                else
                {
                    end = i - 1;
                }
            }

            if (start == 0)
            {
                if (a[start] + 1 == a[start + 1])
                    return 0;
                else
                    return a[start] + 1;
            }
            else if (start == size - 1)
            {
                if (a[start - 1] + 1 == a[start])
                    return 0;
                else
                    return a[start] - 1;
            }
            else
            {
                if (a[start - 1] + 1 == a[start] && a[start] + 1 == a[start + 1])
                    return 0;
                else if (a[start - 1] + 1 < a[start])
                    return a[start] - 1;
                else
                    return a[start] + 1;
            }
        }
	};


    void Test_Search_Missing_In_Consecutive_Sorted_Array()
    {
        int a[10] = {};

        Solution1 s1;

        // 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17
        
        // test case 1: 7, 9, 10, 11, 12, 13, 14, 15, 16, 17
        // test case 2: 7, 8, 10, 11, 12, 13, 14, 15, 16, 17
        // ......
        // test case 8: 7, 8, 9, 10, 11, 12, 13, 14, 15, 17

        a[0] = 7;
        a[9] = 17;

        int next;
        for (int i = 1; i <= 9; i++)
        {
            next = 8;
            for (size_t j = 1; j <= 8; j++)
            {
                if (i == j)
                    next++;
                    // the 1st test case: 8 is skipped
                    // the 2nd test case: 9 is skipped
                    // ......
                    // the 9th test case: actuall I already filled 8 to 15 into a[1] to a[8]. The inner loop finishes, so 16 is skipped
                a[j] = next;
                next++;
            }

            cout << i << " : " << s1.findMissing(a, 10) << '\n';
        }

        //special test case: no missing
        next = 7;
        for (size_t j = 0; j < 10; j++, next++)
            a[j] = next;

        cout << "No missing: " << s1.findMissing(a, 10) << '\n';
    }
}