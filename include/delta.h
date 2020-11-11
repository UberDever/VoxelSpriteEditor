//
// Created by uberdever on 27.07.2020.
//
// Delta is commonly used data structures and functions related to them
#ifndef ALPHABETA_DELTA_H
#define ALPHABETA_DELTA_H

/*
 * For all array data structures:
 *  // arr_t is primitive/structure type for vec/stack/queue/deck.
 *  // For hashtables it is ht_bucket(arr_t) / htl_bucket(arr_t) accordingly
 *  arr_t* ds = NULL;
 *  ds_new(ds);
 *  ds_some_func(ds, ...);
 *  ds_free(ds);
 */

#include "delta/vec.h"
#include "delta/hash_table.h"
#include "delta/stack.h"
#include "delta/queue.h"
#include "delta/deck.h"

#define arr_copy_contents(src, dst, ds_type) \
do {                                         \
    const uint copy_sz__ = ds_type##_get_cap(src) * sizeof(*src) + _##ds_type##_get_raw_elements_num * sizeof(uint);                                         \
    dst = malloc(copy_sz__);\
    memcpy(dst, _##ds_type##_get_raw_data(src), copy_sz__);\
    dst = (void*)((uint*)(dst) + _##ds_type##_get_raw_elements_num);\
} while (0)                                  \
/* 1) dst must be NULL
   2) ds_type is first word in function name (vec, ht, stack ...) */


#define arr_delete_contents(arr, cap, size) \
do {                             \
    for (uint i_ = 0; i_ < cap; i_++)       \
        if (arr[i_])                        \
        {                                   \
            free(arr[i_]);                      \
            arr[i_] = NULL;                 \
        } \
    size = 0;\
} while (0)

#endif //ALPHABETA_DELTA_H
