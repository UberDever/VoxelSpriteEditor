#include "geometry.h"

// Vx returned in CCW direction
// 0--3
// | /|
// |/ |
// 1--2
void generate_quat(v_GLfloat_t *vertices, v_GLuint_t *indices, ChunkVx vx[4], GLuint vx_stride)
{
    GLuint start_index = vertices->size / vx_stride;

    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[0].p.v[i]);
    for (int i = 0; i < 2; i++)
        vec_push(*vertices, vx[0].t.v[i]);
    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[0].n.v[i]);

    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[1].p.v[i]);
    for (int i = 0; i < 2; i++)
        vec_push(*vertices, vx[1].t.v[i]);
    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[1].n.v[i]);

    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[2].p.v[i]);
    for (int i = 0; i < 2; i++)
        vec_push(*vertices, vx[2].t.v[i]);
    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[2].n.v[i]);

    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[3].p.v[i]);
    for (int i = 0; i < 2; i++)
        vec_push(*vertices, vx[3].t.v[i]);
    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[3].n.v[i]);

    vec_push(*indices, start_index + 0);
    vec_push(*indices, start_index + 1);
    vec_push(*indices, start_index + 3);
    vec_push(*indices, start_index + 3);
    vec_push(*indices, start_index + 1);
    vec_push(*indices, start_index + 2);
}

