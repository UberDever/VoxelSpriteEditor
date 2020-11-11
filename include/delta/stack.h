//
// Created by uberdever on 06.08.2020.
//

#ifndef ALPHABETA_STACK_H
#define ALPHABETA_STACK_H

#include "vec.h"

/*
 * Stack is just interface to vector functions
 * Designed for handling pointer data, but otherwise work too
 * Contents of pointed data must be cleared separately via arr_delete_contents
 */
#if 0
#define _stack_get_raw_elements_num _vec_get_raw_elements_num

#define stack_is_empty(st) vec_is_empty(st)
#define stack_is_full(st) vec_is_full(st)
#define stack_get_cap(st) vec_get_cap(st)
#define stack_get_size(st) vec_get_size(st)

#define stack_new(st, c) vec_new(st, c)
#define stack_push(st, el) vec_push(st, el)
#define stack_pop(st) vec_pop(st)
#define stack_top(st) vec_last(st)
#define stack_free(st) vec_free(st)
#endif
#endif //ALPHABETA_STACK_H
