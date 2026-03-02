#!/usr/bin/pyhton

def isInMatrix(i, j, row, col):
  return (i >= 0 and i < row and 
          j >= 0 and j < col)


def DFS_search(distance, visited, area, i, j, row, col):
  print(i, j, area[i][j], distance)
  if area[i][j] == 9:
    return distance  

  visited[i][j] = True

  # above, below, left, and right adjacent
  adjacent = [[-1, 0], [1, 0], [0, -1], [0, 1]]
  
  for r, c in adjacent:
    # if isInMatrix(i+r, j+c, row, col) and area[i+r][j+c] > 0 and visited[i+r][j+c] == False:
    if isInMatrix(i+r, j+c, row, col) and area[i+r][j+c] > 0:
      return DFS_search(distance+1, visited, area, i+r, j+c, row, col)





def minimumDistance(numRows, numColumns, area):
    # WRITE YOUR CODE HERE
    visited = [ [False for col in range(numColumns)] for row in range(numRows)] # init visited area
    return DFS_search(0, visited, area, 0, 0, numRows, numColumns)
    
    
 
def main():
  print(minimumDistance(3, 3, [[1, 0, 0], [1, 0, 0], [1, 9, 0]]))  

if __name__ == '__main__':
  main()



