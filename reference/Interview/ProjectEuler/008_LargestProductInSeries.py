#!/usr/bin/pyhton3

DIGITS = (
  '73167176531330624919225119674426574742355349194934'
  '96983520312774506326239578318016984801869478851843'
  '85861560789112949495459501737958331952853208805511'
  '12540698747158523863050715693290963295227443043557'
  '66896648950445244523161731856403098711121722383113'
  '62229893423380308135336276614282806444486645238749'
  '30358907296290491560440772390713810515859307960866'
  '70172427121883998797908792274921901699720888093776'
  '65727333001053367881220235421809751254540594752243'
  '52584907711670556013604839586446706324415722155397'
  '53697817977846174064955149290862569321978468622482'
  '83972241375657056057490261407972968652414535100474'
  '82166370484403199890008895243450658541227588666881'
  '16427171479924442928230863465674813919123162824586'
  '17866458359124566529476545682848912883142607690042'
  '24219022671055626321111109370544217506941658960408'
  '07198403850962455444362981230987879927244284909188'
  '84580156166097919133875499200524063689912560717606'
  '05886116467109405077541002256983155200055935729725'
  '71636269561882670428252483600823257530420752963450')

def str_to_int_list():
  int_list = []
  for i in range(len(DIGITS)):
    int_list.append(int(DIGITS[i]))
  
  return int_list

def series_product(int_list):
  product = 1
  for i in int_list:
    product *= i
  return product


def largest_product_in_series(n):
  if n < 1 or n > len(DIGITS):
    return
  int_list = str_to_int_list()
  series = int_list[0:n]
  max_product = cur_product = series_product(series) 

  for i in range(n, len(int_list)):    
  #while i < len(int_list):
    series.append(int_list[i])  # add new digit to the end
    popped_value = series[0]
    series.pop(0)  # remove the oldest digit 
    if cur_product == 0:
      cur_product = series_product(series)
    else:
      cur_product = int(cur_product // popped_value) * series[-1]
      '''
      if popped_value == 0:
        print(i, series)
        cur_product = series_product(series)
      else:
        cur_product = int(cur_product // popped_value) * series[-1]
      '''    
    if cur_product > max_product:
      max_product = cur_product
  return max_product


def main():
  for i in range(1, 14):
    print(i, largest_product_in_series(i))

if __name__ == '__main__':
  main()
