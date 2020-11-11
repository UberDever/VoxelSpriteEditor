//
// Created by uberdever on 27.07.2020.
//

#include "../include/alphabeta.h"

static FILE* logOut = NULL;

static void error_handle(const char* str) {
    fprintf(stderr, "%s\n", str); fflush(stderr);
    fprintf(logOut, "%s\n", str); fflush(logOut);
    exit(EXIT_FAILURE);
}

static void warning_handle(const char* str) {
    fprintf(stdout, "%s\n", str); fflush(stdout);
    fprintf(logOut, "%s\n", str); fflush(logOut);
}

static void debug_handle(const char* str) {
    fprintf(stdout, "%s\n", str); fflush(stdout);
    fprintf(logOut, "%s\n", str); fflush(logOut);
}

void alphabeta_init(gamma_info_t scr_info) {
    logOut = fopen("out.log", "w");

    error_attach_handler(error_handle);
    warning_attach_handler(warning_handle);
    debug_attach_handler(debug_handle);

    kbd_set_state_array(SDL_GetKeyboardState(NULL)); /* Set keyboard state array */
    /* Init all systems */

    /* Init graphics */
    g_init(scr_info.w, scr_info.h, scr_info.is_fullscreen, scr_info.win_flags, scr_info.ren_flags);
}

void alphabeta_clean() {
    /* Clean all systems */

    /* Clean graphics */
    g_clean();
}
