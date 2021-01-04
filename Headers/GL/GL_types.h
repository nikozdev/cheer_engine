#ifndef GL_CLASSES_H
#define GL_CLASSES_H

#include <vector>
#include <math/glm/vec2.hpp>
#include <math/glm/vec3.hpp>

#include <GL/GL_structs.h>
#include <GL/GL_Shader.h>

#include <support/ch_managers.h>

// PreDeclarations for usage in declaration
namespace GL
{
	class VertexArr;
	class VertexBuf;
	class BufferLayout;
	class IndexBuf;

	class GL_Window;
	class Shader;
	
	class GraphicsCmp;
	class Mesh;
	class Sprite;
}
// Enums
namespace GL
{
	enum GraphicTypes {
		_DEFAULT = 0, _2D = 1, _3D = 2
	};
}

// OpenGL abstractions
namespace GL
{
	/// Abstraction for Vertex Buffer Object of OpenGL
	/// Initialize with vertex data and it's size - bind in VAO layout
	class VertexBuf
	{
	private:
		GLuint m_id;
		GLsizei m_size = 0;
	public:
		VertexBuf();
		VertexBuf(const float* vData, size_t vDataSize);
		~VertexBuf();

		GLsizei getSize() const { return m_size; }

		void loadData(const float* vData, size_t vDataSize, VertexArr* vertArray = nullptr);

		void bind() const;
		void unbind() const;
	};
	/// Abstraction for Element Buffer Object of OpenGL
	/// Initialize with indicies and it's count
	class IndexBuf
	{
	private:
		GLuint m_id;
		unsigned int m_count = 0;
		bool isBound = false;
	public:
		IndexBuf();
		IndexBuf(unsigned int* indData, unsigned int indAmount);
		~IndexBuf();

		unsigned int getCount() const { return m_count; }

		void loadData(unsigned int* indData, unsigned int indAmount, VertexArr* vertArray = nullptr);
		void bind();
		void unbind();
	};
	/// Every new vertex data has to be in certain layout
	/// It counts offsets and strides for every single vertex you load
	/// Use for glVertexAttribPointer()
	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_elements;
		unsigned int m_stride = 0;
		mutable bool isBound = false;

		void addElement_vec3(const BufferElement& element, unsigned int count);
	public:
		BufferLayout();
		~BufferLayout();

		unsigned int getStride() const { return m_stride; }
		std::vector<BufferElement> getElements() { return m_elements; }
		
		void addElement(const BufferElement& element, unsigned int count);
	};
	/// Abstraction for Vertex Array Object of OpenGL
	/// Initialize it: add VertexBuf and Layout, bind and draw
	/// VertexArr va; VertexBuf(data, size); va.addBuffer(vb);
	/// BufferLayout bufLayout.push<float>(3);
	/// va.bind(); - use before drawing
	class VertexArr
	{
	private:
		GLuint m_id = 0;
		mutable bool isBound = false;
	public:
		VertexArr(const VertexBuf& vb, const BufferLayout& layout);
		VertexArr();
		~VertexArr();

		void addBuffer(const VertexBuf& vb, const BufferLayout& layout);

		void bind() const;
		void unbind() const;
	};
}
// Texture, Material and Graphics Components
namespace GL
{
	class Texture
	{
	private:
		GLuint m_id;
		GLuint m_texUnit;
	public:
		int width = 0, height = 0, channelsNr = 0;

		std::string type;
		std::string path;

		//Constructors&Destructors
		Texture();
		Texture(const char* type,
			const std::string& path = "", GLint texUnit = 0);
		~Texture();

		GLuint getId() { return m_id; }
		GLuint getUnit() const { return m_texUnit; }
		//Functions
		static void bind(Texture* tex);
		static void unbind();
	};
	/// Material class:
	/// --Defines colors and shades of an object
	/// --Has shader ptr (at least default) for drawing
	/// --Handles 3 texture types: diffuse, specular, normal
	/// --IsTextured if there is at least diffuse.
	///	-->If is textured - set textures in shader, else - set default RGBA
	class Material
	{
	private:
		std::string name = "material";

		Shader* shaderPtr = nullptr;

		Texture* diffuse = nullptr;
		Texture* specular = nullptr;
		Texture* normal = nullptr;

		bool isTextured = false;
		bool isBound = false;

		float m_shininness = 32.0f;
	public:
		glm::vec3 m_ambientVec = glm::vec3(0.25f);
		glm::vec3 m_diffuseVec = glm::vec3(0.6f);
		glm::vec3 m_specularVec = glm::vec3(1.0f);
		//Constructors&Destructors
		Material();
		Material(const char* name, Texture* diffuse, Texture* specular = nullptr,
			Texture* normal = nullptr,
			float shininness = 32.0f);
		Material(const char* name, const char* diffusePath, const char* specularPath = nullptr,
			const char* normalPath = nullptr,
			float shininness = 32.0f);
		Material(const char* name,
			glm::vec3 ambientVec = glm::vec3(0.15), glm::vec3 diffuseVec = glm::vec3(0.5f),
			glm::vec3 specularVec = glm::vec3(1.0f), float shininness = 32.0f);
		~Material();
		//Accessors
		std::string getName() const { return name; }
		Shader* getShaderPtr() const { return shaderPtr; }
		void setShaderPtr(Shader* shaderPtr) { this->shaderPtr = shaderPtr; }
		//Functions
		void setInShader();
		void bind();
		void unbind();
	};
}
#endif