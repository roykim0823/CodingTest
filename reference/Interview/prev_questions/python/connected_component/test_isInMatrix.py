import challenge
import pytest

@pytest.mark.parametrize("test_input,expected", [
                            ([-1, 0, 3, 3], False), ([0, 0, 3, 3], True), 
                            ([1, 2, 3, 4], True), ([2, 3, 3, 3], False), ([1, 2, 0, 0], False)])
def test_isInMatrix(test_input, expected):
  output = challenge.isInMatrix(test_input[0], test_input[1], test_input[2], test_input[3])
  assert output == expected
