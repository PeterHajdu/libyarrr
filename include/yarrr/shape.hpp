#pragma once

#include <yarrr/vector.hpp>
#include <vector>

namespace yarrr
{

class Tile final
{
  public:
    typedef Vector< int16_t > Coordinate;

    Tile( const Coordinate& top_left, const Coordinate& bottom_right )
      : top_left( top_left )
      , bottom_right( bottom_right )
    {
    }

    const Coordinate top_left;
    const Coordinate bottom_right;
};

bool operator==( const Tile& l, const Tile& r );
bool operator!=( const Tile& l, const Tile& r );

class Shape
{
  public:
    typedef std::vector< Tile > TileContainer;
    void add_tile( const Tile& );
    const TileContainer& tiles() const;

  private:
    TileContainer m_tiles;
};

bool operator==( const Shape& l, const Shape& r );

}

