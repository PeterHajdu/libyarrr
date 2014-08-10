#include "test_events.hpp"
#include "test_behavior.hpp"
#include <yarrr/object_container.hpp>
#include <yarrr/object.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(an_object_container)
{

  void add_object_with_id( yarrr::Object::Id id )
  {
    yarrr::Object::Pointer new_object( new yarrr::Object( id ) );

    test::Behavior* new_behavior(
          new test::Behavior(
            [ this, id ]()
            {
              deleted_objects.push_back( id );
            } ) );
    behaviors.emplace( id, new_behavior );

    new_object->add_behavior( yarrr::ObjectBehavior::Pointer( new_behavior ) );
    test_container->add_object( std::move( new_object ) );
  }

  void SetUp()
  {
    behaviors.clear();
    test_container.reset( new yarrr::ObjectContainer() );
    deleted_objects.clear();

    add_object_with_id( first_id );
    add_object_with_id( second_id );
  }

  It( stores_objects )
  {
    AssertThat( deleted_objects, IsEmpty() );
  }

  It( can_delete_objects_by_id )
  {
    test_container->delete_object( first_id );
    AssertThat( deleted_objects, Contains( first_id ) );
    AssertThat( deleted_objects, !Contains( second_id ) );
  }

  It( handles_invalid_id_deletion )
  {
    test_container->delete_object( invalid_id );
  }

  bool was_event_dispatched_to( yarrr::Object::Id id )
  {
    return behaviors[ id ]->dispatched_event == &test_event;
  }

  It( dispatches_events_to_all_objects )
  {
    test_container->dispatch( test_event );
    AssertThat( was_event_dispatched_to( first_id ), Equals( true ) );
    AssertThat( was_event_dispatched_to( second_id ), Equals( true ) );
  }

  It( does_not_dispatch_to_deleted_objects )
  {
    test_container->delete_object( first_id );
    test_container->dispatch( test_event );
    AssertThat( was_event_dispatched_to( second_id ), Equals( true ) );
  }

  It( can_tell_if_it_contains_an_object_or_not )
  {
    AssertThat( test_container->has_object_with_id( second_id ), Equals( true ) );
    AssertThat( test_container->has_object_with_id( invalid_id ), Equals( false ) );
  }

  It( can_retrieve_an_object_with_a_given_id )
  {
    test_container->object_with_id( second_id ).dispatcher.dispatch( test_event );
    AssertThat( was_event_dispatched_to( second_id ), Equals( true ) );
  }

  It( can_tell_the_number_of_owned_objects )
  {
    AssertThat( test_container->size(), Equals( 2u ) );
  }

  bool contains_update_for( yarrr::Object::Id id, const std::vector< yarrr::ObjectUpdate::Pointer >& updates )
  {
    return std::any_of(
        std::begin( updates ), std::end( updates ),
        [ id ]( const yarrr::ObjectUpdate::Pointer& update )
        {
          return update->id() == id;
        } );
  }

  It( can_generates_object_updates )
  {
    std::vector< yarrr::ObjectUpdate::Pointer > object_updates( test_container->generate_object_updates() );
    AssertThat( object_updates, HasLength( test_container->size() ) );
    AssertThat( contains_update_for( first_id, object_updates ), Equals( true ) );
    AssertThat( contains_update_for( second_id, object_updates ), Equals( true ) );
  }

  It( creates_an_object_for_an_object_update_with_new_id )
  {
    const yarrr::ObjectUpdate dummy_object_update( invalid_id, yarrr::BehaviorContainer() );
    test_container->handle_object_update( dummy_object_update );
    AssertThat( test_container->has_object_with_id( invalid_id ), Equals( true ) );
  }

  yarrr::ObjectUpdate::Pointer create_object_update_for( yarrr::Object::Id id )
  {
    yarrr::ObjectUpdate::Pointer object_update(
        test_container->object_with_id( id ).generate_update() );
    return object_update;
  }

  It( updates_the_correct_object_if_object_update_id_is_found )
  {
    yarrr::ObjectUpdate::Pointer dummy_object_update( create_object_update_for( second_id ) );
    test_container->handle_object_update( *dummy_object_update );
    AssertThat( behaviors[ second_id ]->was_object_updated, Equals( true ) );
    AssertThat( behaviors[ first_id ]->was_object_updated, Equals( false ) );
  }

  std::unordered_map< yarrr::Object::Id, const test::Behavior* > behaviors;

  test::Event test_event;
  std::vector< yarrr::Object::Id > deleted_objects;
  const yarrr::Object::Id first_id{ 1 };
  const yarrr::Object::Id second_id{ 2 };
  const yarrr::Object::Id invalid_id{ 20 };
  std::unique_ptr< yarrr::ObjectContainer > test_container;
};

