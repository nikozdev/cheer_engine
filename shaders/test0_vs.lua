#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTexCoord;

out vec3 vs_fragPos;
out vec3 vs_normal;
out vec2 vs_UV;

uniform mat4 projView;
uniform mat4 model;

void main()
{
	vs_fragPos = vec4(model * vec4(aPos, 1.0f)).xyz;
	vs_normal = mat3(model) * aNorm;
	vs_UV = vec2(aTexCoord.x, aTexCoord.y * (-1.0f));

	gl_Position = projView * model * vec4(aPos, 1.0f);
}