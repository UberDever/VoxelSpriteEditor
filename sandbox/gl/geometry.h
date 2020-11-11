#ifndef IND3D_GEOMETRY_H
#define IND3D_GEOMETRY_H

#include "../core.h"

void generate_quat(v_GLfloat_t *vertices, v_GLuint_t *indices, ChunkVx vx[4], GLuint vx_stride);
// Pube = Pipe + Cube -> Cube without top and bottom, also potentially without some sides
void generate_pube(v_GLfloat_t *vertices, v_GLuint_t *indices, bool sides[4], int x, int y, v2_t t, GLuint cur_pube_idx);
// Voxel sprite is 3D model, made from sprite and, optionally, heightmap. Looks like minecraft tool without hegihtmap, but with it turns into stylized model
void generate_voxel_sprite(v_GLfloat_t *vertices, v_GLuint_t *indices, const char* sprite_path, const char* heightmap_path, v3_t voxel_dimensions);

#endif