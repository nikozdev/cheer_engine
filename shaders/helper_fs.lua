#version 330 core
//Result of every fragment shader
out vec4 F_color;
//Variables from vertex shader
in vec3 vs_pos;
in vec3 vs_normal;
in vec2 vs_uv;

//Struct with material properties for solid and mapped color
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
//Simplest directional endlessly far positioned light
struct DirectLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular; };
//Source of light with 3 values for attenuation calculation
struct PointLight {
	vec3 position;

	float constK;
	float linK;
	float quadK;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular; };
//Point light but with direction and restriced field of lighting
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

vec4 CalcDirLight(DirectLight DL, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(PointLight PL, vec3 normal, vec3 viewDir, vec3 fragPos);
vec4 CalcSpotLight(SpotLight SL, vec3 normal, vec3 viewDir, vec3 fragPos);

vec4 GetAmbient(vec3 ambL, vec3 ambMat);
vec4 GetDiffuse(vec3 diffL, vec3 diffMat, vec3 normal, vec3 directionL);
vec4 GetSpecular(vec3 specL, vec3 specMat, vec3 normal, vec3 directionL, vec3 viewerDir);

uniform vec3 viewerPos;

uniform Material material;

uniform DirectLight directLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

void main()
{
	vec4 resultRGBA = vec4(0.0f);
	//vec3 viewDirection = normalize(viewerPos - vs_pos);
	//resultRGBA += CalcDirLight(directLight, normalize(vs_normal), viewDirection);
	//resultRGBA += CalcPointLight(pointLight, normalize(vs_normal), viewDirection, vs_pos);
	//resultRGBA += CalcSpotLight(spotLight, normalize(vs_normal), viewDirection, vs_pos);
	
	resultRGBA += texture(material.texture_diffuse, vs_uv).rgba;

	F_color = resultRGBA;
}

vec4 CalcDirLight(DirectLight DL, vec3 normal, vec3 viewDir)
{
	vec4 ambient = vec4(0.2f);
	vec4 diffuse = vec4(0.5f);
	vec4 specular = vec4(1.0f);

	vec3 lightFragDir = normalize(DL.direction);

	if(material.isTextured)
	{
		ambient = GetAmbient(DL.ambient, texture(material.texture_diffuse, vs_uv).rgb);
		diffuse = GetDiffuse(DL.diffuse, texture(material.texture_diffuse, vs_uv).rgb, normal, lightFragDir);
		specular = GetSpecular(DL.specular, texture(material.texture_specular, vs_uv).rgb, normal, lightFragDir, viewDir);
	}
	else
	{
		ambient = GetAmbient(DL.ambient, material.ambient);
		diffuse = GetDiffuse(DL.diffuse, material.diffuse, normal, lightFragDir);
		specular = GetSpecular(DL.specular, material.specular, normal, lightFragDir, viewDir);
	}

	return (ambient + diffuse + specular);
}

vec4 CalcPointLight(PointLight PL, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	vec4 ambient = vec4(0.2f);
	vec4 diffuse = vec4(0.5f);
	vec4 specular = vec4(1.0f);

	vec3 lightFragDir = normalize(PL.position - fragPos);

	if(material.isTextured)
	{
		ambient = GetAmbient(PL.ambient, texture(material.texture_diffuse, vs_uv).rgb);
		diffuse = GetDiffuse(PL.diffuse, texture(material.texture_diffuse, vs_uv).rgb, normal, lightFragDir);
		specular = GetSpecular(PL.specular, texture(material.texture_specular, vs_uv).rgb, normal, lightFragDir, viewDir);
	}
	else
	{
		ambient = GetAmbient(PL.ambient, material.ambient);
		diffuse = GetDiffuse(PL.diffuse, material.diffuse, normal, lightFragDir);
		specular = GetSpecular(PL.specular, material.specular, normal, lightFragDir, viewDir);
	}

	float distLF = length(fragPos - PL.position);
	//Simple quadratic equotion where X = distance between light and fragment
	float attenuation = 1.0f/(PL.constK + PL.linK * distLF + PL.quadK * distLF * distLF);

	return (ambient + diffuse + specular) * attenuation;
}

vec4 CalcSpotLight(SpotLight SL, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	vec4 ambient = vec4(0.2f);
	vec4 diffuse = vec4(0.5f);
	vec4 specular = vec4(1.0f);

	vec3 lightFragDir = normalize(SL.position - fragPos);

	if(material.isTextured)
	{
		ambient = GetAmbient(SL.ambient, texture(material.texture_diffuse, vs_uv).rgb);
		diffuse = GetDiffuse(SL.diffuse, texture(material.texture_diffuse, vs_uv).rgb, normal, lightFragDir);
		specular = GetSpecular(SL.specular, texture(material.texture_specular, vs_uv).rgb, normal, lightFragDir, viewDir);
	}
	else
	{
		ambient = GetAmbient(SL.ambient, material.ambient);
		diffuse = GetDiffuse(SL.diffuse, material.diffuse, normal, lightFragDir);
		specular = GetSpecular(SL.specular, material.specular, normal, lightFragDir, viewDir);
	}
	//The more distant fragment is - the bigger attenuation of light
	float distLF = length(fragPos - SL.position);
	//Simple quadratic equotion where X = distance between light and fragment
	float attenuation = 1.0f/(SL.constK + SL.linK * distLF + SL.quadK * distLF * distLF);
	
	//Vectors of light view and fragment in its field of view
	float fragInCutOff = dot(lightFragDir, normalize(-SL.direction));
	//The lesser angle - the greater cosin. Substract greatest from smallest
	float cutOffDiff = SL.cutOffIn - SL.cutOffOut;
	//Intensity inside of cutOffAngle is somewhere between 1.0 and 0.0
	float intensity = clamp((fragInCutOff - SL.cutOffOut/cutOffDiff), 0.0f, 1.0f);

	return (ambient + diffuse + specular) * attenuation * intensity;
}

vec4 GetAmbient(vec3 ambL, vec3 ambMat)
{//The stronger light - the stronger fragment color
	vec4 ambient = vec4(0.2f);
	ambient = vec4(ambL * ambMat, 1.0f);
	return ambient;
}

vec4 GetDiffuse(vec3 diffL, vec3 diffMat, vec3 normal, vec3 directionL)
{//The greater cosin between fragment and light ray, the stronger fragment color
	vec4 diffuse = vec4(0.5f);
	float diffSc = max(dot(normalize(directionL), normal), 0.0f);

	diffuse = diffSc * vec4(diffL * diffMat, 1.0f);

	return diffuse;
}

vec4 GetSpecular(vec3 specL, vec3 specMat, vec3 normal, vec3 directionL, vec3 viewerDir)
{//The lesser cosin between a viewer and a reflect ray - the stronger color;
	vec4 specular = vec4(1.0f);
	vec3 reflectRay = reflect(normalize(-directionL), normal);
	float specSc = pow(max(dot(viewerDir, reflectRay), 0.0f), material.shininness);

	specular = specSc * vec4(specL * specMat, 1.0f);

	return specular;
}