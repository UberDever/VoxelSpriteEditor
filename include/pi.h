//
// Created by uberdever on 04.08.2020.
//
// Pi contains all pure-functional math, primarily for 2d/3d graphics
// All pure functions are defined inline
// Stick to column-major ordering
// Stick to left-handed system (like not default opengl)
// Right handed is easy, just flip the z sign at ndc transformation. But also change vert ordering
#ifndef ALPHABETA_PI_H
#define ALPHABETA_PI_H

#include "alpha.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_2PI
#define M_2PI 6.283185307179586476925286766559
#endif

#ifndef M_PI_2
#define M_PI_2 1.5707963267948966192313216916398
#endif

#ifndef M_PI_4
#define M_PI_4 0.78539816339744830961566084581988
#endif

#define SQRT2 1.41421356237309504880f
#define SQRT2_2 0.70710678118654752440f

#define DEG2RAD(phi) (0.01745329f * (phi))
#define RAD2DEG(phi) (57.2957795f * (phi))

/*
 *  Newly updated vector math
 */

typedef union v2_t
{
    float32 v[2];
    struct
    {
        float32 x, y;
    };
} v2_t;
#define debug_v2(v) debug("( %f %f )", (v).x, (v).y)

typedef union v3_t
{
    float32 v[3];
    struct
    {
        float32 x, y, z;
    };
} v3_t;
#define debug_v3(v) debug("( %f %f %f )", (v).x, (v).y, (v).z)

typedef union v4_t
{
    float32 v[4];
    struct
    {
        float32 x, y, z, w;
    };
} v4_t;
#define debug_v4(v) debug("( %f %f %f %f)", (v).x, (v).y, (v).z, (v).w)

typedef union m2_t
{
    float32 m[4];
    float32 mat[2][2];
    struct
    {
        float32 m00, m10;
        float32 m01, m11;
    };
} m2_t;

typedef union m3_t
{
    float32 m[9];
    float32 mat[3][3];
    struct
    {
        float32 m00, m10, m20;
        float32 m01, m11, m21;
        float32 m02, m12, m22;
    };
} m3_t;

typedef union m4_t
{
    float32 m[16];
    float32 mat[4][4];
    struct
    {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
    };
} m4_t;

// v2
#if 1
static inline v2_t v2_ones()
{
    return (v2_t){1.f, 1.f};
}

static inline v2_t v2_(const v2_t v)
{
    return (v2_t){v.x, v.y};
}

static inline v2_t v2_add(const v2_t v0, const v2_t v1)
{
    return (v2_t){v0.x + v1.x, v0.y + v1.y};
}

static inline v2_t v2_sub(const v2_t v0, const v2_t v1)
{
    return (v2_t){v0.x - v1.x, v0.y - v1.y};
}

static inline v2_t v2_mul(const v2_t v0, const v2_t v1)
{
    return (v2_t){v0.x * v1.x, v0.y * v1.y};
}

static inline v2_t v2_div(const v2_t v0, const v2_t v1)
{
    return (v2_t){v0.x / v1.x, v0.y / v1.y};
}

static inline v2_t v2_adds(const v2_t v, const float32 scalar)
{
    return (v2_t){v.x + scalar, v.y + scalar};
}

static inline v2_t v2_subs(const v2_t v, const float32 scalar)
{
    return (v2_t){v.x - scalar, v.y - scalar};
}

static inline v2_t v2_muls(const v2_t v, const float32 scalar)
{
    return (v2_t){v.x * scalar, v.y * scalar};
}

static inline v2_t v2_divs(const v2_t v, const float32 scalar)
{
    return (v2_t){v.x / scalar, v.y / scalar};
}

static inline float32 v2_dot(const v2_t v0, const v2_t v1)
{
    return v0.x * v1.x + v0.y * v1.y;
}

static inline float32 v2_area(const v2_t v0, const v2_t v1)
{
    return v0.x * v1.y - v0.y + v1.x;
}

static inline float32 v2_len_sq(const v2_t v)
{
    return v2_dot(v, v);
}

static inline float32 v2_len(const v2_t v)
{
    return sqrtf(v2_len_sq(v));
}

static inline v2_t v2_norm(const v2_t v)
{
    float32 len = v2_len(v);
    return (v2_t){v.x / len, v.y / len};
}

// m2

static inline m2_t m2_identity()
{
    return (m2_t){1, 0, 0, 1};
}

// v3

static inline v3_t v3_ones()
{
    return (v3_t){1.f, 1.f, 1.f};
}

static inline v3_t v3_(const v3_t v)
{
    return (v3_t){v.x, v.y, v.z};
}

