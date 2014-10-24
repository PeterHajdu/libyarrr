#include <yarrr/polygon.hpp>
#include <yarrr/tile.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_polygon)
{
  It(can_be_transformed_to_a_new_origo)
  {
    yarrr::Polygon polygon{ { 10, 15 }, { -32, -342 } };
    yarrr::transform_coordinates_to_new_origo( polygon, yarrr::Coordinate{ 10, 10 } );
    AssertThat( polygon, Equals( yarrr::Polygon{
          { 0, 5 },
          { -42, -352 } } ) );
  }

  It(can_be_rotated_around_its_origo)
  {
    yarrr::Polygon polygon{ { 1, 0 }, { 0 , -1 } };
    yarrr::rotate_with( polygon, 90_degrees );
    AssertThat( polygon, Equals( yarrr::Polygon{
          { 0, 1 },
          { 1, 0 } } ));
  }

  It( generates_polygon_from_a_tile_in_shape_coordinate_system )
  {
    const yarrr::Tile tile{ { -2, 0 }, { 1, -1 } };
    const yarrr::Polygon polygon( yarrr::shape_coordinate_polygon_from( tile ) );
    AssertThat( polygon, HasLength( 4u ) );
    AssertThat(
        polygon.at( 0 ),
        Equals( yarrr::Coordinate{ -yarrr::Tile::unit_length * 2, yarrr::Tile::unit_length } ) );

    AssertThat(
        polygon.at( 1 ),
        Equals( yarrr::Coordinate{ yarrr::Tile::unit_length * 2, yarrr::Tile::unit_length } ) );

    AssertThat(
        polygon.at( 2 ),
        Equals( yarrr::Coordinate{ yarrr::Tile::unit_length * 2, -yarrr::Tile::unit_length } ) );

    AssertThat(
        polygon.at( 3 ),
        Equals( yarrr::Coordinate{ -yarrr::Tile::unit_length * 2, -yarrr::Tile::unit_length } ) );
  }

};

