#include <yarrr/local_physical_behavior.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/object_state_update.hpp>
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

