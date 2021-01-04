#include <GL/GL_Renderer.h>
#include <GL/GL_types.h>
#include <GL/GL_Shader.h>

namespace GL
{
	Renderer::Renderer()
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		InitVertData_def(m_vert_rect2d_4, sizeof(m_vert_rect2d_4) / sizeof(float),
			m_rect_indices, sizeof(m_rect_indices) / sizeof(float));

		printf("GL::RENDERER::CONSTRUCTOR: Renderer has been created without arguments.\n");
	}
	Renderer::Renderer(GL::VertexArr* va,
		GL::IndexBuf* ib, GL::Shader* shader) :
		m_va(va), m_ib(ib),
		m_shader(shader)
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		printf("GL::RENDERER::CONSTRUCTOR: Renderer has been created.\n");
	}

	Renderer::~Renderer()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ibo);

		printf("GL::RENDERER::DESTRUCTOR: Renderer has been destroyed.\n");
	}

	// Vertex data functions
	void Renderer::InitVertData_def(float* vertData, unsigned int vertAmount,
		unsigned int* indData, unsigned int indAmount)
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);
		glEnableVertexArrayAttrib(m_vao, 0);
		glEnableVertexArrayAttrib(m_vao, 1);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vert_rect2d_4),
			nullptr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
			4 * sizeof(float), (void*)(0 * sizeof(float)));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_rect_indices),
			nullptr, GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Render functions
	void Renderer::draw(VertexArr& va, IndexBuf& ib, const Shader* shader,
		GLenum drawType) const
	{//Draw with given objects
		shader->use();
		va.bind();
		ib.bind();
		GLCall(glDrawElements(drawType,
			ib.getCount(), GL_UNSIGNED_INT, nullptr));
		va.unbind();
		ib.unbind();
		shader->stopUse();
	}
	void Renderer::draw(GLenum drawType) const
	{//Draw with own objects
		if (!m_shader | !m_va | !m_ib) return;
			m_shader->use();
			m_va->bind();
			m_ib->bind();
			GLCall(glDrawElements(drawType,
				m_ib->getCount(), GL_UNSIGNED_INT, nullptr))
				m_va->unbind();
			m_ib->unbind();
			m_shader->stopUse();
	}
	void Renderer::drawVAO() const
	{
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)nullptr,
			sizeof(m_vert_rect2d_4), (const void*)m_vert_rect2d_4);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (GLintptr)nullptr,
			sizeof(m_rect_indices), (const void*)m_rect_indices);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Renderer::clear(float color[3])
	{
		GLCall(glClearColor(color[0], color[1], color[2], 1.0f))
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
	}
}

namespace GL
{
	void GL_clearError()
	{//glGetError gets last message and clears errorLog
		while (glGetError() != GL_NO_ERROR);
	}

	bool GL_logCall(const char* funcName,
		const char* file, int line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:		error = "INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:		error = "INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:	error = "INVALID_OPERATION";
				break;
			case GL_STACK_OVERFLOW:		error = "STACK_OVERFLOW";
				break;
			case GL_STACK_UNDERFLOW:	error = "STACK_UNDERFLOW";
				break;
			case GL_OUT_OF_MEMORY:		error = "OUT_OF_MEMORY";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				error = "INVALID_FRAMEBUFFER_OPERATION";
				break;
			}
			printf("GL::RENDERER::GL_LOG_CALL::%s::LINE_%d:\n%s\n",
				funcName, line, error.c_str());
			return false;
		}
		return true;
	}
}