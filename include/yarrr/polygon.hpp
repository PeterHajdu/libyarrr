#pragma once

#include <yarrr/types.hpp>
#include <vector>

namespace yarrr
{
class Tile;

typedef std::vector< Coordinate > Polygon;
Polygon shape_coordinate_polygon_from( const Tile& );

void transform_coordinates_to_new_origo( Polygon&, const Coordinate& new_origo );

void rotate_with( Polygon&, Angle );

Polygon generate_polygon_from(
    const Tile&,
    const Coordinate& center_of_object,
    const Coordinate& center_of_mass_of_shape,
    const Angle& orientation_of_object );

}

