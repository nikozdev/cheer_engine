#ifndef MANAGERS_H
#define MANAGERS_H

#include <vector>
#include <string>

//Predefinitions
namespace GL
{
	class Shader;
	class Texture;
	class Material;
	class Mesh;
	class Sprite;
}
namespace ESC
{
	class Entity;
}
//Enumerations
namespace SP
{
	enum ShaderPrograms {
		CORE_2D = 0, CORE_3D = 1
	};
	enum TextureTypes {
		DIFFUSE = 0, SPECULAR = 1, NORMAL = 2
	};
	enum MaterialTypes {
		DEFAULT_2D = 0, DEFAULT_3D = 1
	};
	enum MeshNames {
		VAMP_LORD_0 = 0, BOOK_0, DRAGON_0
	};
	enum SpriteNames {
		HERO_0 = 0, 
	};
	enum FontNames {
		DEFAULT = 0
	};
}
//Managers
namespace SP
{
	/// Basic template class for singleton managers
	/// --Keeps instances tracking of some type. Has refs to instances of all given types
	/// --Access any instance refere.nce by {MANAGER_NAME}::get().getInstances()[index];
	template<typename T>
	class CH_Manager
	{
	protected:
		int m_instancesNr = 0;
		std::vector<T*> m_instList;

		inline CH_Manager() : m_instList(std::vector<T*>{}) {}
	public:
		CH_Manager(const CH_Manager&) = delete;

		static CH_Manager& get()
		{
			static CH_Manager s_Instance;
			return s_Instance;
		}

		T* getInstance(int num) { return getInstances()->at(num); }

		std::vector<T*>* getInstances()
		{
			if (m_instList.empty())
				return NULL;
			return &m_instList;
		}
		bool hasInstance(T* instance)
		{
			for (auto inst : m_instList)
			{
				if (instance == inst && instance != NULL)
					return true;
			}
			return false;
		}
		void addInstance(T* instance)
		{
			if (hasInstance(instance))
			{//Recreate if it already exists
				return;
				//removeInstance(instance);
			}
			m_instList.push_back(instance);
			m_instancesNr++;
		}
		void removeInstance(T* instance)
		{
			for (int i = 0; i < m_instList.size(); i++)
			{
				if (hasInstance(instance))
				{
					if (instance)
						instance->~T();
					m_instList.erase(m_instList.begin() + i);
					m_instancesNr--;
					break;
				}
			}
		}
		void destroyAll()
		{
			for (int i = 0; i < m_instancesNr; i++)
			{
				removeInstance(m_instList[i]);
			}
		}

		int getNr() const { return m_instancesNr; };
		void incremNr(int newNr = 1) { m_instancesNr += newNr; };
		void decremNr(int newNr = 1) { m_instancesNr -= newNr; };
	};
	/// Keeps tracking of all entities
	/// Counts number of Entities, gets their references into the list, deletes them
	class EntityManager : public CH_Manager<ESC::Entity>
	{
	private:
		EntityManager();
	public:
		static bool isInit;

		static EntityManager& get()
		{
			static EntityManager s_Instance;
			return s_Instance;
		}

		bool hasEntity(const std::string& entName) const;
		bool initManager();
	};
	/// ShaderManager
	/// Keeps tracking of all shaders
	class ShaderManager : public CH_Manager<GL::Shader>
	{
	private:
		ShaderManager();
	public:
		static bool isInit;

		static ShaderManager& get()
		{
			static ShaderManager s_Instance;
			return s_Instance;
		}

		static bool hasShader(const int& id);
		bool initManager();
	};
	/// MaterialManager class
	///  Keeps tracking of all Materials
	class MaterialManager : public CH_Manager<GL::Material>
	{
	private:
		MaterialManager();
	public:
		static bool isInit;

		static MaterialManager& get()
		{
			static MaterialManager s_Instance;
			return s_Instance;
		}

		static bool hasMaterial(const std::string& name);
		bool initManager();
	};
	/// TextureManager class
	/// --Manages texture bindings, cashing and activation
	class TextureManager : public CH_Manager<GL::Texture>
	{
	private:
		TextureManager();
	public:
		static bool isInit;

		static TextureManager& get()
		{
			static TextureManager s_Instance;
			return s_Instance;
		}

		static bool hasTexture(const int& id);
		bool initManager();
	};
}

#endif