#pragma once
#include "element_base.hpp"
#include "../types.hpp"
#include <es_la/dense.hpp>
#include <es/math/function.hpp>
//#include "math/la/Vector.h"
//#include "Util/Array.h"
#include <es_util/type_traits.hpp>
#include <cstddef>
#include <utility>
#include <array>

namespace es_fe
{
class Edge_el : public internal::Triangular_element<0, 1, 0>
{};

class Exp_df_el : public internal::Triangular_element<1, 0, 0>
{
public:
};

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/
} // namespace es_fe