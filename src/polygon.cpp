#include <yarrr/polygon.hpp>
#include <yarrr/tile.hpp>

namespace yarrr
{

Polygon
shape_coordinate_polygon_from( const Tile& tile )
{
  return Polygon{
    yarrr::top_left_corner_of( tile ),
    yarrr::top_right_corner_of( tile ),
    yarrr::bottom_right_corner_of( tile ),
    yarrr::bottom_left_corner_of( tile ) };
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

