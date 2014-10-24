#include <yarrr/shape.hpp>
#include <yarrr/types.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

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

