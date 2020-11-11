//
// Created by uberdever on 06.08.2020.
//

#ifndef ALPHABETA_QUEUE_H
#define ALPHABETA_QUEUE_H

#include "deck.h"

/*
 * Queue is just interface for deck functions
 * Designed for handling pointer data, but otherwise work too
 * Contents of pointed data must be cleared separately via arr_delete_contents
 */

#define _queue_get_raw_elements_num _deck_get_raw_elements_num

#define queue_get_cap(q) deck_get_cap(q)
#define queue_get_size(q) deck_get_size(q)
#define queue_is_full(q) deck_is_full(q)
#define queue_is_empty(q) deck_is_empty(q)

#define queue_new(q, c) deck_new(q, c)
#define queue_free(q) deck_free(q)

#define queue_push(q, el) deck_push_back(q, el)
#define queue_pop(q) deck_pop_front(q)
#define queue_front(q) deck_front(q)

#endif //ALPHABETA_QUEUE_H
