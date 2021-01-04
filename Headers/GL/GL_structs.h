#ifndef GL_STRUCTS_H
#define GL_STRUCTS_H

#include <GL_header.hpp>
#include <support/ch_support.hpp>
#include <math/ch_math.hpp>

// Predeclarations
namespace GL
{
	struct Vertex;
	struct Vertex2;
	struct Vertex3;
	struct LightSource;
	struct DirectLight;
	struct PointLight;
	struct SpotLight;
	struct BufferElement;
}
// Lights
namespace GL
{
	struct LightSource
	{
		glm::vec3 ambient = glm::vec3(0.15f);
		glm::vec3 diffuse = glm::vec3(0.5f);
		glm::vec3 specular = glm::vec3(1.0f);

		static void setInShader(const Shader* shader, DirectLight& lSource);
		static void setInShader(const Shader* shader, PointLight& lSource);
		static void setInShader(const Shader* shader, SpotLight& lSource);
	};
	/// DirectLight structure - is a reflection of the struct "DirectLight" in a "phong" shader
	/// --Ambient, Diffuse, Specular rgb arrays
	/// --Direction xyz array
	struct DirectLight : public LightSource
	{
		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
	};
	/// PointLight structure - is a reflection of the struct "PointLight" in a "phong" shader
	/// --Ambient, Diffuse, Specular rgb arrays
	/// --Position xyz array
	/// --3 attenuation multipliers
	struct PointLight : public LightSource
	{
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		float att_const = 1.0f;
		float att_linear = 0.5f;
		float att_quad = 0.35f;
	};
	/// SpotLight structure - is a reflection of the struct "SpotLight" in a "phong" shader
	/// --Ambient, Diffuse, Specular rgb arrays
	/// --Direction and position xyz arrays
	/// --3 attenuation multipliers
	/// --CutOffOut and CutOffIn angles
	struct SpotLight : public LightSource
	{
		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		float att_const = 1.0f;
		float att_linear = 0.5f;
		float att_quad = 0.35f;
		float cutoffIn = 10.0f;
		float cutoffOut = 15.0f;
	};
	
	struct BufferElement
	{
		GLenum type;
		size_t size;
		unsigned char normal;

		static size_t getSizeOfType(GLenum type)
		{
			switch (type)
			{
			case GL_FLOAT: return 4;
				break;
			case GL_UNSIGNED_INT: return 4;
				break;
			case GL_UNSIGNED_BYTE: return 1;
				break;
			default: return 0;
			}
		}
	};
}
// My basic data structures: vectors and vertices
namespace GL
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uvs;
	};
	struct Vertex2
	{
		float position[2];
		float normal[2];
		float uvs[2];
	};
	struct Vertex3
	{
		float position[3];
		float normal[3];
		float uvs[3];
	};
}
#endif