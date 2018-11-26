#pragma once
#include <es_fe/mesh/mesh2.hpp>
#include <es_fe/mesh/algorithm/colour_cells.hpp>

#include <es_util/iterator.hpp>

#include <cassert>
#include <unordered_map>
#include <vector>

namespace es_fe
{
class Mesh_face_colour_map
{
public:
	using Map = std::unordered_multimap<unsigned int, Face_index>;

public:
	Mesh_face_colour_map(const Mesh2& mesh) : mesh_(mesh)
	{
		generate();
	}

	unsigned int n_colours() const
	{
		assert(!map_.empty());
		return n_colours_;
	}

	void get_cells_with_colour(unsigned int colour, std::vector<Face_index>& faces) const
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

	auto cells_with_colour(unsigned int colour) const
	{
		return es_util::Iterable{es_util::Transform_iterator{map_.cbegin(), get_face_index},
								 es_util::Transform_iterator{map_.cend(), get_face_index}};
	}

private:
	static Face_index get_face_index(const std::pair<unsigned int, Face_index>& p)
	{
		return p.second;
	}

	void generate()
	{
		std::vector<unsigned int> colours;
		n_colours_ = colour_cells(mesh_, colours);

		map_.clear();
		map_.reserve(*mesh_.n_faces());
		for (Face_index face{0}; face < mesh_.n_faces(); ++face)
			map_.emplace(colours[*face], face);
	}

private:
	unsigned int n_colours_ = 0;
	Map map_;

	const Mesh2& mesh_;
};
} // namespace es_fe