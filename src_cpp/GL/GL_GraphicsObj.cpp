#include <GL/GL_GraphicsObj.h>

// Graphics Object
namespace GL
{
	//Constructors&Destructors
	GraphicsObj::GraphicsObj(GraphicTypes graphicType) :
		m_graphType(graphicType), m_materialPtr(nullptr)
	{
		switch (m_graphType)
		{
		case (_2D):
			printf("GRAPHICS_OBJ::CONSTRUCTOR: Sprite has been created.\n");
			m_materialPtr = SP::MaterialManager::get().getInstance(SP::DEFAULT_2D);
			break;
		case (_3D):
			m_materialPtr = SP::MaterialManager::get().getInstance(SP::DEFAULT_3D);
			printf("GRAPHICS_OBJ::CONSTRUCTOR: Mesh has been created.\n");
			break;
		default:
			printf("GRAPHICS_OBJ::CONSTRUCTOR: Default graphics object has been created.\n");
			break;
		}
	}

	GraphicsObj::~GraphicsObj()
	{
		switch (m_graphType)
		{
		case (_2D):
			printf("GRAPHICS_OBJ::DESTRUCTOR: Sprite has been destroyed.\n");
			break;
		case (_3D):
			printf("GRAPHICS_OBJ::DESTRUCTOR: Mesh has been destroyed.\n");
			break;
		default:
			printf("GRAPHICS_OBJ::DESTRUCTOR: Default graphics object has been destroyed.\n");
			break;
		}
	}

	void GraphicsObj::updateModelMatrix()
	{
	}

	void Mesh::updateUniforms()
	{
	}
}
// Mesh
namespace GL
{
	//Constructors&Destructors
	Mesh::Mesh() :
		GraphicsObj(_3D), m_verticesNr(0), m_indicesNr(0)
	{
	}

	Mesh::Mesh(Vertex* vertices, unsigned int verticesNr,
		unsigned int* indices, unsigned int indicesNr, Material* material) :
		GraphicsObj(_3D), m_verticesNr(verticesNr), m_indicesNr(indicesNr)
	{
	}

	Mesh::Mesh(glm::vec3* positions, unsigned int verticesNr,
		unsigned int* indices, unsigned int indicesNr,
		glm::vec3* normals, glm::vec2* uvs, Material* material) :
		GraphicsObj(_3D), m_verticesNr(verticesNr), m_indicesNr(indicesNr)
	{
	}

	Mesh::~Mesh()
	{
	}

	// Functions
	void Mesh::updateVAO(glm::vec3* positions, unsigned int verticesNr, unsigned int* indices,
		unsigned int indicesNr, glm::vec3* normals, glm::vec2* uvs)
	{
	}

	void Mesh::updateVAO(Vertex* vertices, unsigned int verticesNr, unsigned int* indices, unsigned int indicesNr)
	{
	}

	void Mesh::draw(GLenum drawType)
	{
	}
}
// Sprite
namespace GL
{
	//Constructors&Destructors
	Sprite::Sprite() :
		GraphicsObj(_2D)
	{
	}

	Sprite::Sprite(float x, float y, float w, float h,
		Material* materialPtr) :
		GraphicsObj(_2D)
	{
	}

	Sprite::~Sprite()
	{
	}

	// Functions
	void Sprite::initVAO()
	{
	}

	void Sprite::updateVAO(float w, float h)
	{
	}

	void Sprite::updateUniforms()
	{
	}

	void Sprite::draw(GLenum drawType)
	{
	}
}