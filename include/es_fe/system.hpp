#pragma once
#include <es_la/dense.hpp>
#include "types.hpp"
#include "var.hpp"
#include "dof/dof_mapper.hpp"
#include "type_traits.hpp"
#include "solution_view.hpp"
#include <es_util/type_traits.hpp>
#include <es_util/function.hpp>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <string>
#include <tuple>
#include <type_traits>

namespace es_fe
{
template<class Var_list_, template<class> class T_Dof_mapper>
class System
{
public:
	using Var_list = Var_list_;

private:
	friend class Dof_tools<System>;

	using Traits = internal::Vars_traits<Var_list>;
	using Dof_mapper = T_Dof_mapper<Var_list>;

public:
	static constexpr std::size_t dim = Traits::space_dim;
	static constexpr std::size_t n_vars = Var_list::size;

	using Mesh = Mesh_t<dim>;

	template<std::size_t var>
	using Var_t = typename Var_list::template Var<var>;

	template<std::size_t var>
	using Var_dofs = typename Dof_mapper::template Var_dofs<var>;

	using Vars_dofs = typename Dof_mapper::Vars_dofs;

	using Dofs = std::conditional_t<n_vars == 1, Var_dofs<0>, Vars_dofs>;

	template<std::size_t var>
	using Var_vertex_dofs = typename Dof_mapper::template Var_vertex_dofs<var>;

	template<std::size_t var>
	using Var_edge_dofs = typename Dof_mapper::template Var_edge_dofs<var>;

	template<std::size_t var>
	using Solution_view_t = Solution_view<System, var>;

	template<std::size_t var, class Mesh_el_tag>
	using Solution_view_t2 = Solution_view2<System, var, Mesh_el_tag>;

public:
	using Edge_view = typename Mesh::Edge_view;
	using Cell_view = typename Mesh::Cell_view;

private:
	template<class, std::size_t>
	friend class Solution_view;
	template<class, std::size_t, class>
	friend class Solution_view2;

public:
	System(const Mesh& mesh) : mesh_(mesh)
	{}

	Dof_mapper& dof_mapper()
	{
		return dof_mapper_;
	}

	const Dof_mapper& dof_mapper() const
	{
		return dof_mapper_;
	}

	template<std::size_t var>
	Solution_view_t<var> solution_view() const
	{
		debug_check_var_index<var>();
		return {*this};
	}

	template<std::size_t var, class Mesh_el_tag>
	auto solution_view2() const
	{
		debug_check_var_index<var>();
		return Solution_view_t2<var, Mesh_el_tag>{*this};
	}

	//////////////////////////////////////////////////////////////////////////

	template<std::size_t var = 0, class Element_view, typename... Args>
	auto dofs(const Element_view& element, Args&&... args) const
	{
		debug_check_var_index<var>();
		return dof_mapper_.template dofs<var>(element, std::forward<Args>(args)...);
	}

	template<typename... Args>
	auto all_dofs(const Cell_view& cell, Args&&... args) const
	{
		return dof_mapper_.all_dofs(cell, std::forward<Args>(args)...);
	}

	// 	template<std::size_t var>
	// 	void vertex_dofs(mesh::Index vertex, Var_vertex_dofs<var>& dofs_list) const
	// 	{
	// 		dof_mapper_.template vertex_dofs<var>(vertex, dofs_list);
	// 	}

	// Dof_mapper& dof_mapper()
	//{
	//	return dof_mapper_;
	//}

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

	//////////////////////////////////////////////////////////////////////////

	template<std::size_t var = 0>
	auto& variable(es_util::Index<var> = es_util::Index<var>{})
	{
		debug_check_var_index<var>();
		return std::get<var>(vars_);
	}

	template<std::size_t var = 0>
	auto& variable(es_util::Index<var> = es_util::Index<var>{}) const
	{
		debug_check_var_index<var>();
		return std::get<var>(vars_);
	}

	const typename Var_list::Vars& variables() const
	{
		return vars_;
	}

	const Mesh& mesh() const
	{
		return mesh_;
	}

	template<class Symmetry_tag, class... Args>
	decltype(auto) sparsity_pattern(Args&&... args) const
	{
		return dof_mapper_.template sparsity_pattern<Symmetry_tag>(*this,
																   std::forward<Args>(args)...);
	}

	template<class Symmetry_tag, class... Args>
	decltype(auto) sparsity_pattern2(Args&&... args) const
	{
		return dof_mapper_.template sparsity_pattern2<Symmetry_tag>(*this,
																   std::forward<Args>(args)...);
	}

	// 	la::Vector_xd& raw_solution()
	// 	{
	// 		return solution_;
	// 	}
	//
	// 	const la::Vector_xd& raw_solution() const
	// 	{
	// 		return solution_;
	// 	}

	virtual std::string name() const = 0;

	std::size_t memory_size() const
	{
		return dof_mapper_.memory_size();
	}

	template<class... Args>
	void init(Args&&... args)
	{
		dof_mapper_.init(*this, std::forward<Args>(args)...);
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

	template<std::size_t var>
	static void debug_check_var_index()
	{
		static_assert(var < n_vars, "Variable index out of bounds");
	}

protected:
	const Mesh& mesh_;
	Dof_mapper dof_mapper_;

private:
	typename Var_list::Vars vars_;
};

template<class Var_list, template<class> class T_Dof_mapper>
std::ostream& operator<<(std::ostream& out, const System<Var_list, T_Dof_mapper>& system)
{
	out << system.name() << '\n'
		<< "Number of variables: " << system.n_vars << '\n'
		<< "DoFs (free/constrained/total): " << system.n_free_dofs() << '/' << system.n_const_dofs()
		<< '/' << system.n_dofs() << '\n'
		<< "Memory: " << es::util::size_string(system.memory_size()) << '\n';

	return out;
}
} // namespace es_fe