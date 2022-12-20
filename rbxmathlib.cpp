#include "luau_ext.h"
#include <string.h>

#define HANDMADE_MATH_NO_SSE
#include "HandmadeMath.h"

#include "Luau/Common.h"
#include "lualib.h"

static int vector2_new(lua_State* L);

static int vector2_add(lua_State* L) {
	hmm_vec2 t1 = { 0 };
	hmm_vec2 t2 = { 0 };
	
	LUAU_ASSERT(lua_rawgetfield(L, 1, "X") == LUA_TNUMBER);
	t1.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 1, "Y") == LUA_TNUMBER);
	t1.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 2, "X") == LUA_TNUMBER);
	t2.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 2, "Y") == LUA_TNUMBER);
	t2.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	hmm_vec2 out = HMM_AddVec2(t1, t2);

	lua_settop(L, 0);
	vector2_new(L);

	lua_pushstring(L, "X");
	lua_pushnumber(L, out.X);
	lua_rawset(L, -3);

	lua_pushstring(L, "Y");
	lua_pushnumber(L, out.Y);
	lua_rawset(L, -3);

	return 1;
}

static int vector2_mul(lua_State* L) {
	hmm_vec2 t1 = { 0 };
	hmm_vec2 t2 = { 0 };

	LUAU_ASSERT(lua_rawgetfield(L, 1, "X") == LUA_TNUMBER);
	t1.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 1, "Y") == LUA_TNUMBER);
	t1.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 2, "X") == LUA_TNUMBER);
	t2.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 2, "Y") == LUA_TNUMBER);
	t2.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	hmm_vec2 out = HMM_MultiplyVec2(t1, t2);

	lua_settop(L, 0);
	vector2_new(L);

	lua_pushstring(L, "X");
	lua_pushnumber(L, out.X);
	lua_rawset(L, -3);

	lua_pushstring(L, "Y");
	lua_pushnumber(L, out.Y);
	lua_rawset(L, -3);

	return 1;
}

static int vector2_div(lua_State* L) {
	hmm_vec2 t1 = { 0 };
	hmm_vec2 t2 = { 0 };

	LUAU_ASSERT(lua_rawgetfield(L, 1, "X") == LUA_TNUMBER);
	t1.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 1, "Y") == LUA_TNUMBER);
	t1.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 2, "X") == LUA_TNUMBER);
	t2.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 2, "Y") == LUA_TNUMBER);
	t2.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	hmm_vec2 out = HMM_DivideVec2(t1, t2);

	lua_settop(L, 0);
	vector2_new(L);

	lua_pushstring(L, "X");
	lua_pushnumber(L, out.X);
	lua_rawset(L, -3);

	lua_pushstring(L, "Y");
	lua_pushnumber(L, out.Y);
	lua_rawset(L, -3);

	return 1;
}

static int vector2_sub(lua_State* L) {
	hmm_vec2 t1 = { 0 };
	hmm_vec2 t2 = { 0 };

	LUAU_ASSERT(lua_rawgetfield(L, 1, "X") == LUA_TNUMBER);
	t1.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 1, "Y") == LUA_TNUMBER);
	t1.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 2, "X") == LUA_TNUMBER);
	t2.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 2, "Y") == LUA_TNUMBER);
	t2.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	hmm_vec2 out = HMM_SubtractVec2(t1, t2);

	lua_settop(L, 0);
	vector2_new(L);

	lua_pushstring(L, "X");
	lua_pushnumber(L, out.X);
	lua_rawset(L, -3);

	lua_pushstring(L, "Y");
	lua_pushnumber(L, out.Y);
	lua_rawset(L, -3);

	return 1;
}

static int vector2_neg(lua_State* L) {
	hmm_vec2 t1 = { 0 };

	LUAU_ASSERT(lua_rawgetfield(L, 1, "X") == LUA_TNUMBER);
	t1.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, 1, "Y") == LUA_TNUMBER);
	t1.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_settop(L, 0);
	vector2_new(L);

	lua_pushstring(L, "X");
	lua_pushnumber(L, -t1.X);
	lua_rawset(L, -3);

	lua_pushstring(L, "Y");
	lua_pushnumber(L, -t1.Y);
	lua_rawset(L, -3);

	return 1;
}

