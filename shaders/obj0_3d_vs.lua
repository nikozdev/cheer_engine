#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

out vec3 vs_pos;
out vec3 vs_normal;
out vec2 vs_uv;

uniform mat4 model;
uniform mat4 projView;

void main()
{
	vs_pos = vec3(model * vec4(aPos, 1.0f));
	vs_normal = mat3(transpose(inverse(model))) * aNormal;
	vs_uv = aUV;

	gl_Position = projView * vec4(vs_pos, 1.0f);
}