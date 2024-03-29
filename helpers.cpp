#include "lua.h"
#include "luaconf.h"

#include <stdio.h>

LUALIB_API void luaL_openlib(lua_State* L,const char* libname, lua_CFunction func)
{
    lua_pushcfunction(L, func, NULL);
    lua_pushstring(L, libname);
    lua_call(L, 1, 0);
}

/*
** ensure that stack[idx][fname] has a table and push that table
** into the stack
*/
static int luaL_getsubtable (lua_State *L, int idx, const char *fname) {
  lua_getfield(L, idx, fname);
  if (lua_istable(L, -1)) return 1;  /* table already there */
  else {
    lua_pop(L, 1);  /* remove previous result */
    idx = lua_absindex(L, idx);
    lua_newtable(L);
    lua_pushvalue(L, -1);  /* copy to be left at top */
    lua_setfield(L, idx, fname);  /* assign new table to field */
    return 0;  /* false, because did not find table there */
  }
}

/*
** stripped-down 'require'. Calls 'openf' to open a module,
** registers the result in 'package.loaded' table and, if 'glb'
** is true, also registers the result in the global table.
** Leaves resulting module on the top.
*/
    LUALIB_API void luaL_requiref (lua_State *L, const char *modname, lua_CFunction openf, int glb) {
        char temp[128] = {0};
        snprintf(temp,128,"luaopen_%s",modname);
        lua_pushcfunction(L, openf,temp);
        lua_pushstring(L, modname);  /* argument to open function */
        lua_call(L, 1, 1);  /* open module */
        luaL_getsubtable(L, LUA_REGISTRYINDEX, "_LOADED");
        lua_pushvalue(L, -2);  /* make copy of module (call result) */
        lua_setfield(L, -2, modname);  /* _LOADED[modname] = module */
        lua_pop(L, 1);  /* remove _LOADED table */
        if (glb) {
        lua_pushvalue(L, -1);  /* copy of 'mod' */
        lua_setglobal(L, modname);  /* _G[modname] = module */
    }
}