void generate_pube(v_GLfloat_t *vertices, v_GLuint_t *indices, bool adj_check[4], int x, int y, v2_t t, GLuint cur_pube_idx)
{
    const GLfloat s = 0.5f;
    GLfloat vx[] = {
        -s, -s, -s, 1.f, 1.f, 0.f, 0.f, 0.f,
        -s, -s, +s, .25f, 1.f, 0.f, 0.f, 0.f,
        -s, +s, -s, 1.f, 0.f, 0.f, 0.f, 0.f,
        -s, +s, +s, .25f, 0.f, 0.f, 0.f, 0.f,
        +s, -s, -s, .75f, 1.f, 0.f, 0.f, 0.f,
        +s, -s, +s, .5f, 1.f, 0.f, 0.f, 0.f,
        +s, +s, -s, .75f, 0.f, 0.f, 0.f, 0.f,
        +s, +s, +s, .5f, 0.f, 0.f, 0.f, 0.f,
        -s, -s, -s, 0.f, 1.f, 0.f, 0.f, 0.f,
        -s, +s, -s, 0.f, 0.f, 0.f, 0.f, 0.f};
    for (uint i = 0; i < ARR_ONE_DIM_LEN(vx); i += 8)
    {
        vx[i + 0] += x;
        vx[i + 1] += 0;
        vx[i + 2] += y;
        vx[i + 3] += t.x;
        vx[i + 4] += t.y;
    }
    for (uint i = 0; i < ARR_ONE_DIM_LEN(vx); i++)
    {
        vec_push(*vertices, vx[i]);
    }
    GLuint cur_idx = cur_pube_idx * 10;
    if (adj_check[0])
    {
        GLuint ind[] = {0, 2, 4, 2, 6, 4};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
    if (adj_check[1])
    {
        GLuint ind[] = {6, 5, 4, 6, 7, 5};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
    if (adj_check[2])
    {
        GLuint ind[] = {7, 3, 5, 3, 1, 5};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
    if (adj_check[3])
    {
        GLuint ind[] = {3, 9, 1, 9, 8, 1};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
}

typedef uint64_t depth_t;
#define DEPTH_LEVELS (sizeof(depth_t) * CHAR_BIT)

static void generate_voxel_side(v_GLfloat_t *vertices, v_GLuint_t *indices, v3_t p0, v3_t p1, v3_t p2, v3_t p3, color color, int cur_idx)
{
    GLfloat vx[] = {
        p0.x, p0.y, p0.z, color.r, color.g, color.b, 0, 0, 0,
        p1.x, p1.y, p1.z, color.r, color.g, color.b, 0, 0, 0,
        p2.x, p2.y, p2.z, color.r, color.g, color.b, 0, 0, 0,
        p3.x, p3.y, p3.z, color.r, color.g, color.b, 0, 0, 0};
    GLuint ind[] = {
        0, 1, 2, 2, 1, 3};
    for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
    {
        vec_push(*indices, cur_idx + ind[i]);
    }
    for (uint i = 0; i < ARR_ONE_DIM_LEN(vx); i++)
    {
        vec_push(*vertices, vx[i]);
    }
}

static inline void generate_voxel_face(v_GLfloat_t *vertices, v_GLuint_t *indices,
                                       int base_x, int base_y, int base_w, int base_h,
                                       int test_x, int test_y,
                                       int x, int y, int w, int h,
                                       color color, depth_t *segment_depth, depth_t *depth,
                                       int *cur_idx, char direction)
{

    depth_t d = segment_depth[(y * w) + x];
    if (test_x == INT32_MIN) // Invalid value for parameter-based check
        test_x = base_x + x;
    if (test_y == INT32_MIN)
        test_y = base_y + y;
    if ((test_x) >= 0 && (test_y) >= 0 && (test_x < base_w) && (test_y < base_h))
        d &= ~depth[((test_y)*base_w) + (test_x)];
    float32 p_x0, p_x1, p_y0, p_y1;
    switch (direction)
    {
    case 'w':
        p_x0 = 0;
        p_x1 = 0;
        p_y0 = y;
        p_y1 = y + 1;
        break;
    case 'n':
        p_x0 = x + 1;
        p_x1 = x;
        p_y0 = 0;
        p_y1 = 0;
        break;
    case 'e':
        p_x0 = w;
        p_x1 = w;
        p_y0 = y + 1;
        p_y1 = y;
        break;
    case 's':
        p_x0 = x;
        p_x1 = x + 1;
        p_y0 = h;
        p_y1 = h;
        break;
    }
    for (int j = 0; j < DEPTH_LEVELS; j++)
    {
        if (d & (1ULL << (depth_t)j))
        {
            v3_t p0 = (v3_t){(base_x + p_x0), base_h - (base_y + p_y0), j};
            v3_t p1 = (v3_t){(base_x + p_x1), base_h - (base_y + p_y1), j};
            v3_t p2 = (v3_t){(base_x + p_x0), base_h - (base_y + p_y0), j + 1};
            v3_t p3 = (v3_t){(base_x + p_x1), base_h - (base_y + p_y1), j + 1};
            generate_voxel_side(vertices, indices, p0, p1, p2, p3, color, *cur_idx);
            *cur_idx += 4;
        }
    }
}

void generate_voxel_sprite(v_GLfloat_t *vertices, v_GLuint_t *indices, const char *sprite_path, const char *depthmap_path, v3_t voxel_dimensions)
{

    SDL_Surface *sprite = IMG_Load(sprite_path);
    if (!sprite)
        error("Cannot load sprite %s", sprite_path);

    const int sprite_size = sprite->w * sprite->h;
    bool is_conquered[sprite_size];
    memset(is_conquered, 0, sprite_size);
    depth_t depth[sprite_size];
    memset(depth, 0, sprite_size);
    color *pixels = sprite->pixels;
    uint8 r = 0, g = 0, b = 0, a = 0;
    depth_t cur_depth = 0;
    int cur_idx = 0;
    if (depthmap_path)
    {
        FILE *depthmap = fopen(depthmap_path, "rb");
        if (!depthmap)
            error("Cannot open depthmap %s", depthmap_path);
        fread(depth, sizeof(depth_t) * sprite_size, 1, depthmap);
        fclose(depthmap);
    }
    else
    {
        for (int i = 0; i < sprite_size; i++)
        {
            depth[i] |= (1ULL << 31ULL) | (1ULL << 32ULL);
        }
    }
    for (int i = 0; i < sprite_size; i++)
    {
        r = pixels[i].r;
        g = pixels[i].g;
        b = pixels[i].b;
        a = pixels[i].a;
        cur_depth = depth[i];
        if (is_conquered[i] || (a == 0))
            continue;
        bool pixels_left = true;
        int w = 1, h = 1;
        while (1)
        {
            bool is_width_grew = true;
            for (int j = 0; j < h; j++)
            {
                is_width_grew &= (r == pixels[i + w + (j * sprite->w)].r) &&
                                 (g == pixels[i + w + (j * sprite->w)].g) &&
                                 (b == pixels[i + w + (j * sprite->w)].b) &&
                                 (a == pixels[i + w + (j * sprite->w)].a) &&
                                 (cur_depth == depth[i + w + (j * sprite->w)]);
            }
            int test_w = w + (is_width_grew == true);
            bool is_height_grew = true;
            for (int j = 0; j < test_w; j++)
            {
                is_height_grew &= (r == pixels[i + j + ((h)*sprite->w)].r) &&
                                  (g == pixels[i + j + ((h)*sprite->w)].g) &&
                                  (b == pixels[i + j + ((h)*sprite->w)].b) &&
                                  (a == pixels[i + j + ((h)*sprite->w)].a) &&
                                  (cur_depth == depth[i + j + ((h)*sprite->w)]);
            }
            h += (is_height_grew == true);
            w = test_w;

            if (!is_width_grew && !is_height_grew)
            {
                const int i_x = i % sprite->w;
                const int i_y = i / sprite->w;
                depth_t segment_depth[w * h];
                for (int k = 0; k < h; k++)
                {
                    for (int l = 0; l < w; l++)
                    {
                        segment_depth[k * w + l] = depth[(i_y + k) * sprite->w + (i_x + l)];
                    }
                }
                // test frame adjacent pixels, opposite frame direction
                for (int y = 0; (y < h); y++) // west faces
                {
                    generate_voxel_face(vertices, indices, i_x, i_y, sprite->w, sprite->h, i_x - 1, INT32_MIN,
                                        0, y, w, h, (color){.r = r, .g = g, .b = b, .a = a},
                                        segment_depth, depth, &cur_idx, 'w');
                }
                for (int x = 0; (x < w); x++) // north faces
                {
                    generate_voxel_face(vertices, indices, i_x, i_y, sprite->w, sprite->h, INT32_MIN, i_y - 1,
                                        x, 0, w, h, (color){.r = r, .g = g, .b = b, .a = a},
                                        segment_depth, depth, &cur_idx, 'n');
                }
                for (int y = 0; (y < h); y++) // east faces
                {
                    generate_voxel_face(vertices, indices, i_x, i_y, sprite->w, sprite->h, i_x + w, INT32_MIN,
                                        0, y, w, h, (color){.r = r, .g = g, .b = b, .a = a},
                                        segment_depth, depth, &cur_idx, 'e');
                }
                for (int x = 0; (x < w); x++) // south faces
                {
                    generate_voxel_face(vertices, indices, i_x, i_y, sprite->w, sprite->h, INT32_MIN, i_y + h,
                                        x, h - 1, w, h, (color){.r = r, .g = g, .b = b, .a = a},
                                        segment_depth, depth, &cur_idx, 's');
                }
                // generate flip-flop faces (+- z faces)
                depth_t d = segment_depth[0];
                bool prev_bit = false;
                for (int j = 0; j <= DEPTH_LEVELS; j++)
                {
                    bool bit = d & (1ULL << (depth_t)j) ? true : false;
                    if (prev_bit == false && bit == true)
                    {
                        // -z
                        v3_t p0 = (v3_t){(i_x + 0), sprite->h - (i_y + 0), j};
                        v3_t p1 = (v3_t){(i_x + w), sprite->h - (i_y + 0), j};
                        v3_t p2 = (v3_t){(i_x + 0), sprite->h - (i_y + h), j};
                        v3_t p3 = (v3_t){(i_x + w), sprite->h - (i_y + h), j};
                        generate_voxel_side(vertices, indices, p0, p1, p2, p3, (color){.r = r, .g = g, .b = b, .a = a}, cur_idx);
                        cur_idx += 4;
                    }
                    else if (prev_bit == true && (bit == false || j == DEPTH_LEVELS))
                    {
                        // +z
                        v3_t p0 = (v3_t){(i_x + 0), sprite->h - (i_y + h), j};
                        v3_t p1 = (v3_t){(i_x + w), sprite->h - (i_y + h), j};
                        v3_t p2 = (v3_t){(i_x + 0), sprite->h - (i_y + 0), j};
                        v3_t p3 = (v3_t){(i_x + w), sprite->h - (i_y + 0), j};
                        generate_voxel_side(vertices, indices, p0, p1, p2, p3, (color){.r = r, .g = g, .b = b, .a = a}, cur_idx);
                        cur_idx += 4;
                    }
                    prev_bit = bit;
                }
                for (int k = 0; k < h; k++)
                {
                    for (int l = 0; l < w; l++)
                    {
                        is_conquered[(i_y + k) * sprite->w + (i_x + l)] = true;
                    }
                }
                //is_conquered[i_y * sprite->w + i_x] = 7;
                break;
            }
        }
    }
    // for (int k = 0; k < sprite->h; k++)
    // {
    //     for (int l = 0; l < sprite->w; l++)
    //     {
    //         printf("%d ", is_conquered[(k * sprite->w) + l]);
    //     }
    //     printf("\n");
    // }
    for (uint i = 0; i < vertices->size; i += 9)
    {
        vertices->data[i + 0] -= sprite->w / 2;
        vertices->data[i + 0] *= voxel_dimensions.x;
        vertices->data[i + 1] -= sprite->h / 1.5;
        vertices->data[i + 1] *= voxel_dimensions.y;
        vertices->data[i + 2] -= DEPTH_LEVELS / 2;
        vertices->data[i + 2] *= voxel_dimensions.z;
        vertices->data[i + 3] /= 0xFF;
        vertices->data[i + 4] /= 0xFF;
        vertices->data[i + 5] /= 0xFF;
    }
}
