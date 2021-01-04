#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"

#include <map>
#include <vector>
#include <typeindex>
#include <string>

namespace ESC
{
	/// Entity class - the basic class unit.
	/// Can be initialized by name of table in lua script.
	class Entity
	{
	private:
		std::string name;
		::L::L_state* lState;

		std::map<std::type_index, Component*> components;
		std::vector<Entity*> underEnts;
		Entity* overEnt = nullptr;
	public:
		// Constructors&Destructors
		Entity(::L::L_state* lState, const std::string& name);
		~Entity();

		// Accessors
		std::string getName() const
		{
			return name;
		};
		// Modificators
		void setName(std::string newName)
		{
			name = newName;
		};


		// Initialize functions
		static bool loadEntity(const std::string& name, ESC::Entity* ent);
		// Component functions
		void addComponent(std::type_index newID, Component* newCmp);
		void deleteComponents();

		template<typename T>
		T* getComponent()
		{
			auto it = components.find(std::type_index(typeid(T)));
			if (it != components.end())
			{
				return dynamic_cast<T*>(it->second);
			}
			return nullptr;
		}

		// Static lua functions
		/// Constructor for an Entity Lua L_state
		static int newEntity(lua_State* L);
		/// Destructor for an Entity in Lua L_state.
		static int destroyEntity(lua_State* L);
		static int indexEntity(lua_State* L);
		static int newIndexEntity(lua_State* L);

		static int setSprite(lua_State* L);
		static int say(lua_State* L);
	};
}
/*	struct LEntity
	{
	private:
		//Variables
		Entity* ent;
		L::L_state* lState;
		//Initialize functions
		ESC::Entity* loadEntity(const std::string& name);
		template<typename T>
		void addComponent(ESC::Entity* ent, lua_State* L)
		{
			ent->addComponent(std::type_index(typeid(T)), new T(L));
		}
	public:
		//Constructors&Destructors
		LEntity(L::L_state* lState, const std::string& name);
		~LEntity();

		//Accessors
		Entity* getEnt() const { return ent; }

		//Static lua functions
		static int setSprite(lua_State* L);
		static int say(lua_State* L);
	};
}*/

/// --Debug--
/// 04.10.2020 - The main issue: whenever I call lua functions of entity
/// It doesn't gives me userdata of that entity
/// Lua ust destroys it and doesn't gives me Entity reference at all
/// --Debug--
/// 07.10.2020 - Everything is fixed.
/// Entity reference is accessful in meta functions and can be indexed
/// Lua stack was polluted when I tried to make new entity table
/// It is essential to use userdata or lightuserdata but I used ptr = new what caused new memory adress
/// Also, in index method we have to call not table with entity name, but global Entity which contains all members
/// --Debug--
/// 14.10.2020
/// I want to use wrapping of entity in LEntity class for lua. Simple entity - for C++
#endif