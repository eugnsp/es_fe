#pragma once
#include <es_fe/matrix_based/solution_base.hpp>

#include <es_la/dense.hpp>

namespace es_fe
{
template<class System, typename Value>
class Solution_view : public internal::Solution_base<System, const es_la::Vector_x<Value>&>
{
private:
	using Vector = es_la::Vector_x<Value>;
	using Base = internal::Solution_base<System, const Vector&>;

public:
	using Base::Base;
};
} // namespace es_fe
