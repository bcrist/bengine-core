#pragma once
#ifndef BE_CORE_EXTENTS_HPP_
#define BE_CORE_EXTENTS_HPP_

#include "generic_extents.hpp"

namespace be {

typedef generic_extents<F32> range;
typedef generic_extents<F64> drange;
typedef generic_extents<I32> irange;
typedef generic_extents<vec2> rect;
typedef generic_extents<dvec2> drect;
typedef generic_extents<ivec2> irect;
typedef generic_extents<vec3> box;
typedef generic_extents<dvec3> dbox;
typedef generic_extents<ivec3> ibox;

} // be

#endif
