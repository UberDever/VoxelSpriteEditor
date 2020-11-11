//
// Created by uberdever on 06.09.2020.
//

#include "player.h"

float player_height_debug = 0;

void player_init(Player *player)
{
    player->momentum = (v2_t){};
    player->hitbox_radius = C_PLAYER_HITBOX_RADIUS;
    player->max_hp = C_PLAYER_MAX_HP;
    player->hp = C_PLAYER_MAX_HP;
    player->hitscan_range = C_PLAYER_HITSCAN_RANGE;

    player->pos = (v2_t){.x = C_CHUNK_W / 2, .y = C_CHUNK_H / 2};
    player->camera.dir = (v3_t){.x = 0, .y = 0, .z = 1};
    player->camera.plane = (v3_t){.x = C_PLAYER_PLANE_SCALE, .y = 0, .z = 0};
    player->camera.horisontal_rotation = M_PI_2;
    player->camera.sensitivity = 0.0125;

    player->camera.projection_matrix = m4_perspective(C_CAMERA_FOV,
                                                      (float32)g_scr.w / (float32)g_scr.h,
                                                      C_CAMERA_NEAR,
                                                      C_CAMERA_FAR);
}

void player_event(Player *player)
{
    int m_x = mouse_get_rel_x();
    int m_y = mouse_get_rel_y();

    player->camera.horisontal_rotation += (float32)m_x * player->camera.sensitivity;
    player->camera.vertrical_rotation += -(float32)m_y * player->camera.sensitivity;

    if (player->camera.vertrical_rotation > M_PI_2)
        player->camera.vertrical_rotation = M_PI_2;
    if (player->camera.vertrical_rotation < -M_PI_2)
        player->camera.vertrical_rotation = -M_PI_2;

    player->camera.dir.x = cosf(player->camera.horisontal_rotation); //* cosf(player->camera.vertrical_rotation);
    player->camera.dir.y = sinf(player->camera.vertrical_rotation);
    player->camera.dir.z = sinf(player->camera.horisontal_rotation); //* cosf(player->camera.vertrical_rotation);
                                                                     //debug("%f %f %f", player->camera.dir.x, player->camera.dir.y, player->camera.dir.z);
    player->camera.plane = v3_cross(player->camera.dir, (v3_t){0, 1, 0});

    if (kbd_key_pressed(SDLK_w))
    {
        player->momentum.y = C_PLAYER_SPEED;
    }
    if (kbd_key_pressed(SDLK_a))
    {
        player->momentum.x = -C_PLAYER_SPEED;
    }
    if (kbd_key_pressed(SDLK_s))
    {
        player->momentum.y = -C_PLAYER_SPEED;
    }
    if (kbd_key_pressed(SDLK_d))
    {
        player->momentum.x = C_PLAYER_SPEED;
    }
    if (kbd_key_pressed(SDLK_z))
    {
        player_height_debug -= 0.1f;
    }
    if (kbd_key_pressed(SDLK_c))
    {
        player_height_debug += 0.1f;
    }
}

void player_update(Map *map, Player *player)
{
#if 0
    const m2 dir_rotation = {0, -1, 1, 0};
    pi_v2(player->dir, mouse_get_pos_x() - (half_w), mouse_get_pos_y() - half_h);
    pi_v2_mul_m2(dir_rotation, player->dir, player->plane);
    pi_v2_normalize(player->dir, player->dir);
    pi_v2_normalize(player->plane, player->plane);
#endif

    //v2 left_frustum, right_frustum;
    //pi_v2_muls(player->camera.dir, C_PLAYER_DIR_SCALE, right_frustum);
    //pi_v2_muls(player->camera.plane, C_PLAYER_PLANE_SCALE, left_frustum);
    //pi_v2_add(right_frustum, left_frustum, player->left_frustum_ray);
    //left_frustum.x *= -1;
    //left_frustum.y *= -1;
    //pi_v2_add(right_frustum, left_frustum, player->right_frustum_ray);
    //
    //v2 tmp0 = {};
    //pi_v2_muls(player->dir, player->momentum.y, tmp0);
    //v2 tmp1 = {};
    //pi_v2_muls(player->plane, player->momentum.x, tmp1);
    //v2 step = {};
    //pi_v2_add(tmp0, tmp1, step);

    v2_t dir = (v2_t){player->camera.dir.x, player->camera.dir.z};
    v2_t plane = (v2_t){player->camera.plane.x, player->camera.plane.z};
    v2_t step = v2_add(v2_muls(dir, player->momentum.y), v2_muls(plane, player->momentum.x));
    v2_t test_move = {};
    test_move.x = step.x >= 0 ? player->hitbox_radius : -player->hitbox_radius;
    test_move.y = step.y >= 0 ? player->hitbox_radius : -player->hitbox_radius;
    // v2_t test_position = v2_add(player->pos, step);
    // if (tile_is_wall(map, test_position.x + test_move.x, test_position.y))
    //     step.x = 0;
    // if (tile_is_wall(map, test_position.x, test_position.y + test_move.y))
    //     step.y = 0;
    // test_move = v2_muls(test_move, SQRT2_2);
    // if (tile_is_wall(map, test_position.x + test_move.x, test_position.y - test_move.y))
    //     step.x = 0;
    // if (tile_is_wall(map, test_position.x - test_move.x, test_position.y + test_move.y))
    //     step.y = 0;
    // if (tile_is_wall(map, test_position.x + test_move.x, test_position.y + test_move.y))
    // {
    //     step.x = 0;
    //     step.y = 0;
    // }
    player->pos = v2_add(player->pos, step);

    player->momentum.x = 0;
    player->momentum.y = 0;

        // map->data[(int)player->pos.y * map->w + (int)player->pos.x] = 'P';
}

