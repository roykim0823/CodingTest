#!/usr/bin/pyhton3

# O(N)
def larget_gap_in_array(arr):
    return max(arr) - min(arr)

def main():
    arr = [-1, 2, 3, -4, -10, 22]
    print(larget_gap_in_array(arr))

if __name__ == '__main__':
  main()
