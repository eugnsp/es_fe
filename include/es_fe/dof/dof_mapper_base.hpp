#pragma once
#include <es_fe/dof/dof_index.hpp>
#include <es_fe/type_traits.hpp>
#include <es_fe/types.hpp>
#include <es_fe/util/mesh_vars_map.hpp>

#include <es_la/dense.hpp>

#include <cstddef>
#include <tuple>

namespace es_fe::internal
{
template<class Var_list>
class Dof_mapper_base
{
private:
	template<std::size_t size>
	using Dof_index_vector = la::Vector<Dof_index, size>;

	template<class Var>
	using Vars_dofs_fn = Dof_index_vector<Var::Element::n_total_cell_dofs>;

	template<class Var>
	using Vars_vertex_dofs_fn = Dof_index_vector<Var::Element::n_vertex_dofs>;

	template<class Var>
	using Vars_edge_dofs_fn = Dof_index_vector<Var::Element::n_edge_dofs>;

	template<std::size_t vi>
	using Var = typename Var_list::template Nth<vi>;

	using Mesh = Mesh_t<Var_list::space_dim>;

public:
	template<std::size_t vi>
	using Var_dofs = Vars_dofs_fn<Var<vi>>;

	using Vars_dofs = typename Var_list::template Tuple_map<Vars_dofs_fn>;

	template<std::size_t vi>
	using Var_vertex_dofs = Dof_index_vector<Var<vi>::Element::n_vertex_dofs>;

	template<std::size_t vi>
	using Var_edge_dofs = Dof_index_vector<Var<vi>::Element::n_edge_dofs>;

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
} // namespace es_fe::internal
