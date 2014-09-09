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

Describe(an_object)
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );
    test_behavior = new test::Behavior();
    object->add_behavior( yarrr::ObjectBehavior::Pointer( test_behavior ) );
  }

  It( registers_behavior )
  {
    AssertThat( test_behavior->was_registered, Equals( true ) );
  }

  It( dispatches_events_to_registered_listeners )
  {
    test::Event test_event;
    object->dispatcher.dispatch( test_event );
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

  It( can_update_a_behavior )
  {
    test::Behavior* update_behavior( new test::Behavior( test_behavior->id() ) );
    object->update_behavior( yarrr::ObjectBehavior::Pointer( update_behavior ) );
    AssertThat( test_behavior->was_updated(), Equals( true ) );
    AssertThat( test_behavior->updated_with, Equals( update_behavior ) );
  }

  It( updates_only_behavior_with_matching_id )
  {
    const yarrr::ObjectBehavior::Id not_existing_id( test_behavior->id() + 1 );
    test::Behavior* update_behavior( new test::Behavior( not_existing_id ) );
    object->update_behavior( yarrr::ObjectBehavior::Pointer( update_behavior ) );
    AssertThat( test_behavior->was_updated(), Equals( false ) );
  }

  It( adds_updating_behavior_if_id_is_unknown )
  {
    const yarrr::ObjectBehavior::Id not_existing_id( test_behavior->id() + 1 );
    test::Behavior* new_behavior( new test::Behavior( not_existing_id ) );
    object->update_behavior( yarrr::ObjectBehavior::Pointer( new_behavior ) );
    AssertThat( new_behavior->was_registered, Equals( true ) );
  }

  yarrr::Object::Pointer object;
  test::Behavior* test_behavior;
};

Describe( an_object_behavior )
{
  void SetUp()
  {
    object.reset( new yarrr::Object() );
    test_behavior = new test::Behavior();
    behavior.reset( test_behavior );
    behavior->register_to( *object );
    serialized_data = behavior->serialize();
    deserialized_behavior.reset( new test::Behavior() );
    deserialized_behavior->deserialize( serialized_data );
  }

  It( registers_child_objects )
  {
    AssertThat( test_behavior->was_registered, Equals( true ) );
  }

  It( registers_child_as_a_component )
  {
    AssertThat( object->components.has_component< test::Behavior >(), Equals( true ) );
  }

  It( has_an_id )
  {
    AssertThat( behavior->id(), IsGreaterThan( 0u ) );
  }

  It( serializes_and_deserializes_the_same_id )
  {
    AssertThat( deserialized_behavior->id(), Equals( behavior->id() ) );
  }

  It( calls_child_class_serializer_and_deserializer )
  {
    AssertThat( deserialized_behavior->some_data, Equals( test_behavior->some_data ) );
  }

  yarrr::Data serialized_data;
  yarrr::Object::Pointer object;
  yarrr::ObjectBehavior::Pointer behavior;
  test::Behavior* test_behavior;

  std::unique_ptr< test::Behavior > deserialized_behavior;
};

Describe( an_object_update )
{

  void serialize_and_deserialize()
  {
    yarrr::Data serialized_update( object_update->serialize() );
    deserialized_entity = yarrr::EntityFactory::create( serialized_update );
    deserialized_update = static_cast< const yarrr::ObjectUpdate* >( deserialized_entity.get() );
    recreated_object = deserialized_update->create_object();
  }

  void add_test_behavior()
  {
    test::Behavior* new_behavior( new test::Behavior() );
    test_behaviors.push_back( new_behavior );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new_behavior ) );
  }

  void SetUp()
  {
    assert( yarrr::EntityFactory::is_registered( test::Behavior::ctci ) );
    object.reset( new yarrr::Object() );

    for ( size_t i( 0 ); i < number_of_behaviors_to_add; ++i )
    {
      add_test_behavior();
    }

    test_entity = object->generate_update();
    object_update = static_cast< yarrr::ObjectUpdate* >( test_entity.get() );
    serialize_and_deserialize();
  }

  It( has_the_same_id_as_the_object )
  {
    AssertThat( object_update->id(), Equals( object->id ) );
  }

  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::ObjectUpdate::ctci ), Equals( true ) );
  }

  It( serializes_and_deserializes_the_id )
  {
    AssertThat( deserialized_update->id(), Equals( object->id ) );
  }

  It( can_create_new_objects_with_the_same_id )
  {
    AssertThat( recreated_object->id, Equals( object->id ) );
  }

  It( can_create_new_objects_with_the_same_behaviors )
  {
    test::Behavior* behavior_spy( new test::Behavior() );
    recreated_object->add_behavior( yarrr::ObjectBehavior::Pointer( behavior_spy ) );
    AssertThat(
        behavior_spy->number_of_test_behavior_registrations,
        Equals( number_of_behaviors_to_add + 1u ) );
  }

  It( updates_objects_by_updating_the_contained_behaviors )
  {
    object_update->update_object( *object );
    for ( const auto& behavior : test_behaviors )
    {
      AssertThat( behavior->updated_with->id(), Equals( behavior->id() ) );
    }
  }

  It( should_not_update_non_synchronizable_behaviors )
  {
    test::NonSynchronizableBehavior* nonsynchronizable_behavior(
        new test::NonSynchronizableBehavior() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( nonsynchronizable_behavior ) );
    object->generate_update();
    AssertThat( nonsynchronizable_behavior->was_cloned, Equals( false ) );
  }

  yarrr::Object::Pointer recreated_object;
  yarrr::Entity::Pointer test_entity;
  yarrr::Entity::Pointer deserialized_entity;
  const yarrr::ObjectUpdate* deserialized_update;
  const yarrr::ObjectUpdate* object_update;
  yarrr::Object::Pointer object;

  const size_t number_of_behaviors_to_add{ 5 };
  std::vector< const test::Behavior* > test_behaviors;
};

