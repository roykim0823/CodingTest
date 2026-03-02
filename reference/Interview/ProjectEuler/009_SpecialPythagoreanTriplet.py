#!/usr/bin/pyhton3

# A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,
# a^2 + b^2 = c^2
# For example, 32 + 42 = 9 + 16 = 25 = 52.

# There exists exactly one Pythagorean triplet for which a + b + c = 1000.
# Find the product abc.
def pythagorean_triplet(n):
  for a in range(3, n-3):
    for b in range (a + 1, n-1-a):
      c = n - a - b      

      if c*c == a*a + b*b:
        product = a * b * c
        print(product, a, b, c)
        break  


def main():
  pythagorean_triplet(1000)


if __name__ == '__main__':
  main()
