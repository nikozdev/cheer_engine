#include "../../Headers/ESC/Entity.h"
#include "../../Headers/Lua/L_state.h"
#include "../../Headers/CheerEngine.h"

namespace ESC
{
	//Initialization functions
	bool Entity::loadEntity(const std::string& name, Entity* ent)
	{
		auto l_state = ent->lState;
		auto cmpNames = l_state->getStrVec(name.c_str());
		ent->lState->getToStack(name.c_str());
		for (auto& cmpName : cmpNames)
		{
			if (cmpName == "LifeComponent")
			{
				ent->addComponent(std::type_index(typeid(LifeCmp)),
					new LifeCmp(l_state->getLState()));
			}
			else if (cmpName == "GraphicsComponent")
			{
				ent->addComponent(std::type_index(typeid(GraphicsCmp)),
					new GraphicsCmp(l_state->getLState()));
			}
			else if (cmpName == "NpcComponent")
			{
				ent->addComponent(std::type_index(typeid(NpcCmp)),
					new NpcCmp(l_state->getLState()));
			}
		}
		lua_pop(l_state->getLState(), 1);
		return true;
	}

	//Constructors&Destructors
	Entity::Entity(::L::L_state* lState, const std::string& name)
	{
		this->name = name;
		this->lState = lState;
		if (loadEntity(this->name, this))
			printf("ENTITY::CONSTRUCTOR: Entity %s is created.\n", name.c_str());
		else
			printf("ENTITY::CONSTRUCTOR: Failed to create Entity %s.\n", name.c_str());
	}

	Entity::~Entity()
	{
		if (this == NULL) return;
		deleteComponents();
		printf("ENTITY::DESTRUCTOR: %s is destroyed.\n", name.c_str());
	}

	//Components processing
	void Entity::addComponent(std::type_index newID, Component* newCmp)
	{
		components[newID] = newCmp;
	}

	void Entity::deleteComponents()
	{
		if (components.size() == 0 || components.empty())
			return;
		for (auto id_cmp : components)
		{
			if (id_cmp.second != NULL) delete id_cmp.second;
		}
		components.clear();
	}

	//Static Lua functions
	int Entity::setSprite(lua_State* L)
	{
		auto entity = ((Entity*)lua_touserdata(L, -2));

		const char* spriteName = lua_tostring(L, -1);

		return 0;
	}

	int Entity::say(lua_State* L)
	{//entity:say("HelloLua"); equals to say(entity, "HelloLua")
		Entity* entity = static_cast<Entity*>(lua_touserdata(L, -2));
		if (entity == NULL) return 0;

		const char* words = lua_tostring(L, -1);

		auto npcCmp = entity->getComponent<NpcCmp>();
		if (npcCmp)
			npcCmp->say("ESC::" + (entity->getName()) + "::NPC_CMP::CPP_SAY: " + words);
		else {
			printf("ESC::COMPONENT::NPC_CMP::CPP_SAY: %s doesn't have NpcComponent.\n",
				entity->getName().c_str());
		}
		return 0;
	}

	//Basic functions: construction, destruction, index, newindex
	int Entity::newEntity(lua_State* L)
	{//ATTENTION:: Created entity changes stack of the lua L_state!
		CheerEngine* chEngine = static_cast<CheerEngine*>
			(lua_touserdata(L, lua_upvalueindex(1)));
		SP::EntityManager* entManager = static_cast<SP::EntityManager*>
			(lua_touserdata(L, lua_upvalueindex(2)));

		std::string name;
		if (!lua_isstring(L, -1))
			return 0;
		else
			name = static_cast<std::string>(lua_tostring(L, -1));

		if (entManager->hasEntity(name))
		{
			printf("CHEER_ENGINE::NEW_ENTITY: attempt to create new Entity with already existing name.\n");
			lua_getglobal(L, name.c_str());
			return 1;
		}
		void* entPtr = static_cast<ESC::Entity*>(lua_newuserdata(L, sizeof(ESC::Entity)));
		new (entPtr) ESC::Entity(chEngine->getTopState(), name);

		entManager->addInstance(static_cast<ESC::Entity*>(entPtr));

		luaL_getmetatable(L, "meta_Entity");
		lua_setmetatable(L, -2);

		lua_newtable(L);
		lua_setuservalue(L, 1);
		return 1;
	}

	int Entity::destroyEntity(lua_State* L)
	{
		CheerEngine* chEngine = static_cast<CheerEngine*>
			(lua_touserdata(L, lua_upvalueindex(1)));
		SP::EntityManager* entManager = static_cast<SP::EntityManager*>
			(lua_touserdata(L, lua_upvalueindex(2)));

		ESC::Entity* ent = (ESC::Entity*)lua_touserdata(L, -1);
		if (ent == NULL)
		{
			//ent->~Entity();
			//entManager->decremNr();
			return 0;
		}
		else
		{
			entManager->removeInstance(ent);
		}

		return 0;
	}

	int Entity::indexEntity(lua_State* L)
	{//Entity.key
		::SP::EntityManager* entManager = static_cast<SP::EntityManager*>
			(lua_touserdata(L, lua_upvalueindex(2)));

		const char* index = lua_tostring(L, -1);

		ESC::Entity* ent = NULL;
		ent = static_cast<ESC::Entity*>(lua_touserdata(L, -2));
		if (ent == NULL) return 0;

		if (strcmp(index, "name") == 0)
		{
			lua_pushstring(L, ent->getName().c_str());
			return 1;
		}
		else if (strcmp(index, "health") == 0)
		{
			lua_pushnumber(L,
				ent->getComponent<ESC::LifeCmp>()->getHealth());
			return 1;
		}
		else
		{//Get values from script stack
			lua_getuservalue(L, 1);
			if (!lua_isnil(L, -1))
			{
				lua_pushvalue(L, 2);
				lua_gettable(L, -2);
			}
			else {
				lua_getglobal(L, "Entity");
				lua_pushstring(L, index);
				//Get value from table without invoke metamethods
				lua_rawget(L, -2);
			}
			return 1;
		}
	}

	int Entity::newIndexEntity(lua_State* L)
	{
		auto ent = (ESC::Entity*)lua_touserdata(L, -3);
		const char* index = lua_tostring(L, -2);
		//-1 is value we wanna set
		if (strcmp(index, "name") == 0)
		{
			ent->setName((std::string)lua_tostring(L, -1));
			return 1;
		}
		else
		{//We can't override native object unless uservalue table is defined
			lua_getuservalue(L, 1);
			lua_pushvalue(L, 2);
			lua_pushvalue(L, 3);
			lua_settable(L, -3);
		}
		return 0;
	}
}