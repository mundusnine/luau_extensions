/*
** $Id: loadlib.c,v 1.52.1.4 2009/09/09 13:17:16 roberto Exp $
** Dynamic library loader for Lua
**
** This module contains an implementation of loadlib for Unix systems
** that have dlfcn, an implementation for Darwin (Mac OS X), an
** implementation for Windows, and a stub for other systems.
**
** Luau Extensions:
** Modified version from lua 5.1.5 to work with Luau
** Removed dynamic library loading as it would add potential security issues. Maybe readded later.
** Added ../ in search path of require to work with luau-lsp.
** @TODO: 
** - Add luacode as optionnal through define.
** - Add optionnal PhysFS https://github.com/edubart/miniphysfs
*/

//START lprefix.h
/*
** Windows stuff
*/
#if defined(_WIN32) 	/* { */

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS  /* avoid warnings about ISO C functions */
#endif // !_CRT_SECURE_NO_WARNINGS

#endif			/* } */
//END lprefix.h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "lua.h"
#include "luaconf.h"
#include "lualib.h"

#include "luacode.h"

// START luaconf.h 5.1.5
/*
@@ LUA_PATH and LUA_CPATH are the names of the environment variables that
@* Lua check to set its paths.
@@ LUA_INIT is the name of the environment variable that Lua
@* checks for initialization code.
** CHANGE them if you want different names.
*/
#define LUA_PATH        "LUA_PATH"
#define LUA_CPATH       "LUA_CPATH"
#define LUA_INIT	"LUA_INIT"


/*
@@ LUA_PATH_DEFAULT is the default path that Lua uses to look for
@* Lua libraries.
@@ LUA_CPATH_DEFAULT is the default path that Lua uses to look for
@* C libraries.
** CHANGE them if your machine has a non-conventional directory
** hierarchy or if you want to install your libraries in
** non-conventional directories.
*/
#if defined(_WIN32)
/*
** In Windows, any exclamation mark ('!') in the path is replaced by the
** path of the directory of the executable file of the current process.
*/
#define LUA_LDIR	"!\\lua\\"
#define LUA_CDIR	"!\\"
#define LUA_PATH_DEFAULT  \
		".\\?.lua;" "..\\?.lua;" LUA_LDIR"?.lua;"  LUA_LDIR"?\\init.lua;" \
		             LUA_CDIR"?.lua;"  LUA_CDIR"?\\init.lua"
#define LUA_CPATH_DEFAULT \
	".\\?.dll;"  LUA_CDIR"?.dll;" LUA_CDIR"loadall.dll"

#else
#define LUA_ROOT	"/usr/local/"
#define LUA_LDIR	LUA_ROOT "share/lua/5.1/"
#define LUA_CDIR	LUA_ROOT "lib/lua/5.1/"
#define LUA_PATH_DEFAULT  \
		"./?.lua;" "../?.lua;" LUA_LDIR"?.lua;"  LUA_LDIR"?/init.lua;" \
		            LUA_CDIR"?.lua;"  LUA_CDIR"?/init.lua"
#define LUA_CPATH_DEFAULT \
	"./?.so;"  LUA_CDIR"?.so;" LUA_CDIR"loadall.so"
#endif


/*
@@ LUA_DIRSEP is the directory separator (for submodules).
** CHANGE it if your machine does not use "/" as the directory separator
** and is not Windows. (On Windows Lua automatically uses "\".)
*/
#if defined(_WIN32)
#define LUA_DIRSEP	"\\"
#else
#define LUA_DIRSEP	"/"
#endif


/*
@@ LUA_PATHSEP is the character that separates templates in a path.
@@ LUA_PATH_MARK is the string that marks the substitution points in a
@* template.
@@ LUA_EXECDIR in a Windows path is replaced by the executable's
@* directory.
@@ LUA_IGMARK is a mark to ignore all before it when bulding the
@* luaopen_ function name.
** CHANGE them if for some reason your system cannot use those
** characters. (E.g., if one of those characters is a common character
** in file/directory names.) Probably you do not need to change them.
*/
#define LUA_PATHSEP	";"
#define LUA_PATH_MARK	"?"
#define LUA_EXECDIR	"!"
#define LUA_IGMARK	"-"


// END luaconf.h 5.1.5

// START lauxlib.h
#ifdef LUAU_DEBUG
#include "debugger.h"
#else
int debugLevel = 0;
#define debugger_register(L,filename)
#endif

const char* luaL_gsub(lua_State* L, const char* s, const char* p,
    const char* r) {
    const char* wild;
    size_t l = strlen(p);
    luaL_Buffer b;
    luaL_buffinit(L, &b);
    while ((wild = strstr(s, p)) != NULL) {
        luaL_addlstring(&b, s, wild - s,0);  /* push prefix */
        luaL_addstring(&b, r);  /* push replacement in place of pattern */
        s = wild + l;  /* continue after 'p' */
    }
    luaL_addstring(&b, s);  /* push last suffix */
    luaL_pushresult(&b);
    return lua_tostring(L, -1);
}

