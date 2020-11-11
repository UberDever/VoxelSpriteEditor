//
// Created by uberdever on 06.09.2020.
//
#ifndef IND3D_CORE_H
#define IND3D_CORE_H

#include "../include/alphabeta.h"


<<<<<<< HEAD
=======
// 3d typedefs
typedef struct Camera
{
    v3_t dir;
    v3_t plane;
    m4_t view_matrix;
    m4_t projection_matrix;

    float32 sensitivity;

    float32 horisontal_rotation;
    float32 vertrical_rotation;
} Camera;

typedef struct Texture
{
    GLuint id;
    int w, h;
    uint8* data;
} Texture;

//Global typedefs
typedef struct Player
{
    v2_t pos, momentum;
    v2_t left_frustum_ray, right_frustum_ray;
    float32 hitbox_radius;
    Camera camera;
    // Game variables
    float32 hp, max_hp;
    float32 hitscan_range;
} Player;

typedef struct Chunk
{
    Model model;
} Chunk;
vec_register(Chunk);

typedef struct Map
{
    int *data;
    int w, h;
    int tile_w, tile_h;
    // OpenGl stuff
    int chunks_w, chunks_h;
    Texture tileset;
    v_Chunk_t chunks;
} Map;

typedef struct Projectile
{
    v2_t pos, momentum;
    float32 radius, speed;
    bool is_alive;
} Projectile;
vec_register(Projectile);

typedef enum EnemyTypes
{
    EnemyTypes_Void = 0,
    EnemyTypes_Common = 1,
    EnemyTypes_Strong = 2,
    EnemyTypes_Boss = 3,
    EnemyTypes_Common_Range = 4,
    EnemyTypes_Strong_Range = 5,
    EnemyTypes_Boss_Range = 6,
    EnemyTypes_Size = 7
} EnemyTypes;

typedef enum EnemyState
{
    EnemyState_Idle = 0,
    EnemyState_Follow = 1,
    EnemyState_Attack = 2,
    EnemyState_Death = 3,
    EnemyState_IdleRange = 4,
    EnemyState_FollowRange = 5,
    EnemyState_AttackRange = 6,
    EnemyState_DeathRange = 7,
    EnemyState_Size = 8
} EnemyState;

typedef struct Enemy Enemy;

typedef struct EnemyBehaviour
{
    void (*act)(Map *map, Player *player, Enemy *enemy);
    EnemyState state;
} EnemyBehaviour;

struct Enemy
{
    v2_t pos, momentum, dir_to_player;
    float32 hitbox_radius, speed, len_to_player;
    uint state_frame;
    struct EnemyBehaviour *state;
    enum EnemyTypes type;
    bool is_alive;
    // Game variables
    float32 hp, max_hp;
};
PTR_TYPE(Enemy);
vec_register(Enemyptr);
vec_register(Enemy);
vec_register(EnemyBehaviour);

//Globals
extern int half_w, half_h;
extern uint frames;

// TODO: remove later
extern float player_height_debug;

static bool tile_is_wall(Map *map, int x, int y)
{
    const int index = y * map->w + x;
    return map->data[index] != 0 &&
           map->data[index] != 'P' &&
           map->data[index] != 'E';
}

static bool near_player(v2_t entity_pos, v2_t player_pos, int range)
{
    int player_chunk_pos_x = player_pos.x / C_CHUNK_W, player_chunk_pos_y = player_pos.y / C_CHUNK_H;
    int entity_chunk_pos_x = entity_pos.x / C_CHUNK_W, entity_chunk_pos_y = entity_pos.y / C_CHUNK_H;
    return ((abs(player_chunk_pos_x - entity_chunk_pos_x) +
             abs(player_chunk_pos_y - entity_chunk_pos_y)) <= range);
}

static void linecast(Map *map, int start_x, int start_y, int end_x, int end_y, int *wall_x, int *wall_y, bool (*f)(Map *map, int, int))
{
    int dx = abs(end_x - start_x), sx = start_x < end_x ? 1 : -1;
    int dy = -abs(end_y - start_y), sy = start_y < end_y ? 1 : -1;
    int err = dx + dy, e2; //error value e_xy
    while (1)
    { // loop
        if (start_x >= 0 && start_x < map->w && start_y >= 0 && start_y < map->h)
        {
            if (f(map, start_x, start_y))
            {
                *wall_x = start_x;
                *wall_y = start_y;
            }
        }
        e2 = 2 * err;
        if (e2 >= dy)
        { // e_xy+e_x > 0
            if (start_x == end_x)
                break;
            err += dy;
            start_x += sx;
        }
        if (e2 <= dx)
        { // e_xy+e_y < 0
            if (start_y == end_y)
                break;
            err += dx;
            start_y += sy;
        }
    }
}
>>>>>>> 461d48516d810d37fc7cb2b50f8db4ffa092ed7a

#endif