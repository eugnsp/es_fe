#pragma once
#include <es_la/dense.hpp>
#include <es/fe/types.hpp>
#include <es/fe/type_traits.hpp>
#include <es/fe/mesh/base.hpp>
#include <cstddef>
#include <tuple>

namespace fe::internal
{
template<class Var_list>
struct Dof_mapper_traits;

template<class... Vars>
struct Dof_mapper_traits<Var_list<Vars...>>
{
private:
	// TODO : make public ?
	template<std::size_t size>
	using Dof_index_vector = la::Vector<Dof_index, size>;

public:
	// TODO
	using Vars_dofs = std::tuple<Dof_index_vector<Vars::Element::n_total_cell_dofs>...>;
};

template<class Var_list>
class Dof_mapper_base
{
protected:
	using Traits = internal::Vars_traits<Var_list>;
	using Mesh = Mesh_t<Traits::space_dim>;
	using Cell_view = typename Mesh::Cell_view;

public:
	Index n_dofs() const
	{
		return n_dofs_;
	}

	Index n_free_dofs() const
	{
		return n_free_dofs_;
	}

	std::size_t memory_size() const
	{
		return indices_.memory_size();
	}

protected:
	internal::Mesh_var_map<Mesh, Var_list, Dof_index> indices_;

	Index n_dofs_ = 0;
	Index n_free_dofs_ = 0;
};
}