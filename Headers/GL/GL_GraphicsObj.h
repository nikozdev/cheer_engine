#ifndef GL_GRAPHICS_OBJECT_H
#define GL_GRAPHICS_OBJECT_H

#include <GL_header.hpp>
#include <GL/GL_types.h>
#include <math/glm/mat4x4.hpp>

// Graphics Object
namespace GL
{
	/// GraphicsComponent class:
	/// --Abstraction for rendered objects such as sprites and meshes
	/// --Contains essential data and methods for 2D and 3D drawing
	/// --Buffer and Array objects, model matrix with 3 transform vectors,
	/// material, camera pointer for projView matrix
	class GraphicsObj
	{
	protected:
		GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;
		glm::mat4 m_modelMatrix = glm::mat4(1.0f);
		GL::Material* m_materialPtr = nullptr;
		GL_Window* m_wndPtr = nullptr;
		virtual	void updateUniforms() {}
	public:
		GraphicTypes m_graphType = _DEFAULT;

		glm::vec3 m_scale = glm::vec3(1.0f),
			m_position = glm::vec3(0.0f),
			m_rotation = glm::vec3(0.0f);

		//Constructors&Destructors
		GraphicsObj(GraphicTypes graphicType);
		virtual ~GraphicsObj();

		glm::vec3 getScale() const { return m_scale; }
		glm::vec3 getRotation() const { return m_rotation; }
		glm::vec3 getPosition() const { return m_position; }

		Material* getMaterialPtr() const { return m_materialPtr; }
		GL_Window* getWndPtr() const { return m_wndPtr; }

		void setScale(const glm::vec3& scale) { m_scale = scale; }
		void setRotation(const glm::vec3& rotation) { m_rotation = rotation; }
		void setPosition(const glm::vec3& position) { m_position = position; }

		void setMaterialPtr(Material* materialPtr) { m_materialPtr = materialPtr; }
		void setWndPtr(GL_Window* wndPtr) { m_wndPtr = wndPtr; }

		void updateModelMatrix();
		virtual void draw(GLenum drawType = GL_TRIANGLES) {}
	};
}
// Mesh
namespace GL
{
	/// Mesh class for 3D objects:
	/// --Derrived from Graphics Component
	/// --Modified by vertices and indices number and has directory field
	/// --Update VAO creates new data objects for GPU
	/// --"updateUniforms" sets all relevant values into material-shader
	class Mesh : public GraphicsObj
	{
	private:
		unsigned int m_verticesNr = 0, m_indicesNr = 0;
	protected:
		void updateUniforms() override;
	public:
		std::string m_directory = "";

		//Constructors&Destructors
		Mesh();
		Mesh(Vertex* vertices, unsigned int verticesNr,
			unsigned int* indices = nullptr, unsigned int indicesNr = 0,
			GL::Material* material = nullptr);
		Mesh(glm::vec3* positions, unsigned int verticesNr,
			unsigned int* indices = nullptr, unsigned int indicesNr = 0,
			glm::vec3* normals = nullptr,
			glm::vec2* uvs = nullptr,
			GL::Material* material = nullptr);
		~Mesh();
		/// Initialize data by 3 arrays of 3 vectors
		void updateVAO(glm::vec3* positions, unsigned int verticesNr,
			unsigned int* indices = nullptr, unsigned int indicesNr = 0,
			glm::vec3* normals = nullptr,
			glm::vec2* uvs = nullptr);
		/// Initilize data by vertices array
		void updateVAO(Vertex* vertices, unsigned int verticesNr,
			unsigned int* indices = nullptr, unsigned int indicesNr = 0);

		void draw(GLenum drawType = GL_TRIANGLES) override;
	};
}
// Sprite
namespace GL
{
	/// Class Sprite for 2D rendering. Derrived from Graphics component
	/// --Has own vertex and index data, position in x and y, height and width
	/// --Has 4angled shape that can be changed in "updateVAO" function
	/// --Changes its sizes for given width and heihgt and for loaded sprite
	class Sprite : public GraphicsObj
	{
	private:
		float rectVertices[4][3][2] = {
			{	{-0.5f,	-0.5f},	{0.0f, 1.0f},	{0.0f, 0.0f},	},
			{	{0.5f,	-0.5f},	{0.0f, 1.0f},	{1.0f, 0.0f},	},
			{	{-0.5f,	0.5f},	{0.0f, 1.0f},	{0.0f, 1.0f},	},
			{	{0.5f,	0.5f},	{0.0f, 1.0f},	{1.0f, 1.0f},	}
		};
		GLuint rectIndices[6] = {
			0, 1, 2,
			2, 1, 3
		};
		float m_x = 0, m_y = 0, m_width = 1.0f, m_height = 1.0f;

		void initVAO();
	protected:
		//
	public:
		Sprite();
		Sprite(float x, float y, float w = 1.0f, float h = 1.0f,
			Material* materialPtr = nullptr);
		~Sprite();

		void updateVAO(float w, float h);

		virtual	void updateUniforms() override;

		virtual void draw(GLenum drawType = GL_TRIANGLES) override;
	};
}

#endif