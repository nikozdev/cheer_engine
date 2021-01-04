#version 330 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out vec3 vs_pos;
out vec3 vs_normal;
out vec2 vs_uv;

void main()
{
	vs_pos = vec3(a_position);
	vs_normal = vec3(a_normal, 1.0f);
	vs_uv = a_uv;
	gl_Position = vec4(position, 0.5f, 1.0f);
}