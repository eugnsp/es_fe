#pragma once
#include "../types.hpp"
#include "dof_index.hpp"
#include "../type_traits.hpp"
#include "../util/mesh_vars_map.hpp"
#include <es_la/sparse/sparsity_pattern.hpp>
#include <es_fe/util/algorithm.hpp>
#include <es_fe/dof/dof_mapper_base.hpp>
#include <es_util/type_traits.hpp>
#include <es_la/dense.hpp>
#include <es_fe/mesh/base.hpp>
#include <algorithm>
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
	using Base = internal::Dof_mapper_base<Var_list>;

	using typename Base::Mesh;
	using typename Base::Traits;

	using Halfedge_view = typename Mesh::Halfedge_view;
	using Edge_view = typename Mesh::Edge_view;
	using Cell_view = typename Mesh::Cell_view;

	using System = System<Var_list, es_fe::Dof_mapper>;

	static constexpr std::size_t n_vars = Traits::n_vars;

public:
	template<std::size_t var>
	using Var_t = typename Var_list::template Var<var>;

	template<std::size_t var>
	using Var_dofs = la::Vector<Dof_index, Var_t<var>::Element::n_total_cell_dofs>;

	// Use Matrix<..., 1> instead of Vector<...> due to MSVC bug
	using Vars_dofs = typename internal::Dof_mapper_traits<Var_list>::Vars_dofs;

	template<std::size_t var>
	using Var_vertex_dofs = la::Vector<Dof_index, Var_t<var>::Element::n_vertex_dofs>;

	template<std::size_t var>
	using Var_edge_dofs = la::Vector<Dof_index, Var_t<var>::Element::n_edge_dofs>;

