//
// Created by uberdever on 06.09.2020.
//

/*
    Controls:
    left click: add/remove
    right click: pick depth
    middle ckick: select/disselect area
    c: disselect all
    f: write depth to selected pixel

    SV button: save
    AL button: write current depth for all opaque
    WR button: write to selected
    CL button: clear current depth
*/
#include "game.h"

#define SPACE_W 64
#define SPACE_H 64

typedef struct Positions
{
    int x, y;
} Positions;

vec_register(Positions);

static const char *sprite_path = "../sprites/";
static const char *sprite_name = "revolver";
static SDL_Texture *sprite = NULL;
static SDL_Surface *sprite_surface;
static int w, h;
static float32 pixel_w, pixel_h;
static int depthbar_x, depthbar_y, depthbar_cell_w, depthbar_cell_h;
static SDL_Rect sprite_area = {}, render_area = {};
//static v_Positions_t selected_pixels;
static bool *selected_pixels;
static bool is_selection_area = false;
static int write_w, write_h, write_x, write_y;
static int clear_w, clear_h, clear_x, clear_y;
static int save_w, save_h, save_x, save_y;
static int select_all_w, select_all_h, select_all_x, select_all_y;
static int middle_w, middle_h, middle_x, middle_y;
static int picked_pixel_x, picked_pixel_y;
static int selected_area_x = -1, selected_area_y = -1;

#define DEPTH_LEVELS (sizeof(uint64_t) * CHAR_BIT)
static uint64_t depthbar = (1ULL << 31ULL) | (1ULL << 32ULL);

static uint64_t *depthfield = NULL;

void init(void)
{
    g_screen_set_render_color((color){.r = 0, .g = 0, .b = 0.4f * 255, .a = 1});
    char path[100] = {};
    strcat(path, sprite_path);
    strcat(path, sprite_name);
    strcat(path, ".png");
    sprite_surface = IMG_Load(path);
    w = sprite_surface->w;
    h = sprite_surface->h;
    sprite = SDL_CreateTextureFromSurface(g_scr.renderer, sprite_surface);
    if (!sprite)
        error("Cannot load sprite %s", path);
    FILE *depth_in = NULL;
    char depth_path[100] = {};
    strcat(depth_path, sprite_path);
    strcat(depth_path, sprite_name);
    strcat(depth_path, ".zaxis");
    depth_in = fopen(depth_path, "rb");
    ARR_ALLOC(depthfield, sprite_surface->w * sprite_surface->h);
    if (depth_in)
    {
        fread(depthfield, sizeof(uint64_t) * sprite_surface->w * sprite_surface->h, 1, depth_in);
        fclose(depth_in);
    }
    sprite_area.w = 16;
    sprite_area.h = 16;
    render_area.w = g_scr.w - SPACE_W;
    render_area.h = g_scr.h - SPACE_H;
    depthbar_cell_w = render_area.w / DEPTH_LEVELS;
    depthbar_cell_h = SPACE_H / 2 - 1;
    depthbar_x = 0;
    depthbar_y = render_area.h + 1;
    write_x = render_area.w;
    write_y = render_area.h + depthbar_cell_h;
    write_w = SPACE_W - 1;
    write_h = depthbar_cell_h;
    clear_x = render_area.w;
    clear_y = render_area.h;
    clear_w = SPACE_W - 1;
    clear_h = depthbar_cell_h;
    save_x = render_area.w;
    save_y = render_area.h - depthbar_cell_h;
    save_w = SPACE_W - 1;
    save_h = depthbar_cell_h;
    select_all_x = render_area.w;
    select_all_y = render_area.h - 2 * depthbar_cell_h;
    select_all_w = SPACE_W - 1;
    select_all_h = depthbar_cell_h;
    middle_w = depthbar_cell_w;
    middle_h = 2 * depthbar_cell_h - 1;
    middle_x = render_area.w / 2 - depthbar_cell_w / 2;
    middle_y = render_area.h;
    ARR_ALLOC(selected_pixels, w * h);
}

