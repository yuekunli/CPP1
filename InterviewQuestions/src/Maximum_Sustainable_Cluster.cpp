#include <vector>

namespace Maximum_Sustainable_Cluster {

    using namespace std;

    /*
    * I have a series of servers, each of which requires a certain voltage of power supply.
    * Each server requires a processing voltage and a boot voltage.
    *
    * The servers are arranged in a sequential order.
    * 
    * Find a group of servers (a subarray of the sequential order) that is a sustainable group.
    * 
    * Given the maximum power supply voltage, a group is sustainable if the sum of the processing
    * voltage multiplied by number of servers in the group, with the maximum boot voltage
    * in the group added onto that product, is less than the given maximum power supply.
    * 
    * Use binary search to determine the size of the group.
    * Say there are 20 servers, if I want to try group of 7, try every possible subarray.
    * Can I do better in the second step?
    */

    class Solution
    {
        size_t n;

        bool canFormK(vector<int>& proc, vector<int>& boot, long pMax, size_t k)
        {
            long proSum = 0;
            int maxBoot = 0;
            for (size_t i = 0; i < n; i++)
            {
                if (i < k) // if k == 5, I want to try group of 5, when i = 0, 1, 2, 3, 4, I just accumulate processing voltage
                {
                    proSum += proc[i];
                    maxBoot = max(maxBoot, boot[i]);
                }
                if (i >= k - 1)  // when i == 4, I have added the first 5, I need to check the sustainable condition
                {
                    if (i >= k) // starting from i == 5, I need to drop the left most server, and count in a new server
                    {
                        size_t drop = i - k;
                        proSum -= proc[drop];
                        proSum += proc[i];
                        if (boot[drop] == maxBoot)
                        {
                            maxBoot = boot[drop + 1];
                            for (size_t j = drop + 1; j <= i; j++)
                            {
                                if (maxBoot < boot[j])
                                    maxBoot = boot[j];
                            }
                        }
                        else
                        {
                            maxBoot = max(maxBoot, boot[i]);
                        }
                    }
                    if ((proSum * k + maxBoot) <= pMax)
                        return true;
                }
            }
            return false;
        }

    public:

        size_t findMaximumSustainableClusterSize(vector<int> processingPower, vector<int> bootingPower, long powerMax)
        {
            n = processingPower.size();
            size_t start = 1, end = n;
            while (start < end)
            {
                size_t k = start + (end - start) / 2;
                if (canFormK(processingPower, bootingPower, powerMax, k))
                {
                    start = k + 1;
                }
                else
                {
                    end = k - 1;
                }
            }
            if (start == end)
            {
                if (canFormK(processingPower, bootingPower, powerMax, start))
                {
                    return start;
                }
                else
                {
                    return start - 1;
                }
            }
            else
            {
                return start - 1;
            }
        }
    };
}