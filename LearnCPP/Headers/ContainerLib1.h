#pragma once
namespace ContainerLib1 {


/*------------- vector -------------*/

void Test_vector_basic_apis();

/*------------- sequential container in general -------------*/

void Test_RemoveEveryOtherInSeqContainer();

/*------------- set -------------*/

void Test_set_lower_bound();
void Test_set_is_ordered();


/*------------- unordered_set -------------*/

void Test_unordered_set1();
void Test_unordered_set_hash_function_specialization();


/*------------- map -------------*/

void Test_ordered_map();


/*------------- stack -------------*/

void Test_stack();
void Test_stack_emplace();


/*--------- Priority Queue --------*/


void Test_priority_queue();

}