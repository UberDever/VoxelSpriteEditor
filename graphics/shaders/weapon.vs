#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec3 a_normal;

out vec3 v_color;

uniform mat4 u_inverse_modelView;
uniform mat4 u_modelView;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_modelView * vec4(a_position, 1.0);
    v_color = a_color;
}