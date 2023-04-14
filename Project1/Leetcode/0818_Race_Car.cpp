#include<iostream>
#include<string>
#include<sstream>
#include<unordered_set>
#include<deque>


namespace _0818_Race_Car {

	using namespace std;

	class Solution
	{
		/*
		* This solution is a breadth-first-search, it can work, but it adds both
		* possible next states for every state I get from the queue.
		* A state is defined by "position" and "speed". A state has two children.
		* Essentially giving either 'A' or 'R' instruction.
		* If the car is already past the target and it has a positive speed,
		* keeping going the same direction won't get the car nearer the target.
		*/

		struct State
		{
			int pos;
			long long speed;  // speed can go over "signed int" upper limit: 2,147,483,647
			int step;
			string instructions;
			State(int _pos, long long _speed, int _step):pos(_pos), speed(_speed), step(_step){}
			State(int _pos, long long _speed, int _step, string _instructions) :pos(_pos), speed(_speed), step(_step), instructions(_instructions) {}
			State() :pos(0), speed(0), step(0) {}
		};
		struct StateHash
		{
			size_t operator()(State const& s) const
			{
				size_t h1 = hash<int>{}(s.pos);
				size_t h2 = hash<long long>{}(s.speed);
				return h1 ^ (h2 << 1);
			}
		};
		struct StateComp
		{
			bool operator()(State const& a, State const& b) const
			{
				return a.pos == b.pos && a.speed == b.speed;
			}
		};
	public:
		int racecar(int target)
		{
			unordered_set<State, StateHash, StateComp>seen;
			deque<State>q;
			q.emplace_back(0, 1, 0);
			seen.emplace(0, 1, 0);
			while (q.size() > 0)
			{
				State a = q.front();
				q.pop_front();

				if (a.pos == target)
				{
					cout << a.instructions << "\n";
					return a.step;
				}

				State b{ (int)((long long)a.pos + a.speed), a.speed * 2, a.step + 1, a.instructions+"A"};
				
				if (seen.count(b) == 0)
				{
					q.push_back(b);
					seen.insert(b);
				}
				b = State{ a.pos, (a.speed > 0 ? -1 : 1), a.step + 1, a.instructions+"R"};
				if (seen.count(b) == 0)
				{
					q.push_back(b);
					seen.insert(b);
				}
			}
			return 0;
		}
	};


	class Solution2 // accepted
	{
		/*
		* If acceleration gets me closer to the target (not going over the target),
		* I just accelerate. If next acceleration can get me over the target, I have a few choices.
		* First choice is of course going over and reverse.
		* Then the next group of choices is to not go over, but reverse and then go toward the
		* opposite direction a few steps or not and then reverse again.
		* The reason I call them a group of choices is because depending on how many reverse steps
		* I take, the results are different.
		* The very first case in this group is to just make a reverse and immediate make another
		* reverse. It essentially "resets" the speed to 1 and the position is not changed.
		* The next case is obviously making a reverse, accelerate once, and make another reverse.
		* The next case is to accelerate twice between two reverses. And so on so forth.
		* But don't take too many backward steps, what is the limit?
		* -- Don't go beyond the last position.
		*/

		int t;
		int solve(int position, int speed, vector<int>& r)
		{
			if (position == t)
				return 0;

			int d = abs(t - position);
			if (speed == 1 && r[d] != -1)
				return r[d];

			if (speed <= d)
			{
				/*
				* What is the theoretical proof that when the next acceleration don't go beyond the target,
				* the optimal solution is always just accelerate and get closer to the target?
				* Why not make a few reverse steps right now so that I don't have to reverse in the future?
				*/
				int result = 1 + solve(position < t ? position + speed : position - speed, speed * 2, r);

				if (speed == 1)
					r[d] = result;
				return result;
			}
			else
			{
				int result = 2 + solve(position < t ? position + speed : position - speed, 1, r);

				int i = 0;
				int reverseSpeed = 1, reverseDist = 0;

				int lastPosition;
				int result2;
				if (position < t)
				{
					lastPosition = position - speed / 2;
					while (true)
					{
						
						if (position - reverseDist <= lastPosition)
							break;
						result2 = 2 + i + solve(position - reverseDist, 1, r);
						result = min(result, result2);
						reverseDist += reverseSpeed;
						reverseSpeed *= 2;
						i++;
					}
				}
				else
				{
					lastPosition = position + speed / 2;
					while (true)
					{
						if (position + reverseDist >= lastPosition)
							break;
						result2 = 2 + i + solve(position + reverseDist, 1, r);
						result = min(result, result2);
						reverseDist += reverseSpeed;
						reverseSpeed *= 2;
						i++;
					}
				}

				return result;
			}
		}

	public:
		int racecar(int target)
		{
			t = target;

			vector<int> record(target+3, -1); // I'm initializing a few values below, so if "target" is '1', I better make sure record[3] is not out of bounds.
												// theoretically I probably just need to create a vector with size "target+1"
			
			/*
			* meaning of record[x]:
			* the minimum steps to get to target when the distance between current position and
			* the target is x and the speed is 1 (toward the target)
			*/

			record[1] = 1;
			record[3] = 2;
			record[2] = 4;

			return solve(0, 1, record);
		}
	};


	void Test_0818_Race_Car()
	{
		Solution s;
		Solution2 s2;
		int target;
		while (true)
		{
			cout << "target: ";
			cin >> target;
			if (target == 0) break;
			cout << s.racecar(target) << "\n";
			cout << s2.racecar(target) << "\n\n";
		}
	}
}