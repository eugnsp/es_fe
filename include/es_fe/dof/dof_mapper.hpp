#pragma once
#include "../types.hpp"
#include "dof_index.hpp"
#include "../type_traits.hpp"
#include "../util/mesh_vars_map.hpp"
#include <es_la/sparse/sparsity_pattern.hpp>
#include <es_fe/util/algorithm.hpp>
#include <es_fe/dof/dof_mapper_base.hpp>
#include <es_util/type_traits.hpp>

#include <algorithm>
#include <functional>
#include <cassert>
#include <cstddef>
#include <tuple>
#include <type_traits>

namespace es_fe
{

template<class Var_list>
class Dof_mapper : public internal::Dof_mapper_base<Var_list>
{
private:
	static constexpr std::size_t n_vars = Var_list::size;

	using Base = internal::Dof_mapper_base<Var_list>;

	using My_system = System<Var_list, es_fe::Dof_mapper>;
	using Mesh = typename My_system::Mesh;

	using Halfedge_view = typename Mesh::Halfedge_view;
	using Edge_view = typename Mesh::Edge_view;
	using Cell_view = typename Mesh::Cell_view;

	template<std::size_t var>
	using Var = typename Var_list::template Nth<var>;

public:
	template<std::size_t vi>
	using Var_dofs = typename Base::template Var_dofs<vi>;

	using typename Base::Vars_dofs;

	template<std::size_t vi>
	using Var_vertex_dofs = typename Base::template Var_vertex_dofs<vi>;

	template<std::size_t vi>
	using Var_edge_dofs = typename Base::template Var_edge_dofs<vi>;

public:
	void init(const My_system& system)
	{
		this->indices_.init_storage(system.mesh());

		compute_n_dofs(system);
		mark_const_dofs(system);
		assign_indices(system);

		assert(this->indices_.all_of(std::mem_fn(&Dof_index::is_valid)));
	}

	// 	template<std::size_t var>
	// 	Var_edge_dofs<var> dofs(const Halfedge_view& edge) const
	// 	{
	// 		Var_edge_dofs<var> dofs;
	// 		var_edge_dofs<var>(*edge, dofs);
	// 		return dofs;
	// 	}

	template<std::size_t var>
	Var_dofs<var> dofs(const Cell_view& cell, Halfedge_index first_halfedge) const
	{
		Var_dofs<var> dofs;
		var_dofs<var>(cell, first_halfedge, dofs);
		return dofs;
	}

	template<std::size_t var>
	Var_dofs<var> dofs(const Cell_view& cell) const
	{
		Var_dofs<var> dofs;
		var_dofs<var>(cell, dofs);
		return dofs;
	}

	Vars_dofs all_dofs(const Cell_view& cell) const
	{
		Vars_dofs dofs_list;
		dofs2(cell, dofs_list);
		return dofs_list;
	}

	template<std::size_t var>
	void vertex_dofs(Vertex_index vertex, Var_vertex_dofs<var>& dofs_list) const
	{
		var_vertex_dofs<var>(vertex, dofs_list);
	}

	template<std::size_t var>
	void edge_dofs(Edge_index edge, Var_edge_dofs<var>& dofs_list) const
	{
		var_edge_dofs<var>(edge, dofs_list);
	}

	template<class Symmetry_tag>
	la::Sparsity_pattern<Symmetry_tag> sparsity_pattern(const My_system& system) const
	{
		la::Sparsity_pattern<Symmetry_tag> pattern(this->n_free_dofs_);

		std::vector<std::size_t> indices, cols;
		for (auto& cell : system.mesh().faces())
		{
			const auto dofs_list = all_dofs(cell);

			// TODO: variable.dim()
			indices.clear();

			for_each_var<Var_list>([&](auto var) {
				const auto& dofs = std::get<var>(dofs_list);
				const auto n_dofs = static_cast<Local_index>(dofs.size());
				for (Local_index i = 0; i < n_dofs; ++i)
					if (dofs[i].is_free)
						for (Index d = 0; d < system.variable(var).dim(); ++d)
							indices.push_back(dofs[i].index + d * n_dofs);
			});

			for (std::size_t i = 0; i < indices.size(); ++i)
			{
				cols.clear();
				for (std::size_t j = 0; j < indices.size(); ++j)
				{
					if constexpr (std::is_same_v<Symmetry_tag, la::Symmetric_upper>)
					{
						if (indices[j] >= indices[i])
							cols.push_back(indices[j]);
					}
					else
						cols.push_back(indices[j]);
				}

				std::sort(cols.begin(), cols.end());
				pattern.insert(indices[i], cols.begin(), cols.end());
			}
		}

#ifndef NDEBUG
		for (std::size_t r = 0; r < pattern.n_rows(); ++r)
		{
			auto& row = pattern.row(r);
			if (row.empty())
				throw std::runtime_error("Row " + std::to_string(r) + " is empty");
		}

		assert(pattern.is_each_row_not_empty());
		auto err = pattern.check();
		if (err)
			throw std::runtime_error(err.message());
#endif

		return pattern;
	}

private:
	template<std::size_t var>
	void var_dofs(const Cell_view& cell, Var_dofs<var>& dofs) const
	{
		typename Cell_view::Vertex_indices vertices;
		typename Cell_view::Halfedge_indices halfedges;
		cell.get_indices(vertices, halfedges);

		var_dofs_impl<var>(vertices, halfedges, *cell, dofs);
	}

