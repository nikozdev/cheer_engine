#ifndef LUA_STRUCTS_H
#define LUA_STRUCTS_H

#include <Lua_header.hpp>

namespace L
{
	/// Create this structure and set the size.
	/// It is upvalues - data and variables that will be given with cClosures
	/// PushToLua sets all pointers as lightuserdata on the top of lua stack
	struct UpValues
	{
		static constexpr int MAX_AMOUNT = 10;
		void* up_values[MAX_AMOUNT];
		int up_val_amount = 0;

		void alignAmount();
		void pushToLua(lua_State* L);
	};
}
#endif