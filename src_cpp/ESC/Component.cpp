#include "..\..\Headers\ESC\Component.h"
#include "../../Headers/Lua/L_state.h"

#include "../../Headers/ESC_header.hpp"

#include "..\..\Headers\GL\GL_types.h"

namespace ESC
{
	Component::Component(lua_State* L)
		:l_state(L)
	{
		//
	}
}

namespace ESC
{
	ESC::LifeCmp::LifeCmp(lua_State* L) : Component(L),
		maxHealth(1.0f), health(1.0f), healthState(Undead)
	{//Entity table has to be on the top.
		lua_getfield(L, -1, "LifeComponent");
		//The component table is on the top.
		if (lua_istable(L, -1))
		{
			setHealth(getLuaField<float>("health"));
			healthState = static_cast<HealthStates>(getLuaField<int>("health_state"));
			//Pop out table
			lua_pop(L, 1);
			printf("ESC::COMPONENT::HEALTH_CMP::CONSTRUCTOR: HealthCmp has been created with table.\n");
			return;
		}
		printf("ESC::COMPONENT::HEALTH_CMP::CONSTRUCTOR: HealthCmp has been created by default.\n");
	}

	ESC::LifeCmp::~LifeCmp()
	{
		printf("ESC::COMPONENT::HEALTH_CMP::DESTRUCTOR: HealthCmp has been destroyed.\n");
	}

	//Functions
	void ESC::LifeCmp::takeHeal(float healAmount)
	{
		setHealth(health + healAmount);
		if (health / maxHealth > maxHealth / 60.0f)
		{
			healthState = Healthy;
		}
	}

	void ESC::LifeCmp::takeDamage(float damage)
	{
		setHealth(health - damage);
	}

	void ESC::LifeCmp::die()
	{
		healthState = Dead;
	}
}

namespace ESC
{
	//Graphics Component
	//Constructors&Destructors
	ESC::GraphicsCmp::GraphicsCmp(lua_State* L) : Component(L),
		spriteName("empty"), currAnim("empty")
	{//Entity table has to be on the top
		lua_getfield(L, -1, "GraphicsComponent");
		//GraphicsComponent table on the top
		if (lua_istable(L, -1))
		{
			setSpriteName(getLuaField<std::string>("sprite_name"));
			setAnimation(getLuaField<std::string>("anim_name"));

			lua_pop(L, 1);
			printf("ESC::COMPONENT::GRAPHICS_CMP::CONTRUCTOR: GraphicsCmp has been created with table.\n");
			return;
		}
		printf("ESC::COMPONENT::GRAPHICS_CMP::CONSTRUCTOR: GraphicsCmp has been created by default.\n");
	}

	ESC::GraphicsCmp::~GraphicsCmp()
	{
		printf("ESC::COMPONENT::GRAPHICS_CMP::DESTRUCTOR: GraphicsCmp has been destroyed.\n");
	}

	void ESC::GraphicsCmp::loadSprite(std::string spriteName)
	{
		//
	}

	void ESC::GraphicsCmp::changeAnimation(std::string animationName)
	{
		setAnimation(animationName);
	}
}

namespace ESC
{
	ESC::NpcCmp::NpcCmp(lua_State* L) : Component(L),
		canTalk(false), defaultPhrase("Hello World!")
	{//The entity table have to be on the top
		lua_getfield(l_state, -1, "NpcComponent");
		//Now component table is on the top
		if (lua_istable(L, -1))
		{//
			canTalk = getLuaField<bool>("can_talk");
			defaultPhrase = getLuaField<std::string>("greet_phrase");

			lua_pop(L, 1);
			printf("ESC::COMPONENT::NPC_CMP::CONSTRUCTOR: NpcComponent has been created with table.\n");
			return;
		}
		printf("ESC::COMPONENT::NPC_CMP::CONSTRUCTOR: NpcComponent has been created by default.\n");
	}

	ESC::NpcCmp::~NpcCmp()
	{
		printf("ESC::COMPONENT::NPC_CMP::DESTRUCTOR: NpcComponent is destroyed.\n");
	}

	void ESC::NpcCmp::say(std::string words)
	{
		printf("%s\n", words.c_str());
	}
}