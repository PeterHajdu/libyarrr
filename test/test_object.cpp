#include "test_events.hpp"
#include "test_behavior.hpp"
#include <yarrr/object.hpp>
#include <yarrr/entity.hpp>
#include <yarrr/entity_factory.hpp>
#include <thectci/dispatcher.hpp>
#include <thectci/component_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{

yarrr::AutoEntityRegister< test::Behavior > register_test_behavior;

}

Describe(an_object)
{

  void SetUp()
  {
    test_object.reset( new yarrr::Object() );
    test_behavior = new test::Behavior();
    test_object->add_behavior( yarrr::ObjectBehavior::Pointer( test_behavior ) );
  }

  It( registers_behavior )
  {
    AssertThat( test_behavior->was_registered, Equals( true ) );
  }

  It( dispatches_events_to_registered_listeners )
  {
    test::Event test_event;
    test_object->dispatcher.dispatch( test_event );
    AssertThat( test_behavior->dispatched_event, Equals( &test_event ) );
  }

  It( is_identified_by_its_pointer_if_default_constructed )
  {
    yarrr::Object one_object;
    AssertThat( one_object.id, Equals( yarrr::Object::Id( &one_object ) ) );
  }

  It( can_be_constructed_with_a_specific_id )
  {
    yarrr::Object one_object( 123u );
    AssertThat( one_object.id, Equals( 123u ) );
  }

  yarrr::Object::Pointer test_object;
  test::Behavior* test_behavior;
};

Describe( an_object_update )
{

  void serialize_and_deserialize()
  {
    yarrr::Data serialized_update( test_update->serialize() );
    deserialized_entity = yarrr::EntityFactory::create( serialized_update );
    deserialized_update = static_cast< const yarrr::ObjectUpdate* >( deserialized_entity.get() );
    recreated_object = deserialized_update->create_object();
  }

  void add_test_behavior()
  {
    test::Behavior* new_behavior( new test::Behavior() );
    test_behaviors.push_back( new_behavior );
    test_object->add_behavior( yarrr::ObjectBehavior::Pointer( new_behavior ) );
  }

  void SetUp()
  {
    assert( yarrr::EntityFactory::is_registered( test::Behavior::ctci ) );
    test_object.reset( new yarrr::Object() );

    for ( size_t i( 0 ); i < number_of_behaviors_to_add; ++i )
    {
      add_test_behavior();
    }

    test_entity = test_object->generate_update();
    test_update = static_cast< yarrr::ObjectUpdate* >( test_entity.get() );
    serialize_and_deserialize();
  }

  It( has_the_same_id_as_the_object )
  {
    AssertThat( test_update->id(), Equals( test_object->id ) );
  }

  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::ObjectUpdate::ctci ), Equals( true ) );
  }

  It( serializes_and_deserializes_the_id )
  {
    AssertThat( deserialized_update->id(), Equals( test_object->id ) );
  }

  It( can_create_new_objects_with_the_same_id )
  {
    AssertThat( recreated_object->id, Equals( test_object->id ) );
  }

  It( can_create_new_objects_with_the_same_behaviors )
  {
    test::Behavior* behavior_spy( new test::Behavior() );
    recreated_object->add_behavior( yarrr::ObjectBehavior::Pointer( behavior_spy ) );
    AssertThat( behavior_spy->number_of_test_behavior_registrations, Equals( number_of_behaviors_to_add + 1u ) );
  }

  It( can_dispatch_behaviors_as_events_on_an_object )
  {
    const test::Behavior& one_of_the_behaviors( *test_behaviors.back() );
    test_update->update_object( *test_object );
    AssertThat( one_of_the_behaviors.number_of_behaviors_dispatched, Equals( number_of_behaviors_to_add ) );
  }

  yarrr::Object::Pointer recreated_object;
  yarrr::Entity::Pointer test_entity;
  yarrr::Entity::Pointer deserialized_entity;
  const yarrr::ObjectUpdate* deserialized_update;
  const yarrr::ObjectUpdate* test_update;
  yarrr::Object::Pointer test_object;

  const size_t number_of_behaviors_to_add{ 5 };
  std::vector< const test::Behavior* > test_behaviors;
};

