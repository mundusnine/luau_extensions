#include "lua.h"
#include "luaconf.h"

LUALIB_API void luaL_openlib(lua_State* L,const char* libname, lua_CFunction func)
{
    lua_pushcfunction(L, func, NULL);
    lua_pushstring(L, libname);
    lua_call(L, 1, 0);
}