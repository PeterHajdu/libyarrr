#pragma once

#include <yarrr/vector.hpp>
#include <cstdint>
#include <vector>
#include <cmath>

constexpr int64_t operator "" _metres( unsigned long long int metre )
{
  return metre << 8;
}

constexpr int16_t operator "" _degrees( unsigned long long int deg )
{
  return deg << 2;
}

namespace yarrr
{
  constexpr double pi()
  {
    return 3.14159265359;
  }

  typedef std::vector<char> Data;

  typedef Vector<int64_t> Coordinate;
  typedef Vector<int64_t> Velocity;
  typedef int16_t Angle;

  inline Coordinate& metres_to_huplons_in_place( Coordinate& coordinate_in_metres )
  {
    return coordinate_in_metres<<=8;
  }

  inline Coordinate metres_to_huplons( Coordinate coordinate_in_metres )
  {
    return metres_to_huplons_in_place( coordinate_in_metres );
  }

  inline Coordinate::type huplons_to_metres( const Coordinate::type& huplons )
  {
    return huplons>>8;
  }

  inline Coordinate& huplons_to_metres_in_place( Coordinate& coordinate_in_huplons )
  {
    return coordinate_in_huplons>>=8;
  }

  inline Coordinate huplons_to_metres( Coordinate coordinate_in_huplons )
  {
    return huplons_to_metres_in_place( coordinate_in_huplons );
  }

  inline int16_t degree_to_hiplons( int16_t degree )
  {
    return degree << 2;
  }

  inline int16_t hiplon_to_degrees( Angle hiplon )
  {
    return hiplon >> 2;
  }

  inline double hiplon_to_radians( Angle hiplon )
  {
    return hiplon_to_degrees( hiplon ) / 180.0 * pi();
  }

  inline int16_t radian_to_hiplons( double radian )
  {
    return degree_to_hiplons( radian / pi() * 180 );
  }

  template <typename T>
  void rotate( Vector<T>& vector, Angle angle )
  {
    const double angle_in_rad( hiplon_to_radians( angle ) );
    const double cos_angle( cos( angle_in_rad ) );
    const double sin_angle( sin( angle_in_rad ) );
    Vector< T > old( vector );
    vector.x = static_cast< T >( old.x * cos_angle - old.y * sin_angle );
    vector.y = static_cast< T >( old.x * sin_angle + old.y * cos_angle );
  }


  template <typename T>
  T dot_product( const Vector<T>& l, const Vector<T>& r )
  {
    return l.x * r.x + l.y * r.y;
  }


  template <typename T>
  T z_of_cross_product( const Vector<T>& l, const Vector<T>& r )
  {
    return l.x * r.y - l.y * r.x;
  }

  template <typename T>
  Vector<T> vector_with( Angle angle, int length )
  {
    return Vector<T>(
        static_cast< T >( cos( hiplon_to_radians( angle ) ) * length ),
        static_cast< T >( sin( hiplon_to_radians( angle ) ) * length ) );
  }

  template <typename T>
  Vector<T> normalize( Vector<T> vector )
  {
    huplons_to_metres_in_place( vector );
    const double inverse_sqrt( 1_metres * 1.0 / sqrt( vector.x * vector.x + vector.y * vector.y ) );
    return vector * inverse_sqrt;
  }
}

