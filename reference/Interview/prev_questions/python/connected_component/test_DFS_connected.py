import challenge

def test_DFS_connected():
  input = [
        [1, 0, 0, 1, 1],
        [0, 1, 1, 0, 0],
        [1, 0, 1, 1, 1],
        [1, 1, 0, 1, 0]
  ]

  row = len(input)
  col = len(input[0])
  output = [[0 for j in range(col)] for i in range(row)]

  challenge.DFS_connected(output, input, 0, 0, row, col, 2)

  expected = [
        [2, 0, 0, 0, 0],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0]
  ]

  assert output == expected

  challenge.DFS_connected(output, input, 0, 3, row, col, 3)
  
  expected = [
        [2, 0, 0, 3, 3],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0]
  ]
  
  assert output == expected
  
  challenge.DFS_connected(output, input, 1, 1, row, col, 4)  
  
  expected = [
        [2, 0, 0, 3, 3],
        [0, 4, 4, 0, 0],
        [0, 0, 4, 4, 4],
        [0, 0, 0, 4, 0]
  ]
  
  assert output == expected
  
  challenge.DFS_connected(output, input, 2, 0, row, col, 5)  
  
  expected = [
        [2, 0, 0, 3, 3],
        [0, 4, 4, 0, 0],
        [5, 0, 4, 4, 4],
        [5, 5, 0, 4, 0]
  ]
  
  assert output == expected
