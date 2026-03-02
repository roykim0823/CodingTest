import challenge

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
