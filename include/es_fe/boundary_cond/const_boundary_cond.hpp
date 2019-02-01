#pragma once
#include <es_fe/boundary_cond/boundary_cond.hpp>

namespace es_fe
{
template<class Element, bool is_essential_>
class Const_boundary_cond : public Boundary_cond<Element, is_essential_>
{
private:
	using Base = Boundary_cond<Element, is_essential_>;
	
public:
	Const_boundary_cond(const Mesh2& mesh, const Linestring& boundary) :
		Base(mesh, boundary)
	{}	
	
	Const_boundary_cond(const Mesh2& mesh, const Linestring& boundary, double value) :
		Base(mesh, boundary), value_(value)
	{}	
	
	template<class... Any>
	double value(const Any&...) const
	{
		return value_;
	}
	
	void set_value(double value)
	{
		value = value_;
	}

private:
	double value_;
};
}