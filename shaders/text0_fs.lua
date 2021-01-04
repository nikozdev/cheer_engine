#version 330 core

out vec4 fs_color;

in vec2 vs_pos;
in vec2 vs_uv;

void main()
{
	fs_color = vec4(vs_pos.x, vs_pos.y, vs_pos.x, 1.0f);
}