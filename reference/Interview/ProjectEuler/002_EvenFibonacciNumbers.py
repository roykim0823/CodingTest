#!/usr/bin/pyhton3

def fib(n):
  pprev = 2
  prev = 3
  cur = 5
  fib_list = [1, 2]
  sum = 2
  while cur < n:
    fib_list.append(cur)
    if cur % 2 == 0:
      sum += cur
    # update for next prime number
    cur = pprev + prev
    pprev = prev
    prev = cur

  
  return sum, cur, fib_list

def main():
  sum, cur, fib_list = fib(4000000)
  print(sum)
  print(cur)
  print(fib_list)

if __name__ == '__main__':
  main()
