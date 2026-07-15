# Interview Questions from EPI and others

Study repo for *Elements of Programming Interviews* — C++ and Python solutions organized
by chapter (numbering follows the EPI sampler PDF, `../epilight_cpp_new.pdf`). Each chapter
directory has a `README.md` with the full study notes, `cpp/` sources, and `python/` sources.

Difficulty marks in the index below: * = difficult, ** = very hard.

## Build and run — C++

```sh
cd epi_cpp
cmake -S . -B build          # configure all chapters
cmake --build build -j       # build everything
cmake --build build -t parity     # or build a single problem

./build/01_PrimitiveType/parity --test-data-dir ../test_data
```

Each chapter is also a standalone project: `cmake -S 01_PrimitiveType -B 01_PrimitiveType/build`.

## Run — Python

Each chapter's `python/` directory contains symlinks to the shared test framework
(`pylib/`), so solutions run directly from inside the directory:

```sh
cd epi_cpp/01_PrimitiveType/python
python3 parity.py            # finds ../../../test_data automatically
```

Solutions not yet migrated to a chapter live in `../epi_python/` and run the same way
from that directory.

## Problem index

### Part I. Data Structures and Algorithms

#### [Chapter 1. Primitive Types](01_PrimitiveType/README.md)
    1.1 Computing the parity of a word
    1.2 Swap bits
    1.3 Reverse bits
    1.4 Find a closest integer with the same weight
    1.5 Compute x × y without arithmetical operators
    1.6 Compute x / y using only addition, subtraction, and shifts
    1.7 Compute x^y
    1.8 Reverse digits
    1.9 Check if a decimal integer is a palindrome
    1.10 Generate uniform random numbers
    1.11 Rectangle intersection

#### [Chapter 2. Arrays](02_Arrays/README.md)
    2.00 Boot camp: even–odd partition
    2.01 The Dutch national flag problem
    2.02 Increment an arbitrary-precision integer
    2.03 Multiply two arbitrary-precision integers
    2.04 Advancing through an array
    2.05 Delete duplicates from a sorted array
    2.06 Buy and sell a stock once
    2.07 Buy and sell a stock twice
    2.08 Enumerate all primes to n
    2.09 Permute the elements of an array
    2.10 Compute the next permutation
    2.11 Sample offline data
    2.12 Sample online data
    2.13 Compute a random permutation
    2.14 Compute a random subset
    2.15 Generate nonuniform random numbers
    2.16 The Sudoku checker problem
    2.17 Compute the spiral ordering of a 2D array
    2.18 Rotate a 2D array
    2.19 Compute rows in Pascal's Triangle

#### [Chapter 3. Strings](03_Strings/README.md) [TODO]
    3.00 is_string_palindromic.cc
    3.01 string_integer_interconversion.cc
    3.02 convert_base.cc
    3.03 spreadsheet_encoding.cc
    3.04 replace_and_remove.cc
    3.05 is_string_palindromic_punctuation.cc
    3.06 reverse_words.cc
    3.07 phone_number_mnemonic.cc
    3.08 look_and_say.cc
    3.09 roman_to_integer.cc
    3.10 valid_ip_addresses.cc
    3.11 snake_string.cc
    3.12 run_length_compression.cc
    3.13 substring_match.cc

#### [Chapter 4. Linked Lists](04_LinkedLists/README.md)
    4.00 search_in_list.cc &
         insert_in_list.cc &
         delete_from_list.cc
    4.01 sorted_lists_merge.cc
    4.02 reverse_sublist.cc
         reverse_list.cc
    4.03 is_list_cyclic.cc
    4.04 do_terminated_lists_overlap.cc
    4.05 do_lists_overlap.cc
    4.06 delete_node_from_list.cc
    4.07 delete_kth_last_from_list.cc
    4.08 remove_duplicates_from_sorted_list.cc
    4.09 list_cyclic_right_shift.cc
    4.10 even_odd_list_merge.cc
    4.11 is_list_palindromic.cc
    4.12 pivot_list.cc
    4.13 int_as_list_add.cc

#### [Chapter 5. Stacks and Queues](05_StacksAndQueue/README.md)
    5.00 reverse_list.cc
    5.01 stack_with_max.cc
    5.02 evaluate_rpn.cc
    5.03 is_valid_parenthesization.cc
    5.04 directory_path_normalization.cc
    5.05 copy_posting_list.cc #?
    5.06 sunset_view.cc
    5.07 tree_level_order.cc
    5.08 circular_queue.cc
    5.09 queue_from_stacks.cc
    5.10 queue_with_max.cc &
         queue_with_max_using_deque.cc

