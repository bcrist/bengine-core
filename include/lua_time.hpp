#pragma once
#ifndef BE_CORE_LUA_TIME_HPP_
#define BE_CORE_LUA_TIME_HPP_

#include "lua_core_autolink.hpp"
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be {
namespace belua {

///////////////////////////////////////////////////////////////////////////////
int open_time(lua_State* L);

extern const luaL_Reg time_module;

} // be::belua
} // be

#endif
