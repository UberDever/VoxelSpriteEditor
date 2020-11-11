//
// Created by uberdever on 31.07.2020.
//
// Gamma is graphics utility header, pure functions for abstraction from platform
#ifndef ALPHABETA_GAMMA_H
#define ALPHABETA_GAMMA_H

#include "alpha.h"
#include "pi.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

extern struct scr_t {
    uint w, h, pixel_count;
    uint32* screen_buf;
    bool is_fullscreen;
    SDL_Window* window;
    SDL_Renderer* renderer;
} g_scr;

void g_init(uint w, uint h, bool is_fullscreen, SDL_WindowFlags win_flags, SDL_RendererFlags ren_flags);
void g_clean();

void g_screen_set_render_color(color color);

void g_screen_clear();
void g_screen_fill(color color);
void g_screen_put_pixel(uint x, uint y, color color);
void g_screen_draw_line(int start_x, int start_y, int end_x, int end_y, color color);
void g_screen_draw_tri(const v2_t vx0, const v2_t vx1, const v2_t vx2, color color);
void g_screen_draw_quat(const v2_t vx0, const v2_t vx1, const v2_t vx2, const v2_t vx3, color color);
void g_screen_draw_circle(v2_t center, int r, color color);

void g_screen_fill_quat(const v2_t pos, const v2_t size, color color);

void g_screen_draw_symbol(uint x, uint y, uint8 symbol, color color);
void g_screen_draw_number(uint x, uint y, int number, color color);
void g_screen_draw_string(uint x, uint y, const char* string, color color);

void g_screen_buffer_update();
void g_screen_buffer_clear();

void g_screen_present();


#endif //ALPHABETA_GAMMA_H
