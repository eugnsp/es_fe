#pragma once
#include <es_fe/mesh/mesh2/mesh2.hpp>

#include <es_geom/algorithm.hpp>
#include <es_util/error.hpp>

namespace es_fe
{
// Performs some basic checks of mesh data structure consistency
es_util::Error Mesh2::check() const
{
	auto err = Halfedge_structure::check();
	if (err)
		return err;

	// Check for counter-clockwise order of vertices
	for (auto& face : this->faces())
		if (geom::area(face) < 0)
			err.append_ln("face #", **face, " has incorrect vertex order");

	return err;
}
} // namespace es_fe
