#pragma once

#include <yarrr/vector.hpp>
#include <yarrr/types.hpp>
#include <vector>

namespace yarrr
{
typedef std::vector< Coordinate > Polygon;

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

  private:
    yarrr::Coordinate calculate_center() const;
    int calculate_mass() const;
};

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
    void add_tile( const Tile& );
    const TileContainer& tiles() const;

    int mass() const;
    const Coordinate& center_of_mass() const;

    Shape& operator=( const Shape& other );

  private:
    void calculate_center_of_mass_and_mass();

    TileContainer m_tiles;
    Coordinate m_center_of_mass;
    int m_mass;
};

bool operator==( const Shape& l, const Shape& r );

}

