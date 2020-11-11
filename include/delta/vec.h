//
// Created by uberdever on 28.07.2020.
//

#ifndef ALPHABETA_VEC_H
#define ALPHABETA_VEC_H

#include "../alpha.h"

/*
 * Expandable array like std::vector, but more close to c array
 * Designed for handling pointer data, but otherwise work too
 * Contents of pointed data must be cleared separately via arr_delete_contents
 */

#if 0 //Previous, less elegant version
#define VEC_MIN_CAP 16

#define VEC_BASE       \
    struct             \
    {                  \
        uint capacity; \
        uint size;     \
    }

#define vec(T)     \
    struct vec_##T \
    {              \
        VEC_BASE;  \
        T data[];  \
    }

#define vec_reserve(T, sz)                                                     \
    ({                                                                         \
        struct vec_##T *v__ = malloc(sizeof(struct vec_##T) + sizeof(T) * sz); \
        ALLOC_CHECK(v__);                                                      \
        v__->size = 0;                                                         \
        v__->capacity = sz;                                                    \
        v__;                                                                   \
    })

#define vec_new(T, arr...)                                                           \
    ({                                                                               \
        const T _arr[] = {arr};                                                      \
        const uint size = ARR_ONE_DIM_LEN(_arr);                                     \
        const uint capacity = MAX(size, VEC_MIN_CAP);                                \
        struct vec_##T *v__ = malloc(sizeof(struct vec_##T) + sizeof(T) * capacity); \
        ALLOC_CHECK(v__);                                                            \
        v__->size = size;                                                            \
        v__->capacity = capacity;                                                    \
        memcpy(v__->data, _arr, sizeof(_arr));                                       \
        v__;                                                                         \
    })

#define vec_free(v) \
    {               \
        free(v);    \
        v = NULL;   \
    }

#define vec_free_contents(v)           \
    for (uint i = 0; i < v->size; i++) \
    {                                  \
        free(v->data[i]);              \
        v->data[i] = NULL;             \
    }

#define vec_pop(v)                                    \
    ({                                                \
        DEBUG_CHECK(v->size <= 0, "Vector is empty"); \
        v->data[--v->size];                           \
    })

#define vec_ptr(v, i)                                                          \
    ({                                                                         \
        DEBUG_CHECK(i >= v->size || i < 0, "Vector size doesn't match index"); \
        (v->data + i);                                                         \
    })

#define vec_at(v, i) (*(vec_ptr(v, i)))

#define vec_push(v, (el))                                                        \
    ({                                                                         \
        if (v->size >= v->capacity)                                            \
        {                                                                      \
            v->capacity *= 2;                                                  \
            v = realloc(v, sizeof(VEC_BASE) + v->capacity * sizeof(*v->data)); \
        }                                                                      \
        v->data[v->size++] = (el);                                               \
        v;                                                                     \
    })

#define vec_is_empty(v) \
    ((bool)(v->size == 0))

#define vec_is_full(v) \
    ((bool)(v->size >= v->capacity))
#endif

#if 0 //Macros, far less elegant version, than version above
#define VEC_RAW_ELEMENTS_NUM 2

#define vec_is_empty(v) ((bool)(vec_get_size(v) == 0))
#define vec_is_full(v) ((bool)(vec_get_size(v) >= vec_get_cap(v)))
#define vec_get_cap(v) (*_vec_get_raw_data(v))
#define vec_get_size(v) (*((uint *)(v)-1))

#define _vec_get_raw_data(v) ((uint *)(v)-VEC_RAW_ELEMENTS_NUM)
#define _vec_get_raw_elements_num VEC_RAW_ELEMENTS_NUM

#define vec_new(v, c)                                                         \
    do                                                                        \
    {                                                                         \
        (v) = malloc(c * sizeof(*(v)) + VEC_RAW_ELEMENTS_NUM * sizeof(uint)); \
        ALLOC_CHECK(v);                                                       \
        v = (void *)((uint *)(v) + VEC_RAW_ELEMENTS_NUM);                     \
        vec_get_cap(v) = c;                                                   \
        vec_get_size(v) = 0;                                                  \
    } while (0) // Avoiding use of GCC extensions

