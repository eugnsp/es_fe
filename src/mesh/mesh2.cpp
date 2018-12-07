#include <es_fe/mesh/mesh2.hpp>

#include <es_geom/algorithm.hpp>
#include <es_geom/rect.hpp>
#include <es_util/error.hpp>
#include <es_util/string.hpp>

#include <cassert>
#include <algorithm>
#include <ostream>

namespace es_fe
{
// Returns the smallest rectangle that contains the mesh
es_geom::Rect Mesh2::bounding_rect() const
{
	assert(!is_empty());

	const auto first = boundary_vertex_circ();

	es_geom::Point bottom_left = first->vertex();
	es_geom::Point top_right = first->vertex();

	auto circ = first;
	do
	{
		auto& v = circ->vertex();

		bottom_left.x() = std::min(bottom_left.x(), v.x());
		bottom_left.y() = std::min(bottom_left.y(), v.y());

		top_right.x() = std::max(top_right.x(), v.x());
		top_right.y() = std::max(top_right.y(), v.y());

		++circ;
	} while (circ != first);

	return {bottom_left, top_right};
}

// Performs some basic checks of mesh data structure consistency
es_util::Error Mesh2::check() const
{
	auto err = Halfedge_structure::check();
	if (err)
		return err;

	// Check for counter-clockwise order of vertices
	for (auto& face : this->faces())
		if (es_geom::area(face) < 0)
			err.append_ln("face #", **face, " has incorrect vertex order");

	return err;
}

// Outputs human readable information about the mesh
void Mesh2::print(std::ostream& os) const
{
	os << type_string() << '\n'
	   << "Vertices: " << *n_vertices() << '\n'
	   << "Edges:    " << *n_edges() << '\n'
	   << "Faces:    " << *n_faces() << '\n'
	   << "Memory:   " << es_util::size_string(memory_size()) << std::endl;
}
} // namespace es_fe
