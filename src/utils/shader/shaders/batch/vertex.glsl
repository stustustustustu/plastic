#version 330 core

layout(location=0) in vec2 in_pos;
layout(location=1) in vec2 in_tex;
layout(location=2) in vec4 in_color;

out vec2 v_texindex;
out vec4 v_color;

uniform mat4 u_mvp;

void main() {
    gl_Position = u_mvp * vec4(in_pos, 0.0, 1.0);
    v_texindex = in_tex;
    v_color = in_color;
}