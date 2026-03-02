#!/usr/bin/pyhton3

"""
5 4 3
6 1 2
7 8 9 

17 16  15  14  13
18  5   4   3  12
19  6   1   2  11
20  7   8   9  10 
21 22  23  24  25
"""


def construct_spiral(n):
    # n must be odd number
    if n < 0 or n % 2 == 0:
        return 
    # right -> up -> left -> down
    num = 1
    move = 1
    row = col = n//2
    mat = [[ 0 for j in range(n) ] for i in range(n) ]

    mat[row][col] = num
    # print(mat)
    while True:
        for _ in range(move):  # move right
            num += 1
            if num > n*n:
                return mat
            col += 1
            mat[row][col] = num
            # print(mat)

        for _ in range(move):  # move up
            num += 1
            row -= 1
            mat[row][col] = num
            # print(mat)

        move += 1

        for _ in range(move): # move left
            num +=1
            col -=1
            mat[row][col] = num
            # print(mat)

        for _ in range(move): # move down
            num +=1
            row +=1
            mat[row][col] = num
            # print(mat)

        move += 1

def print_mat(mat):
    for i in range(len(mat)):
        print(mat[i])

# Add move Knight (of Checss) in the given matrix

def main():
    for i in range (1, 10, 2):
        print_mat(construct_spiral(i))
        print()


if __name__ == '__main__':
  main()
