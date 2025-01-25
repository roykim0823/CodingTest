#include <cmath>
#include <deque>
#include <vector>
#include "test_framework/generic_test.h"

using std::deque;
using std::vector;

// Given n, return all primes up to and including n.
vector<int> GeneratePrimes(int n) {
  if (n < 2) {
    return {};
  }
  const int size = floor(0.5 * (n - 3)) + 1;  // size ~= sqrt(n)
  vector<int> primes;
  primes.emplace_back(2);
  // is_prime[i] represents whether (2i + 3) is prime or not.
  // Initially, set each to true. Then use sieving to eliminate nonprimes.
  deque<bool> is_prime(size, true);
  for (int i = 0; i < size; ++i) {
    if (is_prime[i]) {
      int p = (i * 2) + 3;
      primes.emplace_back(p);
      // Sieving from p^2, whose value is (4i^2 + 12i + 9). The index in
      // is_prime is (2i^2 + 6i + 3) because is_prime[i] represents 2i + 3.
      //
      // Note that we need to use long long for j because p^2 might overflow.
      for (long long j = 2LL * i * i + 6 * i + 3; j < size; j += p) {
        is_prime[j] = false;
      }
    }
  }
  return primes;
}

vector<int> sievePrime(int n) {

    // Create a boolean array "prime[0..n]" and initialize 
    // all entries it as true. A value in prime[i] will 
    // finally be false if i is Not a prime, else true. 
    vector<bool> prime(n+1, true);
    //memset(prime, true, sizeof(prime)); 

    prime[0]=prime[1]=false;
    for (int p=2; p*p<=n; p++) {
        // If prime[p] is not changed, then it is a prime 
        if (prime[p] == true) {
            // Update all multiples of p greater than or  
            // equal to the square of it 
            // numbers which are multiple of p and are 
            // less than p^2 are already been marked.  
            for (int i=p*p; i<=n; i += p)
                prime[i] = false;
        }
    }
    
    vector<int> primes;
    for (int i=0; i<prime.size(); i++) {
      if (prime[i]) {
        primes.push_back(i);
      }
    }
    return primes;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  GenericTestMain(args, "prime_sieve.cc", "prime_sieve.tsv",
                  &GeneratePrimes, DefaultComparator{}, param_names);
  GenericTestMain(args, "prime_sieve.cc", "prime_sieve.tsv",
                  &sievePrime, DefaultComparator{}, param_names);
}