void player_raycast(Map *map, Player *player)
{ //TODO: REBUILD
#if 0
    int scrW = g_scr.w;
    int scrH = g_scr.h;

    int dS = 0;
    int dE = 0;

    g_screen_fill(COLOR(black));

    for (int x = 0; x < scrW; ++x)
    {
        float32 cam = 2.f * (float)x / (float)scrW - 1;
        v2 ray;
        pi_v2(ray, -player->plane.x, -player->plane.y);
        pi_v2_muls(ray, cam, ray);
        pi_v2_add(ray, player->dir, ray);
        vi2 pos = {player->pos.x, player->pos.y};
        v2 side;
        v2 delta = {fabsf(1.f / ray.x), fabsf(1.f / ray.y)};
        float32 pwd = 0;
        vi2 step;
        bool hit = false, NS = true; //North-South

        if (ray.x > 0)
        {
            step.x = 1;
            side.x = ((float32)pos.x + 1 - player->pos.x) * delta.x;
        }
        else
        {
            step.x = -1;
            side.x = (player->pos.x - pos.x) * delta.x;
        }
        if (ray.y > 0)
        {
            step.y = 1;
            side.y = ((float32)pos.y + 1 - player->pos.y) * delta.y;
        }
        else
        {
            step.y = -1;
            side.y = (player->pos.y - pos.y) * delta.y;
        }
        while (!hit)
        {
            if (side.x < side.y)
            {
                side.x += delta.x;
                pos.x += step.x;
                NS = true;
            }
            else
            {
                side.y += delta.y;
                pos.y += step.y;
                NS = false;
            }
            if (map->data[pos.y * map->w + pos.x])
                hit = true;
        }

        if (NS)
            pwd = (pos.x - player->pos.x + (1.f - step.x) / 2) / ray.x;
        else
            pwd = (pos.y - player->pos.y + (1.f - step.y) / 2) / ray.y;

        int line = (int)(2 * (float32)scrH / pwd);
        if (line) //for evading zero-length lines
        {
            dS = -line / 2 + (scrH) / 2;
            dS = dS < 0 ? 0 : dS;
            dE = line / 2 + (scrH) / 2;
            dE = dE >= scrH ? scrH - 1 : dE;
            //dS = -line / 2 + scrH / 2 - pd->pl->offset; dS = dS < 0 ? 0 : dS;
            //dE = line / 2 + scrH / 2 - pd->pl->offset; dE = dE >= scrH ? scrH - 1 : dE;
        }

        g_screen_draw_line(x, dS, x, dE, COLOR(cyan));
    }
    //SDL_Rect o = {0, 0, g_scr.w, g_scr.h};
    //SDL_UpdateTexture(global::gameScreen(), 0, global::gameScreenBuffer(), sizeof(uint32_t) * global::cfg::scrRenW());
    ////SDL_RenderCopy(global::renderer(), skybox, 0, &o);
    //SDL_RenderCopy(global::renderer(), global::gameScreen(), 0, 0);
    //SDL_RenderPresent(global::renderer());
#endif
}

void player_clean(Player *player)
{
}
