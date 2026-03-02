import challenge
import pytest

@pytest.mark.parametrize("test_input,expected", [
                            ([  
                              [1, 1, 1, 1, 1],
                              [1, 0, 0, 0, 1],
                              [1, 0, 0, 0, 1],
                              [1, 1, 1, 1, 1]
                             ],
                             [  
                              [2, 2, 2, 2, 2],
                              [2, 0, 0, 0, 2],
                              [2, 0, 0, 0, 2],
                              [2, 2, 2, 2, 2]
                             ]),
                            ([  
                              [1, 1, 1, 1, 1],
                              [1, 0, 0, 0, 1],
                              [1, 0, 1, 0, 1],
                              [1, 1, 1, 1, 1]
                             ],
                             [  
                              [2, 2, 2, 2, 2],
                              [2, 0, 0, 0, 2],
                              [2, 0, 2, 0, 2],
                              [2, 2, 2, 2, 2]
                             ]),
                            ([  
                              [1, 0, 1, 0, 1],
                              [0, 0, 1, 0, 1],
                              [1, 1, 1, 0, 1],
                              [0, 0, 0, 1, 1]
                             ],
                             [  
                              [2, 0, 3, 0, 4],
                              [0, 0, 3, 0, 4],
                              [3, 3, 3, 0, 4],
                              [0, 0, 0, 4, 4]
                             ]),
                            ([  
                              [1, 0, 1, 0, 1],
                             ],
                             [  
                              [2, 0, 3, 0, 4],
                             ]),
                            ([  
                              [1, 0],
                             ],
                             [  
                              [2, 0],
                             ]),
                            ([  
                              [],
                             ],
                             [  
                              [],
                             ]),
                             ])
def test_DFS_connected(test_input, expected):
  row = len(test_input)
  col = len(test_input[0])
  output = [[0 for j in range(col)] for i in range(row)]
  label = 2

  challenge.find_connected(output, test_input, row, col, label)

  assert output == expected
