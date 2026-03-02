/* Given a boolean expression consisting of the symbols 0, 1, &, |, and ^, and a desired boolean
   result value result, implement a function to count the number of ways of parenthesizing the 
   expression such that it evaluates to result
*/

/* 
As in other resurive problems, the key to this problem is to figure out the relationship between 
a problem and its sub-problems.

Suppose int f(expression, result) is a function which returns the count of all valid expressions which
evalutes to result. We want to compute f(1^0|0|1, true) (that is, all ways of parenthesizing the expression
1^0|0|1 such that the expression evaluates to true). Each parenthesized expression must have an outermost
pair of parentheses. So, we can say that:
	f(1^0|0|1, true) = 	f(1 ^ (0|0|1), true) +
				       	f((1^0) | (0|1), true) +
						f((1^0|0) | 1, true)
That is, we can iterate through the expression, treating each operator as the first operator to be 
parenthesized.

Now, how do we compute one of these inner expressions, like f((1^0) | (0|1), true)?
Well, inorder for that expression to evaluate to true, either the left half or the right half must 
evaluate to true. So, the expression breaks down as:
	f((1^0) | (0|1), true) = 	f(1^0, true) * (f0|1, true) +
								f(1^0, false) * (f0|1, true) + 
								f(1^0, true) * (f0|1, false) 
We can implement a similar break down for each of the boolean operators:
	f(exp1 | exp2, true) = 	f(exp1, true) * f(exp2, true) +
							f(exp1, false) * f(exp2, true) +
							f(exp1, true) * f(exp2, false) 
	f(exp1 & exp2, true) = 	f(exp1, true) * f(exp2, true) 
	f(exp1 ^ exp2, true) = 	f(exp1, true) * f(exp2, false) +
							f(exp1, false) * f(exp2, true) 
For the false results, we can perform a very similar operations:
	f(exp1 | exp2, true) = 	f(exp1, false) * f(exp2, false) 
	f(exp1 & exp2, true) = 	f(exp1, false) * f(exp2, false) 
							f(exp1, false) * f(exp2, true) +
							f(exp1, true) * f(exp2, false) 
	f(exp1 ^ exp2, true) = 	f(exp1, true) * f(exp2, true) +
							f(exp1, false) * f(exp2, false) 
*/

#include <iostream>
#include <string>
#include <map>

using namespace std;

int num_call;
// Naive
int f(string exp, bool result, int s, int e) {
	num_call++;
	//cout << exp << " " << result << " " << s << " " << e << endl;
	if(s==e) {
		if(exp[s] == '1' && result) {
			return 1;
		} else if (exp[s]=='0' && !result) {
			return 1;
		}
		return 0;
	}

	int c=0;
	if(result) {	// if result is true
		for(int i=s+1; i<=e; i+=2) {
			char op = exp[i];
			if(op=='&') {
				c+=f(exp, true, s, i-1) * f(exp, true, i+1, e);
			} else if(op=='|') {
				c+=f(exp, true, s, i-1) * f(exp, true, i+1, e);
				c+=f(exp, false, s, i-1) * f(exp, true, i+1, e);
				c+=f(exp, true, s, i-1) * f(exp, false, i+1, e);
			} else if (op=='^') {
				c+=f(exp, true, s, i-1) * f(exp, false, i+1, e);
				c+=f(exp, false, s, i-1) * f(exp, true, i+1, e);
			}
		}
	} else {	// if result is false	
		for(int i=s+1; i<=e; i+=2) {
			char op = exp[i];
			if(op=='&') {
				c+=f(exp, false, s, i-1) * f(exp, false, i+1, e);
				c+=f(exp, false, s, i-1) * f(exp, true, i+1, e);
				c+=f(exp, true, s, i-1) * f(exp, false, i+1, e);
			} else if(op=='|') {
				c+=f(exp, false, s, i-1) * f(exp, false, i+1, e);
			} else if (op=='^') {
				c+=f(exp, true, s, i-1) * f(exp, true, i+1, e);
				c+=f(exp, false, s, i-1) * f(exp, false, i+1, e);
			}
		}
	}
	return c;
}

