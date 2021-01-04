#version 330 core

out vec4 FragColor;

in vec2 vs_uv;

struct Material {
	sampler2D texture_diffuse;
};

uniform vec3 lightColor;

uniform Material material;

void main()
{
	FragColor = vec4(texture(material.texture_diffuse, vs_uv).rgb, 1.0f);
}