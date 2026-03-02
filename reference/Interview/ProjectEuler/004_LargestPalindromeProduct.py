#!/usr/bin/pyhton3

def Palindrome(product):
  mid = len(product) // 2

  for i in range(mid):
    if product[i] != product[len(product)-i-1]:
      return False
  return True

def LargetPalindromeProduct(digit=2):
  max_num = int('9' * digit)
  min_num = int('1' + ('0' * (digit-1)))
  # print(max_num, min_num)
  largest = 0
  
  left = right = 999
  min_num = 100
  while left >= min_num:
    right = left
    while right >= min_num:
      product = str(left * right)
      if Palindrome(product):
        palindrome = left * right
        if palindrome > largest:
          largest = palindrome          
          # print(left * right, '=', left, 'x', right)   
          if left < right:
            min_num = left  # update min_num to avoid unncessary search
          else:
            min_num = right
      right -= 1
    left -= 1
  return largest

# Using Formula
# P = 100000x + 10000y + 1000z + 100z + 10y +x
# P = 100001x + 10010y + 1100z
# P = 11(9091x + 910y + 100z)
# Since 11 is prime, at least one of the integer a or b must have a factor of 11

def LargetPalindromeProduct2(digit=3):
  max_num = int('9' * digit)
  min_num = int('1' + ('0' * (digit-1)))
  # print(max_num, min_num)
  largest = 0
  
  left = right = 999
  min_num = 100
  while left >= min_num:
    if left % 11 == 0:
      left_11 = True
    else:
      left_11 = False
    right = left

    while right >= min_num:
      if not left_11 and right % 11 != 0:
        right -= 1
        continue  # both are not divisible by 11, skip
      product = str(left * right)
      if Palindrome(product):
        palindrome = left * right
        if palindrome > largest:
          largest = palindrome          
          # print(left * right, '=', left, 'x', right)   
          if left < right:
            min_num = left  # update min_num to avoid unncessary search
          else:
            min_num = right
      right -= 1
    left -= 1
  return largest

def main():
  print(LargetPalindromeProduct(3))
  print(LargetPalindromeProduct2(3))
  


if __name__ == '__main__':
  main()