int luaL_loadfile(lua_State* L, const char* filename) {
    FILE* f = fopen(filename, "rb+");
    if (f == NULL)
        return 0;
    char contents[4096] = { 0 };
    char temp[256] = { 0 };
    char* out = contents;
    while (fgets(temp, 256, f) != NULL) {
        out = strcat(out, temp);
    }
    size_t bytecodeSize = 0;
    size_t len = strlen(contents);
    lua_CompileOptions opts;
    opts.debugLevel = debugLevel;
    char* bytecode = luau_compile(contents, len, &opts, &bytecodeSize);
    int result = luau_load(L, filename, bytecode, bytecodeSize, 0);
    free(bytecode);
    fclose(f);

    debugger_register(L,filename);

    return result;
}

void luaL_setfuncs(lua_State* L, const luaL_Reg* l, int nup) {
    luaL_checkstack(L, nup, "too many upvalues");
    for (; l->name != NULL; l++) {  /* fill the table with given functions */
        int i;
        for (i = 0; i < nup; i++)  /* copy upvalues to the top */
            lua_pushvalue(L, -nup);
        lua_pushcclosure(L, l->func,l->name, nup);  /* closure with those upvalues */
        lua_rawseti(L, -2, i + 1);
    }
    lua_pop(L, nup);  /* remove upvalues */
}

void luaL_newlib(lua_State* L,const luaL_Reg* l){
  lua_createtable(L, 0, sizeof(l)/sizeof((l)[0]) - 1);
  luaL_setfuncs(L,l,0); 
}
//END

/*
** {======================================================
** 'require' function
** =======================================================
*/


static int readable (const char *filename) {
  FILE *f = fopen(filename, "r");  /* try to open file */
  if (f == NULL) return 0;  /* open failed */
  fclose(f);
  return 1;
}


static const char *pushnexttemplate (lua_State *L, const char *path) {
  const char *l;
  while (*path == *LUA_PATHSEP) path++;  /* skip separators */
  if (*path == '\0') return NULL;  /* no more templates */
  l = strchr(path, *LUA_PATHSEP);  /* find next separator */
  if (l == NULL) l = path + strlen(path);
  lua_pushlstring(L, path, l - path);  /* template */
  return l;
}


static const char *findfile (lua_State *L, const char *name,
                                           const char *pname) {
  const char *path;
  name = luaL_gsub(L, name, ".", LUA_DIRSEP);
  lua_getfield(L, LUA_ENVIRONINDEX, pname);
  path = lua_tostring(L, -1);
  if (path == NULL)
    luaL_error(L, "package. % s must be a string", pname);
  lua_pushliteral(L, "");  /* error accumulator */
  while ((path = pushnexttemplate(L, path)) != NULL) {
    const char *filename;
    filename = luaL_gsub(L, lua_tostring(L, -1), LUA_PATH_MARK, name);
    lua_remove(L, -2);  /* remove path template */
    if (readable(filename))  /* does file exist and is readable? */
      return filename;  /* return that file name */
    lua_pushfstring(L, "\n\tno file  %s", filename);
    lua_remove(L, -2);  /* remove file name */
    lua_concat(L, 2);  /* add entry to possible error message */
  }
  return NULL;  /* not found */
}


static void loaderror (lua_State *L, const char *filename) {
  luaL_error(L, "error loading module %s from file %s:\n\t%s",
                lua_tostring(L, 1), filename, lua_tostring(L, -1));
}


static int loader_Lua (lua_State *L) {
  const char *filename;
  const char *name = luaL_checkstring(L, 1);
  filename = findfile(L, name, "path");
  if (filename == NULL) return 1;  /* library not found in this path */
  if (luaL_loadfile(L, filename) != 0)
    loaderror(L, filename);
  return 1;  /* library loaded successfully */
}

static void print_table_fields(lua_State* L, int index) {
  int n = lua_objlen(L,index);
  for(int i =1; i <= n;++i){
    lua_rawgeti(L,-1,i);
    const char* key = lua_tostring(L, -1);  // get the key as a string
    printf("Field name: %s\n", key);
    lua_pop(L, 1);  // pop the value, but leave the key on the stack
  }
}

static int loader_preload (lua_State *L) {
  const char *name = luaL_checkstring(L, 1);
  lua_getfield(L, LUA_ENVIRONINDEX, "preload");
  print_table_fields(L,-1);
  if (!lua_istable(L, -1))
    luaL_error(L, "package.preload must be a table");
  lua_getfield(L, -1, name);
  if (lua_isnil(L, -1)){  /* not found? */
    print_table_fields(L,-2);
    lua_pushfstring(L, "\n\tno field package.preload['%s']", name);
  }
  return 1;
}


static const int sentinel_ = 0;
#define sentinel	((void *)&sentinel_)


