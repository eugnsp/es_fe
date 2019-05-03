#pragma once
#include <es_fe/dof/dof_index.hpp>
#include <es_fe/dof/dof_mapper_util.hpp>
#include <es_fe/type_traits.hpp>
#include <es_fe/types.hpp>
#include <es_fe/util/algorithm.hpp>
#include <es_fe/util/mesh_vars_map.hpp>

#include <es_la/dense.hpp>

#include <cassert>
#include <cstddef>
#include <utility>

namespace es_fe::internal
{
template<class Var_list>
class Dof_mapper_base
{
private:
	//////////////////////////////////////////////////////////////////////
	//* DoFs */

	template<std::size_t size>
	using Dof_index_vector = es_la::Vector<Dof_index, size>;

	template<class Var>
	using Vars_dofs_fn = Dof_index_vector<Var::Element::n_total_cell_dofs>;

	template<class Var>
	using Vars_vertex_dofs_fn = Dof_index_vector<Var::Element::n_vertex_dofs>;

	template<class Var>
	using Vars_edge_dofs_fn = Dof_index_vector<Var::Element::n_edge_dofs>;

protected:
	template<std::size_t vi>
	using Var = typename Var_list::template Nth<vi>;

	using Mesh = es_fe::Mesh<Var_list::space_dim>;

public:
	template<std::size_t vi>
	using Var_dofs = Vars_dofs_fn<Var<vi>>;

	using Vars_dofs = typename Var_list::template Tuple_map<Vars_dofs_fn>;

	template<std::size_t vi>
	using Var_vertex_dofs = Dof_index_vector<Var<vi>::Element::n_vertex_dofs>;

	template<std::size_t vi>
	using Var_edge_dofs = Dof_index_vector<Var<vi>::Element::n_edge_dofs>;

public:
	template<class System>
	void init(const System& system)
	{
		indices_.init_storage(system.mesh());

		compute_n_dofs(system);
		mark_const_dofs(system);
		assign_indices(system);

		assert(indices_.all_of([](auto index) { return index.is_valid(); }));
	}

	template<class Symmetry_tag, class System>
	static es_la::Sparsity_pattern<Symmetry_tag> sparsity_pattern(const System& system)
	{
		return es_fe::internal::sparsity_pattern<Symmetry_tag>(system);
	}

	///////////////////////////////////////////////////////////////////////
	//* Capacity */

	es_fe::Index n_dofs() const
	{
		return n_dofs_;
	}

	es_fe::Index n_free_dofs() const
	{
		return n_free_dofs_;
	}

	std::size_t memory_size() const
	{
		return indices_.memory_size();
	}

private:
	////////////////////////////////////////////////////////////////////////
	//* Initialization */

	template<class System>
	void compute_n_dofs(const System& system)
	{
		n_dofs_ = 0;
		es_fe::for_each_var_element<Var_list>([this, &system](auto var, auto element_tag) {
			const auto n_dofs = system.variable(var).n_dofs(element_tag);
			const auto n_elements = *system.mesh().n_elements(element_tag);
			n_dofs_ += n_dofs * n_elements;
		});
	}

	template<class System>
	void mark_const_dofs(const System& system)
	{
		n_free_dofs_ = n_dofs_;

		es_fe::for_each_var<Var_list>([this, &system](auto var) {
			const auto& v = system.variable(var);
			v.for_each_ess_bnd_cond([this, &v, &var](const auto& bc) {
				using Element = typename Var_by_var_index<Var_list, decltype(var)>::Element;

				if constexpr (Element::has_vertex_dofs)
					for (es_fe::Vertex_index vertex : bc.vertices())
					{
						Dof_index& dof = indices_.at(vertex, var);
						assert(dof.is_free);

						n_free_dofs_ -= v.n_dofs(Vertex_tag{});
						dof.is_free = false;
					}

				// TODO : looks sloppy
				if constexpr (Var_list::space_dim == 2 && Element::has_edge_dofs)
					for (es_fe::Halfedge_index halfedge : bc.halfedges())
					{
						Dof_index& dof = indices_.at(edge(halfedge), var);
						assert(dof.is_free);

						n_free_dofs_ -= v.n_dofs(Edge_tag{});
						dof.is_free = false;
					}
			});
		});
	}

	template<class System>
	void assign_indices(const System& system)
	{
		es_fe::Index free_index = 0;
		es_fe::Index const_index = n_free_dofs_;

		for_each_var_element<Var_list>(
			[this, &system, &free_index, &const_index](auto var, auto element_tag) {
				using Element_index = es_fe::internal::Element_index_by_tag<decltype(element_tag)>;

				for (Element_index ei{}; ei < system.mesh().n_elements(element_tag); ++ei)
				{
					Dof_index& dof = indices_.at(ei, var);

					auto& index = dof.is_free ? free_index : const_index;
					dof.index = index;
					index += system.variable(var).n_dofs(element_tag);
				}
			});
	}

protected:
	es_fe::internal::Mesh_var_map<Mesh, Var_list, Dof_index> indices_;

	es_fe::Index n_dofs_ = 0;
	es_fe::Index n_free_dofs_ = 0;
};
} // namespace es_fe::internal
