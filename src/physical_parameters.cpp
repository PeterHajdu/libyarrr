#include <yarrr/physical_parameters.hpp>
#include <sstream>

namespace yarrr
{

std::ostream& operator<<( std::ostream& output, const PhysicalParameters& object )
{
  output << "object "
    << object.id << " "
    << object.coordinate.x << " "
    << object.coordinate.y << " "
    << object.velocity.x << " "
    << object.velocity.y << " "
    << object.angle << " "
    << object.vangle << " "
    << object.timestamp << " "
    << "\n";

  return output;
}


bool operator==( const PhysicalParameters& l, const PhysicalParameters& r )
{
  return
    l.coordinate == r.coordinate &&
    l.velocity == r.velocity &&
    l.angle == r.angle &&
    l.vangle == r.vangle &&
    l.timestamp == r.timestamp;
}


void travel_in_time_to( const the::time::Clock::Time& timestamp, PhysicalParameters& object )
{
  const the::time::Difference delta( timestamp - object.timestamp );
  const float ratio( delta * 1.0 / the::time::Clock::ticks_per_second );
  object.coordinate+=object.velocity * ratio;
  object.angle+=object.vangle * ratio;
  object.timestamp = timestamp;
}

}

