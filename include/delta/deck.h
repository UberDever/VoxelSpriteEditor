//
// Created by uberdever on 07.08.2020.
//

#ifndef ALPHABETA_DECK_H
#define ALPHABETA_DECK_H

/*
 * Fixed size deck, with basic operations
 * Designed for handling pointer data, but otherwise work too
 * Contents of pointed data must be cleared separately via arr_delete_contents
 */

#define DECK_RAW_ELEMENTS_NUM 3

#define _deck_get_raw_data(dk) ((uint*)(dk) - DECK_RAW_ELEMENTS_NUM)
#define _deck_get_raw_elements_num DECK_RAW_ELEMENTS_NUM
#define _deck_get_front(dk) (*((uint*)(dk) - 2))
#define _deck_get_back(dk) (*((uint*)(dk) - 1))
#define _deck_get_cap(dk) (*_deck_get_raw_data(dk))

#define deck_new(dk, c)\
do {\
     (dk) = malloc((c + 1) * sizeof(*dk) + DECK_RAW_ELEMENTS_NUM * sizeof(uint));\
     ALLOC_CHECK(dk);\
     dk = (void*)((uint*)(dk) + DECK_RAW_ELEMENTS_NUM);\
     _deck_get_cap(dk) = c + 1;\
     _deck_get_front(dk) = 0;\
     _deck_get_back(dk) = 1;\
} while(0) // Avoiding use of GCC extensions

#define deck_get_raw_data(dk) _deck_get_raw_data(dk)
#define deck_get_cap(dk) ((*_deck_get_raw_data(dk)) - 1)
#define deck_get_size(dk) \
(\
    (_deck_get_front(dk) >= _deck_get_back(dk)) ?\
        (int)(_deck_get_cap(dk) + _deck_get_back(dk) - _deck_get_front(dk) - 1)\
    :\
        (int)(_deck_get_back(dk) - _deck_get_front(dk) - 1)\
)

#define deck_is_full(dk) (_deck_get_front(dk) == _deck_get_back(dk))
//#define deck_is_empty(dk) ( ( (int)( (_deck_get_front(dk) - _deck_get_back(dk) ) % _deck_get_cap(dk) ) ) == -1 || \
//                            ( (int)( (_deck_get_front(dk) - _deck_get_back(dk) ) % _deck_get_cap(dk) ) ) == _deck_get_cap(dk) - 1 )

#define deck_is_empty(dk) ( _deck_get_front(dk) >= _deck_get_back(dk) ? \
                            (_deck_get_front(dk) == _deck_get_cap(dk) - 1) && (_deck_get_back(dk) == 0) :\
                            _deck_get_front(dk) == (_deck_get_back(dk) - 1))

#define deck_free(dk) (free(_deck_get_raw_data(dk)), dk = NULL )

#define deck_push_front(dk, el) \
do {\
    if (!deck_is_full(dk)) {\
        (dk)[_deck_get_front(dk)] = el;\
        _deck_get_front(dk) == 0 ? _deck_get_front(dk) = _deck_get_cap(dk) - 1 : \
                                   _deck_get_front(dk)--;                        \
    }\
} while(0)

#define deck_push_back(dk, el) \
do {                           \
    if (!deck_is_full(dk)) {   \
        (dk)[_deck_get_back(dk)] = el;                           \
        _deck_get_back(dk) = (_deck_get_back(dk) + 1) % _deck_get_cap(dk);                       \
    }                               \
} while(0)

#define deck_pop_front(dk) \
(                          \
    deck_is_empty(dk) ? 0 :   \
    ((_deck_get_front(dk) = (_deck_get_front(dk) + 1) % _deck_get_cap(dk)), (dk)[_deck_get_front(dk)])                       \
)

#define deck_pop_back(dk) \
(                          \
    deck_is_empty(dk) ? 0 :   \
    _deck_get_back(dk) == 0 ? _deck_get_back(dk) = _deck_get_cap(dk) - 1, (dk)[_deck_get_back(dk)] : \
                              (dk)[--_deck_get_back(dk)] \
)

#define deck_back(dk) (deck_is_empty(dk) ? 0 :((dk)[_deck_get_back(dk) == 0 ? _deck_get_cap(dk) - 1 : _deck_get_back(dk) - 1]))
#define deck_front(dk) ( deck_is_empty(dk) ? 0 : ((dk)[(_deck_get_front(dk) + 1 ) % _deck_get_cap(dk)]))

#endif //ALPHABETA_DECK_H
