#include <yarrr/inventory.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

#include "test_services.hpp"

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

Describe( a_loot_dropper )
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );
    std::unique_ptr< yarrr::PhysicalBehavior > physical_behavior( new yarrr::PhysicalBehavior() );
    physical_parameters = &physical_behavior->physical_parameters;
    physical_parameters->coordinate = far_from_the_origo;
    object->add_behavior( yarrr::ObjectBehavior::Pointer( physical_behavior.release() ) );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::LootDropper() ) );
    created_object.reset( nullptr );

    the::ctci::service< yarrr::EngineDispatcher >().register_listener< yarrr::ObjectCreated >(
        [ this ]( const yarrr::ObjectCreated& object_created )
        {
          created_object = std::move( object_created.object );
          created_objects_physical_parameters =
            &created_object->components.component< yarrr::PhysicalBehavior >().physical_parameters;
        } );
    object->dispatcher.dispatch( yarrr::ObjectDestroyed() );
  }

  void TearDown()
  {
    test::clean_engine_dispatcher();
  }

  bool objects_were_created()
  {
    return nullptr != created_object.get();
  }

  It( creates_objects_when_owner_is_destroyed )
  {
    AssertThat( objects_were_created(), Equals( true ) );
  }

  It( creates_objects_with_inventory )
  {
    AssertThat( created_object->components.has_component< yarrr::Inventory >(), Equals( true ) );
  }

  It( creates_objects_with_collider )
  {
    AssertThat( created_object->components.has_component< yarrr::Collider >(), Equals( true ) );
  }

  It( creates_objects_close_to_the_owner )
  {
    yarrr::Coordinate coordinate_difference(
        created_objects_physical_parameters->coordinate -
        physical_parameters->coordinate );
    yarrr::huplons_to_metres_in_place( coordinate_difference );
    AssertThat( yarrr::length_of( coordinate_difference ), IsLessThan( 30 ) );
  }

  yarrr::Object::Pointer object;
  yarrr::Object::Pointer created_object;
  yarrr::PhysicalParameters* created_objects_physical_parameters;
  yarrr::PhysicalParameters* physical_parameters;

  const yarrr::Coordinate far_from_the_origo{ 100000, 1000000000 };
};

