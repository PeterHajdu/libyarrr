#include <yarrr/engine.hpp>
#include "test_graphical_engine.hpp"
#include <yarrr/object.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/command.hpp>
#include <yarrr/entity_factory.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe( an_engine )
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );

    std::unique_ptr< yarrr::PhysicalBehavior > physical_behavior( new yarrr::PhysicalBehavior() );
    physical_parameters = &physical_behavior->physical_parameters;
    object->add_behavior( yarrr::ObjectBehavior::Pointer( physical_behavior.release() ) );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Engine() ) );
  }


  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::Engine::ctci ), Equals( true ) );
  }


  It( updates_local_physical_state_when_a_command_arrives )
  {
    const yarrr::PhysicalParameters old_physical_parameters( *physical_parameters );
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::thruster, 0 ) );
    AssertThat( old_physical_parameters, !Equals( *physical_parameters ) );
  }

  yarrr::PhysicalParameters* physical_parameters;
  yarrr::Object::Pointer object;
};

