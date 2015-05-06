#include "test_behavior.hpp"
#include <yarrr/object.hpp>
#include <yarrr/object_update.hpp>
#include <yarrr/entity_factory.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( an_object_initializer )
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

    object_update = object->generate_update();

    test::Behavior::number_of_test_behavior_registrations = 0;
    serialize_and_deserialize();
  }

  It( has_the_same_id_as_the_object )
  {
    AssertThat( object_update->id(), Equals( object->id() ) );
  }

  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::BasicObjectUpdate::ctci ), Equals( true ) );
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::ObjectInitializer::ctci ), Equals( true ) );
  }

  It( serializes_and_deserializes_the_id )
  {
    AssertThat( deserialized_update->id(), Equals( object->id() ) );
  }

  It( can_create_new_objects_with_the_same_id )
  {
    AssertThat( recreated_object->id(), Equals( object->id() ) );
  }

  It( can_create_new_objects_with_the_same_behaviors )
  {
    AssertThat(
        test::Behavior::number_of_test_behavior_registrations,
        Equals( number_of_behaviors_to_add ) );
  }

  It( updates_objects_by_updating_the_contained_behaviors )
  {
    object_update->update_object( *object );
    for ( const auto& behavior : test_behaviors )
    {
      AssertThat( behavior->was_updated(), Equals( true ) );
      AssertThat( behavior->updated_with->id(), Equals( behavior->id() ) );
    }
  }

  yarrr::Object::Pointer recreated_object;
  yarrr::Entity::Pointer deserialized_entity;
  const yarrr::ObjectUpdate* deserialized_update;
  yarrr::ObjectUpdate::Pointer object_update;
  yarrr::Object::Pointer object;

  const size_t number_of_behaviors_to_add{ 5 };
  std::vector< const test::Behavior* > test_behaviors;
};

