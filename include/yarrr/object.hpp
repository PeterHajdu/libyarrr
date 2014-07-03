#pragma once
#include <yarrr/types.hpp>
#include <thetime/clock.hpp>
#include <string>

namespace yarrr
{
  struct Object
  {
    typedef uint64_t Id;

    Object() = default;
    Object( const std::string& binary_data );

    Id id;
    Coordinate coordinate;
    Velocity velocity;
    Angle angle;
    Angle vangle;
    the::time::Clock::Time timestamp;
  };


  std::ostream& operator<<( std::ostream& output, const Object& object );
  bool operator==( const Object& l, const Object& r );

  const std::string serialize( const Object& object );
  Object deserialize( const std::string& data );

  template <class object>
  void travel_in_time_to( const the::time::Clock::Time& timestamp, object& the_object )
  {
    const the::time::Difference delta( timestamp - the_object.timestamp );
    const float ratio( delta * 1.0 / the::time::Clock::ticks_per_second );
    the_object.coordinate+=the_object.velocity * ratio;
    the_object.angle+=the_object.vangle * ratio;
    the_object.timestamp = timestamp;
  }

}

