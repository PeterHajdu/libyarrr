#include <yarrr/basic_behaviors.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/object_state_update.hpp>
#include <yarrr/command.hpp>
#include <yarrr/ship_control.hpp>
#include <yarrr/event_factory.hpp>
#include <yarrr/object_container.hpp>
#include <thectci/dispatcher.hpp>
#include <thectci/component_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_local_physical_behavior )
{

  void SetUp()
  {
    test_dispatcher.reset( new the::ctci::Dispatcher() );
    test_registry.reset( new the::ctci::ComponentRegistry() );

    local_physical_behavior.reset( new yarrr::LocalPhysicalBehavior() );
    local_physical_behavior->register_to( *test_dispatcher, *test_registry );
  }

  It( registers_itself_as_a_component )
  {
    AssertThat( &test_registry->component<yarrr::LocalPhysicalBehavior>(), Equals( local_physical_behavior.get() ) );
  }

  It( exposes_writable_physical_parameters )
  {
    yarrr::LocalPhysicalBehavior physical_behavior;
    yarrr::PhysicalParameters& physical_parameters( physical_behavior.physical_parameters );
    (void) physical_parameters;
  }

  yarrr::ObjectBehavior::Pointer local_physical_behavior;
  std::unique_ptr< the::ctci::Dispatcher > test_dispatcher;
  std::unique_ptr< the::ctci::ComponentRegistry > test_registry;
};

Describe( a_simple_physics_updater )
{

  void SetUp()
  {
    test_dispatcher.reset( new the::ctci::Dispatcher() );
    test_registry.reset( new the::ctci::ComponentRegistry() );

    local_physical_behavior.register_to( *test_dispatcher, *test_registry );
    simple_physics_updater.register_to( *test_dispatcher, *test_registry );
  }

  It( updates_local_physical_state_on_timer_update_event )
  {
    const yarrr::PhysicalParameters old_physical_parameters(
        local_physical_behavior.physical_parameters );

    test_dispatcher->dispatch( yarrr::TimerUpdate( old_physical_parameters.timestamp + 100000 ) );
    AssertThat( old_physical_parameters, !Equals( local_physical_behavior.physical_parameters ) );
  }

  yarrr::LocalPhysicalBehavior local_physical_behavior;
  yarrr::SimplePhysicsUpdater simple_physics_updater;

  std::unique_ptr< the::ctci::Dispatcher > test_dispatcher;
  std::unique_ptr< the::ctci::ComponentRegistry > test_registry;
};

Describe( a_network_synchronizer )
{

  void SetUp()
  {
    test_dispatcher.reset( new the::ctci::Dispatcher() );
    test_registry.reset( new the::ctci::ComponentRegistry() );

    local_physical_behavior.register_to( *test_dispatcher, *test_registry );
    network_synchronizer.register_to( *test_dispatcher, *test_registry );
  }

  It( updates_local_physical_state_when_object_state_update_arrives )
  {
    const yarrr::PhysicalParameters old_physical_parameters(
        local_physical_behavior.physical_parameters );

    yarrr::PhysicalParameters other_physical_parameters;
    test_dispatcher->dispatch( yarrr::ObjectStateUpdate( other_physical_parameters ) );

    AssertThat( old_physical_parameters, !Equals( local_physical_behavior.physical_parameters ) );
    AssertThat( other_physical_parameters, !Equals( local_physical_behavior.physical_parameters ) );
  }

  yarrr::LocalPhysicalBehavior local_physical_behavior;
  yarrr::NetworkSynchronizer network_synchronizer;

  std::unique_ptr< the::ctci::Dispatcher > test_dispatcher;
  std::unique_ptr< the::ctci::ComponentRegistry > test_registry;
};

Describe( an_engine )
{

  void SetUp()
  {
    test_dispatcher.reset( new the::ctci::Dispatcher() );
    test_registry.reset( new the::ctci::ComponentRegistry() );

    local_physical_behavior.register_to( *test_dispatcher, *test_registry );
    engine.register_to( *test_dispatcher, *test_registry );
  }

  It( updates_local_physical_state_when_a_command_arrives )
  {
    const yarrr::PhysicalParameters old_physical_parameters(
        local_physical_behavior.physical_parameters );

    test_dispatcher->dispatch( yarrr::Command( yarrr::Command::thruster, 0 ) );

    AssertThat( old_physical_parameters, !Equals( local_physical_behavior.physical_parameters ) );
  }

  yarrr::LocalPhysicalBehavior local_physical_behavior;
  yarrr::Engine engine;

  std::unique_ptr< the::ctci::Dispatcher > test_dispatcher;
  std::unique_ptr< the::ctci::ComponentRegistry > test_registry;
};

Describe( a_physical_parameter_serializer )
{

  void SetUp()
  {
    test_dispatcher.reset( new the::ctci::Dispatcher() );
    test_registry.reset( new the::ctci::ComponentRegistry() );

    local_physical_behavior.register_to( *test_dispatcher, *test_registry );
    serializer.register_to( *test_dispatcher, *test_registry );
  }

  It( serializes_local_physical_parameters )
  {
    std::vector< yarrr::Data > serialized_physical_parameters;
    test_dispatcher->dispatch( yarrr::SerializePhysicalParameter( serialized_physical_parameters ) );
    AssertThat( serialized_physical_parameters.empty(), Equals( false ) );

    yarrr::Event::Pointer event( yarrr::EventFactory::create( serialized_physical_parameters.back() ) );
    AssertThat( event.get()!=nullptr, Equals( true ) );

    yarrr::ObjectStateUpdate& object_update( static_cast< yarrr::ObjectStateUpdate& >( *event ) );
    AssertThat( object_update.physical_parameters(), Equals( local_physical_behavior.physical_parameters ) );
  }

  yarrr::LocalPhysicalBehavior local_physical_behavior;
  yarrr::PhysicalParameterSerializer serializer;

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

