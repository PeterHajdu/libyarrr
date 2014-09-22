#include <thetime/clock.hpp>
#include <yarrr/physical_parameters.hpp>
#include <sstream>
#include <cmath>

namespace
{

void
normalize_orientation( yarrr::Angle& angle )
{
  angle %= 360_degrees;
}

}

namespace yarrr
{

std::ostream& operator<<( std::ostream& output, const PhysicalParameters& physical_parameters )
{
  output << "physical_parameters:\n"
    << "  coordinate: "
    << physical_parameters.coordinate.x << ", "
    << physical_parameters.coordinate.y << "\n"
    << "  velocity: "
    << physical_parameters.velocity.x << ", "
    << physical_parameters.velocity.y << "\n"
    << "  orientation: "
    << physical_parameters.orientation << "\n"
    << "  angular velocity: "
    << physical_parameters.angular_velocity << "\n"
    << "  timestamp: "
    << physical_parameters.timestamp << "\n"
    << "  integrity: "
    << physical_parameters.integrity << std::endl;
  return output;
}


bool operator==( const PhysicalParameters& l, const PhysicalParameters& r )
{
  return
    l.coordinate == r.coordinate &&
    l.velocity == r.velocity &&
    l.orientation == r.orientation &&
    l.angular_velocity == r.angular_velocity &&
    l.integrity == r.integrity &&
    l.timestamp == r.timestamp;
}


void travel_in_time_to( const the::time::Clock::Time& timestamp, PhysicalParameters& physical_parameters )
{
  const the::time::Difference delta( timestamp - physical_parameters.timestamp );
  const float ratio( delta * 1.0 / the::time::Clock::ticks_per_second );
  physical_parameters.coordinate+=physical_parameters.velocity * ratio;
  physical_parameters.orientation+=physical_parameters.angular_velocity * ratio;
  physical_parameters.timestamp = timestamp;

  normalize_orientation( physical_parameters.orientation );
}


Coordinate
heading( const PhysicalParameters& parameters, int length )
{
  return vector_with< Coordinate::type >( parameters.orientation, length );
}

PhysicalParameters
weight_arithmetic_mean(
    const PhysicalParameters& l,
    const PhysicalParameters& r,
    double ratio )
{
  PhysicalParameters average( l );

  average.coordinate = l.coordinate * ratio + r.coordinate * ( 1 - ratio );
  average.velocity = l.velocity * ratio + r.velocity * ( 1 - ratio );
  average.angular_velocity = ( l.angular_velocity + r.angular_velocity ) * 0.5;

  average.orientation = radian_to_hiplons( std::atan2(
      std::sin( hiplon_to_radians( l.orientation ) ) * ratio + std::sin( hiplon_to_radians( r.orientation ) ) * ( 1 - ratio ),
      std::cos( hiplon_to_radians( l.orientation ) ) * ratio + std::cos( hiplon_to_radians( r.orientation ) ) * ( 1 - ratio ) ) );

  return average;
}

}

