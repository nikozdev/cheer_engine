#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include <string>
#include <GL_header.hpp>

#define CH_ASSERT(x) if (!(x)) /*__debugbreak()*/\

#define GL_LOG() GL::GL_log(__FILE__, __LINE__);	\
__debugbreak();	\

#define GLCall(x) GL::GL_clearError();\
	x;\
	CH_ASSERT(GL::GL_logCall(#x, __FILE__, __LINE__));\

// Predefenitions
namespace GL
{
	class VertexArr;
	class VertexBuf;
	class IndexBuf;
	class Shader;
}

namespace GL
{
	/// This is entire abstraction of render in OpenGL
	/// VertexArray va; VertexBuffer vb(vertexData, count * size);
	/// BufferLayout layout.push<dataType>(sizeOfVert);
	/// va.AddBuffer(vb, layout); IndexBuffer ib(indicesData, count);
	/// Renderer renderer.draw(va, ib, shader, GL_TRIANGLES/GL_LINES);
	class Renderer
	{
	private:
		VertexArr* m_va = nullptr;
		IndexBuf* m_ib = nullptr;

		Shader* m_shader = nullptr;

		GLuint m_vao = 0, m_vbo = 0, m_ibo = 0;
	public:
		float m_lightColor[3];
		float m_vert_rect2d_4[16] = {
				-0.5f,	-0.5f,		0.0f, 0.0f,
				0.5f,	-0.5f,		1.0f, 0.0f,
				0.5f,	0.5f,		1.0f, 1.0f,
				-0.5f,	0.5f,		0.0f, 1.0f
		};
		unsigned int m_rect_indices[6]{
			0, 1, 2,
			2, 3, 0
		};
		
		Renderer();
		Renderer(GL::VertexArr* va,
			GL::IndexBuf* ib = nullptr, GL::Shader* shader = nullptr);
		~Renderer();

		// Modificators
		void setVertexArray(VertexArr* va) { m_va = va; }
		void setIndexBuffer(IndexBuf* ib) { m_ib = ib; }
		void setShader(Shader* shader) { m_shader = shader; }
		Shader* getShader() const { return m_shader; }
		
		// Vertex functions
		void InitVertData_def(float* vertData, unsigned int vertAmount,
			unsigned int* indData, unsigned int indAmount);

		// Drawing functions
		void draw(VertexArr& va, IndexBuf& ib, const Shader* shader,
			GLenum drawType = GL_TRIANGLES) const;
		void draw(GLenum drawType = GL_TRIANGLES) const;
		void drawVAO() const;

		static void clear(float color[3]);
	};

	void GL_clearError();
	bool GL_logCall(const char* funcName, const char* file, int line);
}

#endif