#include<vector>
#include<algorithm>

namespace _1146_Snapshot_Array {

	using namespace std;

	class SnapshotArray
	{
		vector<int>a;
		vector<vector<int>>recordedValues;
		vector<vector<int>>snaps;
		int totalSnaps = 0;
		int n;
		vector<int>changedIndexesBetweenSnaps;
	public:
		SnapshotArray(int length):n(length)
		{
			a.assign(length, 0);
			recordedValues.assign(length, vector<int>());
			snaps.assign(length, vector<int>());
		}

		void set(int index, int val)
		{
			a[index] = val;
			changedIndexesBetweenSnaps.emplace_back(index);
		}

		int snap()
		{
			int newSnapId = totalSnaps;
			totalSnaps++;
			size_t len = changedIndexesBetweenSnaps.size();
			for (int j = 0; j < len; j++)
			{
				int i = changedIndexesBetweenSnaps[j];
				if (recordedValues[i].size()==0 || a[i] != *recordedValues[i].rbegin())
				{
					recordedValues[i].emplace_back(a[i]);
					snaps[i].emplace_back(newSnapId);
				}
			}
			changedIndexesBetweenSnaps.clear();
			return newSnapId;
		}

		int get(int index, int snap_id)
		{
			if (snaps[index].size() == 0)
				return 0;

			auto it = lower_bound(snaps[index].begin(), snaps[index].end(), snap_id);
			if (it == snaps[index].end())
				return *recordedValues[index].rbegin();

			if (it == snaps[index].begin() && *it > snap_id)
				return 0;
			else if (*it > snap_id)
			{
				it = it - 1;
			}
			size_t d = distance(snaps[index].begin(), it);
			return recordedValues[index][d];
		}
	};
}