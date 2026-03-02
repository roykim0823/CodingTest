#!/usr/bin/pyhton3

import math

def prime_sieve(n):
  """Given n, return a list of all primes up to n."""

  if n < 2:
    return []

  size = math.floor(0.5 * (n-3)) + 1
  prime_list = [2] 

  # is_prime[i] represents whether (2i + 3) is prime or not.
  # Initially, set each to True. Then use sieving to eliminate non-primes.
  is_prime = [True] * size

  for i in range(size):
    if is_prime[i]:
      p = (i *2) + 3
      prime_list.append(p)
      
      # Sieving from p^2, whose value is (4i^2 + 12i + 9).
      # The index in is_prime is (2i^2 + 6i +3) since is_prime[i] represents 2i + 3.
      # Note that we need to use long long for j because p^2 might overflow (for C/C++)
      for j in range(2*i*i + 6*i + 3, size, p):  # range(start, stop, step)
        is_prime[j] = False
  return prime_list

def prime_factors(n):
  """Given n, return a list of all primes up to n."""

  MAXN = 1000000
  if n < 2:
    return []

  size = math.floor(0.5 * (n-3)) + 1
  prime_list = [2] 

  number = n
  if number % 2 == 0:
    prime_factor = [2]
    while number % 2 == 0:      
      number = number // 2
  else:
    prime_factor = []

  # is_prime[i] represents whether (2i + 3) is prime or not.
  # Initially, set each to True. Then use sieving to eliminate non-primes.
  if size > MAXN:
    size = MAXN
  is_prime = [True] * size

  for i in range(size):
    if number == 1:
      break
    if is_prime[i]:
      p = (i *2) + 3
      prime_list.append(p)

      if number % p == 0:
        prime_factor.append(p)
        while number % p == 0:          
          number = number // p     
      
      # Sieving from p^2, whose value is (4i^2 + 12i + 9).
      # The index in is_prime is (2i^2 + 6i +3) since is_prime[i] represents 2i + 3.
      # Note that we need to use long long for j because p^2 might overflow (for C/C++)
      for j in range(2*i*i + 6*i + 3, size, p):  # range(start, stop, step)
        is_prime[j] = False
  return prime_factor, prime_list

  

def main():
  prime_factor, prime_list = prime_factors(600851475143)
  # prime_factor, prime_list = prime_factors(1000)
  print("Prime Factors:", prime_factor)
  print("Answer =", prime_factor[-1])
  # print(prime_list)



if __name__ == '__main__':
  main()
