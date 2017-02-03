#pragma once
#ifndef BE_CORE_LUA_ID_HPP_
#define BE_CORE_LUA_ID_HPP_

#include "lua_core_autolink.hpp"
#include <be/core/id.hpp>
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be {
namespace belua {

///////////////////////////////////////////////////////////////////////////////
int open_id(lua_State* L);

extern const luaL_Reg id_module;

} // be::belua
} // be

#endif