#### [Chapter 6. Binary Trees](06_BinaryTrees/README.md)
    6.01 is_tree_balanced.cc
    6.02 is_tree_symmetric.cc
    6.03 lowest_common_ancestor.cc
    6.04 lowest_common_ancestor_with_parent.cc
    6.05 sum_root_to_leaf.cc
    6.06 path_sum.cc
    6.07 tree_inorder.cc
    6.08 tree_preorder.cc
    6.09 kth_node_in_tree.cc
    6.10 successor_in_tree.cc
    6.11 tree_with_parent_inorder.cc
    6.12 tree_from_preorder_inorder.cc
    6.13 tree_from_preorder_with_null.cc
    6.14 tree_connect_leaves.cc
    6.15 tree_exterior.cc
    6.16 tree_right_sibling.cc
    6.17 [] Implement locking in a binary tree: NotFound!

#### [Chapter 7. Heaps](07_Heaps/README.md)
    7.01 sorted_arrays_merge.cc
    7.02 sort_increasing_decreasing_array.cc
    7.03 sort_almost_sorted_array.cc
    7.04 k_closest_stars.cc
    7.05 online_median.cc
    7.06 k_largest_in_heap.cc
    7.07 [] Implement a stack API using a heap: NotFound!

#### [Chapter 8. Searching](08_Searching/README.md)
    8.01 search_first_key.cc
    8.02 search_entry_equal_to_index.cc
    8.03 search_shifted_sorted_array.cc
    8.04 int_square_root.cc
    8.05 real_square_root.cc
    8.06 search_row_col_sorted_matrix.cc
    8.07 search_for_min_max_in_array.cc
    8.08 kth_largest_in_array.cc
    8.09 absent_value_array.cc
    8.10 search_for_missing_element.cc

#### [Chapter 9. Hash Tables](09_HashTables/README.md)
    9.00 anagrams.cc
    9.01 is_string_permutable_to_palindrome.cc
    9.02 is_anonymous_letter_constructible.cc
    9.03 lru_cache.cc
    9.04 lowest_common_ancestor_close_ancestor.cc
    9.05 search_frequent_items.cc [?]
    9.06 nearest_repeated_entries.cc
    9.07 smallest_subarray_covering_set.cc
    9.08 smallest_subarray_covering_all_values.cc
    9.09 longest_subarray_with_distinct_values.cc
    9.10 longest_contained_interval.cc
    9.11 [] Compute the average of the top three scores
    9.12 string_decompositions_into_dictionary_words.cc
    9.13 collatz_checker.cc
    9.14 [] Implement a hash function for chess: NotFound!

#### [Chapter 10. Sorting](10_Sorting/README.md)
    10.01 intersect_sorted_arrays.cc
    10.02 two_sorted_arrays_merge.cc
    10.03 remove_duplicates.cc
    10.04 calendar_rendering.cc
    10.05 interval_add.cc
    10.06 intervals_union.cc
    10.07 group_equal_entries.cc
    10.08 is_array_dominated.cc
    10.09 sort_list.cc
    10.10 find_salary_threshold.cc
        "C++: h_index.cc
        "C++: smallest_nonconstructible_value.cc

#### [Chapter 11. Binary Search Trees](11_BinarySearchTrees/README.md)
    11.00 search_in_bst.cc
    11.01 is_tree_a_bst.cc
    11.02 search_first_greater_value_in_bst.cc
    11.03 k_largest_values_in_bst.cc
    11.04 lowest_common_ancestor_in_bst.cc
    11.05 bst_from_preorder.cc
    11.06 minimum_distance_3_sorted_arrays.cc
    11.07 a_b_sqrt2.cc

    11.09 bst_from_sorted_array.cc

    11.11 descendant_and_ancestor_in_bst.cc
    11.12 range_lookup_in_bst.cc
    11.13 adding_credits.cc

#### [Chapter 12. Recursion](12_Recursion/README.md)
    12.00 euclidean_gcd.cc
    12.01 hanoi.cc
    12.02 n_queens.cc
    12.03 permutations.cc
    12.04 power_set.cc
    12.05 combinations.cc
    12.06 enumerate_balanced_parentheses.cc
    12.07 enumerate_palindromic_decompositions.cc
    12.08 enumerate_trees.cc
    12.09 sudoku_solve.cc
    12.10 gray_code.cc
    12.11 []

