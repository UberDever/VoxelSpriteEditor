//
// Created by uberdever on 06.09.2020.
//

#include "map.h"

#define chunk_vx_elements 8
#define chunk_vx_positions 3
#define chunk_vx_tex_coords 2
#define chunk_vx_normals 3

static GLuint shader;
static GLuint model_shader;

static GLuint matrix_view, matrix_projection;
static GLuint model_worldView, model_projection;

Model weapon;

static void map_chunks_init(Map *map, const char *vertex_shader, const char *fragment_shader)
{
    map->chunks_w = map->w / C_CHUNK_W;
    map->chunks_h = map->h / C_CHUNK_H;
    vec_new(Chunk, map->chunks, map->chunks_w * map->chunks_h);

    v_GLfloat_t vertices = {};
    v_GLuint_t indices = {};
    vec_new(GLfloat, vertices, C_CHUNK_W * C_CHUNK_H / 4 * 8 * chunk_vx_elements);
    vec_new(GLuint, indices, C_CHUNK_W * C_CHUNK_H / 4 * 8 * 3);
    int ch_half_w = C_CHUNK_W / 2;
    int ch_half_h = C_CHUNK_H / 2;
    GLuint cur_pube_idx = 0;
    for (int i = 0; i < map->chunks_w; i++)
    {
        for (int j = 0; j < map->chunks_h; j++)
        {
            Chunk chunk = {};
            vec_new(GLuint, chunk.model.buffers, 1);

            for (int x = -ch_half_w; x < ch_half_w; x++)
            {
                int map_x = i * C_CHUNK_W + ch_half_w + x;
                for (int y = -ch_half_h; y < ch_half_h; y++)
                {
                    int map_y = j * C_CHUNK_H + ch_half_h + y;
                    //debug("%d %d", map_x, map_y);
                    if (tile_is_wall(map, map_x, map_y))
                    {
                        bool adj_check[4] = {1, 1, 1, 1};
                        if (map_y - 1 < 0)
                            adj_check[0] = false;
                        else if (tile_is_wall(map, map_x, map_y - 1))
                            adj_check[0] = false;
                        if (map_x + 1 >= map->w)
                            adj_check[1] = false;
                        else if (tile_is_wall(map, map_x + 1, map_y))
                            adj_check[1] = false;
                        if (map_y + 1 >= map->h)
                            adj_check[2] = false;
                        else if (tile_is_wall(map, map_x, map_y + 1))
                            adj_check[2] = false;
                        if (map_x - 1 < 0)
                            adj_check[3] = false;
                        else if (tile_is_wall(map, map_x - 1, map_y))
                            adj_check[3] = false;

                        generate_pube(&vertices, &indices, adj_check, x, y, (v2_t){}, cur_pube_idx);
                        cur_pube_idx++;
                    }
                }
            }

            glGenVertexArrays(1, &chunk.model.info.vao);
            glBindVertexArray(chunk.model.info.vao);

            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size, vertices.data, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                0,
                chunk_vx_positions,
                GL_FLOAT,
                GL_FALSE,
                chunk_vx_elements * sizeof(GLfloat),
                (GLvoid *)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(
                1,
                chunk_vx_tex_coords,
                GL_FLOAT,
                GL_FALSE,
                chunk_vx_elements * sizeof(GLfloat),
                (GLvoid *)(chunk_vx_positions * sizeof(GLfloat)));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(
                2,
                chunk_vx_normals,
                GL_FLOAT,
                GL_FALSE,
                chunk_vx_elements * sizeof(GLfloat),
                (GLvoid *)((chunk_vx_positions + chunk_vx_tex_coords) * sizeof(GLfloat)));

            vec_push(chunk.model.buffers, vbo);

            chunk.model.info.idx_count = indices.size;
            GLuint ibo;
            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size,
                         indices.data, GL_STATIC_DRAW);

#if 0
            {
                for (uint k = 0; k < vertices.size; k += 8)
                {
                    debug("%d. p: %f %f %f t: %f %f n: %f %f %f", k / 8, vertices.data[k], vertices.data[k + 1], vertices.data[k + 2], vertices.data[k + 3],
                          vertices.data[k + 4], vertices.data[k + 5], vertices.data[k + 6], vertices.data[k + 7]);
                }

                for (uint k = 0; k < indices.size; k += 3)
                {
                    printf("%d. %d ", k / 3, indices.data[k]);
                    printf("%d ", indices.data[k + 1]);
                    printf("%d\n", indices.data[k + 2]);
                }
                debug("\n%d %d", vertices.size, indices.size);
            }
#endif
            vertices.size = 0;
            indices.size = 0;
            cur_pube_idx = 0;
            vec_push(map->chunks, chunk);
        }
    }

    shader = shader_load_all(vertex_shader, fragment_shader);
    matrix_view = glGetUniformLocation(shader, "u_view");
    matrix_projection = glGetUniformLocation(shader, "u_projection");

    glGenTextures(1, &map->tileset.id);
    glBindTexture(GL_TEXTURE_2D, map->tileset.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, map->tileset.w, map->tileset.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, map->tileset.data);
    //glGenerateMipmap(GL_TEXTURE_2D);
}

