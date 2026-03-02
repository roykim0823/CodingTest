#!/usr/bin/pyhton3

def sum_mul3_and_mul5_below_n(n):
  sum = 0
  sum_list = []
  mul3 = 3
  mul5 = 5
  while mul3 < n or mul5 < n:
    if mul3 == mul5:
      sum_list.append(mul3)
      sum += mul3
      mul3 += 3
      mul5 += 5
    elif mul3 < mul5:
      sum_list.append(mul3)
      sum += mul3
      mul3 += 3
    else:
      sum_list.append(mul5)
      sum += mul5
      mul5 += 5
  # print(sum_list)
  return sum

# using formula
# 3+6+9+...+999 = 3*(1+2+3+...333)
# cf. 1+2+3+...+p = 1/2 * p * (p+1)
def sum_divisible_by_k(n, k):
  p = n // k
  return k * (p*(p+1)) // 2
  

def main():
  print(sum_mul3_and_mul5_below_n(1000))
  print(sum_divisible_by_k(1000,3)+sum_divisible_by_k(1000, 5)-sum_divisible_by_k(1000,15))

if __name__ == '__main__':
  main()