#include <yarrr/inventory.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( an_invenory )
{
  void SetUp()
  {
    object.reset( new yarrr::Object() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Inventory() ) );
  }

  It( registers_itsef_as_a_component )
  {
    AssertThat( object->components.has_component< yarrr::Inventory >(), Equals( true ) );
  }

  It( lets_behaviors_register_themselves )
  {
    yarrr::Inventory inventory;
    inventory.register_item( inventory );
  }

  It( keeps_a_list_of_registered_items )
  {
    yarrr::Inventory inventory;
    inventory.register_item( inventory );

    const yarrr::Inventory::ItemContainer& items( inventory.items() );
    AssertThat( items.size(), Equals( 1 ) );
    AssertThat( &items.back().get(), Equals( &inventory ) );
  }

  yarrr::Object::Pointer object;
};

