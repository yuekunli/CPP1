#include "LibHeaders.h"

namespace _1896_Minimum_Cost_Change_Bool_Expression_Value {

	using namespace std;


	class Solution
	{
	public:
		// this function only manages to evaluate the whole expression
		int minOperationsToFlip(string exp)
		{
			int len = exp.size();
			vector<vector<int>>records(len, vector<int>(len,-1));
			stack<int>openParenIndex;
			stack<int>values;
			for (int i = 0; i < len; i++)
			{
				if (exp[i] == '(')
				{
					openParenIndex.push(i);
					values.push(4);
				}
				if (exp[i] == '0')
					values.push(0);
				if (exp[i] == '1')
					values.push(1);
				if (exp[i] == '&')
					values.push(2);
				if (exp[i] == '|')
					values.push(3);

				if (exp[i] == ')')
				{
					int open = openParenIndex.top();
					openParenIndex.pop();
					int a = values.top();
					values.pop();
					int result = a;
					int op = values.top();
					values.pop();
					a = values.top();
					values.pop();
					while (a != 4)
					{
						if (a == 0 || a == 1)
						{
							if (op == 2)
								result = result & a;
							if (op == 3)
								result = result | a;
						}
						else if (a == 2 || a == 3)
						{
							op = a;
						}
						a = values.top();
						values.pop();
					}
					values.push(result);
					records[open][i] = result;
				}
			}

			int result = values.top();
			values.pop();
			int op = 0;
			while (values.size() > 0)
			{
				int a = values.top();
				values.pop();
				if (a == 0 || a == 1)
				{
					if (op == 2)
						result = result & a;
					if (op == 3)
						result = result | a;
				}
				else if (a == 2 || a == 3)
				{
					op = a;
				}
			}
			return result;
		}
	};


	class Solution2
	{

		/*
		* Whenever I see a closing parenthesis, trace backward until an opening parenthesis.
		* Calculate the minimum cost to flip every three objects tuple.
		* A "three objects tuple" is: left_operand   operator   right_operand
		*    1   &   1   |   0
		*            \_______/
		*               this is a tuple, after this tuple is calculated, it becomes 1 operand, and this 1 operand
		*               becomes part of the next tuple.
		* When I see a closing parenthesis, calculate all the tuples to its left until I see an opening parenthesis
		*/
		enum class Type
		{
			Zero = 0,
			One = 1,
			And = 2,
			Or = 3,
			Paren = 4
		};
		struct A
		{
			Type t;
			int minCostToFlip;
		};

		A evaluate(A n1, A n2, A op)
		{
			if (n1.t == Type::Zero && n2.t == Type::Zero && op.t == Type::And)
				return { Type::Zero, min({n1.minCostToFlip, n2.minCostToFlip}) + 1 };
			else if (n1.t == Type::Zero && n2.t == Type::Zero && op.t == Type::Or)
				return { Type::Zero, min({n1.minCostToFlip, n2.minCostToFlip}) };
			else if (n1.t == Type::Zero && n2.t == Type::One && op.t == Type::And)
				return { Type::Zero, 1 };
			else if (n1.t == Type::Zero && n2.t == Type::One && op.t == Type::Or)
				return { Type::One, 1 };
			else if (n1.t == Type::One && n2.t == Type::Zero && op.t == Type::And)
				return { Type::Zero, 1 };
			else if (n1.t == Type::One && n2.t == Type::Zero && op.t == Type::Or)
				return { Type::One, 1 };
			else if (n1.t == Type::One && n2.t == Type::One && op.t == Type::And)
				return { Type::One, min({n1.minCostToFlip, n2.minCostToFlip}) };
			else // (n1.t == Type::One && n2.t == Type::One && op.t == Type::Or)
				return { Type::One, min({n1.minCostToFlip, n2.minCostToFlip}) + 1 };
		}

	public:

		int minOperationsToFlip(string exp)
		{
			stack<A> a;
			int len = exp.size();
			for (int i = 0; i <= len; i++)
			{
				if (i == len || exp[i] == ')') // when i is "len", it's already out of the original string, but I must do one more iteration because the stack can have previously accumulated values
				{
					A rightOperand = a.top(); a.pop();
					if (a.size() == 0)
					{
						// I just got the last object from the stack, it's the overall result of the original expression,
						// this can happen when i is "len" and the origianl expression is not entirely enclosed in a pair of parentheses
						a.push(rightOperand);
						break;
					}
					A logicOp = a.top(); a.pop();  // get logic operator, but this object from the stack can be just an open parenthesis, in case like this: 1 & (1)
					if (logicOp.t == Type::Paren)
					{
						a.push(rightOperand);
						continue;
					}
					else
					{
						// it's not the "(0)" case
						A leftOperand = a.top(); a.pop();
						A result = evaluate(rightOperand, leftOperand, logicOp);
						while (a.size() > 0)
						{
							logicOp = a.top(); a.pop();
							if (logicOp.t == Type::Paren)
							{
								a.push(result);
								break;
							}
							else
							{
								leftOperand = a.top(); a.pop();
								result = evaluate(result, leftOperand, logicOp);
							}
						}
						if (a.size() == 0)
							a.push(result);
					}
				}
				else if (exp[i] == '(')
					a.push({ Type::Paren });
				else if (exp[i] == '0')
					a.push({ Type::Zero, 1 });
				else if (exp[i] == '1')
					a.push({ Type::One, 1 });
				else if (exp[i] == '|')
					a.push({ Type::Or });
				else // (exp[i] == '&')
					a.push({ Type::And });
			}
			return a.top().minCostToFlip;
		}
	};

	void Test_1896_Minimum_Cost_Change_Bool_Expression_Value()
	{
		Solution2 s;
		string s1;

		while (true)
		{
			s1.clear();
			cout << "expression: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			cout << s.minOperationsToFlip(s1) << "\n\n";
		}
	}
}