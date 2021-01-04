#version 330 core

layout(location = 0) in vec2 m_pos;
layout(location = 1) in vec2 m_uv;

uniform mat4 model;
uniform mat4 projView;

out vec3 vs_pos;
out vec2 vs_uv;

void main()
{
	vs_uv = m_uv;
	vs_pos = vec3(model * vec4(m_pos, 0.0f, 1.0f));
	gl_Position = model * projView * vec4(m_pos, 0.0f, 1.0f);
}