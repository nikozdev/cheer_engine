#ifndef COMPONENT_H
#define COMPONENT_H

#include <assert.h>
#include <string>

#include <Lua_header.hpp>

//predefinitions
namespace L
{
	class L_state;
}

namespace ESC
{
	class Component
	{
	protected:
		lua_State* l_state;

		template<typename T>
		inline T getLuaField(const char* lua_field)
		{//Get each field if it is there, after that - pop this out
			return;
		}
		template<>
		inline bool getLuaField<bool>(const char* lua_field)
		{
			bool result = NULL;
			if (!lua_istable(l_state, -1)) return result;
			lua_getfield(l_state, -1, lua_field);
			if (lua_isboolean(l_state, -1)) result = static_cast<bool>(lua_toboolean(l_state, -1));
			lua_pop(l_state, 1);
			return result;
		}
		template<>
		inline int getLuaField<int>(const char* lua_field)
		{
			int result = NULL;
			if (!lua_istable(l_state, -1)) return result;
			lua_getfield(l_state, -1, lua_field);
			if (lua_isinteger(l_state, -1)) result = static_cast<int>(lua_tointeger(l_state, -1));
			lua_pop(l_state, 1);
			return result;
		}
		template<>
		inline float getLuaField<float>(const char* lua_field)
		{
			float result = NULL;
			if (!lua_istable(l_state, -1)) return result;
			lua_getfield(l_state, -1, lua_field);
			if (lua_isnumber(l_state, -1)) result = static_cast<float>(lua_tonumber(l_state, -1));
			lua_pop(l_state, 1);
			return result;
		}
		template<>
		inline std::string getLuaField<std::string>(const char* lua_field)
		{
			std::string result = "nil";
			if (!lua_istable(l_state, -1)) return result;
			lua_getfield(l_state, -1, lua_field);
			if (lua_isstring(l_state, -1)) result = static_cast<std::string>(lua_tostring(l_state, -1));
			lua_pop(l_state, 1);
			return result;
		}
	public:
		//Constructors&Destructors
		Component(lua_State* L);
		virtual ~Component() {};
	};
}

#endif

#ifndef LIFE_COMP_H
#define LIFE_COMP_H

namespace ESC
{
	enum HealthStates {
		Healthy = 2, Injured = 1, Dead = 0,
		Undead = -1, Ghost = -2
	};
	class LifeCmp : public Component
	{
	private:
		//Life variables
		float health;
		float maxHealth;
		HealthStates healthState;

		//Functions
		void die();
	public:
		//Constructors&Destructors
		LifeCmp(lua_State* L);
		~LifeCmp();

		//Accessors
		float getHealth() { return health; }
		void setHealth(float newAmount) {
			if (newAmount > maxHealth) { newAmount = maxHealth; }
			else if (newAmount < 0) { newAmount = 0; die(); }
			health = newAmount;
			if ((newAmount / maxHealth * 60.0f) <= 60.0f)
			{
				healthState = Injured;
			}
			else if ((newAmount / maxHealth * 60.0f) > 60.0f)
			{
				healthState = Healthy;
			}
		}

		float getMaxHealth() { return maxHealth; }

		HealthStates getHealthState() { return healthState; }

		//Functions
		void takeHeal(float healAmount);
		void takeDamage(float damage);
	};
}

#endif

#ifndef GRAPH_CMP_H
#define GRAPH_CMP_H

namespace ESC
{
	class GraphicsCmp : public Component
	{
	private:
		std::string spriteName;
		std::string currAnim;
	public:
		//Constructors&Destructors
		GraphicsCmp(lua_State* L);
		~GraphicsCmp();
		//Accessors
		std::string getFileName() const
		{
			return spriteName;
		};
		void setSpriteName(std::string newFileName)
		{
			spriteName = newFileName;
		};

		std::string getAnimation() const { return currAnim; };
		void setAnimation(std::string newAnim) { currAnim = newAnim; };

		//Functions
		void loadSprite(std::string spriteName);
		void changeAnimation(std::string animationName);
	};
}

#endif

#ifndef NPC_COMP_H
#define NPC_COMP_H

namespace ESC
{
	class NpcCmp : public Component
	{
	private:
		bool canTalk;
		std::string defaultPhrase;
	public:
		//Constructors&Destructors
		NpcCmp(lua_State* L);
		~NpcCmp();
		//Accessors
		bool getCanTalk() { return canTalk; }

		std::string getDefaultPhrase() { return defaultPhrase; }

		//Functions
		void say(std::string words);
	};
}

#endif