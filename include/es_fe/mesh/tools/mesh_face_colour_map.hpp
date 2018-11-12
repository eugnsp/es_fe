#pragma once
#include <es_fe/mesh/mesh2.hpp>
#include <es_fe/mesh/algorithm/colour_cells.hpp>

#include <cassert>
#include <unordered_map>
#include <vector>

namespace es_fe
{
class Mesh_face_colour_map
{
public:
	using Colour_index = unsigned char;

public:
	Mesh_face_colour_map(const Mesh2& mesh) : mesh_(mesh)
	{
		generate();
	}

	Colour_index n_colours() const
	{
		assert(!map_.empty());
		return n_colours_;
	}

	void get_cells_with_colour(Colour_index colour, std::vector<Face_index>& faces) const
	{
		assert(colour < n_colours_);

		faces.clear();
		const auto range = map_.equal_range(colour);
		std::transform(range.first, range.second, std::back_inserter(faces),
					   [](auto& p) { return p.second; });
	}

	const auto& map() const
	{
		return map_;
	}

private:
	void generate()
	{
		std::vector<Colour_index> colours;
		n_colours_ = colour_cells(mesh_, colours);

		map_.clear();
		map_.reserve(*mesh_.n_faces());
		for (Face_index face{0}; face < mesh_.n_faces(); ++face)
			map_.emplace(colours[*face], face);
	}

private:
	Colour_index n_colours_ = 0;
	std::unordered_multimap<Colour_index, Face_index> map_;

	const Mesh2& mesh_;
};
} // namespace es_fe