static inline void select_pixel(int mx, int my)
{
    int x = mx * sprite_area.w / render_area.w;
    int y = my * sprite_area.h / render_area.h;
    x += sprite_area.x;
    y += sprite_area.y;
    selected_pixels[y * w + x] = !selected_pixels[y * w + x];
    return;
}

void event_handler(void)
{
<<<<<<< HEAD
    if (kbd_key_pressed(SDLK_w))
    {
        sprite_area.y--;
        sprite_area.y = sprite_area.y > 0 ? sprite_area.y : 0;
    }
    if (kbd_key_pressed(SDLK_a))
    {
        sprite_area.x--;
        sprite_area.x = sprite_area.x > 0 ? sprite_area.x : 0;
    }
    if (kbd_key_pressed(SDLK_s))
    {
        sprite_area.y++;
        sprite_area.y = sprite_area.y + sprite_area.h <= h ? sprite_area.y : sprite_area.y - 1;
    }
    if (kbd_key_pressed(SDLK_d))
    {
        sprite_area.x++;
        sprite_area.x = sprite_area.x + sprite_area.w <= w ? sprite_area.x : sprite_area.x - 1;
    }
    if (kbd_key_pushed(SDLK_q))
    {
        sprite_area.w++;
        sprite_area.h++;
        if (sprite_area.w >= w || sprite_area.h >= h)
        {
            sprite_area.w--;
            sprite_area.h--;
        }
        if (sprite_area.y + sprite_area.h > h)
        {
            sprite_area.y--;
            sprite_area.y = sprite_area.y > 0 ? sprite_area.y : 0;
        }
        if (sprite_area.x + sprite_area.w > w)
        {
            sprite_area.x--;
            sprite_area.x = sprite_area.x > 0 ? sprite_area.x : 0;
        }
    }
    if (kbd_key_pushed(SDLK_e))
    {
        sprite_area.w--;
        sprite_area.h--;
        if (sprite_area.w < 1 || sprite_area.h < 1)
        {
            sprite_area.w++;
            sprite_area.h++;
        }
    }
    if (kbd_key_pushed(SDLK_f)) // write
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                if (selected_pixels[y * w + x])
                {
                    depthfield[y * w + x] = depthbar;
                }
            }
        }
    }
    if (kbd_key_pushed(SDLK_c)) // disselect all
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                selected_pixels[y * w + x] = false;
            }
        }
    }
=======
    if (kbd_key_pressed(SDLK_ESCAPE))
        is_program_running = false;
