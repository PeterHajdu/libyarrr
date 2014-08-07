#include <yarrr/basic_behaviors.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/command.hpp>
#include <yarrr/object_container.hpp>
#include <yarrr/dummy_graphical_engine.hpp>
#include <yarrr/entity_factory.hpp>
#include <thectci/dispatcher.hpp>
#include <thectci/component_registry.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

#include "test_graphical_engine.hpp"

using namespace igloo;

namespace
{
  the::ctci::AutoServiceRegister< yarrr::GraphicalEngine, test::GraphicalEngine > test_graphical_engine_register;
}

Describe( a_physical_behavior )
{

  void SetUp()
  {
    test_dispatcher.reset( new the::ctci::Dispatcher() );
    test_registry.reset( new the::ctci::ComponentRegistry() );

    physical_behavior.reset( new yarrr::PhysicalBehavior() );
    physical_behavior->physical_parameters.velocity = { 100, 100 };
    physical_behavior->physical_parameters.coordinate = { 10, 3 };
    physical_behavior->register_to( *test_dispatcher, *test_registry );
  }

  It( registers_itself_as_a_component )
  {
    AssertThat( &test_registry->component<yarrr::PhysicalBehavior>(), Equals( physical_behavior.get() ) );
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

    test_dispatcher->dispatch( yarrr::TimerUpdate( old_physical_parameters.timestamp + 100000 ) );
    AssertThat( old_physical_parameters, !Equals( physical_behavior->physical_parameters ) );
  }


  It( updates_local_physical_state_when_object_update_arrives )
  {
    const yarrr::PhysicalParameters old_physical_parameters(
        physical_behavior->physical_parameters );

    yarrr::PhysicalBehavior other_physical_behavior;
    test_dispatcher->dispatch( other_physical_behavior );

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
  std::unique_ptr< the::ctci::Dispatcher > test_dispatcher;
  std::unique_ptr< the::ctci::ComponentRegistry > test_registry;
};

Describe( an_engine )
{

  void SetUp()
  {
    test_dispatcher.reset( new the::ctci::Dispatcher() );
    test_registry.reset( new the::ctci::ComponentRegistry() );

    physical_behavior.register_to( *test_dispatcher, *test_registry );
    engine.register_to( *test_dispatcher, *test_registry );
  }


  It( updates_local_physical_state_when_a_command_arrives )
  {
    const yarrr::PhysicalParameters old_physical_parameters(
        physical_behavior.physical_parameters );

    test_dispatcher->dispatch( yarrr::Command( yarrr::Command::thruster, 0 ) );

    AssertThat( old_physical_parameters, !Equals( physical_behavior.physical_parameters ) );
  }


  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::Engine::ctci ), Equals( true ) );
  }


  yarrr::PhysicalBehavior physical_behavior;
  yarrr::Engine engine;

  std::unique_ptr< the::ctci::Dispatcher > test_dispatcher;
  std::unique_ptr< the::ctci::ComponentRegistry > test_registry;
};

Describe( a_canon )
{

  void SetUp()
  {
    test_dispatcher.reset( new the::ctci::Dispatcher() );
    test_registry.reset( new the::ctci::ComponentRegistry() );

    object_container.reset( new yarrr::ObjectContainer() );
    canon.reset( new yarrr::Canon( *object_container ) );
    canon->register_to( *test_dispatcher, *test_registry );
  }

  It( creates_objects_only_for_fire_command )
  {
    test_dispatcher->dispatch( yarrr::Command( yarrr::Command::cw, 0 ) );
    AssertThat( *object_container, IsEmpty() );
  }

  It( creates_new_objects )
  {
    test_dispatcher->dispatch( yarrr::Command( yarrr::Command::fire, 0 ) );
    AssertThat( *object_container, HasLength( 1 ) );
  }

  std::unique_ptr< yarrr::ObjectContainer > object_container;
  std::unique_ptr< yarrr::Canon > canon;

  std::unique_ptr< the::ctci::Dispatcher > test_dispatcher;
  std::unique_ptr< the::ctci::ComponentRegistry > test_registry;
};


Describe( a_graphical_behavior )
{

  void SetUp()
  {
    test_dispatcher.reset( new the::ctci::Dispatcher() );
    test_registry.reset( new the::ctci::ComponentRegistry() );

    physical_behavior.register_to( *test_dispatcher, *test_registry );
    graphical_behavior.register_to( *test_dispatcher, *test_registry );

    graphical_engine = static_cast< test::GraphicalEngine* >( &the::ctci::service<yarrr::GraphicalEngine>() );
    graphical_engine->last_focused_to = physical_behavior.physical_parameters.coordinate + yarrr::Coordinate( 10, 10 );
  }


  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::GraphicalBehavior::ctci ), Equals( true ) );
  }

  It( focuses_the_engine_to_the_object_if_it_receives_focus_on_object_event )
  {
    AssertThat( graphical_engine->last_focused_to, !Equals( physical_behavior.physical_parameters.coordinate ) );
    test_dispatcher->dispatch( yarrr::FocusOnObject() );
    AssertThat( graphical_engine->last_focused_to, Equals( physical_behavior.physical_parameters.coordinate ) );
  }


  It( draws_a_ship_with_the_physical_parameters_of_the_object )
  {
    AssertThat( graphical_engine->last_drawn_ship, !Equals( physical_behavior.physical_parameters ) );
    graphical_behavior.draw();
    AssertThat( graphical_engine->last_drawn_ship, Equals( physical_behavior.physical_parameters ) );
  }

  test::GraphicalEngine* graphical_engine;
  yarrr::PhysicalBehavior physical_behavior;
  yarrr::GraphicalBehavior graphical_behavior;

  std::unique_ptr< the::ctci::Dispatcher > test_dispatcher;
  std::unique_ptr< the::ctci::ComponentRegistry > test_registry;
};

