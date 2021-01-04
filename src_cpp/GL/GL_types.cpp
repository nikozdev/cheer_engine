#include <GL/GL_Renderer.h>
#include <GL/GL_types.h>
#include <GL/GL_Window.h>
#include <Support/ch_LoadMaster.h>

// OpenGL aqbstractions
namespace GL
{
	/// VertexBuffer
	VertexBuf::VertexBuf()
	{
		glGenBuffers(1, &m_id);
	}
	VertexBuf::VertexBuf(const float* vData, size_t vDataSize)
	{
		glGenBuffers(1, &m_id);
		loadData(vData, vDataSize);
	}
	VertexBuf::~VertexBuf()
	{
		GLCall(glDeleteBuffers(1, &m_id));
	}

	void VertexBuf::loadData(const float* vData, size_t vDataSize, VertexArr* vertArray)
	{
		m_size = vDataSize;
		bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER,
			vDataSize, vData, GL_STATIC_DRAW));
		unbind();
	}

	void VertexBuf::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}
	void VertexBuf::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
namespace GL
{
	/// IndexBuffer
	IndexBuf::IndexBuf()
	{
		glGenBuffers(1, &m_id);
	}
	IndexBuf::IndexBuf(unsigned int* indData, unsigned int indAmount)
	{
		glGenBuffers(1, &m_id);
		loadData(indData, indAmount);
	}
	IndexBuf::~IndexBuf()
	{
		glDeleteBuffers(1, &m_id);
	}

	void IndexBuf::loadData(unsigned int* indData, unsigned int indAmount, VertexArr* vertArray)
	{
		if (vertArray)
			vertArray->bind();
		m_count = indAmount;
		bind();
		GLCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indAmount * sizeof(unsigned int),
			indData, GL_STATIC_DRAW) );
		if (vertArray)
		{
			vertArray->unbind();
			unbind();
		}
	}

	void IndexBuf::bind()
	{
		if (isBound == true) return;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		isBound = true;
	}
	void IndexBuf::unbind()
	{
		if (isBound == false) return;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		isBound = false;
	}
}
namespace GL
{
	/// BufferLayout
	BufferLayout::BufferLayout()
	{
		//
	}
	BufferLayout::~BufferLayout()
	{
		//
	}

	void BufferLayout::addElement(const BufferElement& element, unsigned int count)
	{
		switch (element.type)
		{
		case GL_FLOAT: case GL_UNSIGNED_INT:
			m_elements.push_back(element);
			m_stride += count * BufferElement::getSizeOfType(element.type);
			break;
		default: return;
		}
	}

	void BufferLayout::addElement_vec3(const BufferElement& element, unsigned int count)
	{
		m_elements.push_back(element);
		m_stride += count * BufferElement::getSizeOfType(element.type) * 3;
	}
}
namespace GL
{
	/// VertexArray
	VertexArr::VertexArr()
	{
		glGenVertexArrays(1, &m_id);
	}
	VertexArr::VertexArr(const VertexBuf& vb, const BufferLayout& layout)
	{
		glGenVertexArrays(1, &m_id);
		addBuffer(vb, layout);
	}
	VertexArr::~VertexArr()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArr::addBuffer(const VertexBuf& vb, const BufferLayout& layout)
	{
		bind();
		vb.bind();
		vb.unbind();
		unbind();
	}

	void VertexArr::bind() const
	{
		if (isBound) return;
		glBindVertexArray(m_id);
		isBound = true;
	}
	void VertexArr::unbind() const
	{
		if (!isBound) return;
		glBindVertexArray(0);
		isBound = false;
	}
}

// Material
namespace GL
{
	///Material
	GL::Material::Material()
	{
		if (SP::MaterialManager::isInit)
			SP::MaterialManager::get().addInstance(this);

		printf("GL_STRUCTS::MATERIAL::CONSTRUCTOR: Material has been created without arguments.\n");
	}

	GL::Material::Material(const char* name,
		Texture* diffuse, Texture* specular,
		Texture* normal, float shininness) :
		name(name), m_shininness(shininness),
		diffuse(diffuse), specular(specular), normal(normal)
	{//Create with prepared texture pointers
		if (SP::MaterialManager::isInit)
			SP::MaterialManager::get().addInstance(this);

		printf("GL::MATERIAL::CONSTRUCTOR: Material %s is created with ptrs.\n", name);
	}

	GL::Material::Material(const char* name,
		const char* diffusePath, const char* specularPath,
		const char* normalPath, float shininness) :
		name(name), m_shininness(shininness)
	{//Create with paths: load textures and allocate memory for them
		if (SP::MaterialManager::isInit)
			SP::MaterialManager::get().addInstance(this);

		this->diffuse = new Texture("texture_diffuse", diffusePath, 0);
		if (specularPath != nullptr && specularPath != "")
			this->specular = new Texture("texture_specular", specularPath, 1);
		if (normalPath != nullptr && normalPath != "")
			this->normal = new Texture("texture_normal", normalPath, 2);
		printf("GL::MATERIAL::CONSTRUCTOR: Material %s has been created with loaded textures.\n", name);
	}

