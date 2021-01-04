#version 330 core

out vec4 px_color;

in vec3 vs_pos;
in vec3 vs_norm;
in vec2 vs_uv;

struct Material {
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D texture_normal;
	sampler2D texture_height;
	
	bool isTextured;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float shininness; };

uniform Material material_2d;

void main()
{
	if (material_2d.isTextured)
	{
		px_color = vec4(texture(material_2d.texture_diffuse, vs_uv).rgba);
	}
	else
	{
		px_color = vec4(material_2d.diffuse, 1.0f);
	}
}