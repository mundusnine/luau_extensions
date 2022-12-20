#pragma once

#include "lua.h"
#include "luaconf.h"

#define LUA_IOLIBNAME	"io"
LUALIB_API int luaopen_io(lua_State* L);

#define LUA_LOADLIBNAME	"package"
LUALIB_API int (luaopen_package)(lua_State* L);

#define LUA_RBLXMATHLIBNAME	"rbxmath"
LUALIB_API int (luaopen_rblx_math)(lua_State* L);


// Helper functions
LUALIB_API void luaL_openlib(lua_State* L,const char* libname, lua_CFunction func);