#pragma once
#ifndef BE_CORE_LUA_LOGGING_HPP_
#define BE_CORE_LUA_LOGGING_HPP_

#include "core_lua_autolink.hpp"
#include <be/core/id.hpp>
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
int open_logging(lua_State* L);

extern const luaL_Reg logging_module;

} // be::belua

#endif
