#pragma once

namespace yarrr
{
  struct Vector
  {
    typedef int64_t type;
    type x;
    type y;
  };

  const Vector& operator+=( Vector& l, const Vector& r )
  {
    l.x += r.x;
    l.y += r.y;
    return l;
  }

  bool operator==( const Vector& l, const Vector& r )
  {
    return
      l.x == r.x &&
      l.y == r.y;
  }

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

