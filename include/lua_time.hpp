#pragma once
#ifndef BE_CORE_LUA_TIME_HPP_
#define BE_CORE_LUA_TIME_HPP_

#include "core_lua_autolink.hpp"
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
int open_time(lua_State* L);

extern const luaL_Reg time_module;

} // be::belua

#endif
