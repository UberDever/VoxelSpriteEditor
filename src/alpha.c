//
// Created by uberdever on 27.07.2020.
//

#include "../include/alpha.h"

/*
 *  Logging
 */

typedef void (*logging_func_t)(const char*);

#define MAX_AT_FUNCS 32
static logging_func_t error_funcs[MAX_AT_FUNCS];
static logging_func_t warn_funcs[MAX_AT_FUNCS];
static logging_func_t debug_funcs[MAX_AT_FUNCS];

static int num_error_funcs = 0;
static int num_warn_funcs = 0;
static int num_debug_funcs = 0;

void error_attach_handler(void(*func)(const char*)) {
    if (num_error_funcs == MAX_AT_FUNCS) {
        warning("Cannot register more than maximum of %i error functions", MAX_AT_FUNCS);
        return;
    }
    error_funcs[num_error_funcs] = (logging_func_t)func;
    num_error_funcs++;
}

void warning_attach_handler(void(*func)(const char*)) {
    if (num_warn_funcs == MAX_AT_FUNCS) {
        warning("Cannot register more than maximum of %i warning functions", MAX_AT_FUNCS);
        return;
    }
    warn_funcs[num_warn_funcs] = (logging_func_t)func;
    num_warn_funcs++;
}

void debug_attach_handler(void(*func)(const char*)) {
    if (num_debug_funcs == MAX_AT_FUNCS) {
        warning("Cannot register more than maximum of %i debug functions", MAX_AT_FUNCS);
        return;
    }
    debug_funcs[num_debug_funcs] = (logging_func_t)func;
    num_debug_funcs++;
}

void error_process(const char* str) {
    for (int i = 0; i < num_error_funcs; i++) {
        error_funcs[i](str);
    }
}

void warning_process(const char* str) {
    for (int i = 0; i < num_warn_funcs; i++) {
        warn_funcs[i](str);
    }
}

void debug_process(const char* str) {
    for (int i = 0; i < num_debug_funcs; i++) {
        debug_funcs[i](str);
    }
}

/*
 *  Keyboard and mouse handling
 */

static p_void_func event_handler = NULL;

static const uint8* kbd_state;
static bool kbd_is_key_repeat = false;
static bool kbd_key_state = SDL_RELEASED;
static bool kbd_is_system_key = false;
static bool kbd_is_key_pressed = false;
static SDL_Keycode kbd_current_keycode = SDLK_UNKNOWN;

static bool mouse_is_entered_window = false;
static bool mouse_is_leaved_window = false;
static bool mouse_is_left_pressed = false;
static bool mouse_is_right_pressed = false;
static bool mouse_is_middle_pressed = false;
static bool mouse_is_left_down = false;
static bool mouse_is_right_down = false;
static bool mouse_is_middle_down = false;
static int mouse_wheel_x = 0;
static int mouse_wheel_y = 0;
static int mouse_rel_x = 0;
static int mouse_rel_y = 0;
static int mouse_x = 0;
static int mouse_y = 0;

bool is_program_running = true;

void kbd_set_state_array(const uint8 *arr) {
    kbd_state = arr;
}

void events_set_handler(p_void_func func) {
    event_handler = func;
}

static void events_clear_state()
{
    mouse_is_left_down = false;
    mouse_is_right_down = false;
}

void events_process()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (unlikely(event.type == SDL_QUIT))
        {
            is_program_running = false;
            return;
        }
        switch (event.type) {
            case SDL_KEYUP:
            {
                kbd_is_system_key = (event.key.keysym.sym & 0x40000000) >> 30;
                kbd_current_keycode = event.key.keysym.sym;
                kbd_is_key_repeat = event.key.repeat;
                kbd_key_state = SDL_RELEASED;
                break;
            }
            case SDL_KEYDOWN:
            {
                kbd_is_system_key = (event.key.keysym.sym & 0x40000000) >> 30;
                kbd_is_key_repeat = event.key.repeat;
                kbd_key_state = SDL_PRESSED;
                kbd_is_key_pressed = true;
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                SDL_CaptureMouse(SDL_TRUE);
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT: mouse_is_left_down = mouse_is_left_pressed = true; break;
                    case SDL_BUTTON_RIGHT: mouse_is_right_down = mouse_is_right_pressed = true; break;
                    case SDL_BUTTON_MIDDLE: mouse_is_middle_down = mouse_is_middle_pressed = true; break;
                }
                mouse_x = event.button.x;
                mouse_y = event.button.y;
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                SDL_CaptureMouse(SDL_FALSE);
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT: mouse_is_left_pressed = false; break;
                    case SDL_BUTTON_RIGHT: mouse_is_right_pressed = false; break;
                    case SDL_BUTTON_MIDDLE: mouse_is_middle_pressed = false; break;
                }
                mouse_x = event.button.x;
                mouse_y = event.button.y;
                break;
            }
            case SDL_MOUSEMOTION:
            {
                mouse_x = event.motion.x;
                mouse_y = event.motion.y;
                mouse_rel_x = event.motion.xrel;
                mouse_rel_y = event.motion.yrel;
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                mouse_wheel_x = event.wheel.x;
                mouse_wheel_y = event.wheel.y;
                break;
            }
            case SDL_WINDOWEVENT:
            {
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_ENTER: mouse_is_entered_window = true; break;
                    case SDL_WINDOWEVENT_LEAVE: mouse_is_leaved_window = true; break;
#if 0
                    case SDL_WINDOWEVENT_SHOWN:
                        SDL_Log("Window %d shown", event.window.windowID);
                        break;
                    case SDL_WINDOWEVENT_HIDDEN:
                        SDL_Log("Window %d hidden", event.window.windowID);
                        break;
                    case SDL_WINDOWEVENT_EXPOSED:
                        SDL_Log("Window %d exposed", event.window.windowID);
                        break;
                    case SDL_WINDOWEVENT_MOVED:
                        SDL_Log("Window %d moved to %d,%d",
                                event.window.windowID, event.window.data1,
                                event.window.data2);
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        SDL_Log("Window %d resized to %dx%d",
                                event.window.windowID, event.window.data1,
                                event.window.data2);
                        break;
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        SDL_Log("Window %d size changed to %dx%d",
                                event.window.windowID, event.window.data1,
                                event.window.data2);
                        break;
                    case SDL_WINDOWEVENT_MINIMIZED:
                        SDL_Log("Window %d minimized", event.window.windowID);
                        break;
                    case SDL_WINDOWEVENT_MAXIMIZED:
                        SDL_Log("Window %d maximized", event.window.windowID);
                        break;
                    case SDL_WINDOWEVENT_RESTORED:
                        SDL_Log("Window %d restored", event.window.windowID);
                        break;
                    case SDL_WINDOWEVENT_ENTER:
                        SDL_Log("Mouse entered window %d",
                                event.window.windowID);
                        mouse_is_entered_window = true;
                        break;
                    case SDL_WINDOWEVENT_LEAVE:
                        SDL_Log("Mouse left window %d", event.window.windowID);
                        mouse_is_leaved_window = true;
                        break;
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        SDL_Log("Window %d gained keyboard focus",
                                event.window.windowID);
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        SDL_Log("Window %d lost keyboard focus",
                                event.window.windowID);
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        SDL_Log("Window %d closed", event.window.windowID);
                        break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
                    case SDL_WINDOWEVENT_TAKE_FOCUS:
                        SDL_Log("Window %d is offered a focus", event.window.windowID);
                        break;
                    case SDL_WINDOWEVENT_HIT_TEST:
                        SDL_Log("Window %d has a special hit test", event.window.windowID);
                        break;
#endif
                    default:
                        SDL_Log("Window %d got unknown event %d",
                                event.window.windowID, event.window.event);
                        break;
#endif
                }
                break;
            }
        }
    }
    event_handler();
    events_clear_state();
}

