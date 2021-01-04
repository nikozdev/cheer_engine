#ifndef SHADER_H
#define SHADER_H

#include <GL_header.hpp>
#include  <GL/GL_structs.h>

#include <math/glm/vec2.hpp>
#include <math/glm/vec3.hpp>
#include <math/glm/vec4.hpp>
#include <math/glm/mat4x4.hpp>

#include <string>
#include <unordered_map>

// Predeclarations
namespace GL
{
	struct LightSource;
	struct DirectLight;
	struct PointLight;
	struct SpotLight;
}

// Shader
namespace GL
{
	/// Shader class - handler for shader program
	/// --Has m_id, 3 shaders: vertex, fragment, [geometry]
	/// --Can set uniforms via bunch of comands
	class Shader
	{
	private:
		enum class ShaderTypes {
			NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
		};
		struct ShaderSource
		{
			std::string vertexSource;
			std::string fragmentSource;
			std::string geometrySource;
		};

		GLuint m_id;
		GLuint m_fragSh, m_vertSh, m_geomSh;
		mutable std::unordered_map<std::string, GLint> m_uniformCashe;

		mutable bool m_isBeeingUsed = false;

		DirectLight m_dirLight;

		GLint getUniformLoc(const std::string &name) const;

		static ShaderSource getShaderCode(const std::string& path);
		static GLuint loadShader(const char* shaderPath, GLenum shaderType);
		static void debugShader(unsigned int shader, GLenum shaderType);
	public:
		Shader();
		Shader(const char* vertexPath, const char* fragmentPath,
			const char* geometryPath = nullptr);
		~Shader();

		GLuint getID() const { return m_id; }
		DirectLight* getDirectLight() { return &m_dirLight; }

		void setDirectLight(const DirectLight& directionalLight) { m_dirLight = directionalLight; }

		void use() const;
		void stopUse() const;
		void del(); 
		
		static void initShader(Shader* shaderPtr, const char* vertexPath, const char* fragmentPath,
			const char* geometryPath = nullptr);
		//Setting uniforms into a shader
		void setBool(const std::string& name, int value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;

		void setVec2(const std::string& name, glm::vec2& value) const;
		void setVec2(const std::string& name,
			float x, float y) const;

		void setVec3(const std::string& name, glm::vec3& value) const;
		void setVec3(const std::string& name,
			float x, float y, float z) const;

		void setVec4(const std::string& name, glm::vec4& value) const;
		void setVec4(const std::string& name,
			float x, float y, float z, float w) const;

		void setMat2(const std::string& name, glm::mat2& value) const;
		void setMat3(const std::string& name, glm::mat3& value) const;
		void setMat4(const std::string& name, glm::mat4& value) const;
	};
}
#endif