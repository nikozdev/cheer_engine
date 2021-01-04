#ifndef LUA_HEADER_H
#define LUA_HEADER_H

extern "C" {
#include <Lua/include/lua.h>
#include <Lua/include/lualib.h>
#include <Lua/include/lauxlib.h>
#include <Lua/include/luaconf.h>
}

#ifdef _WIN32
#pragma comment(lib, "Libs/lua54.lib")
#endif

#endif