static int ll_require (lua_State *L) {
  const char *name = luaL_checkstring(L, 1);
  #ifndef LUAU_BACKEND
  int i;
  //lua_settop(L, 1);  /* _LOADED table will be at index 2 */
  lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
  lua_getfield(L, 2, name);
  if (lua_toboolean(L, -1)) {  /* is it there? */
    if (lua_touserdata(L, -1) == sentinel)  /* check loops */
      luaL_error(L, "loop or previous error loading module %s", name);
    return 1;  /* package is already loaded */
  }
  /* else must load it; iterate over available loaders */
  lua_getfield(L, LUA_ENVIRONINDEX, "loaders");
  if (!lua_istable(L, -1))
    luaL_error(L, "package.loaders must be a table");
  lua_pushliteral(L, "");  /* error message accumulator */
  for (i=1; ; i++) {
    lua_rawgeti(L, -2, i);  /* get a loader */
    if (lua_isnil(L, -1))
      luaL_error(L, "module %s not found:%s",
                    name, lua_tostring(L, -2));
    lua_pushstring(L, name);
    lua_call(L, 1, 1);  /* call it */
    if (lua_isfunction(L, -1))  /* did it find module? */
      break;  /* module loaded successfully */
    else if (lua_isstring(L, -1))  /* loader returned error message? */
      lua_concat(L, 2);  /* accumulate it */
    else
      lua_pop(L, 1);
  }
  lua_pushlightuserdata(L, sentinel);
  lua_setfield(L, 2, name);  /* _LOADED[name] = sentinel */
  lua_pushstring(L, name);  /* pass name as argument to module */
  lua_call(L, 1, 1);  /* run loaded module */
  if (!lua_isnil(L, -1))  /* non-nil return? */
    lua_setfield(L, 2, name);  /* _LOADED[name] = returned value */
  if (lua_touserdata(L, -1) == sentinel) {   /* module did not set a value? */
    lua_pushboolean(L, 1);  /* use true as result */
    lua_pushvalue(L, -1);  /* extra copy to be returned */
    lua_setfield(L, 2, name);  /* _LOADED[name] = true */
  }
  #else
  char temp[256] = {0};
  snprintf(temp,256,"CACHED_%s",name);
  lua_getglobal(L,temp);
  if(lua_isnoneornil(L,-1)){
    lua_newtable(L);
    lua_setglobal(L,temp);
    lua_getglobal(L,temp);
  }
  #endif
  
  return 1;
}

/* }====================================================== */





/* auxiliary mark (for internal use) */
#define AUXMARK		"\1"

static void setpath (lua_State *L, const char *fieldname, const char *envname,
                                   const char *def) {
  const char *path = getenv(envname);
  if (path == NULL)  /* no environment variable? */
    lua_pushstring(L, def);  /* use default */
  else {
    /* replace ";;" by ";AUXMARK;" and then AUXMARK by default path */
    path = luaL_gsub(L, path, LUA_PATHSEP LUA_PATHSEP,
                              LUA_PATHSEP AUXMARK LUA_PATHSEP);
    luaL_gsub(L, path, AUXMARK, def);
    lua_remove(L, -2);
  }
  lua_setfield(L, -2, fieldname);
}

static const luaL_Reg pk_funcs[] = {
  /* placeholders */
  {"preload", NULL},
  {"cpath", NULL},
  {"path", NULL},
  {"loaders", NULL},
  {"loaded", NULL},
  {NULL, NULL}
};

static const luaL_Reg ll_funcs[] = {
  {"require", ll_require},
  {NULL, NULL}
};


static const lua_CFunction loaders[] =
  {loader_preload, loader_Lua, NULL};
static const char* loader_names[] =
{ "preload", "Lua", NULL};

LUALIB_API int luaopen_package (lua_State *L) {
    int i;
    /* create new type _LOADLIB */
    luaL_newmetatable(L, "_LOADLIB");
    /* create `package' table */
    luaL_register(L, "package", pk_funcs);
    lua_pushvalue(L, -1);
    lua_replace(L, LUA_ENVIRONINDEX);
    /* create `loaders' table */
    lua_createtable(L, sizeof(loaders) / sizeof(loaders[0]) - 1, 0);
    /* fill it with pre-defined loaders */
    for (i = 0; loaders[i] != NULL; i++) {
        lua_pushcfunction(L, loaders[i],loader_names[i]);
        lua_rawseti(L, -2, i + 1);
    }
    lua_setfield(L, -2, "loaders");  /* put it in field `loaders' */
    setpath(L, "path", LUA_PATH, LUA_PATH_DEFAULT);  /* set field `path' */
    /* store config information */
    lua_pushliteral(L, LUA_DIRSEP "\n" LUA_PATHSEP "\n" LUA_PATH_MARK "\n"
        LUA_EXECDIR "\n" LUA_IGMARK);
    lua_setfield(L, -2, "config");
    /* set field `loaded' */
    luaL_findtable(L, LUA_REGISTRYINDEX, "_LOADED", 2);
    lua_setfield(L, -2, "loaded");
    /* set field `preload' */
    lua_newtable(L);
    lua_setfield(L, -2, "preload");
    lua_pushvalue(L, LUA_GLOBALSINDEX);
    luaL_register(L, NULL, ll_funcs);  /* open lib into global table */
    lua_pop(L, 1);
    return 1;  /* return 'package' table */
}