// Dynamic Programming: remove all redundant computation
int f_dyn(string exp, bool result, int s, int e, map<string, int> &q) {
	num_call++;
	//cout << exp << " " << result << " " << s << " " << e << endl;
	string key = ""+to_string(result)+to_string(s)+to_string(e);	// to_string in c++11
	if(q.find(key)!=q.end()) // find
		return q[key];

	if(s==e) {
		if(exp[s] == '1' && result) {
			return 1;
		} else if (exp[s]=='0' && !result) {
			return 1;
		}
		return 0;
	}

	int c=0;
	if(result) {	// if result is true
		for(int i=s+1; i<=e; i+=2) {
			char op = exp[i];
			if(op=='&') {
				c+=f_dyn(exp, true, s, i-1, q) * f_dyn(exp, true, i+1, e, q);
			} else if(op=='|') {
				c+=f_dyn(exp, true, s, i-1, q) * f_dyn(exp, true, i+1, e, q);
				c+=f_dyn(exp, false, s, i-1, q) * f_dyn(exp, true, i+1, e, q);
				c+=f_dyn(exp, true, s, i-1, q) * f_dyn(exp, false, i+1, e, q);
			} else if (op=='^') {
				c+=f_dyn(exp, true, s, i-1, q) * f_dyn(exp, false, i+1, e, q);
				c+=f_dyn(exp, false, s, i-1, q) * f_dyn(exp, true, i+1, e, q);
			}
		}
	} else {	// if result is false	
		for(int i=s+1; i<=e; i+=2) {
			char op = exp[i];
			if(op=='&') {
				c+=f_dyn(exp, false, s, i-1, q) * f_dyn(exp, false, i+1, e, q);
				c+=f_dyn(exp, false, s, i-1, q) * f_dyn(exp, true, i+1, e, q);
				c+=f_dyn(exp, true, s, i-1, q) * f_dyn(exp, false, i+1, e, q);
			} else if(op=='|') {
				c+=f_dyn(exp, false, s, i-1, q) * f_dyn(exp, false, i+1, e, q);
			} else if (op=='^') {
				c+=f_dyn(exp, true, s, i-1, q) * f_dyn(exp, true, i+1, e, q);
				c+=f_dyn(exp, false, s, i-1, q) * f_dyn(exp, false, i+1, e, q);
			}
		}
	}
	q[key]=c;	// insert new result in the hash
	return c;
}

/* If we knew how many ways there were of parenthesizing an expression, then we could compute
f(exp = false ) by doing total(exp) - f(exp = true).
This is a closed form expression for the number of ways of parenthesizing an expression, but you
wouldn't be exprected to know it. It is given by the Catalan numbers, where n is the number of operators:
	Cn = (2n)! / (n+1)!n!
With this adjustment, the code looks like the following.
*/

int total(int n) {
	int t=n;
	for(int i=n-1; i>0; i--) 
		t*=i;
	return t;
}

// Final versoin
int ff(string exp, bool result, int s, int e, map<string, int> &q) {
	num_call++;
	//cout << exp << " " << result << " " << s << " " << e << endl;
	string key = ""+to_string(result)+to_string(s)+to_string(e);	// to_string in c++11

	int c=0;
	if(q.find(key)!=q.end()) // find
		c= q[key];
	else {	// Not Found
		if(s==e) {
			if (exp[s]=='1') c=1;
			else c=0;
		}

		for(int i=s+1; i<=e; i+=2) {
			char op = exp[i];
			if(op=='&') {
				c+=ff(exp, true, s, i-1, q) * ff(exp, true, i+1, e, q);
			} else if(op=='|') {
				int left_ops = (i-1-s)/2; 	// parens on left
				int right_ops = (e-i-1)/2; 	// parens on right
				int total_ways = total(left_ops) * total(right_ops);
				int total_false = ff(exp, false, s, i-1, q) * ff(exp, false, i+1, e, q);
				c+=total_ways-total_false;
			} else if (op=='^') {
				c+=ff(exp, true, s, i-1, q) * ff(exp, false, i+1, e, q);
				c+=ff(exp, false, s, i-1, q) * ff(exp, true, i+1, e, q);
			}
		}
		q[key]=c;
	}

	if(result) { 
		return c;
	} else {
		int num_ops = (e-s) /2 ;
		return total(num_ops)-c;
	}
}

int main() {
	string exp="1^0|0|1";
	map<string, int> q;
	map<string, int> r;
	num_call=0;
	cout << f(exp, true, 0, 6) << " "; 
	cout << num_call << endl;
	
	num_call=0;
	cout << f(exp, false, 0, 6) << " ";
	cout << num_call << endl;
	
	num_call=0;
	cout << f_dyn(exp, true, 0, 6, q) << " ";
	cout << num_call << endl;
	
	num_call=0;
	cout << f_dyn(exp, false, 0, 6, q) << " ";
	cout << num_call << endl;
	
	num_call=0;
	cout << ff(exp, true, 0, 6, r) << " ";
	cout << num_call << endl;
	
	num_call=0;
	cout << ff(exp, false, 0, 6, r) << " ";
	cout << num_call << endl;
}



