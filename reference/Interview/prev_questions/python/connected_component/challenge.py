def isInMatrix(i, j, row, col):
  return (i >= 0 and i < row and 
          j >= 0 and j < col)

def DFS_connected(output, input, i, j, row, col, label):
  output[i][j] = label

  # above, below, left, and right adjacent
  adjacent = [[-1, 0], [1, 0], [0, -1], [0, 1]]
  
  for r, c in adjacent:
    if isInMatrix(i+r, j+c, row, col) and input[i+r][j+c] == 1 and output[i+r][j+c] == 0:
      DFS_connected(output, input, i+r, j+c, row, col, label)


def find_connected(output, input, row, col, label):
  for i in range(row):
    for j in range(col):
      if input[i][j] == 1 and output[i][j] == 0:
        DFS_connected(output, input, i, j, row, col, label)
        label += 1

def connected_components(input):
  """Find connected components by depth-first-search."""
  row = len(input)
  col = len(input[0])
  # initialize the output (similar to visited component in DFS)
  # output marks the label value instead of True
  output = [[0 for j in range(col)] for i in range(row)]
  label = 2  # lable [2, N]

  find_connected(output, input, row, col, label)

  return output
