#include <yarrr/tile.hpp>
//todo: remove after polygon is extracted
#include <yarrr/shape.hpp>
#include <yarrr/types.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_tile)
{

  Describe( instantiation )
  {
    It( has_constructor_with_top_left_and_bottom_right_coordinate_parameters )
    {
      yarrr::Tile tile( { 1, 8 }, { 5, 2 } );
      AssertThat( tile.top_left, Equals( yarrr::Tile::Coordinate{ 1, 8 } ) );
      AssertThat( tile.bottom_right, Equals( yarrr::Tile::Coordinate{ 5, 2 } ) );
    }

    It( fixes_mixed_up_x_coordinate )
    {
      yarrr::Tile tile( { 5, 8 }, { 1, 2 } );
      AssertThat( tile.top_left, Equals( yarrr::Tile::Coordinate{ 1, 8 } ) );
      AssertThat( tile.bottom_right, Equals( yarrr::Tile::Coordinate{ 5, 2 } ) );
    }

    It( fixes_mixed_up_y_coordinate )
    {
      yarrr::Tile tile( { 1, 2 }, { 5, 8 } );
      AssertThat( tile.top_left, Equals( yarrr::Tile::Coordinate{ 1, 8 } ) );
      AssertThat( tile.bottom_right, Equals( yarrr::Tile::Coordinate{ 5, 2 } ) );
    }
  };

  It( has_equality_operator )
  {
    yarrr::Tile tile( { 1, 2 }, { 5, 8 } );
    AssertThat( tile, Equals( tile ) );

    yarrr::Tile another_tile( { 2, 3 }, { 6, 7 } );
    AssertThat( tile, !Equals( another_tile ) );
  }

  Describe( center_calculation )
  {

    It( is_in_the_middle_of_a_single_unit_tile )
    {
      yarrr::Tile tile{ { 0, 0 }, { 0, 0 } };
      const yarrr::Coordinate center_of_tile(
          yarrr::Tile::unit_length * 0.5,
          yarrr::Tile::unit_length * 0.5 );

      AssertThat( tile.center, Equals( center_of_tile ) );
    }

    It( is_in_the_middle_of_a_single_unit_tile_not_in_the_origo )
    {
      yarrr::Tile tile{ { 10, 10 }, { 10, 10 } };
      const yarrr::Coordinate center_of_tile(
          yarrr::Tile::unit_length * 10.5,
          yarrr::Tile::unit_length * 10.5 );

      AssertThat( tile.center, Equals( center_of_tile ) );
    }

    It( is_in_the_middle_of_a_tile_around_the_origo )
    {
      yarrr::Tile tile{ { -1, 0 }, { 0, -1 } };
      const yarrr::Coordinate center_of_tile( 0, 0 );
      AssertThat( tile.center, Equals( center_of_tile ) );
    }

  };

  Describe( mass_calculation )
  {
    It( is_the_area_of_the_tile )
    {
      yarrr::Tile tile{ { -1, 0 }, { 0, -1 } };
      AssertThat( tile.mass, Equals( 4 ) );
    }
  };

  //todo: move to polygon file
  Describe( polygon_generation )
  {
    It( generates_the_polygon_in_shape_coordinate_system )
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

  Describe( contains_point_functionality )
  {
    It( can_tell_if_it_contains_a_point )
    {
      AssertThat( tile.does_contain( coordinate_outside ), Equals( false ) );
      AssertThat( tile.does_contain( coordinate_inside ), Equals( true ) );
    }

    const yarrr::Tile tile{ { 10, 10 }, { 11, 10 } };
    const yarrr::Coordinate coordinate_outside{ 10 * yarrr::Tile::unit_length, 10 * yarrr::Tile::unit_length };
    const yarrr::Coordinate coordinate_inside{ 10 * yarrr::Tile::unit_length + 1, 10 * yarrr::Tile::unit_length + 1 };
  };

};