#define vec_push(v, el)                                                                          \
    do                                                                                           \
    {                                                                                            \
        if (vec_is_full(v))                                                                      \
        {                                                                                        \
            vec_get_cap(v) *= 2;                                                                 \
            v = realloc(_vec_get_raw_data(v), vec_get_cap(v) * sizeof(*(v)) + 2 * sizeof(uint)); \
            v = (void *)((uint *)(v) + VEC_RAW_ELEMENTS_NUM);                                    \
        }                                                                                        \
        memcpy(&(v)[vec_get_size(v)++], &el, sizeof(el));                                        \
    } while (0)

#define vec_pop(v) (vec_get_size(v) ? (v)[vec_get_size(v)-- - 1] : 0)

#define vec_last(v) ((v)[vec_get_size(v) - 1])

#define vec_free(v) (free(_vec_get_raw_data(v)), v = NULL)

#define vec_clear(v) (memset(v, 0, sizeof(*v) * vec_get_cap(v)))

#endif

#define vec_register(T)                                                 \
    typedef struct v_##T##_t                                            \
    {                                                                   \
        uint size, cap;                                                 \
        void (*new)(struct v_##T##_t *, uint);                          \
        void (*free)(struct v_##T##_t *);                               \
        void (*push)(struct v_##T##_t * this, T element);               \
        T(*pop)                                                         \
        (struct v_##T##_t * this);                                      \
        bool (*full)(struct v_##T##_t * this);                          \
        bool (*empty)(struct v_##T##_t * this);                         \
        T(*back)                                                        \
        (struct v_##T##_t * this);                                      \
                                                                        \
        T *data;                                                        \
    } v_##T##_t;                                                        \
                                                                        \
    static void vec_##T##_new(struct v_##T##_t *this, uint cap)         \
    {                                                                   \
        ARR_ALLOC(this->data, cap);                                     \
        ALLOC_CHECK(this->data);                                        \
        this->cap = cap;                                                \
        this->size = 0;                                                 \
    }                                                                   \
    static void vec_##T##_free(struct v_##T##_t *this)                  \
    {                                                                   \
        free(this->data);                                               \
        this->cap = 0;                                                  \
        this->size = 0;                                                 \
    }                                                                   \
    static void vec_##T##_push(struct v_##T##_t *this, T element)       \
    {                                                                   \
        if (this->size >= this->cap)                                    \
        {                                                               \
            this->cap *= 2;                                             \
            this->data = realloc(this->data, this->cap * sizeof(T));    \
        }                                                               \
        this->data[this->size++] = (element);                             \
    }                                                                   \
    static T vec_##T##_pop(struct v_##T##_t *this)                      \
    {                                                                   \
        return this->size ? this->data[--this->size] : this->data[0];   \
    }                                                                   \
    static bool vec_##T##_full(struct v_##T##_t *this)                  \
    {                                                                   \
        return this->size >= this->cap;                                 \
    }                                                                   \
    static bool vec_##T##_empty(struct v_##T##_t *this)                 \
    {                                                                   \
        return this->size;                                              \
    }                                                                   \
    static T vec_##T##_back(struct v_##T##_t *this)                     \
    {                                                                   \
        return this->size ? this->data[this->size - 1] : (T){0}; \
    }

#define vec_new(T, name, cap)       \
    (name).new = vec_##T##_new;     \
    (name).free = vec_##T##_free;   \
    (name).pop = vec_##T##_pop;     \
    (name).push = vec_##T##_push;   \
    (name).empty = vec_##T##_empty; \
    (name).full = vec_##T##_full;   \
    (name).back = vec_##T##_back;   \
    (name).new(&(name), cap);

#define vec_new_ptr(T, name, cap)       \
    (name) = malloc(sizeof(v_##T##_t)); \
    (name)->new = vec_##T##_new;        \
    (name)->free = vec_##T##_free;      \
    (name)->pop = vec_##T##_pop;        \
    (name)->push = vec_##T##_push;      \
    (name)->empty = vec_##T##_empty;    \
    (name)->full = vec_##T##_full;      \
    (name)->back = vec_##T##_back;      \
    (name)->new (name, cap);

#define vec_free(v) (v).free(&(v))
#define vec_push(v, el) (v).push(&(v), (el))
#define vec_pop(v) (v).pop(&(v))
#define vec_empty(v) (v).empty(&(v))
#define vec_full(v) (v).full(&(v))
#define vec_back(v) (v).back(&(v))

#define vec_free_ptr(v) \
    (v)->free(v);         \
    free(v);

#endif //ALPHABETA_VEC_H
