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
      yarrr::Tile tile( { 1, 2 }, { 5, 8 } );
      AssertThat( tile.top_left, Equals( yarrr::Tile::Coordinate{ 1, 2 } ) );
      AssertThat( tile.bottom_right, Equals( yarrr::Tile::Coordinate{ 5, 8 } ) );
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

};

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
};

Describe(a_shape)
{
  void SetUp()
  {
    a_tile.reset( new yarrr::Tile( { 1, 2 }, { 5, 8 } ) );
    shape.reset( new yarrr::Shape() );
  }

  It( is_empty_when_constructed )
  {
    AssertThat( shape->tiles(), IsEmpty() );
  }

  It( accepts_tiles_to_store )
  {
    shape->add_tile( *a_tile );
    AssertThat( shape->tiles(), HasLength( 1u ) );
    AssertThat( shape->tiles().back(), Equals( *a_tile ) );
  }

  It( has_assignement_operator )
  {
    shape->add_tile( *a_tile );
    yarrr::Shape new_shape;
    new_shape = *shape;
    AssertThat( new_shape, Equals( *shape ) );
  }


  Describe( shape_equality )
  {
    It( has_to_have_the_same_amount_of_tiles )
    {
      yarrr::Shape a;
      yarrr::Shape b;

      AssertThat( a, Equals( b ) );
      a.add_tile( yarrr::Tile( { 0, 0 }, { 1, 1 } ) );

      AssertThat( a, !Equals( b ) );
    }

    It( has_to_have_the_same_tiles )
    {
      yarrr::Shape a;
      yarrr::Shape b;

      a.add_tile( yarrr::Tile( { 0, 0 }, { 1, 1 } ) );
      b.add_tile( yarrr::Tile( { 0, 0 }, { 1, 1 } ) );

      AssertThat( a, Equals( b ) );

      a.add_tile( yarrr::Tile( { 3, 3 }, { 4, 4 } ) );
      b.add_tile( yarrr::Tile( { 5, 5 }, { 9, 9 } ) );

      AssertThat( a, !Equals( b ) );
    }

  };

  Describe( center_of_mass_calculation )
  {

    void SetUp()
    {
      shape.reset( new yarrr::Shape() );
    }

    It( is_in_the_middle_of_a_single_tile )
    {
      const yarrr::Tile tile{ { -100, 0 }, { 100, 0 } };
      shape->add_tile( tile );
      AssertThat( shape->center_of_mass(), Equals( tile.center ) );
    }

    It( is_between_two_tiles_with_the_same_mass )
    {
      shape->add_tile( yarrr::Tile{ { -2, 1 }, { -2, 1 } } );
      shape->add_tile( yarrr::Tile{ { 1, -2 }, { 1, -2 } } );
      AssertThat( shape->center_of_mass(), Equals( yarrr::Coordinate{ 0, 0 } ) );
    }

    It( is_closer_to_the_bigger_tile )
    {
      yarrr::Tile top_left{ { -2, 1 }, { -2, 1 } };
      shape->add_tile( top_left );
      yarrr::Tile bottom_right{ { 1, -2 }, { 2, -3 } };
      shape->add_tile( bottom_right );

      AssertThat(
          yarrr::length_of( top_left.center - shape->center_of_mass() ),
          IsGreaterThan(
          yarrr::length_of( bottom_right.center - shape->center_of_mass() ) ) );
    }

    std::unique_ptr< yarrr::Shape > shape;
  };

  std::unique_ptr< yarrr::Tile > a_tile;
  std::unique_ptr< yarrr::Shape > shape;
};

