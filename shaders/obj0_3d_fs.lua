#version 330 core

out vec4 FragColor;

in vec3 vs_normal;
in vec3 vs_pos;
in vec2 vs_uv;

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

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular; };

struct PointLight {
	vec3 position;

	float constK;
	float linK;
	float quadK;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular; };

struct SpotLight {
	vec3 position;
	vec3 direction;

	float cutOffIn;
	float cutOffOut;

	float constK;
	float linK;
	float quadK;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular; };

uniform vec3 emisGlow;

uniform vec3 viewPos;

uniform DirLight dirLight;
uniform PointLight pntLight;
uniform SpotLight sptLight;
uniform Material material;

vec3 CalcAmb(vec3 lAmb);
vec3 CalcDiff(vec3 lDiff, vec3 lDir, vec3 normal);
vec3 CalcSpec(vec3 lSpec, vec3 lDir, vec3 normal, vec3 viewDir);

vec3 CalcDirLight(DirLight dLight, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pLight, vec3 normal, vec3 viewDir, vec3 fPos);
vec3 CalcSpotLight(SpotLight sLight, vec3 normal, vec3 viewDir, vec3 fPos);

void main()
{
	vec3 emission = vec3(0.0f);
	vec3 result = vec3(0.3f);

	vec3 normal = normalize(vs_normal);
	//From view point to fragment
	vec3 viewDir = normalize(viewPos - vs_pos);

	emission = texture(material.texture_emission, vs_uv).rgb * emisGlow;

	result += CalcDirLight(dirLight, normal, viewDir);
	result += CalcPointLight(pntLight, normal, viewDir, vs_pos);
	result += CalcSpotLight(sptLight, normal, viewDir, vs_pos);
	
	result += emission;

	FragColor = vec4(result, 1.0f);
}

vec3 CalcAmb(vec3 lAmb)
{
	return lAmb * vec3(texture(material.texture_diffuse, vs_uv));
}

vec3 CalcDiff(vec3 lDiff, vec3 lDir, vec3 normal)
{
	float diff = max(dot(normal, lDir), 0.0f);
	return ( lDiff * diff * vec3(texture(material.texture_diffuse, vs_uv)) );
}

vec3 CalcSpec(vec3 lSpec, vec3 lDir, vec3 normal, vec3 viewDir)
{
	vec3 reflectDir = reflect(-lDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininness);

	return ( lSpec * spec * vec3(texture(material.texture_specular, vs_uv)) );
}

vec3 CalcDirLight(DirLight dLight, vec3 normal, vec3 viewDir)
{
	vec3 lightFragDir = normalize(dLight.direction);

	vec3 ambientL = CalcAmb(dLight.ambient);
	vec3 diffuseL = CalcDiff(dLight.diffuse, lightFragDir, normal);
	vec3 specularL = CalcSpec(dLight.specular, lightFragDir, normal, viewDir);

	return (ambientL + diffuseL + specularL);
}

vec3 CalcPointLight(PointLight pLight, vec3 normal, vec3 viewDir, vec3 fPos)
{
	vec3 lightFragDir = normalize(pLight.position - fPos);

	vec3 ambientL = CalcAmb(pLight.ambient);
	vec3 diffuseL = CalcDiff(pLight.diffuse, lightFragDir, normal);
	vec3 specularL = CalcSpec(pLight.specular, lightFragDir, normal, viewDir);

	float distance = length(pLight.position - fPos);
	float attenuation = 1.0f / (pLight.constK +
	pLight.linK * distance + pLight.quadK * (distance * distance));
	
	return ( (ambientL + diffuseL + specularL) * attenuation );
}

vec3 CalcSpotLight(SpotLight sLight, vec3 normal, vec3 viewDir, vec3 fPos)
{
	vec3 lightFragDir = normalize(sLight.position - fPos);

	vec3 ambientL = CalcAmb(sLight.ambient);
	vec3 diffuseL = CalcDiff(sLight.diffuse, lightFragDir, normal);
	vec3 specularL = CalcSpec(sLight.specular, lightFragDir, normal, viewDir);

	float distance = length(sLight.position - fPos);
	float attenuation = 1.0f / (sLight.constK +
	sLight.linK * distance + sLight.quadK * (distance * distance));
	
	vec3 lightDir = normalize(sLight.direction);
	float theta = dot(lightDir, normalize(-sLight.direction))
	float epsilon = sLight.cutOffIn - sLight.cutOffOut;
	float intensity = clamp ( (theta - sLight.cutOffOut) / epsilon, 0.0f, 1.0f);

	return ( (ambientL + diffuseL + specularL) * attenuation * intensity );
}