	GL::Material::Material(const char* name,
		glm::vec3 ambientVec, glm::vec3 diffuseVec,
		glm::vec3 specularVec, float shininness) :
		name(name), m_shininness(shininness),
		m_ambientVec(ambientVec), m_diffuseVec(diffuseVec), m_specularVec(specularVec)
	{//Create material with RGB configuration
		if (SP::MaterialManager::isInit)
			SP::MaterialManager::get().addInstance(this);

		printf("GL::MATERIAL::CONSTRUCTOR: Material %s is created with vectors.\n", name);
	}

	GL::Material::~Material()
	{
		if (diffuse) delete diffuse;
		if (specular) delete specular;
		if (normal) delete normal;
		{
			diffuse = nullptr;
			specular = nullptr;
			normal = nullptr;
		}
		printf("MATERIAL::DESTRUCTOR: Material %s is destroyed.\n", name.c_str());
	}

	void GL::Material::setInShader()
	{//"material_name.map_type = textureUnit"
		if (shaderPtr == NULL) return;
		shaderPtr->use();
		
		isTextured = (diffuse != nullptr);
		GLCall( shaderPtr->setBool(name + ".isTextured", isTextured))
		
		//Set all textures
		if (isTextured)
		{
			GLCall( shaderPtr->setInt(name + "." + diffuse->type, diffuse->getUnit()))
			if (specular)
				GLCall( shaderPtr->setInt(name + "." + specular->type, specular->getUnit()))
			if (normal)
				GLCall( shaderPtr->setInt(name + "." + normal->type, normal->getUnit()))
		}
		else
		{//material {vec3 ambient = vec3(ambientVec)}
			GLCall(shaderPtr->setVec3(name + ".ambient", m_ambientVec))
			GLCall(shaderPtr->setVec3(name + ".diffuse", m_diffuseVec))
			GLCall(shaderPtr->setVec3(name + ".specular", m_specularVec))
		}
		GLCall(shaderPtr->setFloat(name + ".shininness", m_shininness));
		GLCall(LightSource::setInShader(shaderPtr, *shaderPtr->getDirectLight()))
	}

	void GL::Material::bind()
	{
		if (isBound) return;
		isBound = true;
		//Bind all textures if they are
		if (diffuse)
			Texture::bind(this->diffuse);
		if (specular != nullptr)
			Texture::bind(this->specular);
		if (normal != nullptr)
			Texture::bind(this->normal);
	}

	void GL::Material::unbind()
	{
		Texture::unbind();
		//Unset all textures
		this->isBound = false;
	}
}
// Texture
namespace GL
{
	/// Texture
	GL::Texture::Texture() :
		channelsNr(0), width(0), height(0),
		m_id(0), m_texUnit(0), type("")
	{
		glGenTextures(1, &m_id);
		if (SP::TextureManager::isInit)
			SP::TextureManager::get().addInstance(this);

		printf("GL_STRUCTS::TEXTURE::CONSTRUCTOR: Texture has been created without arguments.\n");
	}
	GL::Texture::Texture(const char* newType,
		const std::string& newPath, GLint newTexUnit) :
		type(newType), path(newPath), m_texUnit(newTexUnit),
		m_id(0), width(0), height(0), channelsNr(0)
	{
		glGenTextures(1, &m_id);
		if (SP::TextureManager::isInit)
			SP::TextureManager::get().addInstance(this);

		if (path != "")
		{
			if (SP::LoadMaster::loadTexture_def(path.c_str(), this))
				printf("GL::GL_STRUCTS::TEXTURE::CONSTRUCTOR: texture %s has been created.\n",
					path.c_str());
			else
				printf("GL::GL_STRUCTS::TEXTURE::CONSTRUCTOR: Failder to load texture %s.\n",
					path.c_str());
			return;
		}
		printf("GL::GL_STRUCTS::TEXTURE::CONSTRUCTOR: texture %s has been created.\n",
			type.c_str());
	}
	GL::Texture::~Texture()
	{
		if (m_id)
		{
			glDeleteTextures(1, &m_id);
		}
		if (path != "")
		{
			printf("GL_STRUCTS::TEXTURE::DESTRUCTOR: texture %s has been destroyed.\n",
				path.c_str());
			return;
		}
		printf("GL_STRUCTS::TEXTURE::DESTRUCTOR: texture %s has been destroyed.\n",
			type.c_str());
	}

	//Functions
	void GL::Texture::bind(Texture* tex)
	{
		glActiveTexture(GL_TEXTURE0 + tex->getUnit());
		glBindTexture(GL_TEXTURE_2D, tex->getId());
	}
	void GL::Texture::unbind()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}