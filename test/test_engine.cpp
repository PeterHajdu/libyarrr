#include <yarrr/engine.hpp>
#include "test_particle_factory.hpp"
#include <yarrr/object.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/command.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/particle.hpp>
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
    physical_parameters->coordinate = { 100, 200 };
    object->add_behavior( yarrr::ObjectBehavior::Pointer( physical_behavior.release() ) );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Engine() ) );

    particle_factory = static_cast< test::ParticleFactory* >( &the::ctci::service< yarrr::ParticleFactory >() );
    particle_factory->reset();
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

  It( creates_particles_when_it_is_on )
  {
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::thruster, 0 ) );
    AssertThat( particle_factory->was_particle_created, Equals( true ) );
  }

  It( created_particles_are_close_to_the_object )
  {
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::thruster, 0 ) );
    yarrr::Coordinate coordinate_difference(
        particle_factory->last_particle_parameters.coordinate -
        physical_parameters->coordinate );
    yarrr::huplons_to_metres_in_place( coordinate_difference );

    AssertThat( yarrr::length_of( coordinate_difference ), IsLessThan( 30 ) );
  }

  test::ParticleFactory* particle_factory;
  yarrr::PhysicalParameters* physical_parameters;
  yarrr::Object::Pointer object;
};

