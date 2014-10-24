#pragma once

#include <yarrr/tile.hpp>
#include <yarrr/types.hpp>
#include <vector>

namespace yarrr
{
typedef std::vector< Coordinate > Polygon;

Coordinate relative_to_absolute(
    const Coordinate& relative_coordinate,
    const Coordinate& center_of_mass_relative,
    const Coordinate& center_of_object_absolute,
    const Angle& orientation_of_object );

Coordinate center_of_mass_relative_to_absolute(
    const Coordinate& relative_to_center_of_mass_coordinate,
    const Coordinate& center_of_object_absolute,
    const Angle& orientation_of_object );

Polygon shape_coordinate_polygon_from( const Tile& );

void transform_coordinates_to_new_origo( Polygon&, const Coordinate& new_origo );

void rotate_with( Polygon&, Angle );

Polygon generate_polygon_from(
    const Tile&,
    const Coordinate& center_of_object,
    const Coordinate& center_of_mass_of_shape,
    const Angle& orientation_of_object );

bool operator==( const Tile& l, const Tile& r );
bool operator!=( const Tile& l, const Tile& r );

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

Coordinate transform_absolute_to_relative_to_center_of_mass(
  const Coordinate& absolute_coordinate,
  const Coordinate& absolute_center_of_mass,
  const Angle& orientation );
}

