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

Describe(relative_coordinate)
{
  It( can_be_transformed_to_absolute_coordinate )
  {
    const yarrr::Coordinate absolute_coordinate(
        yarrr::relative_to_absolute(
          relative_coordinate,
          center_of_mass_relative,
          center_of_object_absolute,
          orientation_of_object ) );

    AssertThat( absolute_coordinate, Equals( yarrr::Coordinate( 100_metres, 105_metres ) ) );
  }

  It( can_be_transformed_to_absolute_coordinate_from_center_of_mass_relative_coordinate )
  {
    const yarrr::Coordinate absolute_coordinate(
        yarrr::center_of_mass_relative_to_absolute(
          relative_coordinate,
          center_of_object_absolute,
          orientation_of_object ) );

    AssertThat( absolute_coordinate, Equals( yarrr::Coordinate( 100_metres, 110_metres ) ) );
  }

  const yarrr::Coordinate relative_coordinate{ -10_metres, 0 };
  const yarrr::Coordinate center_of_mass_relative{ -5_metres, 0 };
  const yarrr::Coordinate center_of_object_absolute{ 100_metres, 100_metres };
  const yarrr::Angle orientation_of_object{ -90_degrees };
};

Describe(absolute_coordinate)
{
  It(can_be_transformed_to_relative_to_center_of_mass)
  {
    const yarrr::Coordinate relative_to_center_of_mass(
        yarrr::transform_absolute_to_relative_to_center_of_mass(
          absolute_coordinate,
          absolute_center_of_mass,
          orientation ) );
    AssertThat( relative_to_center_of_mass,
        Equals( yarrr::Coordinate{ 5_metres, 0 } ) );
  }

  const yarrr::Coordinate absolute_coordinate{ 0, 10_metres };
  const yarrr::Coordinate absolute_center_of_mass{ 0, 5_metres };
  const yarrr::Angle orientation{ 90_degrees };
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

  Describe( mass_calculation )
  {
    void SetUp()
    {
      shape.reset( new yarrr::Shape() );
      shape->add_tile( a_tile );
    }

    It( is_the_mass_of_the_tile_in_case_of_single_tile_shape )
    {
      AssertThat( shape->mass(), Equals( a_tile.mass ) );
    }

    It( is_the_sum_mass_of_all_tiles_in_case_of_a_multi_tile_shape )
    {
      shape->add_tile( another_tile );
      AssertThat( shape->mass(), Equals( a_tile.mass + another_tile.mass ) );
    }

    const yarrr::Tile a_tile{ { 1, 4 }, { 3, 1 } };
    const yarrr::Tile another_tile{ { 1, 1 }, { 2, 0 } };
    std::unique_ptr< yarrr::Shape > shape;
  };

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

  Describe( radius_calculation )
  {

    void SetUp()
    {
      shape.reset( new yarrr::Shape() );
    }

    It( is_the_most_distant_point_of_a_single_unit_tile )
    {
      shape->add_tile( { { 10, 10 }, { 10, 10 } } );
      AssertThat( shape->radius(),
          Equals( yarrr::length_of(
              yarrr::Coordinate{
              yarrr::Tile::unit_length,
              yarrr::Tile::unit_length } * 0.5 ) ) );
    }

    It( is_the_most_distant_point_of_a_bigger_single_tile )
    {
      shape->add_tile( { { 10, 10 }, { 14, 14 } } );
      AssertThat( shape->radius(),
          Equals( yarrr::length_of(
              yarrr::Coordinate{
              yarrr::Tile::unit_length,
              yarrr::Tile::unit_length } * 2.5 ) ) );
    }

    It( is_the_most_distant_point_of_all_tiles )
    {
      shape->add_tile( { { -1, 0 }, { -1, 0 } } );
      shape->add_tile( { { 0, 0 }, { 0, 0 } } );
      AssertThat( shape->radius(),
          Equals( yarrr::length_of(
              yarrr::Coordinate{
              yarrr::Tile::unit_length,
              yarrr::Tile::unit_length * 0.5 } ) ) );
    }

    std::unique_ptr< yarrr::Shape > shape;
  };

  Describe( instantiation )
  {
    It( has_a_default_constructor )
    {
      yarrr::Shape a_shape;
      (void)a_shape;
    }

    It( can_be_constructed_with_a_vector_of_tiles )
    {
      yarrr::Shape default_constructed_shape;
      default_constructed_shape.add_tile( tile_a );
      default_constructed_shape.add_tile( tile_b );

      const yarrr::Shape vector_constructed_shape{ { tile_a, tile_b } };
      AssertThat( default_constructed_shape, Equals( vector_constructed_shape ) );
    }

    const yarrr::Tile tile_a{ { 0, 0 }, { 1, 1 } };
    const yarrr::Tile tile_b{ { -1, -1 }, { -1, -1 } };
  };

  Describe( containment )
  {
    It( can_tell_if_a_coordinate_is_not_contained_at_all )
    {
      AssertThat( shape.does_contain( coordinate_outside_tiles ), Equals( false ) );
    }

    It( can_tell_if_any_of_its_tiles_contain_a_coordinate_relative_to_the_center_of_mass )
    {
      AssertThat( shape.does_contain( coordinate_inside_tile_b ), Equals( true ) );
      AssertThat( shape.does_contain( coordinate_inside_tile_a ), Equals( true ) );
    }

    const yarrr::Shape shape{ {
      yarrr::Tile{ { 0, 0 }, { 0, 0 } },
      yarrr::Tile{ { 1, 0 }, { 1, 0 } } } };

    const yarrr::Coordinate center_of_mass{ shape.center_of_mass() };
    const yarrr::Coordinate diff{ yarrr::Tile::unit_length / 2, 0 };
    const yarrr::Coordinate coordinate_inside_tile_a{ diff * -1 };
    const yarrr::Coordinate coordinate_inside_tile_b{ diff };
    const yarrr::Coordinate coordinate_outside_tiles{ diff * 2 };
  };

  std::unique_ptr< yarrr::Tile > a_tile;
  std::unique_ptr< yarrr::Shape > shape;
};

