#pragma once
#include <es_fe/types.hpp>
#include <es_fe/type_traits.hpp>
#include <es_fe/var_list.hpp>
#include <es_util/array.hpp>
#include <es_util/algorithm.hpp>
#include <array>
#include <algorithm>
#include <cstddef>
#include <vector>

namespace es_fe::internal
{
// A class that maps a mesh element index and a variable index
// into an element of given type:
// (Element_index) x (Var_index) -> (T)
template<typename T, class Element_index, class... Vars>
class Mesh_element_var_map_impl
{
private:
	using Element_tag = Element_tag_by_index<Element_index>;

public:
	template<class Mesh>
	void init_storage(const Mesh& mesh, Index n_layers)
	{
		values_.assign(n_layers * block_size() * *n_elements(mesh, Element_tag{}), T{});
	}

	template<std::size_t var>
	typename std::vector<T>::reference at(Element_index element, Var_index<var>, Index layer = 0, Index n_layers = 1)
	{
		return values_[layer + n_layers * pos<var>(element)];
	}

	template<std::size_t var>
	typename std::vector<T>::const_reference at(Element_index element, Var_index<var>, Index layer = 0, Index n_layers = 1) const
	{
		return values_[layer + n_layers * pos<var>(element)];
	}

// 	template<std::size_t var>
// 	T sum(Var_index<var>) const
// 	{
// 		T s{};
// 		for (std::size_t i = pos<var>(0); i < values_.size(); i += block_size())
// 			s += values_[i];
// 		return s;
// 	}

	template<class Pred>
	bool all_of(Pred pred) const
	{
		return std::all_of(values_.begin(), values_.end(), pred);
	}

	std::size_t memory_size() const
	{
		return values_.capacity() * sizeof(T);
	}

private:
	static constexpr auto has_dof_flags = 
		es_util::make_array(Vars::Element::has_dofs(Element_tag{})...);

	template<std::size_t var>
	static constexpr std::size_t pos(Element_index element)
	{
		static_assert(has_dof_flags[var], "Variable has no DoF of this type");

		constexpr auto offset = es_util::array_sum_n<var, std::size_t>(has_dof_flags, 0);
		return offset + (*element) * block_size();
	}

	static constexpr std::size_t block_size()
	{
		return es_util::array_sum<std::size_t>(has_dof_flags, 0);
	}

private:
	std::vector<T> values_;
};

template<typename T, class Element_index, class Var_list>
class Mesh_element_var_map;

template<typename T, class Element_index, class... Vars>
class Mesh_element_var_map<T, Element_index, Var_list<Vars...>>
	: public Mesh_element_var_map_impl<T, Element_index, Vars...> { };
}