>>>>>>> 461d48516d810d37fc7cb2b50f8db4ffa092ed7a

    pixel_w = (float32)render_area.w / sprite_area.w;
    pixel_h = (float32)render_area.h / sprite_area.h;

    int mx = mouse_get_pos_x();
    int my = mouse_get_pos_y();
    if (mx <= render_area.w && my <= render_area.h) //pixel space
    {
        if (mouse_left_down())
        {
            int x = mx * sprite_area.w / render_area.w;
            int y = my * sprite_area.h / render_area.h;
            x += sprite_area.x;
            y += sprite_area.y;
            picked_pixel_x = x;
            picked_pixel_y = y;
            selected_pixels[y * w + x] = !selected_pixels[y * w + x];
        }
        if (mouse_right_down())
        {
            int x = mx * sprite_area.w / render_area.w;
            int y = my * sprite_area.h / render_area.h;
            x += sprite_area.x;
            y += sprite_area.y;
            picked_pixel_x = x;
            picked_pixel_y = y;
        }
        if (mouse_middle_down())
        {
            int x = mx * sprite_area.w / render_area.w;
            int y = my * sprite_area.h / render_area.h;
            x += sprite_area.x;
            y += sprite_area.y;
            if (selected_area_x > x)
            {
                SWAP_INT(selected_area_x, x);

            }
            if (selected_area_y > y)
            {
                SWAP_INT(selected_area_y, y);
            }
            if (is_selection_area)
            {
                for (int p_y = selected_area_y; p_y <= y; p_y++)
                {
                    for (int p_x = selected_area_x; p_x <= x; p_x++)
                    {
                        selected_pixels[p_y * w + p_x] = !selected_pixels[p_y * w + p_x];
                    }
                }
                selected_area_x = -1;
                selected_area_y = -1;
            } else
            {
                selected_area_x = x;
                selected_area_y = y;
            }
            is_selection_area = !is_selection_area;
        }
    }
    else if (mx <= render_area.w && my > render_area.h && my <= render_area.h + depthbar_cell_h)
    {
        if (mouse_left_down())
        {
            int selected_cell = mx / depthbar_cell_w;
            depthbar ^= 1ULL << selected_cell;
        }
    }
    else if (mx > render_area.w && my > render_area.h + depthbar_cell_h) // write
    {
        if (mouse_left_down())
        {
            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    if (selected_pixels[y * w + x])
                    {
                        depthfield[y * w + x] = depthbar;
                    }
                }
            }
        }
    }
    else if (mx > render_area.w && my > render_area.h && my <= render_area.h + depthbar_cell_h) // clear
    {
        if (mouse_left_down())
            depthbar = 0;
    }
    else if (mx > render_area.w && my > render_area.h - depthbar_cell_h && my <= render_area.h) // save
    {
        if (mouse_left_down())
        {
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    if (((color *)sprite_surface->pixels)[i * w + j].a == 0)
                        depthfield[i * w + j] = 0;
                }
            }

            FILE *depth_out = NULL;
            char file_name[100] = {};
            strcat(file_name, sprite_path);
            strcat(file_name, sprite_name);
            strcat(file_name, ".zaxis");
            debug("saved to %s", file_name);
            depth_out = fopen(file_name, "wb+");
            fwrite(depthfield, sizeof(uint64_t) * w * h, 1, depth_out);
            fclose(depth_out);
        }
    }
    else if (mx > render_area.w && my > render_area.h - 2 * depthbar_cell_h && my <= render_area.h - depthbar_cell_h) // write for all
    {
        if (mouse_left_down())
        {
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    depthfield[i * w + j] = ((color *)sprite_surface->pixels)[i * w + j].a != 0 ? depthbar : 0;
                }
            }
        }
    }
}

void begin_frame(void)
{
    g_screen_buffer_clear();
    g_screen_clear();
}

void update(void)
{
}

static void draw_depthbar(int x, int y, uint64_t value)
{
    for (int i = 0; i < DEPTH_LEVELS; i++)
    {
        v2_t p0 = (v2_t){depthbar_x + i * depthbar_cell_w + x, depthbar_y + y};
        v2_t p1 = (v2_t){depthbar_x + i * depthbar_cell_w + x + depthbar_cell_w, depthbar_y + y};
        v2_t p2 = (v2_t){depthbar_x + i * depthbar_cell_w + x + depthbar_cell_w, depthbar_y + depthbar_cell_h + y};
        v2_t p3 = (v2_t){depthbar_x + i * depthbar_cell_w + x, depthbar_y + depthbar_cell_h + y};
        bool is_set = value & (1ULL << (i)) ? true : false;
        color cell_color = is_set ? COLOR(red) : COLOR(green);
        g_screen_draw_quat(p0, p1, p2, p3, cell_color);
        if (is_set)
        {
            g_screen_draw_line(p0.x, p0.y, p2.x, p2.y, cell_color);
            g_screen_draw_line(p1.x, p1.y, p3.x, p3.y, cell_color);
        }
    }
}