/* Keyboard events */

bool kbd_key_pressed(SDL_Keycode code) {
    return kbd_state[SDL_GetScancodeFromKey(code)] && !kbd_is_system_key;
}

bool kbd_key_pushed(SDL_Keycode code) {
    const bool result = kbd_state[SDL_GetScancodeFromKey(code)] && !kbd_is_system_key && !kbd_is_key_repeat && kbd_key_state;
    kbd_key_state = result ? SDL_RELEASED : kbd_key_state;
    return result;
}

bool kbd_key_held(SDL_Keycode code) {
    return kbd_state[SDL_GetScancodeFromKey(code)] && !kbd_is_system_key && kbd_is_key_repeat;
}

bool kbd_key_released(SDL_Keycode code) {
    const bool result = (code == kbd_current_keycode) && !kbd_is_system_key && !kbd_key_state;
    kbd_current_keycode = result ? SDLK_UNKNOWN : kbd_current_keycode;
    return result;
}

bool kbd_system_key_pushed(SDL_Keycode code) {
    const bool result = kbd_state[SDL_GetScancodeFromKey(code)] && kbd_is_system_key && !kbd_is_key_repeat && kbd_key_state;
    kbd_key_state = SDL_RELEASED;
    return result;
}

bool kbd_system_key_held(SDL_Keycode code) {
    return kbd_state[SDL_GetScancodeFromKey(code)] && kbd_is_system_key && kbd_is_key_repeat;
}

bool kbd_system_key_pressed(SDL_Keycode code) {
    return kbd_state[SDL_GetScancodeFromKey(code)] && kbd_is_system_key && !kbd_is_key_repeat;
}

bool kbd_system_key_released(SDL_Keycode code) {
    const bool result = code == kbd_current_keycode && kbd_is_system_key && !kbd_key_state;
    kbd_current_keycode = SDLK_UNKNOWN;
    return result;
}

/* Mouse events */

bool mouse_enter_window() {
    bool result = mouse_is_entered_window;
    mouse_is_entered_window = false;
    return result;
}

bool mouse_leave_window() {
    bool result = mouse_is_leaved_window;
    mouse_is_leaved_window = false;
    return result;
}

int mouse_get_pos_x() {
    return mouse_x;
}

int mouse_get_pos_y() {
    return mouse_y;
}

int mouse_get_rel_x() {
    int result = mouse_rel_x;
    mouse_rel_x = 0;
    return result;
}

int mouse_get_rel_y() {
    int result = mouse_rel_y;
    mouse_rel_y = 0;
    return result;
}

bool mouse_left_pressed() {
    return mouse_is_left_pressed;
}

bool mouse_right_pressed() {
    return mouse_is_right_pressed;
}

bool mouse_middle_pressed() {
    return mouse_is_middle_pressed;
}

bool mouse_left_down() {
    if (mouse_is_left_down)
    {
        mouse_is_left_down = false;
        return true;
    }
    return false;
}

bool mouse_right_down() {
    if (mouse_is_right_down)
    {
        mouse_is_right_down = false;
        return true;
    }
    return false;
}

bool mouse_middle_down() {
    if (mouse_is_middle_down)
    {
        mouse_is_middle_down = false;
        return true;
    }
    return false;
}

bool mouse_scroll_up() {
    if (mouse_wheel_y > 0)
    {
        mouse_wheel_y = 0;
        return true;
    }
    return false;
}

bool mouse_scroll_down() {
    if (mouse_wheel_y < 0)
    {
        mouse_wheel_y = 0;
        return true;
    }
    return false;
}



