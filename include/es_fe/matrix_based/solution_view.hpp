#pragma once
#include <es_fe/types.hpp>
#include <es_fe/math/jacobian.hpp>
#include <es_fe/geom/rect.hpp>
#include <es_fe/geom/point.hpp>

#include <es_la/base/expression.hpp>
#include <es_util/type_traits.hpp>

#include <cstddef>
#include <utility>

namespace es_fe
{
template<class Solver, std::size_t var>
class Solution_view
{
public:
	using System = typename Solver::System;
	using Mesh = typename Solver::Mesh;

public:
	Solution_view(const Solver& solver) : solver_(solver)
	{}

	// TODO : vector variables (dim > 1, static / dynamic)
	template<typename... Args>
	double operator()(const typename Mesh::Face_view& face, Point pt, Args&... args) const
	{
		pt = to_ref_triangle(face, pt);

		const auto dofs = solver_.system().template dofs<var>(face, std::forward<Args>(args)...);

		double value = 0;
		for (Local_index dof = 0; dof < dofs.size(); ++dof)
			value += System::template Var_t<var>::Element::basis(dof, pt) *
					 solver_.solution_[dofs[dof].index];

		return value;
	}

	const Mesh& mesh() const
	{
		return solver_.mesh();
	}

private:
	// Maps a given point on given face to a point in the corresponding reference triangle
	static Point to_ref_triangle(const typename Mesh::Face_view& face, const Point& pt)
	{
		const auto j = es_fe::inv_jacobian(face);
		la::Vector_2d p0 = pt - face.vertex_circ()->vertex();
		return j * p0;
	}

private:
	const Solver& solver_;
};

// template<class System, std::size_t var>
// class Solution_view;

// template<class System, std::size_t var, class Mesh_el_tag>
// class Solution_view2;
// } // namespace es_fe

// namespace la::internal
// {
// template<class System, std::size_t var>
// struct Traits_impl<es_fe::Solution_view<System, var>>
// {
// 	using Value = double;
// 	using Access_tag = Read_only_tag;

// 	static constexpr std::size_t rows = 0;
// 	static constexpr std::size_t cols = System::template Var_t<var>::n_dofs(es_fe::Vertex_tag{});
// };
// } // namespace la::internal

// namespace es_fe
// {
// template<class System, std::size_t var>
// class Solution_view : public la::Expression<Solution_view<System, var>>

// //: public internal::Field_base<
// //	typename System::Mesh, typename System::template Var_t<var>,
// //	Solution_view<System, var>>
// {
// private:
// 	using Mesh = typename System::Mesh;
// 	using Var = typename System::template Var_t<var>;
// 	using Base = internal::Field_base<Mesh, Var, Solution_view>;

// 	// 	// TODO : should be a matrix
// 	// 	class ViewAtVertices //: public math::LA::VectorXExprBase<ViewAtVertices, double>
// 	// 	{
// 	// 	public:
// 	// 		ViewAtVertices(const Solution_view& view, Local_index index)
// 	// 			: view_(view), index_(index)
// 	// 		{ }
// 	//
// 	// 		std::size_t size() const
// 	// 		{
// 	// 			return view_.system().mesh().n_vertices();
// 	// 		}
// 	//
// 	// 		double operator[](std::size_t vertex) const
// 	// 		{
// 	//  			auto dof = view_.system_.template
// 	//  vertexDofs<t_var>(static_cast<mesh::Index>(vertex)); 			return dof.is_valid() &&
// 	//  dof.is_active() ? 				view_.system_.solution_[dof.index + index_] :
// 0/*Const::nan*/;
// 	// 			throw;
// 	// 		}
// 	//
// 	// 		double val(mesh::Index vertex, std::size_t layer) const
// 	// 		{
// 	//  			auto dof = view_.system_.template vertexDofs<t_var>(vertex, layer);
// 	//  			return dof.is_valid() && dof.is_active() ?
// 	//  				view_.system_.solution_[dof.index + index_] : 0/*Const::nan*/;
// 	// 			throw;
// 	// 		}
// 	//
// 	// 	private:
// 	// 		const Solution_view& view_;
// 	// 		const Local_index index_;
// 	// 	};

// public:
// 	Solution_view(const System& system) : system_(system)
// 	{}

// 	std::size_t rows() const
// 	{
// 		return system_.mesh().n_vertices();
// 	}

// 	static constexpr std::size_t cols()
// 	{
// 		return Var::n_dofs(es_fe::Vertex_tag{});
// 	}

// 	double operator()(std::size_t row, std::size_t col) const
// 	{
// 		typename System::template Var_vertex_dofs<var> dofs_list;
// 		system_.dof_mapper_.template vertex_dofs<var>(static_cast<Index>(row), dofs_list);

// 		return system_.solution_[dofs_list(0, 0).index];
// 	}

// 	// 	const System& system() const
// 	// 	{
// 	// 		return system_;
// 	// 	}

// 	//	template<class T_Quadr>
// 	// 	void atQuadr(const typename T_System::template Var_dofs<t_var>& dofs,
// 	// 				 LA::Vector<double, T_Quadr::n_points>& values) const
// 	// 	{
// 	// 		values.zero();
// 	// for (std::size_t i = 0; i < values.size(); ++i)
// 	//	for (std::size_t j = 0; j < dofs.size(); ++j)
// 	//		values[i] += Element_quadr<typename Var::Element, T_Quadr>::basis(i, j) *
// 	//system_.solution_[dofs[j].index];
// 	// 	}

// 	// template<class T_Quadr>
// 	// void atQuadr(const typename T_System::Mesh::Cell_view& cell,
// 	//			 LA::Vector<double, T_Quadr::n_points>& values) const
// 	//{
// 	// 		typename T_System::template Var_dofs<t_var> dofs;
// 	//
// 	// 		system_.template dofs<t_var>(dofs, cell);
// 	// 		atQuadr<T_Quadr>(dofs, values);
// 	//}

// 	// 	ViewAtVertices viewAtVertices(Local_index index = 0) const
// 	// 	{
// 	// 		assert(index < var().degree);
// 	// 		return {*this, index};
// 	// 	}

// private:
// 	const Var& variable() const
// 	{
// 		return system_.template variable<var>();
// 	}

// private:
// 	const System& system_;
// };

// template<class System, std::size_t var, class Mesh_el_tag>
// class Solution_view2 //: public la::Expr<Solution_view<System, var>>
// {
// private:
// 	class View_for_dim
// 	{
// 	public:
// 		View_for_dim(const System& system, Local_index dim) : system_(system), dim_(dim)
// 		{}

// 		double operator()(Index vertex, Index layer) const
// 		{
// 			typename System::template Var_vertex_dofs<var> vertex_dofs;
// 			system_.dof_mapper_.template vertex_dofs<0>(vertex, vertex_dofs, layer);
// 			return system_.solution_[vertex_dofs[0].index];
// 		}

// 	private:
// 		const System& system_;
// 		const Local_index dim_;
// 	};

// public:
// 	Solution_view2(const System& system) : system_(system)
// 	{}

// 	auto for_dim(Local_index dim = 0) const
// 	{
// 		return View_for_dim{system_, dim};
// 	}

// private:
// 	const System& system_;
// };
} // namespace es_fe
