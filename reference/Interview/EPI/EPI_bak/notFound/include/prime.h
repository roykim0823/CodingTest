#include <vector>
#include <cstring> 	// memset
#include <cmath>
#include <deque>
#include <iostream>
using namespace std; 
  
// Naive Prime
bool primeNaive(int n) {
	if( n < 2 ) 
		return false;
	
	// Better one use sqrt(n) instead of n in the for loop
	// not prime number x = a*b at least, then a>sqrt then b<sqrt
	// so check sqrt means check the b that is enough
	// for(int i=2; i<=sqrt(n); i++) {
	for(int i=2; i<n; i++) {
		if (n % i == 0) 
			return false;
	}
	return true;
}

vector<bool> sievePrime(int n) {

    // Create a boolean array "prime[0..n]" and initialize 
    // all entries it as true. A value in prime[i] will 
    // finally be false if i is Not a prime, else true. 
    vector<bool> prime(n+1, true); 
    //memset(prime, true, sizeof(prime)); 
      
    prime[0]=prime[1]=false;
    for (int p=2; p*p<=n; p++) 
    { 
        // If prime[p] is not changed, then it is a prime 
        if (prime[p] == true) 
        { 
            // Update all multiples of p greater than or  
            // equal to the square of it 
            // numbers which are multiple of p and are 
            // less than p^2 are already been marked.  
            for (int i=p*p; i<=n; i += p) 
                prime[i] = false; 
        } 
    }
	return prime;
}

// isPrime[] : isPrime[i] is true if number is prime  
// prime[] : stores all prime number less than N 
// SPF[] that store smallest prime factor of number 
// [for Exp : smallest prime factor of '8' and '16' 
// is '2' so we put SPF[8] = 2 , SPF[16] = 2 ] 
const long MAX_SIZE=1000;
vector<long long >isprime(MAX_SIZE , true); 
vector<long long >prime; 
vector<long long >SPF(MAX_SIZE); 
  
// function generate all prime number less then N in O(n) 
vector<long long> improved_seive(int N) 
{ 
// isPrime[] : isPrime[i] is true if number is prime  
// prime[] : stores all prime number less than N 
// SPF[] that store smallest prime factor of number 
// [for Exp : smallest prime factor of '8' and '16' 
// is '2' so we put SPF[8] = 2 , SPF[16] = 2 ] 
vector<long long >isprime(MAX_SIZE , true); 
vector<long long >prime; 
vector<long long >SPF(MAX_SIZE);

    // 0 and 1 are not prime 
    isprime[0] = isprime[1] = false ; 
  
    // Fill rest of the entries 
    for (long long int i=2; i<N ; i++) 
    { 
        // If isPrime[i] == True then i is 
        // prime number 
        if (isprime[i]) 
        { 
            // put i into prime[] vector 
            prime.push_back(i); 
  
            // A prime number is its own smallest 
            // prime factor 
            SPF[i] = i; 
        } 
  
        // Remove all multiples of  i*prime[j] which are 
        // not prime by making isPrime[i*prime[j]] = false 
        // and put smallest prime factor of i*Prime[j] as prime[j] 
        // [ for exp :let  i = 5 , j = 0 , prime[j] = 2 [ i*prime[j] = 10 ] 
        // so smallest prime factor of '10' is '2' that is prime[j] ] 
        // this loop run only one time for number which are not prime 
        for (long long int j=0; 
             j < (int)prime.size() && 
             i*prime[j] < N && prime[j] <= SPF[i]; 
             j++) 
        { 
            isprime[i*prime[j]]=false; 
  
            // put smallest prime factor of i*prime[j] 
            SPF[i*prime[j]] = prime[j] ; 
        } 
    } 

    return prime;
} 

/*****************************************************************************/
// Given n, return the primes from 1 to n.
// Improved version
vector<int> generate_primes_from_1_to_n(int n) {
  // is_prime[i] represents (2i + 3) is prime or not.
  
  int size = floor(0.5 * (n - 3)) + 1;
  deque<bool> is_prime(size, true);
  
  vector<int> primes;  		// stores the primes from 1 to n.
  primes.emplace_back(2);	// start with 2
  
  for (long i = 0; i < size; ++i) {
    if (is_prime[i]) {
      int p = (i << 1) + 3;
      primes.emplace_back(p);
      
	  // Sieving from p^2, whose index is 2i^2 + 6i + 3.
      for (long j = ((i * i) << 1) + 6 * i + 3; j < size; j += p) {
        is_prime[j] = false;
      }
    }
  }
  return primes;
}

/* Prime CCI solution */
void setPrime(vector<bool> &flags, int prime);
int getNextPrime(vector<bool> &flags, int prime);

vector<bool> listPrime(const int &n) {
	vector<bool> flags(n+1, true);
	vector<int> primeList;
	int prime=2;

	while(prime<=n) {
		// set flag on in the remaining multiples of the prime
		setPrime(flags, prime);

		// Find next prime number
		prime = getNextPrime(flags, prime);

		if(prime>=(int)flags.size())
			break;
	}
	return flags;
}

// cross off remaining multiples of primes
void setPrime(vector<bool> &flags, int prime) {
	for(size_t i=prime*prime; i<flags.size(); i+=prime) 
		flags[i]=false;
}

int getNextPrime(vector<bool> &flags, int prime)
{
	size_t next = prime+1;
	while(next < flags.size() && !flags[next]) {
		next++;
	}
	return next;
}

void printPrimeList(vector<bool> flags) {
	for(int i=2; i<flags.size(); i++) {
		if(flags[i]==true)
			cout << i << " "; 
	}
	cout << endl;
}
