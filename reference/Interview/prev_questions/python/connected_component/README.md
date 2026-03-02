# Coding challenge for Deep Learning Engineer

## Prerequisites

You'll need either a C++ or Python toolchain. While you can use whatever tools
you'd like for development, we'll test your code using the included Dockerfiles.
From the appropriate language directory, you can build using this command:

```
docker build -t bcai-challenge .
```

and run the tests with this one:

```
docker run --rm bcai-challenge
```

## Task

Complete the ConnectedComponents() method in challenge.cc or
connected_components() method in challenge.py, **adding whatever unit tests you
need** to ensure your implementation is correct.

The connected components method accepts a vector of vectors of integers in C++
or a list of lists of integers in Python. All input values will be zero or one.
All inner vectors will have the same length (meaning the input is rectangular).
In C++, ConnectedComponents should modify the input in-place. In Python,
connected_components() should return a new list of lists. Zeros should be left
unmodified. Ones should be replaced with a label chosen from the range \[2, N\].
All *adjacent* non-zero elements should be assigned the same label. Only treat
the positions immediately above, below, left, and right of a given coordinate as
adjacent. Diagonals are explicitly not adjacent. Do not wrap around the borders.
Labels should be assigned starting with 2 at the upper left corner, incrementing
by one for each new connected region when searching left-to-right and
top-to-bottom.

One sample test is included. In C++:

```C++
TEST(Challenge, ConnectedComponents) {
    vector<vector<int>> v{
            {1, 0, 0, 1, 1},
            {0, 1, 1, 0, 0},
            {1, 0, 1, 1, 1},
            {1, 1, 0, 1, 0}
    };

    vector<vector<int>> answer{
            {2, 0, 0, 3, 3},
            {0, 4, 4, 0, 0},
            {5, 0, 4, 4, 4},
            {5, 5, 0, 4, 0}
    };

    ConnectedComponents(v);

    ASSERT_EQ(v, answer);
}
```

and in Python:

```Python
def test_connected_components():
    input = [
        [1, 0, 0, 1, 1],
        [0, 1, 1, 0, 0],
        [1, 0, 1, 1, 1],
        [1, 1, 0, 1, 0]
    ]

    expected_output = [
        [2, 0, 0, 3, 3],
        [0, 4, 4, 0, 0],
        [5, 0, 4, 4, 4],
        [5, 5, 0, 4, 0]
    ]

    output = challenge.connected_components(input)

    assert output == expected_output
```

With a correct implementation, this test should pass unmodified.

You can use whatever functionality from the standard library you choose. Please
do not bring in third-party libraries. Assume C++11 for C++ and Python 3 for
Python. It should not be necessary to modify anything except challenge.cc for
C++ or challenge.py/test_connected_components.py for Python.

These are the criteria we'll use for evaluating solutions:

1. Correct is better than incorrect
2. Simple is better than complex
3. Tested is better than untested
4. Scalable is better than not scalable (production data could potentially be much larger)
5. Reusability counts
6. Maintainability counts
7. Limitations are expected, but should be documented

You have five days to submit your solution, but we don't anticipate candidates spending more than a few hours on this challenge. Good luck!
