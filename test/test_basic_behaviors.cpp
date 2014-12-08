#include "test_services.hpp"
#include "test_synchronizable_behavior.hpp"
#include <yarrr/inventory.hpp>
#include <yarrr/canon.hpp>
#include <yarrr/test_graphical_engine.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/ship_control.hpp>
#include <yarrr/object_container.hpp>
#include <yarrr/dummy_graphical_engine.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/delete_object.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/shape_behavior.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <yarrr/timer_update.hpp>
#include <thectci/dispatcher.hpp>
#include <thectci/component_registry.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_physical_behavior )
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );

    physical_behavior.reset( new yarrr::PhysicalBehavior() );
    physical_behavior->physical_parameters.velocity = { 100, 100 };
    physical_behavior->physical_parameters.coordinate = { 10, 3 };
    physical_behavior->register_to( *object );
    test::assert_that_it_is_a_synchronizable_behavior< yarrr::PhysicalBehavior >();
  }

  It( exposes_writable_physical_parameters )
  {
    yarrr::PhysicalParameters& physical_parameters( physical_behavior->physical_parameters );
    (void) physical_parameters;
  }

  It( updates_local_physical_state_on_timer_update_entity )
  {
    const yarrr::PhysicalParameters old_physical_parameters(
        physical_behavior->physical_parameters );

    object->dispatcher.dispatch( yarrr::TimerUpdate( old_physical_parameters.timestamp + 100000 ) );
    AssertThat( old_physical_parameters, !Equals( physical_behavior->physical_parameters ) );
  }


  It( updates_local_physical_state_when_object_update_arrives )
  {
    const yarrr::PhysicalParameters old_physical_parameters(
        physical_behavior->physical_parameters );

    yarrr::PhysicalBehavior other_physical_behavior;
    physical_behavior->update( other_physical_behavior );

    AssertThat( old_physical_parameters, !Equals( physical_behavior->physical_parameters ) );
    AssertThat( other_physical_behavior.physical_parameters, !Equals( physical_behavior->physical_parameters ) );
  }

  It( can_be_serialized_and_deserialized )
  {
    yarrr::Data serialized_physical_behavior( physical_behavior->serialize() );
    yarrr::Entity::Pointer entity( yarrr::EntityFactory::create( serialized_physical_behavior ) );
    const yarrr::PhysicalBehavior& deserialized_physical_behavior(
        static_cast< const yarrr::PhysicalBehavior& >( *entity ) );
    AssertThat( deserialized_physical_behavior.physical_parameters, Equals( physical_behavior->physical_parameters ) );
  }

  It( can_be_cloned )
  {
    yarrr::ObjectBehavior::Pointer clone( physical_behavior->clone() );
    const yarrr::PhysicalBehavior& clone_physical_behavior(
        static_cast< const yarrr::PhysicalBehavior& >( *clone ) );
    AssertThat( clone_physical_behavior.physical_parameters, Equals( physical_behavior->physical_parameters ) );
  }

  It( has_fast_coordinate_retriever )
  {
    const yarrr::Coordinate new_coordinate{ 123, -32 };
    yarrr::coordinate_of( *object ) = new_coordinate;
    AssertThat( physical_behavior->physical_parameters.coordinate, Equals( new_coordinate ) );
  }

  It( has_fast_velocity_retriever )
  {
    const yarrr::Velocity new_velocity{ 123, -32 };
    yarrr::velocity_of( *object ) = new_velocity;
    AssertThat( physical_behavior->physical_parameters.velocity, Equals( new_velocity ) );
  }

  std::unique_ptr< yarrr::PhysicalBehavior > physical_behavior;
  yarrr::Object::Pointer object;
};

Describe( graphical_behaviors )
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::ShapeBehavior() ) );

    physical_behavior.register_to( *object );

    shape_graphics.reset( new yarrr::ShapeGraphics() );
    shape_graphics->register_to( *object );

    laser_graphics.reset( new yarrr::LaserGraphics() );
    laser_graphics->register_to( *object );

    graphical_engine = static_cast< test::GraphicalEngine* >( &the::ctci::service<yarrr::GraphicalEngine>() );
    graphical_engine->last_focused_to = nullptr;
  }

  It( handles_draw_even_without_registration )
  {
    shape_graphics.reset( new yarrr::ShapeGraphics() );
    shape_graphics->draw();
  }

  It( behaves_like_a_decent_synchronizable_behavior )
  {
    test::assert_that_it_is_a_synchronizable_behavior< yarrr::ShapeGraphics >();
    test::assert_that_it_is_a_synchronizable_behavior< yarrr::LaserGraphics>();
  }

  It( focuses_the_engine_to_the_object_if_it_receives_focus_on_object_event )
  {
    object->dispatcher.dispatch( yarrr::FocusOnObject() );
    AssertThat( graphical_engine->last_focused_to, Equals( object.get() ) );
  }

  It( draws_a_laser_with_the_physical_parameters_of_the_object )
  {
    laser_graphics->draw();
    AssertThat( graphical_engine->last_drawn_laser, Equals( object.get() ) );
  }

  It( draws_object_with_shape )
  {
    AssertThat( graphical_engine->last_drawn_object_with_shape, !Equals( object.get() ) );
    shape_graphics->draw();
    AssertThat( graphical_engine->last_drawn_object_with_shape, Equals( object.get() ) );
  }

  test::GraphicalEngine* graphical_engine;
  yarrr::PhysicalBehavior physical_behavior;

  std::unique_ptr< yarrr::ShapeGraphics > shape_graphics;
  std::unique_ptr< yarrr::LaserGraphics > laser_graphics;
  yarrr::Object::Pointer object;
};

Describe( self_destructor )
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );
    self_destructor.reset( new yarrr::SelfDestructor( object_id, lifespan ) );

    self_destructor->register_to( *object );
    object->dispatcher.dispatch( yarrr::TimerUpdate( now ) );

    deleted_object_id = 0u;
    was_object_deleted = false;

    the::ctci::service< yarrr::EngineDispatcher >().register_listener< yarrr::DeleteObject >(
        [ this ]( const yarrr::DeleteObject& delete_object )
        {
          was_object_deleted = true;
          deleted_object_id = delete_object.object_id();
        } );
  }

  void TearDown()
  {
    test::clean_engine_dispatcher();
  }

  It( is_not_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::SelfDestructor::ctci ), Equals( false ) );
  }

  It( should_not_delete_object_from_the_container_if_lifespan_is_not_exceeded )
  {
    object->dispatcher.dispatch( yarrr::TimerUpdate( now + less_than_the_lifespan ) );
    AssertThat( was_object_deleted, Equals( false ) );
  }

  It( deletes_the_object_from_the_container_if_lifespan_is_exceeded )
  {
    object->dispatcher.dispatch( yarrr::TimerUpdate( now + lifespan ) );
    AssertThat( was_object_deleted, Equals( true ) );
    AssertThat( deleted_object_id, Equals( object_id ) );
  }

  yarrr::Object::Id deleted_object_id;
  bool was_object_deleted;

  std::unique_ptr< yarrr::SelfDestructor > self_destructor;
  yarrr::Object::Pointer object;

  yarrr::Object::Id object_id{ 100u };
  const the::time::Time now{ 100u };
  const the::time::Time lifespan{ 1000u };
  const the::time::Time less_than_the_lifespan{ 500u };
};

