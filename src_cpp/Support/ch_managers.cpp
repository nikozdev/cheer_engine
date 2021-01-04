#include <support\ch_managers.h>

#include <ESC\Entity.h>
#include <GL\GL_Shader.h>
#include <GL\GL_types.h>

extern std::string dir;

//Predeclarations
namespace SP
{
	bool EntityManager::isInit = false;
	bool ShaderManager::isInit = false;
	bool TextureManager::isInit = false;
	bool MaterialManager::isInit = false;
}
/// EntityManager
namespace SP
{
	EntityManager::EntityManager() : CH_Manager()
	{
		initManager();
		isInit = true;
	}
	bool EntityManager::initManager()
	{
		return true;
	}
	bool EntityManager::hasEntity(const std::string& entName) const
	{
		for (auto ent : m_instList)
		{
			if (ent->getName() == entName)
				return true;
		}
		return false;
	}
}
/// ShaderManager
namespace SP
{
	ShaderManager::ShaderManager() : CH_Manager()
	{
		initManager();
		isInit = true;
	}

	bool ShaderManager::initManager()
	{
		addInstance(new GL::Shader((dir + "shaders/core_2d_vs.lua").c_str(), (dir + "shaders/core_2d_fs.lua").c_str()) );
		addInstance(new GL::Shader((dir + "shaders/obj0_3d_vs.lua").c_str(), (dir + "shaders/obj0_3d_phong_fs.lua").c_str()) );

		return true;
	}
	
	bool ShaderManager::hasShader(const int& id)
	{
		for (int i = 0; i < ShaderManager::get().getNr(); i++)
		{
			if (ShaderManager::get().getInstances()->at(i)->getID() == id)
				return true;
		}
		return false;
	}
}
/// MaterialManager
namespace SP
{
	MaterialManager::MaterialManager() : CH_Manager()
	{
		initManager();
		isInit = true;
	}

	bool MaterialManager::initManager()
	{
		addInstance(new GL::Material("material_2d"));
		//TextureManager::get().getInstances()->at(TextureTypes::DIFFUSE)));
		m_instList.at(DEFAULT_2D)->setShaderPtr(
			SP::ShaderManager::get().getInstances()->at(ShaderPrograms::CORE_2D));

		addInstance(new GL::Material("material_3d"));
			//TextureManager::get().getInstances()->at(TextureTypes::DIFFUSE)));
		m_instList.at(DEFAULT_3D)->setShaderPtr(
			SP::ShaderManager::get().getInstances()->at(ShaderPrograms::CORE_3D));
		
		return true;
	}

	bool MaterialManager::hasMaterial(const std::string& name)
	{
		for (int i = 0; i < MaterialManager::get().getNr(); i++)
		{
			if (MaterialManager::get().getInstances()->at(i)->getName() == name)
				return true;
		}
		return false;
	}
}
/// TextureManager
namespace SP
{
	TextureManager::TextureManager() : CH_Manager()
	{
		initManager();
		isInit = true;
	}

	bool TextureManager::initManager()
	{
		//addInstance(new GL::Texture("texture_diffuse", "resources/maps/GlassTex.png", 0));
		//addInstance(new GL::Texture("texture_specular", "resources/maps/GlassTex.png", 1));

		return true;
	}

	bool TextureManager::hasTexture(const int& id)
	{
		for (int i = 0; i < TextureManager::get().getNr(); i++)
		{
			if (TextureManager::get().getInstances()->at(i)->getId() == id)
				return true;
		}
		return false;
	}
}