public:
	void init(const System& system)
	{
		indices_.init_storage(system.mesh());

		compute_n_dofs(system);
		mark_const_dofs(system);
		assign_indices(system);

		assert(indices_.all_of([](auto& dof_index) { return dof_index.is_valid(); }));
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
	la::Sparsity_pattern<Symmetry_tag> sparsity_pattern(const System& system) const
	{
		la::Sparsity_pattern<Symmetry_tag> pattern(n_free_dofs_);

		std::vector<std::size_t> indices, cols;
		for (auto& cell : system.mesh().faces())
		{
			const auto dofs_list = all_dofs(cell);

			// TODO: variable.dim()
			indices.clear();

			for_each_var<Var_list>([&](auto var) {
				const auto& dofs = std::get<var>(dofs_list);
				for (std::size_t i = 0; i < dofs.size(); ++i)
					if (dofs[i].is_free)
						for (Index d = 0; d < system.template variable<var>().dim(); ++d)
							indices.push_back(dofs[i].index + d);
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
		assert(pattern.is_each_row_not_empty());
		auto err = pattern.check();
		if (!err.is_OK())
			throw std::runtime_error(err.string());
#endif

		return pattern;
	}

private:
	template<std::size_t var, class Dofs>
	void var_dofs(const Cell_view& cell, Dofs& dofs) const
	{
		typename Cell_view::Vertex_indices vertices;
		typename Cell_view::Halfedge_indices halfedges;
		cell.get_indices(vertices, halfedges);

		var_dofs_impl<var>(vertices, halfedges, *cell, dofs);
	}

	template<std::size_t var, class Dofs>
	void var_dofs(const Cell_view& cell, Halfedge_index first_halfedge, Dofs& dofs) const
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
		static_assert(Var_t<var>::Element::has_vertex_dofs, "Variable has no DoF of this type");

		const Dof_index& first_dof = indices_.at(vertex, Var_index<var>{});

		// resize?
		for (std::size_t j = 0; j < dofs.cols(); ++j)
			for (std::size_t i = 0; i < dofs.rows(); ++i)
				dofs(i, j) = first_dof + static_cast<Local_index>(i + j * dofs.rows());
	}

	template<std::size_t var, class Dofs>
	void var_edge_dofs(Edge_index edge, Dofs& dofs) const
	{
		static_assert(Var_t<var>::Element::has_dof(Edge_tag{}), "Variable has no DoF of this type");

		const Dof_index& first_dof = indices_.at(edge, Var_index<var>{});

		// resize?
		for (std::size_t j = 0; j < dofs.cols(); ++j)
			for (std::size_t i = 0; i < dofs.rows(); ++i)
				dofs(i, j) = first_dof + static_cast<Local_index>(i + j * dofs.rows());
	}

	void compute_n_dofs(const System& system)
	{
		n_dofs_ = 0;
		for_each_var_element<Var_list>([this, &system](auto var, auto element_tag) {
			n_dofs_ +=
				system.variable(var).n_dofs(element_tag) * *n_elements(system.mesh(), element_tag);
		});
	}

	void mark_const_dofs(const System& system)
	{
		n_free_dofs_ = n_dofs_;
		for_each_var<Var_list>([this, &system](auto var) {
			const auto& v = system.variable(var);
			v.for_each_ess_bnd_cond([this, &v, &var](const auto& bc) {
				using Element = typename Var_by_var_index<Var_list, decltype(var)>::Element;
				if constexpr (Element::has_vertex_dofs)
					for (auto vertex = bc.begin_vertex(); vertex != bc.end_vertex(); ++vertex)
					{
						Dof_index& dof = indices_.at(*vertex, var);
						assert(dof.is_free);

						n_free_dofs_ -= v.n_dofs(Vertex_tag{});
						dof.is_free = false;
					}

				// 					if constexpr (std::is_same_v<Mesh, mesh::Mesh2>)
				if constexpr (Element::has_edge_dofs)
					for (auto edge = bc.begin_edge(); edge != bc.end_edge(); ++edge)
					{
						Dof_index& dof = indices_.at(*edge, var);
						assert(dof.is_free);

						n_free_dofs_ -= v.n_dofs(Edge_tag{});
						dof.is_free = false;
					}
			});
		});
	}

	void assign_indices(const System& system)
	{
		Index free_index = 0;
		Index const_index = this->n_free_dofs_;

		for_each_var_element<Var_list>(
			[this, &system, &free_index, &const_index](auto var, auto element_tag) {
				using Element_index = internal::Element_index_by_tag<decltype(element_tag)>;
				for (Element_index element{}; element < n_elements(system.mesh(), element_tag);
					 ++element)
				{
					Dof_index& dof = indices_.at(element, var);

					auto& index = dof.is_free ? free_index : const_index;
					dof.index = index;
					index += system.variable(var).n_dofs(element_tag);
				}
			});
	}

	template<class Dofs, std::size_t... vars>
	void dofs_impl(const typename Mesh::Cell_view& cell, Dofs& dofs,
				   std::index_sequence<vars...>) const
	{
		typename Cell_view::Vertex_indices vertices;
		typename Cell_view::Halfedge_indices halfedges;
		cell.get_indices(vertices, halfedges);

		(var_dofs_impl<vars>(vertices, halfedges, *cell, std::get<vars>(dofs)), ...);
	}

	// template<std::size_t var, class Dofs>
	// void var_dofs_impl(
	//	const typename Cell_view::Vertex_indices& vertices,
	//	const typename Cell_view::Edge_with_dir_indices& edges,
	//	Face_index cell, Dofs& dofs) const
	//{
	//	using Var = Var_t<var>;

	//	// TODO : replace indexing with iterator (when iterators are ready in la::)
	//	std::size_t i = 0;
	//	auto first_degree_dofs = dofs.template col<0>();
	//	if constexpr (Var::has_dof(Vertex_tag{}))
	//		for (const auto vertex : vertices)
	//			var_dofs_impl2<var>(vertex, first_degree_dofs, i);

	//	if constexpr (Var::has_dof(Edge_tag{}))
	//		for (const auto& edge : edges)
	//			var_dofs_impl2<var>(edge.first, first_degree_dofs, i, edge.second);

	//	if constexpr (Var::has_dof(Face_tag{}))
	//		var_dofs_impl2<var>(cell, first_degree_dofs, i);

	//	assert(i == dofs.rows());

	//	for (Local_index j = 1; j < dofs.cols(); ++j)
	//		for (Local_index i = 0; i < dofs.rows(); ++i)
	//			dofs(i, j) = dofs(i, 0) + j;
	//}

	template<std::size_t var, class Dofs>
	void var_dofs_impl(const typename Cell_view::Vertex_indices& vertices,
					   const typename Cell_view::Halfedge_indices& halfedges, Face_index cell,
					   Dofs& dofs) const
	{
		using Var = Var_t<var>;

		// TODO : replace indexing with iterator (when iterators are ready in la::)
		std::size_t i = 0;
		auto first_degree_dofs = dofs.template col<0>();
		if constexpr (Var::Element::has_vertex_dofs)
			for (const auto vertex : vertices)
				var_dofs_impl2<var>(vertex, first_degree_dofs, i);

		if constexpr (Var::Element::has_edge_dofs)
			for (const auto& halfedge : halfedges)
				var_dofs_impl2<var>(edge(halfedge), first_degree_dofs, i,
									is_first_halfedge(halfedge));

		if constexpr (Var::Element::has_face_dofs)
			var_dofs_impl2<var>(cell, first_degree_dofs, i);

		assert(i == dofs.rows());

		for (Local_index j = 1; j < dofs.cols(); ++j)
			for (Local_index i = 0; i < dofs.rows(); ++i)
				dofs(i, j) = dofs(i, 0) + j;
	}

	template<std::size_t var, class Element_index, class Dofs>
	void var_dofs_impl2(Element_index element, Dofs& dofs, std::size_t& i,
						bool reversed = false) const
	{
		using Var = Var_t<var>;

		const Dof_index& first_dof = indices_.at(element, Var_index<var>{});

		// constexpr auto n = _System::template VarType<_index>::Element::dofs(_Tag{ });
		// for (LocalIndexType k = 0; k < n; ++k)
		//	*it++ = firstDofIndex + (!reversed ? k : static_cast<LocalIndexType>(n - k - 1));

		constexpr auto n = Var::Element::n_dofs(internal::Element_tag_by_index<Element_index>{});
		for (Local_index k = 0; k < n; ++k)
		{
			const auto j = reversed ? n - k - 1 : k;
			dofs[i++] =
				first_dof + static_cast<Local_index>(j) * Var::dim(); // HACK for static-dim vars
		}
	}

private:
	using Base::indices_;
	using Base::n_dofs_;
	using Base::n_free_dofs_;
};
} // namespace es_fe
