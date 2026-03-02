#include <iostream>
#include "primeFactor.h"
#include "printVector.h"


int main() {
	std::vector<int> factors;
	int num;
	num=12;
	std::cout << num << std::endl;
	factors = prime_factors(num);
	print(factors);
	num=20;
	std::cout << num << std::endl;
	factors = prime_factors(num);
	print(factors);
	num=315;
	std::cout << num << std::endl;
	factors = prime_factors(num);
	print(factors);
	num=12246;
	std::cout << num << std::endl;
	factors = prime_factors(num);
	print(factors);

    std::cout << "Using Sieve's factors" << std::endl;
	precompute_sieve();
	num=12;
	std::cout << num << std::endl;
	factors = sieve_factors(num);
	print(factors);
	num=20;
	std::cout << num << std::endl;
	factors = sieve_factors(num);
	print(factors);
	num=315;
	std::cout << num << std::endl;
	factors = sieve_factors(num);
	print(factors);
	num=12246;
	std::cout << num << std::endl;
	factors = sieve_factors(num);
	print(factors);
	
	// Use Sieve 
	std::cout << "Sieve's Smallest Prime for numbers" << std::endl;
	for(int i=0; i<40; i++) {
		std::cout << spf[i] << " ";
		if((i+1)%10 == 0) 
			std::cout << std::endl;
	}
	return 0;
}
	
