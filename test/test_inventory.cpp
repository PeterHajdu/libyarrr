#include <yarrr/inventory.hpp>
#include <yarrr/object.hpp>

#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

#include "test_synchronizable_behavior.hpp"
#include "test_services.hpp"
#include "test_item.hpp"

using namespace igloo;

Describe( an_invenory )
{
  void SetUp()
  {
    object.reset( new yarrr::Object() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Inventory() ) );

    inventory.reset( new yarrr::Inventory() );
    item.reset( new test::Item() );
  }

  It( is_synchronizable )
  {
    test::assert_that_it_is_a_synchronizable_behavior< yarrr::Inventory >();
  }

  It( lets_items_register_themselves )
  {
    inventory->register_item( *item );
  }

  It( keeps_a_list_of_registered_items )
  {
    inventory->register_item( *item );

    const yarrr::Inventory::ItemContainer& items( inventory->items() );
    AssertThat( items.size(), Equals( 1u ) );
    AssertThat( &items.back().get(), Equals( item.get() ) );
  }

  It( should_force_synchronization_if_an_item_is_added )
  {
    AssertThat( inventory->should_synchronize(), Equals( true ) );
    inventory->register_item( *item );
    AssertThat( inventory->should_synchronize(), Equals( true ) );
  }

  yarrr::Object::Pointer object;
  std::unique_ptr< yarrr::Inventory > inventory;
  std::unique_ptr< test::Item > item;
};