void render(void)
{
    SDL_RenderCopy(g_scr.renderer, sprite, &sprite_area, &render_area);
    for (float32 x = 0; x <= render_area.w; x += pixel_w)
    {
        g_screen_draw_line(x, 0, x, render_area.h, COLOR(light_gray));
    }
    for (float32 y = 0; y <= render_area.h; y += pixel_h)
    {
        g_screen_draw_line(0, y, render_area.w, y, COLOR(light_gray));
    }
    draw_depthbar(0, 0, depthbar);
    draw_depthbar(0, depthbar_cell_h - 1, depthfield[picked_pixel_y * w + picked_pixel_x]);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (selected_pixels[y * w + x])
            {
                Positions pos = (Positions){x, y};
                pos.x = (pos.x - sprite_area.x) * pixel_w;
                pos.y = (pos.y - sprite_area.y) * pixel_h;
                if (pos.x + pixel_w > render_area.w || pos.y + pixel_h > render_area.h)
                    continue;
                v2_t p0 = (v2_t){pos.x, pos.y};
                v2_t p1 = (v2_t){pos.x + pixel_w, pos.y};
                v2_t p2 = (v2_t){pos.x + pixel_w, pos.y + pixel_h};
                v2_t p3 = (v2_t){pos.x, pos.y + pixel_h};
                color line_color = COLOR(red);
                g_screen_draw_quat(p0, p1, p2, p3, line_color);
                g_screen_draw_line(p0.x, p0.y, p2.x, p2.y, line_color);
                g_screen_draw_line(p1.x, p1.y, p3.x, p3.y, line_color);
            }
        }
    }
    debug("%d %d", selected_area_x, selected_area_y);
    if (selected_area_x != -1 && selected_area_y != -1)
    {
        Positions pos = (Positions){selected_area_x, selected_area_y};
        pos.x = (pos.x - sprite_area.x) * pixel_w;
        pos.y = (pos.y - sprite_area.y) * pixel_h;
        if (pos.x + pixel_w <= render_area.w && pos.y + pixel_h <= render_area.h)
        {
            v2_t p0 = (v2_t){pos.x, pos.y};
            v2_t p1 = (v2_t){pos.x + pixel_w, pos.y};
            v2_t p2 = (v2_t){pos.x + pixel_w, pos.y + pixel_h};
            v2_t p3 = (v2_t){pos.x, pos.y + pixel_h};
            color line_color = COLOR(yellow);
            g_screen_draw_quat(p0, p1, p2, p3, line_color);
            g_screen_draw_line(p0.x, p0.y, p2.x, p2.y, line_color);
            g_screen_draw_line(p1.x, p1.y, p3.x, p3.y, line_color);
        }
    }
    g_screen_draw_quat((v2_t){write_x, write_y},
                       (v2_t){write_x + write_w, write_y},
                       (v2_t){write_x + write_w, write_y + write_h},
                       (v2_t){write_x, write_y + write_h},
                       COLOR(white));
    g_screen_draw_quat((v2_t){save_x, save_y},
                       (v2_t){save_x + save_w, save_y},
                       (v2_t){save_x + save_w, save_y + save_h},
                       (v2_t){save_x, save_y + save_h},
                       COLOR(white));
    g_screen_draw_quat((v2_t){select_all_x, select_all_y},
                       (v2_t){select_all_x + select_all_w, select_all_y},
                       (v2_t){select_all_x + select_all_w, select_all_y + select_all_h},
                       (v2_t){select_all_x, select_all_y + select_all_h},
                       COLOR(white));
    g_screen_draw_quat((v2_t){clear_x, clear_y},
                       (v2_t){clear_x + clear_w, clear_y},
                       (v2_t){clear_x + clear_w, clear_y + clear_h},
                       (v2_t){clear_x, clear_y + clear_h},
                       COLOR(white));
    g_screen_draw_quat((v2_t){middle_x, middle_y},
                       (v2_t){middle_x + middle_w, middle_y},
                       (v2_t){middle_x + middle_w, middle_y + middle_h},
                       (v2_t){middle_x, middle_y + middle_h},
                       COLOR(gray));
    g_screen_draw_string(select_all_x, select_all_y, "AL", COLOR(green));
    g_screen_draw_string(save_x, save_y, "SV", COLOR(red));
    g_screen_draw_string(clear_x, clear_y, "CL", COLOR(cyan));
    g_screen_draw_string(write_x, write_y, "WR", COLOR(yellow));
}

void end_frame(void)
{
    g_screen_buffer_update();
    g_screen_present();
}

void clean(void)
{
    SDL_FreeSurface(sprite_surface);
    free(selected_pixels);
}
