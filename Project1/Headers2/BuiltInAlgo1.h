#pragma once
namespace BuiltInAlgo1 {

//====== non-modifying sequence operations

void Test_allof_noneof_anyof();
void Test_for_each();
void Test_for_each_n();
void Test_count_if();
void Test_mismatch();
void Test_find_end();
void Test_find_first_of();
void Test_adjacent_find();
void Test_search();
void Test_search_n();


//====== Modifying sequence operations

void Test_copy_copyif();
void Test_copy_n();
void Test_copy_backward();
void Test_move();
void Test_move_n();
void Test_fill();
void Test_fill_n();
void Test_generate();
void Test_generate_n();
void Test_transform();
void Test_remove();
void Test_remove_copy_if();
void Test_replace_if();
void Test_replace_copy_if();
void Test_swap();
void Test_swap_ranges();
void Test_iter_swap();
void Test_reverse();
void Test_reverse_copy();
void Test_rotate();
void Test_rotate_copy();
#if 0
void Test_shift();  // since C++20
#endif
void Test_random_shuffle();
void Test_sample();
void Test_unique();
void Test_unique_copy();
void Test_is_partitioned();
void Test_partition();
void Test_partition_copy();
void Test_stable_partition();
void Test_partition_point();


//====== Sorting operations

void Test_is_sorted();
void Test_is_sorted_until();
void Test_is_sorted_until2();
void Test_sort();
void Test_partial_sort();
void Test_partial_sort_copy();
void Test_stable_sort();
void Test_nth_element();

//====== Binary search operations on sorted ranges

void Test_lower_bound();
void Test_lower_bound_comp_function_arg_order();
void Test_upper_bound();
void Test_binary_search();
void Test_equal_range();
void Test_equal_range2();

//====== Other operations on sorted ranges

void Test_merge();
#if 0
void Test_range_merge();  // since C++20
#endif
void Test_inplace_merge();

//====== Set operations  (on sorted ranges)

void Test_include();
void Test_set_difference();
void Test_set_intersection();
void Test_set_symmetric_difference();
void Test_set_union();

//====== Heap operations

void Test_is_heap();
void Test_is_heap_until();
void Test_make_heap();
void Test_push_heap();
void Test_pop_heap();
void Test_heap_sort();

//===== Minimum/maximum operations

void Test_max();
void Test_max_element();
void Test_minmax_element();
void Test_clamp();

//===== Comparison operations

void Test_equal();
void Test_lexi_comp();

//===== Permutation operations

void Test_is_permu();
void Test_next_permu();
void Test_prev_permu();

//===== Numeric operations

void Test_iota();
void Test_accumulate();
void Test_accumulate_return_type();
void Test_inner_product();
void Test_adjacent_difference();
void Test_partial_sum();
void Test_reduce();
void Test_exclusive_inclusive_scan();
void Test_transform_reduce();
void Test_tramsform_exclusive_inclusive_scan();

//===== Operations on uninitialized memory

void Test_uninitialized_copy();
void Test_uninitialized_copy_n();
void Test_uninitialized_copy_n2();
void Test_uninitialized_fill();
void Test_uninitialized_fill_n();
void Test_uninitialized_move();
void Test_uninitialized_move_n();
void Test_uninitialized_default_construct_construct_n();
void Test_uninitialized_value_construct_construct_n();
void Test_destroy();
}