#pragma once
#include <yarrr/types.hpp>
#include <thetime/clock.hpp>
#include <string>

namespace yarrr
{
  typedef Vector<int64_t> Coordinate;
  typedef Vector<int64_t> Velocity;
  typedef int16_t Angle;

  struct Object
  {
    Object() = default;
    Object( const std::string& binary_data );

    int id;
    Coordinate coordinate;
    Velocity velocity;
    Angle angle;
    Angle vangle;
  };


  std::ostream& operator<<( std::ostream& output, const Object& object );
  bool operator==( const Object& l, const Object& r );

  const std::string serialize( const Object& object );
  Object deserialize( const std::string& data );

  template <class object>
  void time_step( object& the_object )
  {
    the_object.coordinate+=the_object.velocity;
    the_object.angle+=the_object.vangle;
  }

}

