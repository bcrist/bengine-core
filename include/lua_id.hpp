#pragma once
#ifndef BE_CORE_LUA_ID_HPP_
#define BE_CORE_LUA_ID_HPP_

#include "core_lua_autolink.hpp"
#include <be/core/id.hpp>
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
int open_id(lua_State* L);

extern const luaL_Reg id_module;

} // be::belua

#endif
