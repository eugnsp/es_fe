#pragma once
#include <es_util/type_traits.hpp>

#include <cstddef>
#include <tuple>

namespace es_fe
{
// A variables list class used to represent a collection of variables as a single type
template<class... Variables>
struct Var_list
{
	static_assert(es_util::all_same<Variables::space_dim...>);

	// Returns the number of variables in the list
	static constexpr std::size_t size = sizeof...(Variables);

	// Returns the space dimension of variables
	static constexpr auto space_dim = es_util::Head<Variables...>::space_dim;

	// Returns the type of a variable in the list with the given index
	template<std::size_t var>
	using Nth = es_util::Nth<var, Variables...>;

	template<template<class Variable> class Fn>
	using Tuple_map = std::tuple<Fn<Variables>...>;

	// Returns the list of variables as an `std::tuple`
	using Tuple = Tuple_map<es_util::Identity>;
};
} // namespace es_fe
