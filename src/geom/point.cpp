#include <es_fe/geom/point.hpp>

#include <iomanip>
#include <ostream>
#include <sstream>

namespace es_fe
{
std::ostream& operator<<(std::ostream& os, const Point& p)
{
	std::stringstream ss;

	ss << std::fixed << std::setprecision(2) << '(' << p.x() << ", " << p.y() << ')';
	os << ss.str() << std::flush;

	return os;
}
} // namespace es_fe
