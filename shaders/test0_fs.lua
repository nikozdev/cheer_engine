#version 330 core

in vec3 vs_fragPos;
in vec3 vs_normal;
in vec2 vs_UV;

out vec4 fragColor;

struct Material {
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D texture_normal;
	sampler2D texture_height;
	sampler2D texture_emission;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininness; };

uniform Material material;

void main()
{
	//if (material.texture_diffuse)
		fragColor = texture(material.texture_diffuse, vs_UV);
/*	else
		fragColor = vec4(material.ambient, 1.0f);*/
}