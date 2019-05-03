#pragma once
#include <es_fe/mesh/view/base.hpp>
#include <es_fe/types.hpp>

#include <iosfwd>

namespace es_fe
{
template<>
class Element_view<Vertex_tag, Mesh1> : public internal::Element_view_base<Vertex_tag, Mesh1>
{
public:
public:
	using Element_view_base::Element_view_base;

	double vertex() const;

	// Outputs human readable information about the view
	void print(std::ostream&) const;
};

// Outputs human readable information about the view
inline std::ostream& operator<<(std::ostream& os, const Element_view<Vertex_tag, Mesh1>& view)
{
	view.print(os);
	return os;
}
} // namespace es_fe
