#version 330 core

in vec2 v_texindex;
in vec4 v_color;

out vec4 f_color;

uniform sampler2D u_texture;

void main() {
    f_color = texture(u_texture, v_texindex);
    vec4 color = v_color;
    color.a = 1.0;
    f_color *= color;
}