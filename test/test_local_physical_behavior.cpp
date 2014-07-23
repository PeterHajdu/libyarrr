#include <yarrr/local_physical_behavior.hpp>
#include <yarrr/physical_parameters.hpp>
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

