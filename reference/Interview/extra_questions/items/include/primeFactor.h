/* Find all prime factors
   1. While n is divisible by 2, and divide n by 2
   2. After step 1, n must be odd. Now start a loop from i=3 to square root of n.
      while i divides n, increase i by 2 and continue
   3. if n is a prime number and is greater than 2, then n will not become 1 by above
      two steps, then n is facter if it is greater than 2.
*/
#include <vector>
#include <cmath>

std::vector<int> prime_factors(int n) 
{
	std::vector<int> primeFactors;

	// the number of 2s that divide n
	while(n%2 == 0) {
		primeFactors.push_back(2);
		n = n/2;
	}

	// n must be odd. So we can skip one element (Note i = i+2)
	for(int i=3; i<=sqrt(n); i = i+2) {
		// while i divides n, divide n
		while(n%i == 0) {
			primeFactors.push_back(i);
			n=n/i;
		}
	}

	// This condition is to handle the case when n is a prime number greater than 2
	if(n>2)
		primeFactors.push_back(n);

	return primeFactors;
}

/* Prime Factorizatoin using Sieve O(log n) for multiple queris.
   	- Key Concept: 
		Our idea is to store the Smallest Prime Factor(SPF) for every number. 
		Then to calculate the prime factorization of the given number by dividing 
		the given number recursively with its smallest prime factor till it becomes 1.
	- To calculate to smallest prime factor for every number we will use the sieve of 
	  eratosthenes. In original Sieve, every time we mark a number as not-prime, 
	  we store the corresponding smallest prime factor for that number 
	  (Refer this article for better understanding).
	  	Now, after we are done with precalculating the smallest prime factor for every 
	  number we will divide our number n (whose prime factorziation is to be calculated) 
	  by its corresponding smallest prime factor till n becomes 1.
*/
	
/* Pseudo Code for prime factorization assuming
	SPFs are computed :

		PrimeFactors[] // To store result

		i = 0  // Index in PrimeFactors

		while n != 1 :
			// SPF : smallest prime factor
    		PrimeFactors[i] = SPF[n]
    		i++
    		n = n / SPF[n]
*/

const int MAXN = 100001;

// stores smallest prime factor for every number
int spf[MAXN];

// Calculating SPF (Smallest Prime Factor) for every
// number till MAXN.
// Time Complexity : O(nloglogn)
void precompute_sieve()
{
    spf[1] = 1;
    for (int i=2; i<MAXN; i++)

        // marking smallest prime factor for every
        // number to be itself.
        spf[i] = i;

    // separately marking spf for every even
    // number as 2
    for (int i=4; i<MAXN; i+=2)
        spf[i] = 2;

    for (int i=3; i*i<MAXN; i++)
    {
        // checking if i is prime
        if (spf[i] == i)
        {
            // marking SPF for all numbers divisible by i
            for (int j=i*i; j<MAXN; j+=i)

                // marking spf[j] if it is not
                // previously marked
                if (spf[j]==j)
                    spf[j] = i;
        }
    }
}

// A O(log n) function returning primefactorization
// by dividing by smallest prime factor at every step
std::vector<int> sieve_factors(int x)
{
    std::vector<int> ret;
    while (x != 1)
    {
        ret.push_back(spf[x]);
        x = x / spf[x];
    }
    return ret;
} 