#### [Chapter 13. Dynamic Programming](13_DynamicProgramming/README.md)
    13.00 fibonacci.cc &
          max_sum_subarray.cc
    13.01 number_of_score_combinations.cc
    13.02 levenshtein_distance.cc
    13.03 number_of_traversals_matrix.cc
    13.04 binomial_coefficients.cc
    13.05 is_string_in_matrix.cc
    13.06 knapsack.cc
    13.07 is_string_decomposable_into_words.cc
    13.08 minimum_weight_path_in_a_triangle.cc
    13.09 picking_up_coins.cc
    13.10 number_of_traversals_staircase.cc
    13.11 pretty_printing.cc
    13.12 longest_nondecreasing_subsequence.cc

#### [Chapter 14. Greedy Algorithms and Invariants](14_GreedyInvariants/README.md)
    14.00 making_change.cc
    14.01 task_pairing.cc
    14.02 minimum_waiting_time.cc
    14.03 minimum_points_covering_intervals.cc      
          two_sum.cc
    14.04 three_sum.cc
    14.05 majority_element.cc
    14.06 refueling_schedule.cc
    14.07 max_trapped_water.cc
          container_with_most_water.cc
    14.08 largest_rectangle_under_skyline.cc

#### [Chapter 15. Graphs](15_Graphs/README.md)
    15.01 search_maze.cc
    15.02 matrix_connected_regions.cc
    15.03 matrix_enclosed_regions.cc
    15.04 deadlock_detection.cc
    15.05 graph_clone.cc
    15.06 is_circuit_wirable.cc
    15.07 string_transformability.cc
    15.08 max_teams_in_photograph.cc
    15.09 [] Compute a shortest path with fewest edges: NotFound!

#### [Chapter 16. Parallel Computing](16_ParallelComputing/README.md)
    16.00 semaphore.h

### Part II. Domain Specific Problems

#### [Chapter 17. Design Problems](17_DesignProblems/README.md)
    *(study notes only)*

#### [Chapter 18. Language Questions](18_LanguageQuestions/README.md)
    *(study notes only)*

#### [Chapter 19. Object-Oriented Design](19_ObjectOrientedDesign/README.md)
    *(study notes only)*

#### [Chapter 20. Common Tools](20_CommonTools/README.md)
    *(study notes only)*

### Part III. The Honors Class

#### [Chapter 21. Honors Class](21_HonorsClass/README.md)
    21.01 gcd.cc
    21.02 first_missing_positive_entry.cc
    21.03 buy_and_sell_stock_k_times.cc
    21.04 max_product_all_but_one.cc
    21.05 longest_increasing_subarray.cc
    21.06 rotate_array.cc
    21.07 rook_attack.cc
    21.08 left_right_justify_text.cc
    21.09 zip_list.cc
    21.10 copy_posting_list.cc
    21.11 longest_substring_with_matching_parentheses.cc
    21.12 max_of_sliding_window.cc
    21.13 tree_postorder.cc
    21.14 bonus.cc
    21.15 search_unknown_length_array.cc
    21.16 kth_largest_element_in_two_sorted_arrays.cc
    21.17 kth_largest_element_in_long_array.cc
    21.18 element_appearing_once.cc
    21.19 line_through_most_points.cc
    21.20 []Find the shortest unique prefix: NotFound!
    21.21 []Find the most visited pages in a window: NotFound!
    21.22 sorted_list_to_bst.cc
    21.23 bst_to_sorted_list.cc
    21.24 bst_merge.cc
    21.25 [] The view from above: NotFound!      
    21.26 regular_expression.cc
    21.27 insert_operators_in_string.cc       
    21.28 count_inversions.cc
    21.29 drawing_skyline.cc
    21.30 defective_jugs.cc
    21.31 maximum_subarray_in_circular_array.cc
    21.32 max_safe_height.cc
    21.33 max_submatrix.cc & max_square_submatrix.cc
    21.34 huffman_coding.cc
    21.35 max_water_trappable.cc!!
          trapping_rainwater.cc
    21.36 [] Search for a pair-sum in an abs-sorted array: NotFound!
    21.37 search_frequent_items.cc
    21.38 longest_subarray_with_sum_constraint.cc
    21.39 road_network.cc
    21.40 arbitrage.cc
