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

    item = new test::TestItem( tile_coordinate );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( item ) );
  }

  It( has_tile_coordinates )
  {
    AssertThat( item->tile_coordinate, Equals( tile_coordinate ) );
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

  yarrr::Object::Pointer object;
  yarrr::Inventory* inventory;
  yarrr::Item* item;
  yarrr::Tile::Coordinate tile_coordinate;
};

