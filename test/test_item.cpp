#include <yarrr/item.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/object.hpp>
#include <yarrr/shape.hpp>
#include <yarrr/shape_behavior.hpp>

#include <igloo/igloo_alt.h>

#include "test_item.hpp"

using namespace igloo;


Describe( an_item )
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );

    inventory = new yarrr::Inventory();
    object->add_behavior( yarrr::ObjectBehavior::Pointer( inventory ) );

    yarrr::ShapeBehavior* shape_behavior( new yarrr::ShapeBehavior() );
    shape = &shape_behavior->shape;
    object->add_behavior( yarrr::ObjectBehavior::Pointer( shape_behavior ) );
    shape->add_tile( yarrr::Tile( { 0, 1 }, { 1, 0 } ) );

    item = new test::TestItem( tile_coordinate, serialized_member_value );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( item ) );
  }

  It( has_tile_coordinate )
  {
    AssertThat( item->tile_coordinate(), Equals( tile_coordinate ) );
  }

  It( calculates_relative_to_center_of_mass_coordinate )
  {
    const yarrr::Coordinate relative_coordinate{
        static_cast< int64_t >( ( 0.5 + tile_coordinate.x ) * yarrr::Tile::unit_length ),
        static_cast< int64_t >( ( 0.5 + tile_coordinate.y ) * yarrr::Tile::unit_length ) };

    const yarrr::Coordinate relative_to_center_of_mass_coordinate{
        relative_coordinate -
        shape->center_of_mass() };

    AssertThat( item->relative_coordinate(), Equals( relative_to_center_of_mass_coordinate ) );
  }

  It( serializes_and_deserializes_tile_coordinate )
  {
    const auto serialized_item( item->serialize() );
    test::TestItem deserialized_item;
    deserialized_item.deserialize( serialized_item );
    AssertThat( deserialized_item.tile_coordinate(), Equals( item->tile_coordinate() ) );
  }

  It( calls_serialize_and_deserialize_item_when_serializing )
  {
    const auto serialized_item( item->serialize() );
    test::TestItem deserialized_item;
    deserialized_item.deserialize( serialized_item );
    AssertThat( deserialized_item.serialized_member, Equals( serialized_member_value ) );
  }

  It( is_an_object_behavior )
  {
    yarrr::ObjectBehavior& item_as_behavior( static_cast< yarrr::ObjectBehavior& >( *item ) );
    (void) item_as_behavior;
  }

  It( has_a_name )
  {
    AssertThat( item->name(), Contains( "test item" ) );
  }

  It( registers_itself_in_the_inventory_by_default )
  {
    AssertThat( inventory->items(), HasLength( 1u ) );
    AssertThat( &inventory->items().back().get(), Equals( item ) );
  }

  const int serialized_member_value{ 10 };
  yarrr::Object::Pointer object;
  yarrr::Inventory* inventory;
  yarrr::Item* item;
  yarrr::Shape* shape;
  const yarrr::Tile::Coordinate tile_coordinate{ 10, 0 };
};

