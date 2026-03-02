#!/usr/bin/pyhton

def gcd(a, b):
  if b == 0:
    return a
  return gcd(b, a % b)


def gcd_in_many(num, arr):
  gcd_value = arr[0]
  for idx in range(1, len(arr)):
    gcd_value = gcd(gcd_value, arr[idx])
  
  return gcd_value

 
def main():
  print(gcd_in_many(5, [2, 3, 4, 5, 6]))
  print(gcd_in_many(5, [2, 4, 6, 8, 10]))

if __name__ == '__main__':
  main()