static int vector2_cross(lua_State* L) {
	int n = lua_gettop(L);    /* number of arguments */
	if (n == 1) {
		luaL_error(L, "Call function Cross with function notation i.e. ':' not '.' ");
		lua_pushnumber(L, 0);
		return 1;
	}
	hmm_vec3 t1 = { 0 };
	hmm_vec3 t2 = { 0 };

	LUAU_ASSERT(lua_rawgetfield(L, -2, "X") == LUA_TNUMBER);
	t1.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, -2, "Y") == LUA_TNUMBER);
	t1.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, -1, "X") == LUA_TNUMBER);
	t2.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, -1, "Y") == LUA_TNUMBER);
	t2.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);

	double out = HMM_Cross(t1,t2).Z;
	
	lua_pushnumber(L, out);

	return 1;
}

static int vector2_dot(lua_State* L) {
	int n = lua_gettop(L);    /* number of arguments */
	if (n == 1) {
		luaL_error(L, "Call function Dot with function notation i.e. ':' not '.' ");
		lua_pushnumber(L, 0);
		return 1;
	}
	hmm_vec2 t1 = { 0 };
	hmm_vec2 t2 = { 0 };

	LUAU_ASSERT(lua_rawgetfield(L, -2, "X") == LUA_TNUMBER);
	t1.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, -2, "Y") == LUA_TNUMBER);
	t1.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, -1, "X") == LUA_TNUMBER);
	t2.X = lua_tonumber(L, -1);
	lua_pop(L, 1);
	LUAU_ASSERT(lua_rawgetfield(L, -1, "Y") == LUA_TNUMBER);
	t2.Y = lua_tonumber(L, -1);
	lua_pop(L, 1);

	double out = HMM_DotVec2(t1, t2);

	lua_pushnumber(L, out);

	return 1;
}

static int vector2_new(lua_State* L) {

	int n = lua_gettop(L);    /* number of arguments */
	lua_newtable(L);
	lua_pushstring(L, "X");
	double tempX = n >= 1 ? lua_tonumber(L, 1) : 0;
	lua_pushnumber(L, n >= 1 ? lua_tonumber(L, 1) : 0);
	lua_settable(L, -3);

	lua_pushstring(L, "Y");
	double tempY = n == 2 ? (float)lua_tonumber(L, 2) : 0;
	lua_pushnumber(L, tempY);
	lua_settable(L, -3);

	lua_pushcfunction(L, vector2_cross, "vec2Cross");
	lua_setfield(L, -2, "Cross");

	lua_pushcfunction(L, vector2_dot, "vec2Dot");
	lua_setfield(L, -2, "Dot");

	luaL_getmetatable(L, "Vector2Meta");
	LUAU_ASSERT(lua_istable(L, -1));
	lua_setmetatable(L, -2);

	return 1;
}

/* Vector2
* 	Magnitude: number
	Unit: Vector2
	X: number
	Y: number
	function Cross(self, other: Vector2): number
	function Dot(self, v: Vector2): number
	function Lerp(self, v: Vector2, alpha: number): Vector2
	function __add(self, other: Vector2): Vector2
	function __div(self, other: Vector2 | number): Vector2
	function __mul(self, other: Vector2 | number): Vector2
	function __sub(self, other: Vector2): Vector2
	function __unm(self): Vector2
*/

LUALIB_API int (luaopen_rblx_math)(lua_State* L) {

	
	lua_newtable(L);
	int vec2Idx = lua_gettop(L);
	lua_pushvalue(L, vec2Idx);
	lua_setglobal(L, "Vector2");

	lua_pushcfunction(L, vector2_new,"vec2New");
	lua_setfield(L, -2, "new");

	luaL_newmetatable(L, "Vector2Meta");
	lua_pushstring(L, "__add");
	lua_pushcfunction(L, vector2_add, "vec2Add");
	lua_settable(L, -3);

	lua_pushstring(L, "__sub");
	lua_pushcfunction(L, vector2_sub, "vec2Sub");
	lua_settable(L, -3);

	lua_pushstring(L, "__mul");
	lua_pushcfunction(L, vector2_mul, "vec2Mul");
	lua_settable(L, -3);

	lua_pushstring(L, "__div");
	lua_pushcfunction(L, vector2_div, "vec2Div");
	lua_settable(L, -3);

	lua_pushstring(L, "__unm");
	lua_pushcfunction(L, vector2_neg, "vec2ToNeg");
	lua_settable(L, -3);


	return 1;
}