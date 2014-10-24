#pragma once

#include <yarrr/tile.hpp>
#include <yarrr/types.hpp>
#include <vector>

namespace yarrr
{

class Shape
{
  public:
    typedef std::vector< Tile > TileContainer;
    Shape();
    Shape( const TileContainer& );
    void add_tile( const Tile& );
    const TileContainer& tiles() const;

    int mass() const;
    const Coordinate& center_of_mass() const;

    Shape& operator=( const Shape& other );

    Coordinate::type radius() const;

    bool does_contain( const Coordinate& relative_to_center_of_mass ) const;

  private:
    void calculate_center_of_mass_and_mass();
    void calculate_radius();

    TileContainer m_tiles;
    Coordinate m_center_of_mass;
    int m_mass;
    Coordinate::type m_radius;
};

bool operator==( const Shape& l, const Shape& r );

}

