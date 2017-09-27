#pragma once
#ifndef BE_CORE_GLM_HPP_
#define BE_CORE_GLM_HPP_

#include "enum_vec_fwd.hpp"
#include <glm/fwd.hpp>

namespace glm {

template<typename T, precision P = defaultp>
struct tdualquat;

} // glm
namespace be {

using vec1 = glm::highp_f32vec1;
using vec2 = glm::highp_f32vec2;
using vec3 = glm::highp_f32vec3;
using vec4 = glm::highp_f32vec4;

using dvec1 = glm::highp_f64vec1;
using dvec2 = glm::highp_f64vec2;
using dvec3 = glm::highp_f64vec3;
using dvec4 = glm::highp_f64vec4;

using ivec1 = glm::highp_i32vec1;
using ivec2 = glm::highp_i32vec2;
using ivec3 = glm::highp_i32vec3;
using ivec4 = glm::highp_i32vec4;

using uvec1 = glm::highp_u32vec1;
using uvec2 = glm::highp_u32vec2;
using uvec3 = glm::highp_u32vec3;
using uvec4 = glm::highp_u32vec4;

using bvec1 = glm::highp_bvec1_t;
using bvec2 = glm::highp_bvec2;
using bvec3 = glm::highp_bvec3;
using bvec4 = glm::highp_bvec4;

using quat = glm::highp_f32quat;
using dualquat = glm::tdualquat<glm::f32, glm::highp>;

using RGB  = glm::highp_u8vec3;
using RGBA = glm::highp_u8vec4;

using mat2   = glm::highp_f32mat2x2;
using mat2x3 = glm::highp_f32mat2x3;
using mat2x4 = glm::highp_f32mat2x4;
using mat3x2 = glm::highp_f32mat3x2;
using mat3   = glm::highp_f32mat3x3;
using mat3x4 = glm::highp_f32mat3x4;
using mat4x2 = glm::highp_f32mat4x2;
using mat4x3 = glm::highp_f32mat4x3;
using mat4   = glm::highp_f32mat4x4;

using dmat2   = glm::highp_f64mat2x2;
using dmat2x3 = glm::highp_f64mat2x3;
using dmat2x4 = glm::highp_f64mat2x4;
using dmat3x2 = glm::highp_f64mat3x2;
using dmat3   = glm::highp_f64mat3x3;
using dmat3x4 = glm::highp_f64mat3x4;
using dmat4x2 = glm::highp_f64mat4x2;
using dmat4x3 = glm::highp_f64mat4x3;
using dmat4   = glm::highp_f64mat4x4;

} // be

#endif
