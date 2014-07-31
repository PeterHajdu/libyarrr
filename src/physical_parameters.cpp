#include <yarrr/physical_parameters.hpp>
#include <sstream>

namespace yarrr
{

std::ostream& operator<<( std::ostream& output, const PhysicalParameters& physical_parameters )
{
  output << "physical_parameters "
    << physical_parameters.coordinate.x << " "
    << physical_parameters.coordinate.y << " "
    << physical_parameters.velocity.x << " "
    << physical_parameters.velocity.y << " "
    << physical_parameters.angle << " "
    << physical_parameters.vangle << " "
    << physical_parameters.timestamp << " "
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


void travel_in_time_to( const the::time::Clock::Time& timestamp, PhysicalParameters& physical_parameters )
{
  const the::time::Difference delta( timestamp - physical_parameters.timestamp );
  const float ratio( delta * 1.0 / the::time::Clock::ticks_per_second );
  physical_parameters.coordinate+=physical_parameters.velocity * ratio;
  physical_parameters.angle+=physical_parameters.vangle * ratio;
  physical_parameters.timestamp = timestamp;
}

}

