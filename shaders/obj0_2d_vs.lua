#version 330 core

layout(location = 0) in vec2 m_pos;
layout(location = 1) in vec2 m_norm;
layout(location = 2) in vec2 m_uv;

out vec3 vs_pos;
out vec3 vs_norm;
out vec2 vs_uv;

uniform mat4 model;
uniform mat4 projView;

void main()
{
	vs_pos = vec3(model * projView * vec4(m_pos, 0.0f, 1.0f));
	vs_norm = vec3(vec4(m_norm, 0.0f, 1.0f));
	vs_uv = m_uv;
	gl_Position = vec4(vs_pos, 1.0f);
	gl_Position = vec4(m_pos, 0.0f, 1.0f);
}