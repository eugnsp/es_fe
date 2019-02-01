#pragma once
#include <es_fe/type_traits.hpp>
#include <es_fe/types.hpp>
#include <es_fe/dof/dof_mapper.hpp>

#include <es_la/dense.hpp>
#include <es_util/string.hpp>
#include <es_util/type_traits.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace es_fe
{
template<class Var_list_, template<class> class Dof_mapper_>
class System
{
private:
	using Var_list = Var_list_;
	using My_dof_mapper = Dof_mapper_<Var_list>;

public:
	static constexpr std::size_t space_dim = Var_list::space_dim;
	static constexpr std::size_t n_vars = Var_list::size;

public:
	using Mesh = internal::Mesh_t<space_dim>;

	template<std::size_t var>
	using Var_t = typename Var_list::template Nth<var>;

	template<std::size_t var>
	using Var_dofs = typename My_dof_mapper::template Var_dofs<var>;

	using Vars_dofs = typename My_dof_mapper::Vars_dofs;

	using Dofs = std::conditional_t<n_vars == 1, Var_dofs<0>, Vars_dofs>;

	template<std::size_t var>
	using Var_vertex_dofs = typename My_dof_mapper::template Var_vertex_dofs<var>;

	template<std::size_t var>
	using Var_edge_dofs = typename My_dof_mapper::template Var_edge_dofs<var>;

public:
	System(const Mesh& mesh) : mesh_(mesh)
	{}

	template<class... Args>
	void init(Args&&... args)
	{
		dof_mapper_.init(*this, std::forward<Args>(args)...);
	}

	//////////////////////////////////////////////////////////////////////
	//* Degrees of freedom */

	Index n_dofs() const
	{
		return dof_mapper_.n_dofs();
	}

	Index n_free_dofs() const
	{
		return dof_mapper_.n_free_dofs();
	}

	Index n_const_dofs() const
	{
		return n_dofs() - n_free_dofs();
	}

	const My_dof_mapper& dof_mapper() const
	{
		return dof_mapper_;
	}

	template<std::size_t var_index = 0, class Element_view, typename... Args>
	auto dofs(const Element_view& element, Args&&... args) const
	{
		debug_check_var_index<var_index>();
		return dof_mapper_.template dofs<var_index>(element, std::forward<Args>(args)...);
	}

	template<typename... Args>
	auto all_dofs(const typename Mesh::Cell_view& cell, Args&&... args) const
	{
		return dof_mapper_.all_dofs(cell, std::forward<Args>(args)...);
	}

	// 	template<std::size_t var>
	// 	void vertex_dofs(mesh::Index vertex, Var_vertex_dofs<var>& dofs_list) const
	// 	{
	// 		dof_mapper_.template vertex_dofs<var>(vertex, dofs_list);
	// 	}

	//////////////////////////////////////////////////////////////////////
	//* Variables */

	template<std::size_t var = 0>
	auto& variable(es_util::Index<var> = {})
	{
		debug_check_var_index<var>();
		return std::get<var>(vars_);
	}

	template<std::size_t var = 0>
	auto& variable(es_util::Index<var> = {}) const
	{
		debug_check_var_index<var>();
		return std::get<var>(vars_);
	}

	auto& variables() const
	{
		return vars_;
	}

	///////////////////////////////////////////////////////////////////////

	const Mesh& mesh() const
	{
		return mesh_;
	}

	template<class Symmetry_tag, class... Args>
	decltype(auto) sparsity_pattern(Args&&... args) const
	{
		return dof_mapper_.template sparsity_pattern<Symmetry_tag>(
			*this, std::forward<Args>(args)...);
	}

	template<class Symmetry_tag, class... Args>
	decltype(auto) sparsity_pattern2(Args&&... args) const
	{
		return dof_mapper_.template sparsity_pattern2<Symmetry_tag>(
			*this, std::forward<Args>(args)...);
	}

	virtual std::string name() const
	{
		return "Unnamed system";
	}

	std::size_t memory_size() const
	{
		return dof_mapper_.memory_size();
	}

private:
	// 	template<std::size_t var = 0>
	// 	void set_constraints()
	// 	{
	// 		using Var = Var_t<var>;
	//
	// 		if constexpr (Var::has_bnd_cond)
	// 		{
	// 			const Var& v = variable<var>();
	// 			for (auto& bc : v.bnd_conds())
	// 			{
	// 				if constexpr (Var::has_dof(Vertex_tag{}))
	// 					set_constraints_at_vertices<var>(bc);
	//
	// 				if constexpr (Var::has_dof(Edge_tag{}))
	// 					set_constraints_at_edges<var>(bc);
	// 			}
	// 		}
	//
	// 		if constexpr (var + 1 < n_vars)
	// 			set_constraints<var + 1>();
	// 	}

	// 	template<std::size_t var, class Bnd_cond>
	// 	void set_constraints_at_vertices(const Bnd_cond& bc)
	// 	{
	// 		static_assert(Var_t<var>::has_dof(Vertex_tag{}), "Variable has no DoFs at vertices");
	//
	// // 		Var_vertex_dofs<var> vertex_dofs;
	// // 	//	la::Matrix_x<Index> vertex_dof_indices;		// TODO : static size
	// //
	// // 		for (auto vertex = bc->begin_vertex(); vertex != bc->end_vertex(); ++vertex)
	// // 		{
	// // 			dof_mapper_.template vertex_dofs<var>(*vertex, vertex_dofs);
	// // 			//vertex_dof_indices.resize(vertex_dofs.rows(), vertex_dofs.cols());
	// // 			for (std::size_t j = 0; j < vertex_dofs.cols(); ++j)
	// // 				for (std::size_t i = 0; i < vertex_dofs.rows(); ++i)
	// // 				{
	// // 					assert(vertex_dofs(i, j).is_free == false);
	// // //					vertex_dof_indices(i, j) = vertex_dofs(i, j).index;
	// // 				}
	// //
	// // 			//bc->set_values_at_vertex(*vertex, solution_.view(vertex_dof_indices));
	// // 			bc->set_values_at_vertex(*vertex, solution_[vertex_dofs(0, 0).index]);
	// // 		}
	// 	}
	//
	// 	template<std::size_t var, class Bnd_cond>
	// 	void set_constraints_at_edges(const Bnd_cond& bc)
	// 	{
	// 		static_assert(Var_t<var>::has_dof(Edge_tag{}), "Variable has no DoFs at edges");
	// //		throw;
	//
	// 		// TODO : Edge direction???
	// // 				for (auto edge = bc->begin_edge(); edge != bc->end_edge(); ++edge)
	// // 				{
	// // 					Dof_index& dof = indices_.at(Edge_tag{}, Var_index<t_var>{}, *edge);
	// // 					assert(!dof.is_free);
	// //
	// // 					// TODO : submatrix in v.degree index and n_dofs(Edge_tag{})
	// // 					bc->get_values_at_edge(*edge, solution_[dof.index]);
	// // 				}
	//
	// 	}

	template<std::size_t var_index>
	static void debug_check_var_index()
	{
		static_assert(var_index < n_vars, "Variable index out of bounds");
	}

private:
	My_dof_mapper dof_mapper_;
	typename Var_list::Tuple vars_;
	const Mesh& mesh_;
};

template<class Var_list, template<class> class Dof_mapper>
std::ostream& operator<<(std::ostream& out, const System<Var_list, Dof_mapper>& system)
{
	out << system.name() << '\n'
		<< "Number of variables: " << system.n_vars << '\n'
		<< "DoFs (free/constrained/total): " << system.n_free_dofs() << '/' << system.n_const_dofs()
		<< '/' << system.n_dofs() << '\n'
		<< "Memory: " << es_util::size_string(system.memory_size()) << '\n';

	return out;
}
} // namespace es_fe
