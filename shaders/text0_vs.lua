#version 330 core

layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_uv;

out vec2 vs_pos;
out vec2 vs_uv;

uniform mat4 projView;

void main()
{
	gl_Position = projView * vec4(a_pos, 0.0f, 1.0f);
	vs_pos = a_pos;
	vs_uv = a_uv;
}