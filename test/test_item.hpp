#pragma once

#include <yarrr/object.hpp>
#include <yarrr/shape.hpp>

namespace test
{

class TestItem : public yarrr::Item
{
  public:
    add_polymorphic_ctci( "yarrr_test_item" );

    TestItem()
      : Item( yarrr::always_synchronize(), "test item name", { 0, 0 } )
    {
    }

    TestItem( const yarrr::Tile::Coordinate& tile_coordinate )
      : Item( yarrr::always_synchronize(), "test item name", tile_coordinate )
    {
    }

    TestItem( const Id& id )
      : Item( yarrr::always_synchronize(), id, "test item name", { 0, 0 } )
    {
    }

    virtual yarrr::ObjectBehavior::Pointer clone() const override
    {
      return Pointer( new TestItem( id() ) );
    }
};

}


