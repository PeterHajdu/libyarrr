#include <yarrr/polygon.hpp>
#include <yarrr/tile.hpp>

namespace yarrr
{

Polygon
shape_coordinate_polygon_from( const Tile& tile )
{
  const int left{ tile.top_left.x };
  const int right{ tile.bottom_right.x + 1 };
  const int top{ tile.top_left.y + 1 };
  const int bottom{ tile.bottom_right.y };
  return Polygon{
      Coordinate{ left, top } * Tile::unit_length,
      Coordinate{ right, top } * Tile::unit_length,
      Coordinate{ right, bottom } * Tile::unit_length,
      Coordinate{ left, bottom } * Tile::unit_length };
}

void
transform_coordinates_to_new_origo( Polygon& polygon , const Coordinate& origo )
{
  for ( auto& point : polygon )
  {
    point.x-=origo.x;
    point.y-=origo.y;
  }
}

void
rotate_with( Polygon& polygon, Angle angle )
{
  for ( auto& point : polygon )
  {
    rotate( point, angle );
  }
}

Polygon
generate_polygon_from(
    const Tile& tile,
    const Coordinate& center_of_object,
    const Coordinate& center_of_mass_of_shape,
    const Angle& orientation_of_object )
{
  Polygon polygon( shape_coordinate_polygon_from( tile ) );
  transform_coordinates_to_new_origo( polygon, center_of_mass_of_shape );
  rotate_with( polygon, orientation_of_object );
  transform_coordinates_to_new_origo( polygon, center_of_object * -1 );
  return polygon;
}

}

