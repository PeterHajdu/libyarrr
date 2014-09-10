#include <yarrr/shape.hpp>
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

  std::unique_ptr< yarrr::Tile > a_tile;
  std::unique_ptr< yarrr::Shape > shape;
};

