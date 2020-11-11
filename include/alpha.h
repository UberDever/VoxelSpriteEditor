//
// Created by uberdever on 26.07.2020.
//
// Alpha is general utility header, contain all important structs and functions, used across all project
#ifndef ALPHABETA_ALPHA_H
#define ALPHABETA_ALPHA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/*
 * DEBUG mode
 */

#define DEBUG 1

#if DEBUG
#define DEBUG_CHECK(COND, MSG) \
        do {if (COND) error(MSG);} while(0)
#else
#define DEBUG_CHECK(COND, MSG)
#endif

/*
 * Type defines
 */

#if defined _WIN32
typedef unsigned int uint;
#else
#include <sys/types.h>
#endif

typedef unsigned char bool;
typedef unsigned char uint8;
typedef signed char int8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef signed long long int int64;
typedef unsigned int uint32;
typedef unsigned long long int uint64;
typedef float float32;
typedef double float64;

#define true 1
#define false 0

/*
 * Color handling
 */

typedef struct color
{
    union {
        uint as_uint;
#ifdef LITTLE_ENDIAN
        struct {uint8 r; uint8 g; uint8 b; uint8 a;};
#else
        struct {uint8 a; uint8 b; uint8 g; uint8 r;};
#endif
    };
} color;
enum colors {
    color_red           = 0xFF0000FF,
    color_green         = 0xFF00FF00,
    color_blue          = 0xFFFF0000,
    color_white         = 0xFFFFFFFF,
    color_black         = 0x0,
    color_magenta       = 0xFFA100FF,
    color_cyan          = 0xFFFFFF00,
    color_yellow        = 0xFF00FFFF,
    color_orange        = 0xFF00A1FF,
    color_gray          = 0xFF808080,
    color_light_gray    = 0xFFD3D3D3,
    color_golden        = 0xFF00D7FF,
    color_dark_golden   = 0xFF20A5DA
};

#ifdef LITTLE_ENDIAN
#define COLOR(x) ((struct color){color_##x}) //Available colors: r, g, b, white, black, magenta, cyan, yellow, orange, gray, light_gray, golden, dark_golden
#else
#define COLOR(x) ((color){~(color_##x) & 0xFFFFFFFF})
#endif

#define COLOR2FLOAT(c_src, c_dst) \
    (c_dst).a = (float32)(c_src).a / 255.f;\
    (c_dst).r = (float32)(c_src).r / 255.f;\
    (c_dst).g = (float32)(c_src).g / 255.f;\
    (c_dst).b = (float32)(c_src).b / 255.f;\

/*
 *  Optimizing conditions
 */
#define likely(x)   __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)

/*
 *  Logging
 */

#define TO_STR(x) (#x)

#define ERROR_BUFFER_SIZE   512
#define DEBUG_BUFFER_SIZE   512
#define WARNING_BUFFER_SIZE 512

/* Register functions for events */
void error_attach_handler(void(*func)(const char*));
void warning_attach_handler(void(*func)(const char*));
void debug_attach_handler(void(*func)(const char*));

/* Call registered functions */
void error_process(const char*);
void warning_process(const char*);
void debug_process(const char*);

char error_buf[ERROR_BUFFER_SIZE];
char error_str[ERROR_BUFFER_SIZE];

char warning_buf[WARNING_BUFFER_SIZE];
char warning_str[WARNING_BUFFER_SIZE];

char debug_buf[DEBUG_BUFFER_SIZE];
char debug_str[DEBUG_BUFFER_SIZE];

#define error(MSG, ...) { \
  snprintf(error_str,(ERROR_BUFFER_SIZE-1), "[ERROR] <%s:%s:%i> ", __FILE__, __func__, __LINE__); \
  snprintf(error_buf,(ERROR_BUFFER_SIZE-1), MSG, ##__VA_ARGS__); strcat(error_str, error_buf);    \
  error_process(error_str); }

#define warning(MSG, ...) { \
  snprintf(warning_str,(WARNING_BUFFER_SIZE-1), "[WARNING] <%s:%s:%i> ", __FILE__, __func__, __LINE__); \
  snprintf(warning_buf,(WARNING_BUFFER_SIZE-1), MSG, ##__VA_ARGS__); strcat(warning_str, warning_buf);  \
  warning_process(warning_str); }

#if DEBUG
#define debug(MSG, ...) { \
  snprintf(debug_str,(DEBUG_BUFFER_SIZE-1), "[DEBUG] <%s:%s:%i> ", __FILE__, __func__, __LINE__); \
  snprintf(debug_buf,(DEBUG_BUFFER_SIZE-1), MSG, ##__VA_ARGS__); strcat(debug_str, debug_buf);    \
  debug_process(debug_str); }
#define debug_int(x) debug("%d", x)
#else
#define debug(MSG, ...)
#define debug_int(x)
#endif

#define ALLOC_CHECK(PTR) { if(unlikely(PTR == NULL)) { error("Failed to allocate memory"); } }

/*
 *  Useful macros
 */

//Bitset macros from http://c-faq.com/misc/bitsets.html
#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET_SET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITSET_CLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITSET_TEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITSET_SLOT_NUMBER(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)

#define ARR_ALLOC(pointer, size) (pointer) = calloc((size), sizeof(*pointer))
#define ARR_ONE_DIM_LEN(x) (sizeof(x) / sizeof(*x))
#define ARR_TWO_DIM_LEN(x) (sizeof(x) / sizeof(**x))

#define ZERO(object) memset(&object, 0, sizeof(object))

#define MIN(a,b) (((a)<(b))?(a):(b)) //standart freeBSD / debian macros
#define MAX(a,b) (((a)>(b))?(a):(b))

int tmp_swap_int_var_; // Little hack here
float32 tmp_swap_float_var_; // Little hack here
#define SWAP_INT(a, b) (tmp_swap_int_var_ = a, a = b, b = tmp_swap_int_var_)
#define SWAP_FLOAT(a, b) (tmp_swap_float_var_ = a, a = b, b = tmp_swap_float_var_)

#define PTR_TYPE(type) typedef type* type##ptr
typedef void (*p_void_func)(void); //convenient pointer to void function(void)

/*
 *  Keyboard and mouse handling
 */

extern bool is_program_running; //For SDL_QUIT event

void kbd_set_state_array(const uint8* arr);

void events_process();
void events_set_handler(p_void_func func);

bool kbd_key_pressed(SDL_Keycode code); //for all numeric/alphabetic keys, true until sdl_keyup
bool kbd_key_held(SDL_Keycode code); //until keyrepeat event appears, return false
bool kbd_key_pushed(SDL_Keycode code); //single push
bool kbd_key_released(SDL_Keycode code);
bool kbd_system_key_pressed(SDL_Keycode code); //for all system keys, including F1-F12
bool kbd_system_key_held(SDL_Keycode code); //until keyrepeat event appears, return false
bool kbd_system_key_pushed(SDL_Keycode code);//single push
bool kbd_system_key_released(SDL_Keycode code);

bool mouse_enter_window();
bool mouse_leave_window();
bool mouse_left_pressed();
bool mouse_right_pressed();
bool mouse_middle_pressed();
bool mouse_left_down();
bool mouse_right_down();
bool mouse_middle_down();
bool mouse_scroll_up();
bool mouse_scroll_down();
int mouse_get_pos_x();
int mouse_get_pos_y();
int mouse_get_rel_x();
int mouse_get_rel_y();

#endif //ALPHABETA_ALPHA_H