void map_init(Map *map, const char *path, const char *tileset_path, const char *vertex_shader, const char *fragment_shader)
{
    map->tile_w = C_MAP_TILE_W;
    map->tile_h = C_MAP_TILE_H;

    FILE *map_source = NULL;
    map_source = fopen(path, "r");
    if (!map_source)
        error("Cannot load map, path: %s", path);

    fseek(map_source, 0, SEEK_END);
    uint map_len = ftell(map_source) + 1;
    fseek(map_source, 0, SEEK_SET);
    for (; 1; map->w++)
    {
        if (getc(map_source) == '\n' || map->w >= 1000)
            break;
    }
    fseek(map_source, 0, SEEK_SET);
    map->h = map_len / (map->w + 1);
    //debug("%d %d %d", map->w, map->h, map_len)
    ARR_ALLOC(map->data, map_len);
    for (uint i = 0; i < map->h; i++)
    {
        for (uint j = 0; j < map->w; j++)
        {
            map->data[i * map->w + j] = getc(map_source) - '0';
            //printf("%d", map->data[i * map->w + j]);
        }
        getc(map_source);
        //printf("\n");
    }

    SDL_Surface *texture = IMG_Load(tileset_path);
    if (!texture)
        error("Cannot load texture %s", tileset_path);
    map->tileset.data = texture->pixels;
    map->tileset.w = texture->w;
    map->tileset.h = texture->h;
    map_chunks_init(map, vertex_shader, fragment_shader);

    //TODO: REMOVE
    vec_new(GLuint, weapon.buffers, 1);
    const uint model_vx_attr0 = 3;
    const uint model_vx_attr1 = 3;
    const uint model_vx_attr2 = 3;
    const uint model_vx = model_vx_attr0 + model_vx_attr1 + model_vx_attr2;
    v_GLfloat_t vertices = {};
    v_GLuint_t indices = {};
    vec_new(GLfloat, vertices, 2048);
    vec_new(GLuint, indices, 1024);
    generate_voxel_sprite(&vertices, &indices,
                          "../../graphics/sprites/revolver.png", //_sight
                          "../../graphics/depthmaps/revolver.zaxis",
                          (v3_t){0.1f, 0.1f, 0.1f});

    glGenVertexArrays(1, &weapon.info.vao);
    glBindVertexArray(weapon.info.vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size, vertices.data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        model_vx_attr0, // positions
        GL_FLOAT,
        GL_FALSE,
        model_vx * sizeof(GLfloat),
        (GLvoid *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        model_vx_attr1, // colors
        GL_FLOAT,
        GL_FALSE,
        model_vx * sizeof(GLfloat),
        (GLvoid *)(model_vx_attr0 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        model_vx_attr2, // normals
        GL_FLOAT,
        GL_FALSE,
        model_vx * sizeof(GLfloat),
        (GLvoid *)((model_vx_attr0 + model_vx_attr1) * sizeof(GLfloat)));

    vec_push(weapon.buffers, vbo);

    weapon.info.idx_count = indices.size;
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size,
                 indices.data, GL_STATIC_DRAW);

    model_shader = shader_load_all("../../graphics/shaders/weapon.vs", "../../graphics/shaders/weapon.fs");
    model_worldView = glGetUniformLocation(model_shader, "u_modelView");
    model_projection = glGetUniformLocation(model_shader, "u_projection");
}

void map_render(Map *map, Player *player, v_Enemy_t *enemies, v_Projectile_t *projectiles, const int pos_x, const int pos_y, const int size_x, const int size_y)
{
    // TODO REMOVE
    glUseProgram(model_shader);
    glUniformMatrix4fv(model_projection, 1, GL_FALSE, player->camera.projection_matrix.m);
    v3_t view_pos = (v3_t){};
    view_pos.x = fmodf(player->pos.x, C_CHUNK_W) - C_CHUNK_W / 2 - .5f;
    view_pos.y = player_height_debug; //flight mode
    view_pos.z = fmodf(player->pos.y, C_CHUNK_H) - C_CHUNK_H / 2 - .5f;
    m4_t weapon_model = m4_identity(); //m4_mul(m4_translation((v3_t){view_pos.x - 1, -.3, view_pos.z + 1.3}), m4_rotation_y(DEG2RAD(100)));
    player->camera.view_matrix = m4_mul(m4_look_at(view_pos,
                                                   player->camera.dir,
                                                   (v3_t){0, 1, 0}),
                                        weapon_model);
    glUniformMatrix4fv(model_worldView, 1, GL_FALSE, player->camera.view_matrix.m);
    glBindVertexArray(weapon.info.vao);
    glDrawElements(GL_TRIANGLES, weapon.info.idx_count, GL_UNSIGNED_INT, 0);

    int player_pos_in_chunks_x = (int)player->pos.x / C_CHUNK_W;
    int player_pos_in_chunks_y = (int)player->pos.y / C_CHUNK_H;
    //chunk_index = 1;
    //debug("%d", chunk_index);

    glUseProgram(shader);
    glBindTexture(GL_TEXTURE_2D, map->tileset.id);
    glUniformMatrix4fv(matrix_projection, 1, GL_FALSE, player->camera.projection_matrix.m);
    int index = 0;
    for (int x_rel = -C_RANGE; x_rel < C_RANGE; x_rel++)
    {
        for (int y_rel = -C_RANGE; y_rel < C_RANGE; y_rel++)
        {
            if (abs(x_rel) + abs(y_rel) <= C_RANGE)
            {
                int chunk_pos_x = player_pos_in_chunks_x + x_rel;
                int chunk_pos_y = player_pos_in_chunks_y + y_rel;
                if (pi_aabb_box_x_point(chunk_pos_x, chunk_pos_y, 0, 0, map->chunks_w, map->chunks_h))
                {
                    v3_t view_pos = (v3_t){};
                    view_pos.x = fmodf(player->pos.x, C_CHUNK_W) - C_CHUNK_W / 2 - .5f - C_CHUNK_W * x_rel;
                    view_pos.y = player_height_debug; //flight mode
                    view_pos.z = fmodf(player->pos.y, C_CHUNK_H) - C_CHUNK_H / 2 - .5f - C_CHUNK_H * y_rel;
                    player->camera.view_matrix = m4_look_at(view_pos,
                                                            player->camera.dir,
                                                            (v3_t){0, 1, 0});
                    glUniformMatrix4fv(matrix_view, 1, GL_FALSE, player->camera.view_matrix.m);
                    int chunk_index = chunk_pos_x * map->chunks_h + chunk_pos_y;
                    glBindVertexArray(map->chunks.data[chunk_index].model.info.vao);
                    //glDrawElements(GL_TRIANGLES, map->chunks.data[chunk_index].model.info.idx_count, GL_UNSIGNED_INT, 0);
                    index++;
                }
            }
        }
    }
    //glBindVertexArray(0);

#if 0
    //Map
    vi2 offset, map_screen_start, map_screen_end, map_screen_size;
    offset[0] = map_screen_start[0] = pos[0] - (size[0] + .5f) * map->tile_w;
    offset[1] = map_screen_start[1] = pos[1] - (size[1] + .5f) * map->tile_h;
    v2 player_offset;
    pi_v2_copy(player->pos, player_offset);
    player_offset[0] -= (int)player_offset[0];
    player_offset[1] -= (int)player_offset[1];
    const v2 map_start = {player->pos[0] - size[0], player->pos[1] - size[1]};
    const int map_w = 2 * size[0] + 1, map_h = 2 * size[1] + 1;

    map_screen_size[0] = map_w * map->tile_w;
    map_screen_size[1] = map_h * map->tile_h;
    map_screen_end[0] = map_screen_start[0] + map_screen_size[0];
    map_screen_end[1] = map_screen_start[1] + map_screen_size[1];
    g_screen_fill_quat(VI2TOV2(offset), VI2TOV2(map_screen_size), COLOR(black));
    for (int i = 0; i < map_h; i++)
    {
        for (int j = 0; j < map_w; j++)
        {
            const v2 vx0 = {offset[0] + (j - player_offset[0] + .5) * map->tile_w,
                            offset[1] + (i - player_offset[1] + .5) * map->tile_h};
            const v2 vx1 = {vx0[0] + map->tile_w, vx0[1] + map->tile_h};
            const float32 map_y = (map_start[1] + i);
            const float32 map_x = map_start[0] + j;
            if (map_y >= 0 && map_x >= 0 && map_x < map->w && map_y < map->h)
            {
                if (tile_is_wall(map, map_x, map_y))
                {
                    g_screen_fill_quat(vx0, (v2){map->tile_w, map->tile_h}, COLOR(yellow));
                } /*else if (map->data[(int)map_y * map->w + (int)map_x] == 'E')
                {
                    g_screen_fill_quat(vx0, (v2){map->tile_w, map->tile_h}, COLOR(red));
                }*/
            }
            else
            {
                g_screen_draw_quat(vx0,
                                   (v2){vx0[0] + map->tile_w, vx0[1]},
                                   vx1,
                                   (v2){vx0[0], vx0[1] + map->tile_h}, COLOR(white));
            }
        }
    }
    //Player
    offset[0] = pos[0];
    offset[1] = pos[1];
    // g_screen_draw_line(offset[0],
    //                    offset[1],
    //                    offset[0] + player->dir[0] * (map->tile_w << 1),
    //                    offset[1] + player->dir[1] * (map->tile_h << 1), COLOR(red));
    g_screen_draw_line(offset[0],
                       offset[1],
                       offset[0] + player->right_frustum_ray[0] * (map->tile_w << 4),
                       offset[1] + player->right_frustum_ray[1] * (map->tile_h << 4), COLOR(cyan));
    g_screen_draw_line(offset[0],
                       offset[1],
                       offset[0] + player->left_frustum_ray[0] * (map->tile_w << 4),
                       offset[1] + player->left_frustum_ray[1] * (map->tile_h << 4), COLOR(cyan));
    g_screen_draw_circle(VI2TOV2(offset), player->hitbox_radius * map->tile_w, COLOR(green));

    v2 hitscan_left, hitscan_right, hitscan_left_long, hitscan_right_long;
    pi_v2_muls(player->plane, player->hitscan_range, hitscan_right);
    pi_v2_muls(hitscan_right, -1, hitscan_left);
    pi_v2_muls(player->plane, player->hitscan_range, hitscan_right_long);
    pi_v2_muls(hitscan_right_long, -1, hitscan_left_long);
    v2 tmp_dir;
    pi_v2_muls(player->dir, 10, tmp_dir);
    pi_v2_add(hitscan_left_long, tmp_dir, hitscan_left_long);
    pi_v2_add(hitscan_right_long, tmp_dir, hitscan_right_long);
    g_screen_draw_line(offset[0] + hitscan_left[0] * map->tile_w,
                       offset[1] + hitscan_left[1] * map->tile_h,
                       offset[0] + hitscan_left_long[0] * map->tile_w,
                       offset[1] + hitscan_left_long[1] * map->tile_h,
                       COLOR(magenta));
    g_screen_draw_line(offset[0] + hitscan_right[0] * map->tile_w,
                       offset[1] + hitscan_right[1] * map->tile_h,
                       offset[0] + hitscan_right_long[0] * map->tile_w,
                       offset[1] + hitscan_right_long[1] * map->tile_h,
                       COLOR(magenta));

    //Enemies
    offset[0] = pos[0] - player->pos[0] * map->tile_w;
    offset[1] = pos[1] - player->pos[1] * map->tile_h;
    for (uint i = 0; i < enemies->cap; i++)
    {
        if (enemies->data[i].is_alive)
        {
            const int rad = enemies->data[i].hitbox_radius * map->tile_w;
            v2 screen_pos = {offset[0] + enemies->data[i].pos[0] * map->tile_w, offset[1] + enemies->data[i].pos[1] * map->tile_h};
            //debug_v2(offset)
            //debug("%d %d", map_screen_pos[0], map_screen_pos[1])
            if ((int)screen_pos[0] - rad > map_screen_start[0] && (int)screen_pos[1] - rad > 0 &&
                (int)screen_pos[0] < (g_scr.w - rad) && (int)screen_pos[1] < (map_screen_end[1] - rad))
            {
                color enemy_color;
                const EnemyState state = (int)enemies->data[i].state->state;
                switch (state)
                {
                case EnemyState_Follow:
                    enemy_color = COLOR(orange);
                    break;

                case EnemyState_Attack:
                    enemy_color = COLOR(red);
                    break;

                default:
                    enemy_color = COLOR(light_gray);
                    break;
                }
                g_screen_draw_circle(screen_pos, rad, enemy_color);
            }
        }
    }
    //Projectiles
    offset[0] = pos[0] - player->pos[0] * map->tile_w;
    offset[1] = pos[1] - player->pos[1] * map->tile_h;
    for (uint i = 0; i < projectiles->cap; i++)
    {
        if (projectiles->data[i].is_alive)
        {
            const int rad = projectiles->data[i].radius * map->tile_w;
            v2 screen_pos = {offset[0] + projectiles->data[i].pos[0] * map->tile_w, offset[1] + projectiles->data[i].pos[1] * map->tile_h};
            if ((int)screen_pos[0] - rad > map_screen_start[0] && (int)screen_pos[1] - rad > 0 &&
                (int)screen_pos[0] < (g_scr.w - rad) && (int)screen_pos[1] < (map_screen_end[1] - rad))
                g_screen_draw_circle(screen_pos, rad, COLOR(blue));
        }
    }
    //Map frame
    g_screen_fill_quat((v2){g_scr.w - map_w * map->tile_w, 0},
                       (v2){map->tile_w, map_h * map->tile_h},
                       COLOR(dark_golden));
    g_screen_fill_quat((v2){g_scr.w - (map_w - 1) * map->tile_w, (map_h - 1) * map->tile_h},
                       (v2){map_w * map->tile_w, map->tile_h},
                       COLOR(dark_golden));

    // HUD
    color player_hp_color_start = COLOR(red);
    color player_hp_color_end = COLOR(green);
    color player_hp_color;
    player_hp_color.r = pi_lerp(player_hp_color_start.r, player_hp_color_end.r, MAX(player->hp / player->max_hp, 0));
    player_hp_color.g = pi_lerp(player_hp_color_start.g, player_hp_color_end.g, MAX(player->hp / player->max_hp, 0));
    player_hp_color.b = pi_lerp(player_hp_color_start.b, player_hp_color_end.b, MAX(player->hp / player->max_hp, 0));
    g_screen_draw_number(5, g_scr.h - 40, player->hp, player_hp_color);
    g_screen_draw_number(115, g_scr.h - 40, enemies->data[0].state_frame, COLOR(green));
#endif
}

void map_clean(Map *map)
{
    free(map->data);
    vec_free(map->chunks);
}
