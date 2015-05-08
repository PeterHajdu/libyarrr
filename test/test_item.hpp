#pragma once

#include <yarrr/object.hpp>
#include <yarrr/shape.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/item.hpp>

namespace test
{

class Item : public yarrr::Item
{
  public:
    add_polymorphic_ctci( "yarrr_test_item" );

    Item()
      : yarrr::Item( yarrr::always_synchronize(), "test item name", { 0, 0 } )
      , serialized_member( 0 )
    {
    }

    Item( const yarrr::Tile::Coordinate& tile_coordinate, int serialized_member )
      : yarrr::Item( yarrr::always_synchronize(), "test item name", tile_coordinate )
      , serialized_member( serialized_member )
    {
    }

    Item( const Id& id )
      : yarrr::Item( yarrr::always_synchronize(), id, "test item name", { 0, 0 } )
      , serialized_member( 0 )
    {
    }

    virtual yarrr::ObjectBehavior::Pointer clone() const override
    {
      return Pointer( new Item( id() ) );
    }

    int serialized_member;
    virtual void serialize_item( yarrr::Serializer& serializer ) const override final
    {
      serializer.push_back( serialized_member );
    }

    virtual void deserialize_item( yarrr::Deserializer& deserializer ) override final
    {
      serialized_member = deserializer.pop_front< int >();
    }
};

}