static inline v3_t v3_add(const v3_t v0, const v3_t v1)
{
    return (v3_t){v0.x + v1.x, v0.y + v1.y, v0.z + v1.z};
}

static inline v3_t v3_sub(const v3_t v0, const v3_t v1)
{
    return (v3_t){v0.x - v1.x, v0.y - v1.y, v0.z - v1.z};
}

static inline v3_t v3_mul(const v3_t v0, const v3_t v1)
{
    return (v3_t){v0.x * v1.x, v0.y * v1.y, v0.z * v1.z};
}

static inline v3_t v3_div(const v3_t v0, const v3_t v1)
{
    return (v3_t){v0.x / v1.x, v0.y / v1.y, v0.z / v1.z};
}

static inline v3_t v3_adds(const v3_t v, const float32 scalar)
{
    return (v3_t){v.x + scalar, v.y + scalar, v.z + scalar};
}

static inline v3_t v3_subs(const v3_t v, const float32 scalar)
{
    return (v3_t){v.x - scalar, v.y - scalar, v.z - scalar};
}

static inline v3_t v3_muls(const v3_t v, const float32 scalar)
{
    return (v3_t){v.x * scalar, v.y * scalar, v.z * scalar};
}

static inline v3_t v3_divs(const v3_t v, const float32 scalar)
{
    return (v3_t){v.x / scalar, v.y / scalar, v.z / scalar};
}

static inline float32 v3_dot(const v3_t v0, const v3_t v1)
{
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

static inline v3_t v3_cross(const v3_t v0, const v3_t v1)
{
    return (v3_t){
        v0.y * v1.z - v0.z * v1.y,
        v0.z * v1.x - v0.x * v1.z,
        v0.x * v1.y - v0.y * v1.x};
}

static inline float32 v3_len_sq(const v3_t v)
{
    return v3_dot(v, v);
}

static inline float32 v3_len(const v3_t v)
{
    return sqrtf(v3_len_sq(v));
}

static inline v3_t v3_norm(const v3_t v)
{
    float32 len = v3_len(v);
    return (v3_t){v.x / len, v.y / len, v.z / len};
}

static inline m4_t mat4(
    float m00, float m10, float m20, float m30,
    float m01, float m11, float m21, float m31,
    float m02, float m12, float m22, float m32,
    float m03, float m13, float m23, float m33)
{
    return (m4_t){
        .mat[0][0] = m00, .mat[1][0] = m10, .mat[2][0] = m20, .mat[3][0] = m30, .mat[0][1] = m01, .mat[1][1] = m11, .mat[2][1] = m21, .mat[3][1] = m31, .mat[0][2] = m02, .mat[1][2] = m12, .mat[2][2] = m22, .mat[3][2] = m32, .mat[0][3] = m03, .mat[1][3] = m13, .mat[2][3] = m23, .mat[3][3] = m33};
}

static inline m4_t m4_perspective(float vertical_field_of_view_in_deg, float aspect_ratio, float near_view_distance, float far_view_distance)
{
    float fovy_in_rad = vertical_field_of_view_in_deg / 180 * M_PI;
    float f = 1.0f / tanf(fovy_in_rad / 2.0f);
    float ar = aspect_ratio;
    float nd = near_view_distance, fd = far_view_distance;

    return mat4(
        f / ar, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (fd + nd) / (nd - fd), (2 * fd * nd) / (nd - fd),
        0, 0, -1, 0);
}

static inline m4_t m4_identity()
{
    return mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}

static inline m4_t m4_translation(v3_t offset)
{
    return mat4(
        1, 0, 0, offset.x,
        0, 1, 0, offset.y,
        0, 0, 1, offset.z,
        0, 0, 0, 1);
}

static inline m4_t m4_scaling(v3_t scale)
{
    float x = scale.x, y = scale.y, z = scale.z;
    return mat4(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1);
}

static inline m4_t m4_rotation_x(float angle_in_rad)
{
    float s = sinf(angle_in_rad), c = cosf(angle_in_rad);
    return mat4(
        1, 0, 0, 0,
        0, c, -s, 0,
        0, s, c, 0,
        0, 0, 0, 1);
}

static inline m4_t m4_rotation_y(float angle_in_rad)
{
    float s = sinf(angle_in_rad), c = cosf(angle_in_rad);
    return mat4(
        c, 0, s, 0,
        0, 1, 0, 0,
        -s, 0, c, 0,
        0, 0, 0, 1);
}

static inline m4_t m4_rotation_z(float angle_in_rad)
{
    float s = sinf(angle_in_rad), c = cosf(angle_in_rad);
    return mat4(
        c, -s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}

static inline m4_t m4_transpose(m4_t matrix)
{
    return mat4(
        matrix.m00, matrix.m01, matrix.m02, matrix.m03,
        matrix.m10, matrix.m11, matrix.m12, matrix.m13,
        matrix.m20, matrix.m21, matrix.m22, matrix.m23,
        matrix.m30, matrix.m31, matrix.m32, matrix.m33);
}

static inline m4_t m4_mul(m4_t a, m4_t b)
{
    m4_t result;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float sum = 0;
            for (int k = 0; k < 4; k++)
            {
                sum += a.mat[k][j] * b.mat[i][k];
            }
            result.mat[i][j] = sum;
        }
    }

    return result;
}

