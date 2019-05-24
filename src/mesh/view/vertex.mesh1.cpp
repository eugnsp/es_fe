#include <es_fe/mesh/mesh1.hpp>
#include <es_fe/types.hpp>

namespace es_fe
{
using V = Element_view<Vertex_tag, Mesh1>;

Point1 V::vertex() const
{
	return mesh_.vertex(index_);
}
} // namespace es_fe
