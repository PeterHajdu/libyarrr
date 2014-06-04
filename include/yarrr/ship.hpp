#pragma once
#include <yarrr/types.hpp>

namespace yarrr
{
  typedef Vector Coordinate;
  typedef Vector Velocity;
  typedef int16_t Angle;

  struct Ship
  {
    Coordinate coordinate;
    Velocity velocity;
    Angle angle;
    Angle vangle;
  };

  template <class object>
  void time_step( object& the_object )
  {
    the_object.coordinate+=the_object.velocity;
    the_object.angle+=the_object.vangle;
  }

}

