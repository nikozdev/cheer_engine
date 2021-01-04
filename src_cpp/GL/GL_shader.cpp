#include <GL/GL_Shader.h>
#include <support/ch_managers.h>
#include <math/glm/vector_relational.hpp>
#include <math/glm/matrix.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

namespace GL
{
	Shader::Shader() :
		m_id(0), m_vertSh(0), m_fragSh(0), m_geomSh(0)
	{
		if (SP::ShaderManager::isInit)
			SP::ShaderManager::get().addInstance(this);
		glUseProgram(0);
		m_id = glCreateProgram();
		printf("GL::SHADER::CONSTRUCTOR: Shader is created without arguments\n");
	}

	Shader::Shader(const char* vertexPath,
		const char* fragmentPath, const char* geometryPath) :
		m_geomSh(0)
	{
		if (SP::ShaderManager::isInit)
			SP::ShaderManager::get().addInstance(this);
		glUseProgram(0);
		m_id = glCreateProgram();
		Shader::initShader(this, vertexPath, fragmentPath, geometryPath);

		printf("GL::SHADER::CONSTRUCTOR: Shader program is created\n");
	}

	Shader::~Shader()
	{
		del();
		printf("GL::SHADER::DESTRUCTOR: Shader program is deleted\n");
	}

	void Shader::use() const
	{
		if (m_isBeeingUsed) return;
		glUseProgram(m_id);
		this->m_isBeeingUsed = true;
	}
	void Shader::stopUse() const
	{
		if (m_isBeeingUsed == false) return;
		glUseProgram(0);
		this->m_isBeeingUsed = false;
	}
	void Shader::del()
	{
		stopUse();
		glDetachShader(m_id, m_vertSh);
		glDetachShader(m_id, m_fragSh);
		glDeleteShader(m_vertSh);
		glDeleteShader(m_fragSh);
		if (m_geomSh)
		{
			glDetachShader(m_id, m_geomSh);
			glDeleteShader(m_geomSh);
		}
		glDeleteProgram(m_id);
	}

	void Shader::initShader(Shader* shaderPtr, const char* vertexPath, const char* fragmentPath, const char* geometryPath)
	{
		shaderPtr->m_vertSh = Shader::loadShader(vertexPath, GL_VERTEX_SHADER);
		shaderPtr->m_fragSh = Shader::loadShader(fragmentPath, GL_FRAGMENT_SHADER);
		if (geometryPath != nullptr)
		{
			shaderPtr->m_geomSh = Shader::loadShader(geometryPath, GL_GEOMETRY_SHADER);
		}
		glAttachShader(shaderPtr->m_id, shaderPtr->m_vertSh);
		glAttachShader(shaderPtr->m_id, shaderPtr->m_fragSh);
		if (geometryPath != nullptr)
		{
			glAttachShader(shaderPtr->m_id, shaderPtr->m_geomSh);
		}
		glLinkProgram(shaderPtr->m_id);
		debugShader(shaderPtr->m_id, GL_PROGRAM);
	}

	//Uniforms
	void Shader::setBool(const std::string& name, int value) const
	{
		glUniform1i(getUniformLoc(name), value);
	}
	void Shader::setInt(const std::string& name, int value) const
	{
		glUniform1i(getUniformLoc(name), value);
	}
	void Shader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(getUniformLoc(name), value);
	}

	void Shader::setVec2(const std::string& name,
		glm::vec2& value) const
	{
		glUniform2fv(getUniformLoc(name), 1, &value[0]);
	}
	void Shader::setVec2(const std::string& name,
		float x, float y)  const
	{
		glUniform2f(getUniformLoc(name), x, y);
	}

	void Shader::setVec3(const std::string& name, glm::vec3& value) const
	{
		glUniform3fv(getUniformLoc(name), 1, &value[0]);
	}
	void Shader::setVec3(const std::string& name,
		float x, float y, float z) const
	{
		glUniform3f(getUniformLoc(name), x, y, z);
	}

	void Shader::setVec4(const std::string& name, glm::vec4& value) const
	{
		glUniform4fv(getUniformLoc(name), 1, &value[0]);
	}
	void Shader::setVec4(const std::string& name,
		float x, float y, float z, float w) const
	{
		glUniform4f(getUniformLoc(name), x, y, z, w);
	}

	void Shader::setMat2(const std::string& name, glm::mat2& value) const
	{
		glUniformMatrix2fv(getUniformLoc(name), 1, GL_FALSE, &value[0][0]);
	}
	void Shader::setMat3(const std::string& name, glm::mat3& value) const
	{
		glUniformMatrix3fv(getUniformLoc(name), 1, GL_FALSE, &value[0][0]);
	}
	void Shader::setMat4(const std::string& name, glm::mat4& value) const
	{
		glUniformMatrix4fv(getUniformLoc(name), 1, GL_FALSE, &value[0][0]);
	}
	GLint Shader::getUniformLoc(const std::string& name) const
	{
		if (m_uniformCashe.find(name) != m_uniformCashe.end())
			return m_uniformCashe[name];

		GLint location = glGetUniformLocation(m_id, name.c_str());
		m_uniformCashe[name] = location;
		return location;
	}

	//Shader Loading
	Shader::ShaderSource Shader::getShaderCode(const std::string& path)
	{
		std::ifstream codeReader(path);
		std::string line;
		std::stringstream strStreams[3];

		ShaderTypes type = ShaderTypes::NONE;
		while (getline(codeReader, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("#vertex") != std::string::npos)
					type = ShaderTypes::VERTEX;
				else if (line.find("#fragment") != std::string::npos)
					type = ShaderTypes::FRAGMENT;
				else if (line.find("#geometry") != std::string::npos)
					type = ShaderTypes::GEOMETRY;
			}
			else
			{//Build source
				strStreams[(int)type] << line << '\n';
			}
		}
		//Translate from streams of bytes to struct of strings
		return { strStreams[0].str(), strStreams[1].str(), strStreams[2].str() };
	}
	//Static functions
	GLuint Shader::loadShader(const char* shaderPath, GLenum shaderType)
	{
		GLuint newShader;
		std::string shaderCode;
		std::ifstream shFile;
		std::stringstream shStream;

		shFile.exceptions(std::ifstream::badbit || std::ifstream::failbit);
		try
		{
			shFile.open(shaderPath);
			shStream << shFile.rdbuf();
			shFile.close();
			shaderCode = shStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "SHADER::CONSTRUCTOR::LOAD_SHADER: Error with file reading" << std::endl;
		}
		const char* newShaderCode = shaderCode.c_str();
		try
		{
			newShader = glCreateShader(shaderType);
			glShaderSource(newShader, 1, &newShaderCode, NULL);
			glCompileShader(newShader);
			debugShader(newShader, shaderType);
		}
		catch (std::bad_alloc e)
		{
			std::cerr << "SHADER::CONSTRUCTOR\nShader program isn't created\n" << e.what() << std::endl;
			glDeleteShader(newShader);
		}
		return newShader;
	}

	void Shader::debugShader(unsigned int shader, GLenum type)
	{
		int success;
		char infoLog[1024];

		if (type != GL_PROGRAM)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << type <<
					" shader isn't compiled because..." << std::endl <<
					infoLog << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << type <<
					" isn't linked because..." << std::endl <<
					infoLog << std::endl;
			}
		}
	}
}