static inline m4_t m4_look_at(v3_t from, v3_t dir, v3_t up)
{
    v3_t z = v3_muls(dir, -1);
    v3_t x = v3_norm(v3_cross(up, z));
    v3_t y = v3_cross(z, x);

    return mat4(
        x.x, x.y, x.z, -v3_dot(from, x),
        y.x, y.y, y.z, -v3_dot(from, y),
        z.x, z.y, z.z, -v3_dot(from, z),
        0, 0, 0, 1);
}

static void m4_fprintp(FILE *stream, m4_t matrix, int width, int precision)
{
    m4_t m = matrix;
    int w = width, p = precision;
    for (int r = 0; r < 4; r++)
    {
        fprintf(stream, "| %*.*f %*.*f %*.*f %*.*f |\n",
                w, p, m.mat[0][r], w, p, m.mat[1][r], w, p, m.mat[2][r], w, p, m.mat[3][r]);
    }
}

static void m4_print(m4_t matrix)
{
    m4_fprintp(stdout, matrix, 6, 2);
}

static void m4_printp(m4_t matrix, int width, int precision)
{
    m4_fprintp(stdout, matrix, width, precision);
}

static void m4_fprint(FILE *stream, m4_t matrix)
{
    m4_fprintp(stream, matrix, 6, 2);
}

#endif
/*
 *  General purpose math
 */

static float32
wrap_angle(const float32 angle)
{
    const float32 mod = fmod(angle, 2 * M_PI);
    return mod > M_PI ? mod - 2 * M_PI : mod;
}

static bool pi_aabb_box_x_point(int p_x, int p_y, int b0_x, int b0_y, int b1_x, int b1_y)
{
    return ((p_x >= b0_x) && (p_y >= b0_y)) && ((p_x < b1_x) && (p_y < b1_y));
}

static inline bool pi_aabb_box_x_point_by_size(int x, int y, int box_x, int box_y, int w, int h)
{
    return pi_aabb_box_x_point(x, y, box_x, box_y, box_x + w, box_y + h);
}

static bool pi_raycast(float32 pos_x, float32 pos_y, float32 ray_x, float32 ray_y, float32 *intersect_x, float32 *intersect_y, bool (*f)(int, int))
{
    float32 side_x, side_y;
    float32 delta_x = fabsf(1.f / ray_x), delta_y = fabsf(1.f / ray_y);
    int step_x, step_y;
    bool hit = false;
    if (ray_x > 0)
    {
        step_x = 1;
        side_x = ((int)pos_x - pos_x + 1) * delta_x;
    }
    else
    {
        step_x = -1;
        side_x = (pos_x - (int)pos_x) * delta_x;
    }
    if (ray_y > 0)
    {
        step_y = 1;
        side_y = ((int)pos_y - pos_y + 1) * delta_y;
    }
    else
    {
        step_y = -1;
        side_y = (pos_y - (int)pos_y) * delta_y;
    }
    while (!hit)
    {
        if (side_x < side_y)
        {
            side_x += delta_x;
            pos_x += step_x;
        }
        else
        {
            side_y += delta_y;
            pos_y += step_y;
        }
        if (f(pos_x, pos_y))
            hit = true;
    }
    if (intersect_x && intersect_y)
    {
        *intersect_x = pos_x;
        *intersect_y = pos_y;
    }
    return hit;
}

static uint pi_log10(uint number)
{
    uint counter = 0;
    while (number /= 10)
        counter++;
    return counter;
}

static uint pi_log2(uint number)
{
    uint counter = 0;
    while (number >>= 1)
        counter++;
    return counter;
}

#define pi_lerp(start, end, amount) ((float32)(start) + (float32)((end) - (start)) * (float32)(amount))

#define pi_clamp(x, min, max) (MIN(MAX(x, min), max))

#endif //ALPHABETA_PI_H
