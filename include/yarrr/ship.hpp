#pragma once
#include <yarrr/types.hpp>
#include <string>

namespace yarrr
{
  typedef Vector<int64_t> Coordinate;
  typedef Vector<int64_t> Velocity;
  typedef int16_t Angle;

  struct Ship
  {
    Ship() = default;
    Ship( const std::string& binary_data );

    Coordinate coordinate;
    Velocity velocity;
    Angle angle;
    Angle vangle;
  };


  std::ostream& operator<<( std::ostream& output, const Ship& ship );
  bool operator==( const Ship& l, const Ship& r );

  const std::string serialize( const Ship& ship );
  Ship deserialize( const std::string& data );

  template <class object>
  void time_step( object& the_object )
  {
    the_object.coordinate+=the_object.velocity;
    the_object.angle+=the_object.vangle;
  }

}

