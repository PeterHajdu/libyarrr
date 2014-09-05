#include "test_services.hpp"
#include <yarrr/inventory.hpp>
#include <yarrr/test_graphical_engine.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/engine.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/command.hpp>
#include <yarrr/object_container.hpp>
#include <yarrr/dummy_graphical_engine.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/delete_object.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/destruction_handlers.hpp>
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
  }

  It( registers_itself_as_a_component )
  {
    AssertThat( &object->components.component<yarrr::PhysicalBehavior>(), Equals( physical_behavior.get() ) );
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
    object->dispatcher.dispatch( other_physical_behavior );

    AssertThat( old_physical_parameters, !Equals( physical_behavior->physical_parameters ) );
    AssertThat( other_physical_behavior.physical_parameters, !Equals( physical_behavior->physical_parameters ) );
  }

  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::PhysicalBehavior::ctci ), Equals( true ) );
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

  std::unique_ptr< yarrr::PhysicalBehavior > physical_behavior;
  yarrr::Object::Pointer object;
};

Describe( a_canon )
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Inventory() ) );
    inventory = &object->components.component< yarrr::Inventory >();

    physical_behavior.register_to( *object );
    canon.reset( new yarrr::Canon() );
    canon->register_to( *object );

    was_canon_fired = false;

    the::ctci::service< yarrr::EngineDispatcher >().register_listener< yarrr::ObjectCreated >(
          [ this ]( const yarrr::ObjectCreated& )
          {
            was_canon_fired = true;
          } );
  }

  void TearDown()
  {
    test::clean_engine_dispatcher();
  }

  It( creates_objects_only_for_fire_command )
  {
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::cw, 0 ) );
    AssertThat( was_canon_fired, Equals( false ) );
  }

  It( creates_new_objects_when_canon_is_fired )
  {
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::fire, 0 ) );
    AssertThat( was_canon_fired, Equals( true ) );
  }

  It( registeres_itself_to_the_inventory )
  {
    AssertThat( inventory->items(), HasLength( 1 ) );
    AssertThat( &inventory->items().back().get(), Equals( canon.get() ) );
  }

  bool was_canon_fired;

  yarrr::Inventory* inventory;
  yarrr::PhysicalBehavior physical_behavior;
  std::unique_ptr< yarrr::Canon > canon;

  yarrr::Object::Pointer object;
};


Describe( graphical_behaviors )
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );

    physical_behavior.register_to( *object );

    ship_graphics.reset( new yarrr::ShipGraphics() );
    ship_graphics->register_to( *object );

    laser_graphics.reset( new yarrr::LaserGraphics() );
    laser_graphics->register_to( *object );

    graphical_engine = static_cast< test::GraphicalEngine* >( &the::ctci::service<yarrr::GraphicalEngine>() );
    graphical_engine->last_focused_to = physical_behavior.physical_parameters.coordinate + yarrr::Coordinate( 10, 10 );
  }


  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::ShipGraphics::ctci ), Equals( true ) );
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::LaserGraphics::ctci ), Equals( true ) );
  }

  It( focuses_the_engine_to_the_object_if_it_receives_focus_on_object_event )
  {
    AssertThat( graphical_engine->last_focused_to, !Equals( physical_behavior.physical_parameters.coordinate ) );
    object->dispatcher.dispatch( yarrr::FocusOnObject() );
    AssertThat( graphical_engine->last_focused_to, Equals( physical_behavior.physical_parameters.coordinate ) );
  }


  It( draws_a_ship_with_the_physical_parameters_of_the_object )
  {
    AssertThat( graphical_engine->last_drawn_ship, !Equals( physical_behavior.physical_parameters ) );
    ship_graphics->draw();
    AssertThat( graphical_engine->last_drawn_ship, Equals( physical_behavior.physical_parameters ) );
  }

  It( draws_a_laser_with_the_physical_parameters_of_the_object )
  {
    AssertThat( graphical_engine->last_drawn_laser, !Equals( physical_behavior.physical_parameters ) );
    laser_graphics->draw();
    AssertThat( graphical_engine->last_drawn_laser, Equals( physical_behavior.physical_parameters ) );
  }

  test::GraphicalEngine* graphical_engine;
  yarrr::PhysicalBehavior physical_behavior;

  std::unique_ptr< yarrr::ShipGraphics > ship_graphics;
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

Describe( ship_creator )
{

  void SetUp()
  {
    object = yarrr::create_ship();
  }

  It ( creates_objects_with_physical_parameters )
  {
    AssertThat( object->components.has_component< yarrr::PhysicalBehavior >(), Equals( true ) );
  }

  It ( creates_objects_with_an_engine )
  {
    AssertThat( object->components.has_component< yarrr::Engine >(), Equals( true ) );
  }

  It ( creates_objects_with_ship_graphics )
  {
    AssertThat( object->components.has_component< yarrr::ShipGraphics >(), Equals( true ) );
  }

  It ( creates_objects_with_a_canon )
  {
    AssertThat( object->components.has_component< yarrr::Canon >(), Equals( true ) );
  }

  It ( creates_objects_with_a_collider )
  {
    AssertThat( object->components.has_component< yarrr::Collider >(), Equals( true ) );
  }

  It ( creates_objects_with_a_damage_causer )
  {
    AssertThat( object->components.has_component< yarrr::DamageCauser >(), Equals( true ) );
  }

  It ( creates_objects_with_respawn_when_destroyed )
  {
    AssertThat( object->components.has_component< yarrr::RespawnWhenDestroyed >(), Equals( true ) );
  }

  It ( creates_objects_with_inventory )
  {
    AssertThat( object->components.has_component< yarrr::Inventory >(), Equals( true ) );
  }

  It ( creates_objects_with_loot_dropper )
  {
    AssertThat( object->components.has_component< yarrr::LootDropper >(), Equals( true ) );
  }

  yarrr::Object::Pointer object;
};

Describe( laser_creator )
{

  void SetUp()
  {
    ships_physical_parameters.vangle = 100;
    object = yarrr::create_laser( ships_physical_parameters );

    AssertThat( object->components.has_component< yarrr::PhysicalBehavior >(), Equals( true ) );
    laser_parameters = object->components.component<yarrr::PhysicalBehavior>().physical_parameters;
  }

  It ( creates_objects_with_laser_graphics )
  {
    AssertThat( object->components.has_component< yarrr::LaserGraphics >(), Equals( true ) );
  }

  It ( creates_objects_with_selfdestructor )
  {
    AssertThat( object->components.has_component< yarrr::SelfDestructor >(), Equals( true ) );
  }

  It ( creates_objects_that_do_not_spin )
  {
    AssertThat( laser_parameters.vangle, Equals( 0 ) );
  }

  It ( creates_objects_that_move_with_a_different_velocity )
  {
    AssertThat( laser_parameters.velocity, !Equals( ships_physical_parameters.velocity ) );
  }

  It ( creates_objects_with_a_collider )
  {
    AssertThat( object->components.has_component< yarrr::Collider >(), Equals( true ) );
  }

  It ( creates_objects_with_a_damage_causer )
  {
    AssertThat( object->components.has_component< yarrr::DamageCauser >(), Equals( true ) );
  }

  It ( creates_objects_with_delete_when_destroyed )
  {
    AssertThat( object->components.has_component< yarrr::DeleteWhenDestroyed >(), Equals( true ) );
  }

  yarrr::PhysicalParameters ships_physical_parameters;
  yarrr::PhysicalParameters laser_parameters;
  yarrr::Object::Pointer object;
};

