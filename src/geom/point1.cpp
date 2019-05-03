#include <es_fe/geometry/point1.hpp>

#include <iomanip>
#include <ostream>
#include <sstream>

namespace es_fe
{
std::ostream& operator<<(std::ostream& os, const Point1& p)
{
	std::stringstream ss;

	ss << std::fixed << std::setprecision(2) << '(' << p.x() << ')';
	os << ss.str() << std::flush;

	return os;
}
} // namespace es_fe
