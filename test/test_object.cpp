#include "test_events.hpp"
#include "test_behavior.hpp"
#include "test_remote_object.hpp"

#include <yarrr/object.hpp>
#include <yarrr/object_update.hpp>
#include <yarrr/object_creator.hpp>
#include <yarrr/entity.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/canon.hpp>
#include <yarrr/shape_behavior.hpp>

#include <thectci/dispatcher.hpp>
#include <thectci/component_registry.hpp>
#include <thetime/timer.hpp>

#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{

yarrr::AutoEntityRegister< test::Behavior > register_test_behavior;

}

Describe(the_component_of)
{
  It( returns_the_reference_of_the_requested_component )
  {
    yarrr::Object object;
    test::Behavior* behavior( new test::Behavior() );
    object.add_behavior( yarrr::ObjectBehavior::Pointer( behavior ) );
    AssertThat( &yarrr::component_of< test::Behavior >( object ), Equals( behavior ) );
  }
};

Describe(the_has_component)
{
  It( checks_if_an_object_has_a_component )
  {
    yarrr::Object object;
    AssertThat( yarrr::has_component< test::Behavior >( object ), Equals( false ) );
    object.add_behavior( yarrr::ObjectBehavior::Pointer( new test::Behavior() ) );
    AssertThat( yarrr::has_component< test::Behavior >( object ), Equals( true ) );
  }
};

Describe(an_object)
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );
    rarely_synchronized_behavior = new test::Behavior( yarrr::synchronize_nth( 200 ) );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( rarely_synchronized_behavior ) );

    always_synchronized_behavior = new test::Behavior( yarrr::always_synchronize() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( always_synchronized_behavior ) );

    not_existing_behavior_id = rarely_synchronized_behavior->id() - 1;
  }

  It( registers_behavior )
  {
    AssertThat( rarely_synchronized_behavior->was_registered, Equals( true ) );
  }


  It( dispatches_events_to_registered_listeners )
  {
    test::Event test_event;
    object->dispatcher.dispatch( test_event );
    AssertThat( rarely_synchronized_behavior->dispatched_event, Equals( &test_event ) );
  }

  It( can_be_constructed_with_a_specific_id )
  {
    yarrr::Object one_object( 123u );
    AssertThat( one_object.id(), Equals( 123u ) );
  }

  It( can_update_a_behavior )
  {
    test::Behavior* update_behavior( new test::Behavior( rarely_synchronized_behavior->id() ) );
    object->update_behavior( yarrr::ObjectBehavior::Pointer( update_behavior ) );
    AssertThat( rarely_synchronized_behavior->was_updated(), Equals( true ) );
    AssertThat( rarely_synchronized_behavior->updated_with, Equals( update_behavior ) );
  }

  It( updates_only_behavior_with_matching_id )
  {
    test::Behavior* update_behavior( new test::Behavior( not_existing_behavior_id ) );
    object->update_behavior( yarrr::ObjectBehavior::Pointer( update_behavior ) );
    AssertThat( rarely_synchronized_behavior->was_updated(), Equals( false ) );
  }

  It( adds_updating_behavior_if_id_is_unknown )
  {
    test::Behavior* new_behavior( new test::Behavior( not_existing_behavior_id ) );
    object->update_behavior( yarrr::ObjectBehavior::Pointer( new_behavior ) );
    AssertThat( new_behavior->was_registered, Equals( true ) );
  }

  It( generates_object_initializer_as_first_update )
  {
    yarrr::ObjectUpdate::Pointer update( object->generate_update() );
    AssertThat( update->polymorphic_ctci(), Equals( yarrr::ObjectInitializer::ctci ) );
  }

  It( puts_all_behaviors_in_an_object_initializer )
  {
    yarrr::ObjectUpdate::Pointer update( object->generate_update() );
    AssertThat( rarely_synchronized_behavior->was_cloned, Equals( true ) );
    AssertThat( always_synchronized_behavior->was_cloned, Equals( true ) );
  }

  It( generates_object_updater_as_second_update )
  {
    object->generate_update();
    yarrr::ObjectUpdate::Pointer update( object->generate_update() );
    AssertThat( update->polymorphic_ctci(), Equals( yarrr::BasicObjectUpdate::ctci ) );
  }

  It( puts_only_behaviors_needing_synchronization_to_the_update )
  {
    object->generate_update();
    rarely_synchronized_behavior->was_cloned = false;
    always_synchronized_behavior->was_cloned = false;
    yarrr::ObjectUpdate::Pointer update( object->generate_update() );
    AssertThat( rarely_synchronized_behavior->was_cloned, Equals( false ) );
    AssertThat( always_synchronized_behavior->was_cloned, Equals( true ) );
  }

  It( can_be_cloned )
  {
    yarrr::Object::Pointer object_clone( object->clone() );
  }

  It( clones_object_id )
  {
    yarrr::Object::Pointer object_clone( object->clone() );
    AssertThat( object_clone->id(), Equals( object->id() ) );
  }

  It( clones_behaviors_when_cloned )
  {
    yarrr::Object::Pointer object_clone( object->clone() );
    AssertThat( rarely_synchronized_behavior->was_cloned, Equals( true ) );
    AssertThat( always_synchronized_behavior->was_cloned, Equals( true ) );
    AssertThat( yarrr::has_component< test::Behavior >( *object_clone ), Equals( true ) );
  }

  yarrr::Object::Pointer object;
  test::Behavior* rarely_synchronized_behavior;
  test::Behavior* always_synchronized_behavior;
  yarrr::ObjectBehavior::Id not_existing_behavior_id;
};

Describe( ship_synchronization_procedure )
{
  void SetUp()
  {
    original_object = yarrr::create_ship();
    original_inventory = &yarrr::component_of< yarrr::Inventory >( *original_object );

    synchronized_object = test::create_remote_object_from( *original_object );
    AssertThat( yarrr::has_component< yarrr::ShapeBehavior >( *synchronized_object ), Equals( true ) );
    AssertThat( yarrr::has_component< yarrr::Inventory >( *synchronized_object ), Equals( true ) );

    synchronized_inventory = &yarrr::component_of< yarrr::Inventory >( *synchronized_object );

    number_of_initial_items = original_inventory->items().size();
  }

  It( creates_synchronized_ship_with_the_same_items )
  {
    AssertThat( synchronized_inventory->items().size(), Equals( original_inventory->items().size() ) );
  }

  It( adds_new_behaviors_during_an_update_procedure )
  {
    original_object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Canon() ) );
    AssertThat( original_inventory->items().size(), Equals( number_of_initial_items + 1 ) );
    auto object_update( original_object->generate_update() );

    object_update->update_object( *synchronized_object );
    AssertThat( synchronized_inventory->items().size(), Equals( number_of_initial_items + 1 ) );
  }

  size_t number_of_initial_items;

  yarrr::Object::Pointer original_object;
  yarrr::Inventory* original_inventory;

  yarrr::Object::Pointer synchronized_object;
  yarrr::Inventory* synchronized_inventory;
};


