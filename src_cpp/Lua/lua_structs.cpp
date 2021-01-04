#include <Lua\lua_structs.h>

void L::UpValues::alignAmount()
{
	for (int i = 0; i < MAX_AMOUNT; i++)
		if (up_values[i] != NULL)
			up_val_amount = i + 1;
}

void L::UpValues::pushToLua(lua_State* L)
{
	alignAmount();
	for (int i = 0; i < up_val_amount; i++)
	{
		lua_pushlightuserdata(L, up_values[i]);
	}
}