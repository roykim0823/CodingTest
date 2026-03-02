#include "test_framework/generic_test.h"

// Compute GCD of two numbers without using multiplication, division or the modulus operators
// - Use recursion, the base case being where one of the arguments is 0
// - Otherwise, we check if none, one of both numbers are even.
//   - If both are even, we compute the GCD of these numbers divided by 2, and return pair.
//   - If both are odd, we subtract the smaller from the larger and return the GCD of the resulting pair.
long long Gcd(long long x, long long y) {
  if (x > y) {
    return Gcd(y, x);
  } else if (x == 0) {
    return y;
  } else if (!(x & 1) && !(y & 1)) {  // x and y are even.
    return Gcd(x >> 1, y >> 1) << 1;
  } else if (!(x & 1) && y & 1) {  // x is even, and y is odd.
    return Gcd(x >> 1, y);
  } else if (x & 1 && !(y & 1)) {  // x is odd, and y is even.
    return Gcd(x, y >> 1);
  }
  return Gcd(x, y - x);  // Both x and y are odd.
}

/* GCD (Greatest Common Divisor) of HCF (Highest Common Factor) of two number is
   the largest number that divides both of them
   A. Simple Solutions
    - Find all prime factors of both numbers, then find intersection of all factors
      presented in both numbers. Finally, return product of elements in the intersection
   B. Efficient solution
    - Use Euclidean algorithm which is the main algorithm used for this purpose.
      The idea is GCD of two numbers doesn't change if smaller number is subtracted from
      a bigger number.
*/

long long gcd_by_sub(long long x, long long y) {
  if (x > y) {
    return gcd_by_sub(y, x);
  } else if (x == 0) {
    return y;
  } else if (x == y) {
    return y;
  }
  // x <= y
  return gcd_by_sub(x, y - x);  
}

long long gcd_by_mod(long long x, long long y) {
  if (x > y) {
    return gcd_by_mod(y, x);
  } else if (x == 0) {
    return y;
  } else if (x == y) {
    return y;
  }
  return gcd_by_mod(x, y % x);
}

/* Extended Euclidean Algorithm
   - Find integer coefficients x and y such that: ax + by = gcd (a, b)

   Examples:
        Input: a = 30, b = 20
        Output: gcd = 10
        x = 1, y = -1
        (Note that 30*1 + 20*(-1) = 10)

        Input: a = 35, b = 15
        Output: gcd = 5
        x = 1, y = -2
        (Note that 10*0 + 5*1 = 5)
   
   The extended Euclidean algorithm updates results of gcd(a, b) using the results 
   calculated by recursive call gcd(b%a, a). Let values of x and y calculated by 
   the recursive call be x1 and y1. x and y are updated using below expressions.

   x = y1 - floor[b/a] * x1
   y = x1
*/
long gcd_euc(int a, int b, int &x, int &y) {
  // Base case
  if(a == 0) {
    x=0; y=1;
    return b;
  }

  int x1, y1;     // to store results of recursive call
  int gcd = gcd_euc((b%a), a, x1, y1);

  // Update x and y using results of recursive
  x = y1 - (b/a) * x1;
  y = x1;
  return gcd;
}
/* The GCD of three or more numbers equals the product of the prime factors common
   to all the numbers, but it can be also calculated by repeatedly taking the GCD of
   pairs of numbers.
   ex) gcd(a, b, c) 
        = gcd(a, gcd(b, c))
        = gcd(gdc(a, b), c)
        = gcd(gdc(a, c), b)
*/

int generalizedGCD(int num, int* arr) {
        int result = arr[0];
        for(int i=1; i<num; i++)
                result = Gcd(result, arr[i]);

        return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "y"};
  GenericTestMain(args, "gcd.cc", "gcd.tsv", &Gcd, DefaultComparator{}, param_names);
  //GenericTestMain(args, "gcd.cc", "gcd.tsv", &gcd_by_sub, DefaultComparator{}, param_names);  // segmentation fault
  GenericTestMain(args, "gcd.cc", "gcd.tsv", &gcd_by_mod, DefaultComparator{}, param_names);
}