	template<std::size_t var>
	void var_dofs(const Cell_view& cell, Halfedge_index first_halfedge, Var_dofs<var>& dofs) const
	{
		typename Cell_view::Vertex_indices vertices;
		typename Cell_view::Halfedge_indices halfedges;
		cell.get_indices(first_halfedge, vertices, halfedges);

		var_dofs_impl<var>(vertices, halfedges, *cell, dofs);
	}

	template<class Dofs>
	void dofs2(const Cell_view& cell, Dofs& dofs) const
	{
		dofs_impl(cell, dofs, std::make_index_sequence<n_vars>{});
	}

	template<std::size_t var, class Dofs>
	void var_vertex_dofs(Vertex_index vertex, Dofs& dofs) const
	{
		static_assert(Var<var>::Element::has_vertex_dofs);

		const Dof_index& first_dof = this->indices_.at(vertex, Var_index<var>{});
		for (Local_index i = 0; i < dofs.size(); ++i)
			dofs[i] = first_dof + i;
	}

	template<std::size_t var, class Dofs>
	void var_edge_dofs(Edge_index edge, Dofs& dofs) const
	{
		static_assert(Var<var>::Element::has_edge_dofs);

		const Dof_index& first_dof = this->indices_.at(edge, Var_index<var>{});
		for (std::size_t i = 0; i < dofs.size(); ++i)
			dofs[i] = first_dof + i;
	}

	void compute_n_dofs(const My_system& system)
	{
		this->n_dofs_ = 0;
		for_each_var_element<Var_list>([this, &system](auto var, auto element_tag) {
			this->n_dofs_ +=
				system.variable(var).n_dofs(element_tag) * *system.mesh().n_elements(element_tag);
		});
	}

	void mark_const_dofs(const My_system& system)
	{
		this->n_free_dofs_ = this->n_dofs_;
		for_each_var<Var_list>([this, &system](auto var) {
			const auto& v = system.variable(var);
			v.for_each_ess_bnd_cond([this, &v, &var](const auto& bc) {
				using Element = typename Var_by_var_index<Var_list, decltype(var)>::Element;
				if constexpr (Element::has_vertex_dofs)
					for (Vertex_index vertex : bc.vertices())
					{
						Dof_index& dof = this->indices_.at(vertex, var);
						assert(dof.is_free);

						this->n_free_dofs_ -= v.n_dofs(Vertex_tag{});
						dof.is_free = false;
					}

				// 					if constexpr (std::is_same_v<Mesh, mesh::Mesh2>)
				if constexpr (Element::has_edge_dofs)
					for (Halfedge_index halfedge : bc.halfedges())
					{
						Dof_index& dof = this->indices_.at(edge(halfedge), var);
						assert(dof.is_free);

						this->n_free_dofs_ -= v.n_dofs(Edge_tag{});
						dof.is_free = false;
					}
			});
		});
	}

	void assign_indices(const My_system& system)
	{
		Index free_index = 0;
		Index const_index = this->n_free_dofs_;

		for_each_var_element<Var_list>(
			[this, &system, &free_index, &const_index](auto var, auto element_tag)
			{
				using Element_index = internal::Element_index_by_tag<decltype(element_tag)>;
				for (Element_index ei{}; ei < system.mesh().n_elements(element_tag); ++ei)
				{
					Dof_index& dof = this->indices_.at(ei, var);

					auto& index = dof.is_free ? free_index : const_index;
					dof.index = index;
					index += system.variable(var).n_dofs(element_tag);
				}
			});
	}

	template<class Dofs, std::size_t... vars>
	void dofs_impl(
		const typename Mesh::Cell_view& cell, Dofs& dofs, std::index_sequence<vars...>) const
	{
		typename Cell_view::Vertex_indices vertices;
		typename Cell_view::Halfedge_indices halfedges;
		cell.get_indices(vertices, halfedges);

		(var_dofs_impl<vars>(vertices, halfedges, *cell, std::get<vars>(dofs)), ...);
	}

	template<std::size_t var, class Dofs>
	void var_dofs_impl(
		[[maybe_unused]] const typename Cell_view::Vertex_indices& vertices,
		[[maybe_unused]] const typename Cell_view::Halfedge_indices& halfedges,
		[[maybe_unused]] Face_index cell,
		Dofs& dofs) const
	{
		using Element = typename Var<var>::Element;

		// TODO : replace indexing with iterator (when iterators are ready in la::)
		std::size_t i = 0;
		auto first_degree_dofs = dofs.template col<0>();
		if constexpr (Element::has_vertex_dofs)
			for (const auto vertex : vertices)
				var_dofs_impl2<var>(vertex, first_degree_dofs, i);

		if constexpr (Element::has_edge_dofs)
			for (const auto& halfedge : halfedges)
				var_dofs_impl2<var>(
					edge(halfedge), first_degree_dofs, i, is_first_halfedge(halfedge));

		if constexpr (Element::has_face_dofs)
			var_dofs_impl2<var>(cell, first_degree_dofs, i);

		assert(i == dofs.size());
	}

	template<std::size_t var, class Element_index, class Dofs>
	void var_dofs_impl2(
		Element_index element, Dofs& dofs, std::size_t& i, bool reversed = false) const
	{
		using Element = typename Var<var>::Element;

		const Dof_index& first_dof = this->indices_.at(element, Var_index<var>{});

		constexpr auto n = Element::n_dofs(internal::Element_tag_by_index<Element_index>{});
		for (Local_index k = 0; k < n; ++k)
			dofs[i++] = first_dof + (reversed ? n - k - 1 : k);
	}
};
} // namespace es_fe
