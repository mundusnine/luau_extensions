#pragma once

#define LUAU_VERSION_NUM 0566

#ifndef LUAUC_PLUGIN_IMPL
#include "lua.h"
#include "lualib.h"
#include "luaconf.h"
#else
#include "luauc_plugin_api.h"
#endif

#define LUA_IOLIBNAME	"io"
LUALIB_API int luaopen_io(lua_State* L);

#define LUA_LOADLIBNAME	"package"
LUALIB_API int (luaopen_package)(lua_State* L);

#define LUA_RBLXMATHLIBNAME	"rbxmath"
LUALIB_API int (luaopen_rblx_math)(lua_State* L);


// Helper functions
LUALIB_API void luaL_openlib(lua_State* L,const char* libname, lua_CFunction func);
LUALIB_API void luaL_requiref (lua_State *L, const char *modname,lua_CFunction openf, int glb);
LUALIB_API void luaL_newlib(lua_State* L,const luaL_Reg* l);
