#include <vector>
#include <algorithm>
#include <numeric>

namespace Minimum_Heaviest_Set {

	using namespace std;

    /*
    * A set of packages. Each has a weight.
    * Pick one or a few packages to form a group, so that the total weight of the group
    * is heavier than the total weight of the rest of the packages.
    * The fewer number of packages in the picked group, the better.
    * 
    * The algorithm is easy, just sort the packages by their weight and pick the heaviest one by one
    * until the total weight of the picked group overweights the rest.
    * 
    * There are a few practical things that need attention.
    */

	class Solution1
	{

	public:

        vector<int> minimalHeaviestSet(vector<int> arr)
        {
            size_t n = arr.size();
            sort(arr.begin(), arr.end());

            //long long total = accumulate(arr.begin(), arr.end(), 0);
            // note that this doesn't work because accumulate infers that '0' is of type int, so its return value is of type int,
            // if arr is long and each element is big, the sum will overflow the upper limit of int.
            
            long long total = 0;
            for (size_t i = 0; i < n; i++)
            {
                total += (long long)arr[i];
            }

            long long setWeight = 0;

            //for (size_t i = n - 1; i >= 0; i--) // note that leads to dead loop because size_t is unsigned
                                                    // and I don't really need to check i == 0 case
                                                    // because it's impossible that I have to put *every* package in the selected group
                                                    // in order to have the selected group overweight the rest.
            for (size_t i = n - 1; i >= 1; i--)
            {
                setWeight += arr[i];
                total -= arr[i];
                if (setWeight > total)
                {
                    size_t setSize = n - i;
                    vector<int>answer(setSize);
                    copy(arr.begin() + i, arr.end(), answer.begin());
                    return answer;
                }
            }
            return {};
        }
	};

    class Solution2
    {

    public:

        vector<int> minimalHeaviestSet(vector<int> arr)
        {
            size_t n = arr.size();
            sort(arr.begin(), arr.end());

            // specify "long long" as template parameter, and use long long integer literal "0ll" as argument
            long long total = accumulate<typename vector<int>::iterator, long long>(arr.begin(), arr.end(), 0ll);

            long long setWeight = 0;

            //for (size_t i = n - 1; i >= 0; i--) // ATTENTION! This leads to dead loop because size_t is unsigned
                                                    // and I don't really need to check i == 0 case
                                                    // because it's impossible that I have to put *every* package in the selected group
                                                    // in order to have the selected group overweight the rest.
            for (size_t i = n - 1; i >= 1; i--)
            {
                setWeight += arr[i];
                total -= arr[i];
                if (setWeight > total)
                {
                    size_t setSize = n - i;
                    vector<int>answer(setSize);
                    copy(arr.begin() + i, arr.end(), answer.begin());
                    return answer;
                }
            }
            return {};
        }
    };
}