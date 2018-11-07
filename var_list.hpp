#pragma once
#include <es_util/type_traits.hpp>

namespace fe
{
template<class... Variables>
struct Var_list
{
	static constexpr std::size_t size = sizeof...(Variables);

	template<std::size_t var>
	using Var = es_util::Nth<var, Variables...>;

	using Vars = std::tuple<Variables...>;
};

template<std::size_t var, class Var_list> 
using Nth_var_t = typename Var_list::template Nth_var_t<var>;

}
