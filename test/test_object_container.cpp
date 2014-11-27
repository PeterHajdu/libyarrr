#include "test_events.hpp"
#include "test_behavior.hpp"
#include "test_services.hpp"
#include <yarrr/object_container.hpp>
#include <yarrr/object.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/delete_object.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <yarrr/shape_behavior.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(an_object_container)
{

  void add_object_with_id( yarrr::Object::Id id )
  {
    yarrr::Object::Pointer new_object( new yarrr::Object( id ) );

    test::Behavior* new_behavior(
          new test::Behavior(
            yarrr::always_synchronize(),
            [ this, id ]()
            {
              deleted_objects.push_back( id );
            } ) );
    behaviors.emplace( id, new_behavior );

    new_object->add_behavior( yarrr::ObjectBehavior::Pointer( new_behavior ) );
    container->add_object( std::move( new_object ) );
  }

  void SetUp()
  {
    behaviors.clear();
    container.reset( new yarrr::ObjectContainer() );
    deleted_objects.clear();

    add_object_with_id( first_id );
    add_object_with_id( second_id );
  }

  It( exposes_the_object_container )
  {
    const yarrr::ObjectContainer& const_container( *container );
    const yarrr::ObjectContainer::Objects& objects( const_container.objects() );
    AssertThat( objects, HasLength( 2u ) );
  }

  It( stores_objects )
  {
    AssertThat( deleted_objects, IsEmpty() );
  }

  It( can_delete_objects_by_id )
  {
    container->delete_object( first_id );
    AssertThat( deleted_objects, Contains( first_id ) );
    AssertThat( deleted_objects, !Contains( second_id ) );
  }

  It( handles_invalid_id_deletion )
  {
    container->delete_object( invalid_id );
  }

  bool was_event_dispatched_to( yarrr::Object::Id id )
  {
    return behaviors[ id ]->dispatched_event == &test_event;
  }

  It( dispatches_events_to_all_objects )
  {
    container->dispatch( test_event );
    AssertThat( was_event_dispatched_to( first_id ), Equals( true ) );
    AssertThat( was_event_dispatched_to( second_id ), Equals( true ) );
  }

  It( does_not_dispatch_to_deleted_objects )
  {
    container->delete_object( first_id );
    container->dispatch( test_event );
    AssertThat( was_event_dispatched_to( second_id ), Equals( true ) );
  }

  It( can_tell_if_it_contains_an_object_or_not )
  {
    AssertThat( container->has_object_with_id( second_id ), Equals( true ) );
    AssertThat( container->has_object_with_id( invalid_id ), Equals( false ) );
  }

  It( can_retrieve_an_object_with_a_given_id )
  {
    container->object_with_id( second_id ).dispatcher.dispatch( test_event );
    AssertThat( was_event_dispatched_to( second_id ), Equals( true ) );
  }

  It( can_tell_the_number_of_owned_objects )
  {
    AssertThat( container->size(), Equals( 2u ) );
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

  It( can_generate_object_updates )
  {
    std::vector< yarrr::ObjectUpdate::Pointer > object_updates( container->generate_object_updates() );
    AssertThat( object_updates, HasLength( container->size() ) );
    AssertThat( contains_update_for( first_id, object_updates ), Equals( true ) );
    AssertThat( contains_update_for( second_id, object_updates ), Equals( true ) );
  }

  It( creates_an_object_for_an_object_update_with_new_id )
  {
    const yarrr::BasicObjectUpdate dummy_object_update( invalid_id, yarrr::BehaviorContainer() );
    container->handle_object_update( dummy_object_update );
    AssertThat( container->has_object_with_id( invalid_id ), Equals( true ) );
  }

  yarrr::ObjectUpdate::Pointer create_object_update_for( yarrr::Object::Id id )
  {
    yarrr::ObjectUpdate::Pointer object_update(
        container->object_with_id( id ).generate_update() );
    return object_update;
  }

  It( updates_the_correct_object_if_object_update_id_is_found )
  {
    yarrr::ObjectUpdate::Pointer dummy_object_update( create_object_update_for( second_id ) );
    container->handle_object_update( *dummy_object_update );
    AssertThat( behaviors[ second_id ]->was_updated(), Equals( true ) );
    AssertThat( behaviors[ first_id ]->was_updated(), Equals( false ) );
  }

  std::unordered_map< yarrr::Object::Id, const test::Behavior* > behaviors;

  test::Event test_event;
  std::vector< yarrr::Object::Id > deleted_objects;
  const yarrr::Object::Id first_id{ 1 };
  const yarrr::Object::Id second_id{ 2 };
  const yarrr::Object::Id invalid_id{ 20 };
  std::unique_ptr< yarrr::ObjectContainer > container;

};

namespace
{

yarrr::Object::Pointer
create_test_laser( int layer, const yarrr::PhysicalParameters& physical_parameters )
{
  yarrr::Object::Pointer laser( new yarrr::Object() );
  laser->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::PhysicalBehavior( physical_parameters ) ) );
  laser->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::ShapeBehavior() ) );
  yarrr::component_of< yarrr::ShapeBehavior >( *laser ).shape.add_tile( yarrr::Tile{ { 0, 0 }, { 0, 0 } } );

  laser->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Collider( layer ) ) );
  laser->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::DamageCauser( 10 ) ) );
  laser->add_behavior( yarrr::delete_when_destroyed() );
  return laser;
}

}

Describe( object_container_collision_checks )
{

  void add_number_of_objects( size_t n )
  {
    for ( size_t i( 0 ); i < n; ++i )
    {
      yarrr::Object::Pointer laser( create_test_laser( i, physical_parameters ) );
      created_objects.push_back( laser->id() );
      container->add_object( std::move( laser ) );
    }
  }

  void SetUp()
  {
    container.reset( new yarrr::ObjectContainer() );
    created_objects.clear();
    deleted_objects.clear();

    the::ctci::service< yarrr::EngineDispatcher >().register_listener< yarrr::DeleteObject >(
        [ this ]( const yarrr::DeleteObject& delete_object )
        {
          deleted_objects.push_back( delete_object.object_id() );
        } );
  }

  void TearDown()
  {
    test::clean_engine_dispatcher();
  }

  void assert_deleted_all_created()
  {
    for ( const auto& object : created_objects )
    {
      AssertThat( deleted_objects, Contains( object ) );
    }
  }

  It( can_check_object_collision )
  {
    add_number_of_objects( 3 );
    container->check_collision();
    assert_deleted_all_created();
  }

  It( should_not_collide_object_without_a_collider )
  {
    add_number_of_objects( 3 );
    container->add_object( yarrr::Object::Pointer( new yarrr::Object() ) );
    container->check_collision();
    assert_deleted_all_created();
  }

  It( should_not_collide_an_object_with_itself )
  {
    add_number_of_objects( 1 );
    container->add_object( yarrr::Object::Pointer( new yarrr::Object() ) );
    container->check_collision();
    AssertThat( deleted_objects, IsEmpty() );
  }

  std::vector< yarrr::Object::Id > created_objects;
  std::vector< yarrr::Object::Id > deleted_objects;
  yarrr::PhysicalParameters physical_parameters;
  std::unique_ptr< yarrr::ObjectContainer > container;
};

