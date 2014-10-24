#pragma once

#include <yarrr/vector.hpp>
#include <yarrr/types.hpp>

namespace yarrr
{

class Tile final
{
  public:
    typedef Vector< int16_t > Coordinate;
    static const int16_t unit_length;

    Tile( const Coordinate& top_left, const Coordinate& bottom_right );

    const Coordinate top_left;
    const Coordinate bottom_right;
    const yarrr::Coordinate center;
    const int mass;

    bool does_contain( const yarrr::Coordinate& ) const;
  private:
    yarrr::Coordinate calculate_center() const;
    int calculate_mass() const;
};

bool operator==( const Tile& l, const Tile& r );
bool operator!=( const Tile& l, const Tile& r );

Coordinate top_left_corner_of( const Tile& );
Coordinate top_right_corner_of( const Tile& );
Coordinate bottom_right_corner_of( const Tile& );
Coordinate bottom_left_corner_of( const Tile& );

}

